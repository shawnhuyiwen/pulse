/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/


#include "stdafx.h"
#include "equipment/BagValveMask.h"
#include "controller/Controller.h"
#include "controller/Substances.h"
#include "controller/Circuits.h"
#include "controller/Compartments.h"
#include "PulseConfiguration.h"
#include "patient/actions/SEIntubation.h"
#include "system/equipment/bag_valve_mask/actions/SEBagValveMaskConfiguration.h"
#include "system/equipment/bag_valve_mask/actions/SEBagValveMaskInstantaneous.h"
#include "engine/SEActionManager.h"
#include "engine/SEEquipmentActionCollection.h"
#include "engine/SEPatientActionCollection.h"
#include "engine/SEEventManager.h"
#include "circuit/fluid/SEFluidCircuit.h"
#include "compartment/fluid/SEGasCompartment.h"
#include "compartment/fluid/SELiquidCompartment.h"
#include "compartment/substances/SEGasSubstanceQuantity.h"
#include "compartment/substances/SELiquidSubstanceQuantity.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceFraction.h"
#include "substance/SESubstanceConcentration.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarPressureTimePerVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarTime.h"
#include "utils/GeneralMath.h"

/*
========================
  Constructors
========================
*/

BagValveMask::BagValveMask(PulseData& data) : PulseBagValveMask(data)
{
  Clear();
}

BagValveMask::~BagValveMask ()
{
  Clear();
}

void BagValveMask::Clear()
{
  SEBagValveMask::Clear();
  m_Environment = nullptr;
  m_Reservoir = nullptr;
  m_ReservoirAerosol = nullptr;
  m_ReservoirNode = nullptr;
  m_ConnectionNode = nullptr;
  m_AmbientNode = nullptr;
  m_ReservoirToBag = nullptr;
  m_EnvironmentToPositiveEndExpiratoryPressurePort = nullptr;
  m_ConnectionToEnvironment = nullptr;
  m_ValveToEnvironment = nullptr;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes system properties to valid homeostatic values.
//--------------------------------------------------------------------------------------------------
void BagValveMask::Initialize()
{
  PulseSystem::Initialize();
  SetConnection(eBagValveMask_Connection::Off);
  m_CurrentBreathState = eBreathState::Exhale;
  m_CurrentPeriodTime_s = 0.0;
  m_SqueezePressure_cmH2O = SEScalar::dNaN();
  m_SqueezeFlow_L_Per_s = SEScalar::dNaN();
  StateChange();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes parameters for the mechanical ventilator class
///
/// \details
/// Initializes member variables and system level values on the common data model.
//--------------------------------------------------------------------------------------------------
void BagValveMask::SetUp()
{
  // Compartments
  m_Environment = m_data.GetCompartments().GetGasCompartment(pulse::EnvironmentCompartment::Ambient);
  m_Reservoir = m_data.GetCompartments().GetGasCompartment(pulse::BagValveMaskCompartment::Reservoir);
  m_ReservoirAerosol = m_data.GetCompartments().GetLiquidCompartment(pulse::BagValveMaskCompartment::Reservoir);

  // Nodes
  m_ReservoirNode = m_data.GetCircuits().GetBagValveMaskCircuit().GetNode(pulse::BagValveMaskNode::Reservoir);
  m_ConnectionNode = m_data.GetCircuits().GetBagValveMaskCircuit().GetNode(pulse::BagValveMaskNode::Connection);
  m_AmbientNode = m_data.GetCircuits().GetBagValveMaskCircuit().GetNode(pulse::EnvironmentNode::Ambient);

  // Paths
  m_ConnectionToEnvironment = m_data.GetCircuits().GetBagValveMaskCircuit().GetPath(pulse::BagValveMaskPath::ConnectionToEnvironment);
  m_ValveToEnvironment = m_data.GetCircuits().GetBagValveMaskCircuit().GetPath(pulse::BagValveMaskPath::ValveToEnvironment);
  m_ReservoirToBag = m_data.GetCircuits().GetBagValveMaskCircuit().GetPath(pulse::BagValveMaskPath::ReservoirToBag);

  m_DefaultOpenResistance_cmH2O_s_Per_L = m_data.GetConfiguration().GetDefaultOpenFlowResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  m_DefaultClosedResistance_cmH2O_s_Per_L = m_data.GetConfiguration().GetDefaultClosedFlowResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
}

void BagValveMask::StateChange()
{
  if (m_data.GetAirwayMode() != eAirwayMode::BagValveMask)
    return;
  SetConnection();
  m_CurrentBreathState = eBreathState::Inhale;
  m_CurrentPeriodTime_s = 0.0;

  // If you have one substance, make sure its Oxygen and add the standard CO2 and N2 to fill the difference

  //Set the substance volume fractions ********************************************
  std::vector<SESubstanceFraction*> gasFractions = GetFractionInspiredGases();

  //Reset the substance quantities at the connection
  for (SEGasSubstanceQuantity* subQ : m_Reservoir->GetSubstanceQuantities())
    subQ->SetToZero();

  //Start by setting everything to ambient
  for (auto s : m_Environment->GetSubstanceQuantities())
  {
    m_Reservoir->GetSubstanceQuantity(s->GetSubstance())->GetVolumeFraction().Set(s->GetVolumeFraction());
  }

  //Has fractions defined
  for (auto f : gasFractions)
  {
    const SESubstance& sub = f->GetSubstance();
    double fraction = f->GetFractionAmount().GetValue();

    //Do this, just in case it's something new
    m_data.GetSubstances().AddActiveSubstance(sub);

    //Now set it on the ventilator compartment
    //It has a infinate volume, so this will keep the same volume fraction no matter what's going on around it
    m_Reservoir->GetSubstanceQuantity(sub)->GetVolumeFraction().SetValue(fraction);
  }

  double totalFractionDefined = 0.0;
  for (auto s : m_Reservoir->GetSubstanceQuantities())
  {
    totalFractionDefined += m_Reservoir->GetSubstanceQuantity(s->GetSubstance())->GetVolumeFraction().GetValue();
  }

  //Add or remove Nitrogen to balance
  double gasFractionDiff = 1.0 - totalFractionDefined;
  double currentN2Fraction = m_Reservoir->GetSubstanceQuantity(m_data.GetSubstances().GetN2())->GetVolumeFraction().GetValue();
  if (currentN2Fraction + gasFractionDiff < 0.0)
  {
    double FiO2 = m_Reservoir->GetSubstanceQuantity(m_data.GetSubstances().GetO2())->GetVolumeFraction().GetValue();

    /// \error Error: FiO2 setting + ambient fractions other than N2 is greater than 1.0. Setting FiO2 to max value
    m_ss << "FiO2 setting + ambient fractions other than N2 is greater than 1.0. Setting FiO2 to max value of " << FiO2 + currentN2Fraction + gasFractionDiff << ".";
    Error(m_ss);
    m_Reservoir->GetSubstanceQuantity(m_data.GetSubstances().GetN2())->GetVolumeFraction().SetValue(0.0);
    m_Reservoir->GetSubstanceQuantity(m_data.GetSubstances().GetO2())->GetVolumeFraction().SetValue(FiO2 + currentN2Fraction + gasFractionDiff);
  }
  else
  {
    Info("Adding "+cdm::to_string(currentN2Fraction + gasFractionDiff) + "% of N2 to the system");
    m_Reservoir->GetSubstanceQuantity(m_data.GetSubstances().GetN2())->GetVolumeFraction().SetValue(currentN2Fraction + gasFractionDiff);
  }

  //Set the aerosol concentrations ********************************************
  std::vector<SESubstanceConcentration*> liquidConcentrations = GetConcentrationInspiredAerosols();

  //Reset the substance quantities at the ventilator
  for (SELiquidSubstanceQuantity* subQ : m_ReservoirAerosol->GetSubstanceQuantities())
    subQ->SetToZero();

  if (!liquidConcentrations.empty())
  {
    //Has fractions defined
    for (auto f : liquidConcentrations)
    {
      const SESubstance& sub = f->GetSubstance();
      SEScalarMassPerVolume concentration = f->GetConcentration();

      //Do this, just in case it's something new
      m_data.GetSubstances().AddActiveSubstance(sub);
      //Now set it on the connection compartment
      //It has infinite volume, so this will keep the same volume fraction no matter what's going on around it
      m_ReservoirAerosol->GetSubstanceQuantity(sub)->GetConcentration().Set(concentration);
    }
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Connect to the patient via the ventilator mask, an endotracheal tube, or no connection
///
/// \param  enumBagValveMaskConnection
/// Connectoin type : Mask, tube, or off
/// 
/// \details
/// If the enum is set to mask, then the mask is applied to the face
/// If the enum is set to tube, then the ventilator is connected to the tube
/// If the enum is set to off, the airway mode is set to free.
//--------------------------------------------------------------------------------------------------
void BagValveMask::SetConnection(eBagValveMask_Connection c)
{
  if (m_Connection == c)
    return; // No Change
  // Update the Pulse airway mode when this changes
  SEBagValveMask::SetConnection(c);
  if (c == eBagValveMask_Connection::Mask && m_data.GetIntubation() == eSwitch::Off)
  {
    m_data.SetAirwayMode(eAirwayMode::BagValveMask);
    return;
  }
  else if (c == eBagValveMask_Connection::Tube && m_data.GetIntubation() == eSwitch::On)
  {
    m_data.SetAirwayMode(eAirwayMode::BagValveMask);
    return;
  }
  else if (c == eBagValveMask_Connection::Mask && m_data.GetIntubation() == eSwitch::On)
    Error("Connection failed : Cannot apply mechanical ventilator mask if patient is intubated.");
  else if (c == eBagValveMask_Connection::Tube && m_data.GetIntubation() == eSwitch::Off)
    Error("Connection failed : Cannot apply mechanical ventilator to tube if patient is not intubated.");
  // Make sure we are active to make sure we go back to free
  m_data.SetAirwayMode(eAirwayMode::Free);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Removes the connection to the patient
///
/// \details
/// If the mask is on or the tube is connected, it is removed and the airway mode is set to free. The action is then removed from 
/// the action manager.
//--------------------------------------------------------------------------------------------------
void BagValveMask::InvalidateConnection()
{
  // Set airway mode to free
  m_data.SetAirwayMode(eAirwayMode::Free);
  // THEN invalidate
  m_Connection = eBagValveMask_Connection::Off;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes gas volumes and volume fractions supplied by the mechanical ventilator depending on the airway mode
///
/// \details
/// The gas volumes and volume fractions are initialized and updated based on the airway mode (mask, free, or tube)
/// and the volume associated with each airway mode.
//--------------------------------------------------------------------------------------------------
void BagValveMask::SetConnection()
{
  switch (m_data.GetAirwayMode())
  {
  case eAirwayMode::Free:
    m_Connection = eBagValveMask_Connection::Off;
    break;
  case eAirwayMode::BagValveMask:
    if (m_Connection == eBagValveMask_Connection::Mask)
    {
      if (m_data.GetIntubation() == eSwitch::On)// Somebody intubated while we had the mask on
      {
        Info("Mechanical Ventilator has been disconnected due to an intubation.");
        m_data.SetAirwayMode(eAirwayMode::Free);
        return;
      }

      //Keep the baseline resistance to ground = an open switch
      //Leaks handled later:L);
    }
    else if (m_Connection == eBagValveMask_Connection::Tube)
    {
      if (m_data.GetIntubation() == eSwitch::Off)// Somebody removed intubated while we were connected to it
      {
        Info("Mechanical Ventilator has been disconnected removal of intubation.");
        m_data.SetAirwayMode(eAirwayMode::Free);
        return;
      }

      //Keep the baseline resistance to ground = an open switch
      //Leaks handled later:L);
    }
    break;
  default:
    Fatal("Unhandled Airway Mode.");
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Mechanical ventilator preprocess function
///
/// \details
/// Applies all the settings and calculates the instantaneous driver value.
//--------------------------------------------------------------------------------------------------
void BagValveMask::PreProcess()
{
  if (m_data.GetActions().GetEquipmentActions().HasBagValveMaskInstantaneous())
  {
    if (m_data.GetActions().GetEquipmentActions().GetBagValveMaskInstantaneous().HasPressure())
    {
      m_SqueezePressure_cmH2O = m_data.GetActions().GetEquipmentActions().GetBagValveMaskInstantaneous().GetPressure(PressureUnit::cmH2O);
    }
    else if (m_data.GetActions().GetEquipmentActions().GetBagValveMaskInstantaneous().HasFlow())
    {
      m_SqueezeFlow_L_Per_s = m_data.GetActions().GetEquipmentActions().GetBagValveMaskInstantaneous().GetFlow(VolumePerTimeUnit::L_Per_s);
    }
    SetSqeezeDriver();
    return;
  }

  if (m_data.GetActions().GetEquipmentActions().HasBagValveMaskConfiguration())
  {
    SEBagValveMask::Clear();
    ProcessConfiguration(m_data.GetActions().GetEquipmentActions().GetBagValveMaskConfiguration(), m_data.GetSubstances());
    m_data.GetActions().GetEquipmentActions().RemoveBagValveMaskConfiguration();
  }
  //Do nothing if the ventilator is off and not initialized
  if (GetConnection() == eBagValveMask_Connection::Off)
  {
    m_CurrentBreathState = eBreathState::Exhale;
    m_CurrentPeriodTime_s = 0.0;
    return;
  }
  
  SetConnection();
  CalculateInspiration();
  CalculateExpiration();
  SetSqeezeDriver();
  SetResistances();

  m_CurrentPeriodTime_s += m_data.GetTimeStep_s();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Mechanical ventilator process function
///
/// \details
/// The current Pulse implementation has no specific process functionality for the mechanical ventilator.
/// Mechanical ventilator processing is currently done in the Respiratory System with the combined circuit methodology.
//--------------------------------------------------------------------------------------------------
void BagValveMask::Process(bool solve_and_transport)
{
  ComputeExposedModelParameters();
}
void BagValveMask::ComputeExposedModelParameters()
{

}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Mechanical ventilator postprocess function
///
/// \details
/// The substance volumes and the volume fractions are updated for all of the nodes in the mechanical ventilator
/// circuit during post process. 
/// The ventilator volumes are updated based on the previously calculated nodal analysis.
//--------------------------------------------------------------------------------------------------
void BagValveMask::PostProcess(bool solve_and_transport)
{

}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Set the instantaneous squeeze pressure or flow on the circuit source.
//--------------------------------------------------------------------------------------------------
void BagValveMask::SetSqeezeDriver()
{
  if (!std::isnan(m_SqueezePressure_cmH2O) && !std::isnan(m_SqueezeFlow_L_Per_s))
    {
      /// \error Error: Bag Valve Mask squeeze pressure and flow both set, only one allowed. Using the pressure value.
      Error("Bag Valve Mask squeeze pressure and flow both set, only one allowed. Using the pressure value.");
    }
  else if (std::isnan(m_SqueezePressure_cmH2O) && std::isnan(m_SqueezeFlow_L_Per_s))
  {
    /// \error Error:  Bag Valve Mask squeeze pressure or flow must be set. Using a pressure of 0.
    Error(" Bag Valve Mask squeeze pressure or flow must be set. Using a pressure of 0.");

    m_SqueezePressure_cmH2O = 0.0;
  }

  if (!std::isnan(m_SqueezePressure_cmH2O))
  {
    if (m_ReservoirToBag->HasNextFlowSource())
    {
      m_ReservoirToBag->GetNextFlowSource().Invalidate();
      m_data.GetCircuits().GetRespiratoryAndMechanicalVentilationCircuit().StateChange();
    }
    m_ReservoirToBag->GetNextPressureSource().SetValue(m_SqueezePressure_cmH2O, PressureUnit::cmH2O);
  }
  else if (!std::isnan(m_SqueezeFlow_L_Per_s))
  {
    if (m_ReservoirToBag->HasNextPressureSource())
    {
      m_ReservoirToBag->GetNextPressureSource().Invalidate();
      m_data.GetCircuits().GetRespiratoryAndMechanicalVentilationCircuit().StateChange();
    }
    m_ReservoirToBag->GetNextFlowSource().SetValue(m_SqueezeFlow_L_Per_s, VolumePerTimeUnit::L_Per_s);
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Determine the instantaneous driver pressure during inspiration.
//--------------------------------------------------------------------------------------------------
void BagValveMask::CalculateInspiration()
{
  if (m_CurrentBreathState != eBreathState::Inhale)
  {
    return;
  }

  // Set the exhaust valve to not allow any flow
  m_ValveToEnvironment->GetNextResistance().SetValue(m_DefaultOpenResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);

  // Check trigger
  double breathFrequency_Per_s = 0.0;
  double totalBreathTime_s = 0.0;
  if (HasBreathFrequency())
  {
    breathFrequency_Per_s = GetBreathFrequency(FrequencyUnit::Per_s);
    totalBreathTime_s = 1.0 / breathFrequency_Per_s;
  }

  double inspiratoryExpiratoryRatio = 0.5; //Default
  if (HasInspiratoryExpiratoryRatio())
  {
    inspiratoryExpiratoryRatio = GetInspiratoryExpiratoryRatio().GetValue();
  }

  double inspirationPeriod_s = inspiratoryExpiratoryRatio * totalBreathTime_s / (1.0 + inspiratoryExpiratoryRatio);

  if (m_CurrentPeriodTime_s >= inspirationPeriod_s)
  {
    CycleMode();
    return;
  }

  // Calculate source - constant pressure or flow during inspiration phase
  if (HasPositiveInspiratoryPressure())
  {
    m_SqueezePressure_cmH2O = GetPositiveInspiratoryPressure(PressureUnit::cmH2O);
    m_SqueezeFlow_L_Per_s = SEScalar::dNaN();
  }
  else if (HasSqueezeVolume())
  {
    m_SqueezeFlow_L_Per_s = GetSqueezeVolume(VolumeUnit::L) / inspirationPeriod_s;
    m_SqueezePressure_cmH2O = SEScalar::dNaN();
  }
  else
  {
    /// \error Fatal: Inspiration mode needs to be set.
    Fatal("Inspiration mode needs to be set.");
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Determine the instantaneous driver pressure during expiration.
//--------------------------------------------------------------------------------------------------
void BagValveMask::CalculateExpiration()
{
  if (m_CurrentBreathState != eBreathState::Exhale)
  {
    return;
  }

  // Check trigger
  double breathFrequency_Per_s = 0.0;
  double totalBreathTime_s = 0.0;
  if (HasBreathFrequency())
  {
    breathFrequency_Per_s = GetBreathFrequency(FrequencyUnit::Per_s);
    totalBreathTime_s = 1.0 / breathFrequency_Per_s;
  }

  double inspiratoryExpiratoryRatio = 0.5; //Default
  if (HasInspiratoryExpiratoryRatio())
  {
    inspiratoryExpiratoryRatio = GetInspiratoryExpiratoryRatio().GetValue();
  }

  double inspirationPeriod_s = inspiratoryExpiratoryRatio * totalBreathTime_s / (1.0 + inspiratoryExpiratoryRatio);
  double expirationPeriod_s = totalBreathTime_s - inspirationPeriod_s;

  if (m_CurrentPeriodTime_s >= expirationPeriod_s)
  {
    CycleMode();
    return;
  }

  m_SqueezePressure_cmH2O = 0.0;
  if (HasValvePositiveEndExpiredPressure())
  {
    m_SqueezePressure_cmH2O = GetValvePositiveEndExpiredPressure(PressureUnit::cmH2O);
  }

  m_SqueezeFlow_L_Per_s = SEScalar::dNaN();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Go to next breath mode based on current mode.
///
/// \details
/// Inhale to pause, pause to exhale, or exhale to inhale.
//--------------------------------------------------------------------------------------------------
void BagValveMask::CycleMode()
{
  m_CurrentPeriodTime_s = 0.0;

  if (m_CurrentBreathState == eBreathState::Inhale)
  {
    m_CurrentBreathState = eBreathState::Exhale;
  }
  else if (m_CurrentBreathState == eBreathState::Exhale)
  {
    m_CurrentBreathState = eBreathState::Inhale;
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Set ventilator circuit resistances.
///
/// \details
/// If no values are specified, defaults will be used.
//--------------------------------------------------------------------------------------------------
void BagValveMask::SetResistances()
{
  //jbw - Do, and set filter volume somewhere
}
