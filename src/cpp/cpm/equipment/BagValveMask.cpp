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
#include "system/equipment/bag_valve_mask/actions/SEBagValveMaskAutomated.h"
#include "system/equipment/bag_valve_mask/actions/SEBagValveMaskInstantaneous.h"
#include "system/equipment/bag_valve_mask/actions/SEBagValveMaskSqueeze.h"
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
#include "patient/SEPatient.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarMass.h"
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
  m_Filter = nullptr;
  m_ReservoirToBag = nullptr;
  m_BagToValve = nullptr;
  m_ValveToFilter = nullptr;
  m_FilterToConnection = nullptr;
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
/// Initializes parameters for the bag valve mask class
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
  m_Filter = m_data.GetCircuits().GetBagValveMaskCircuit().GetNode(pulse::BagValveMaskNode::Filter);

  // Paths
  m_ReservoirToBag = m_data.GetCircuits().GetBagValveMaskCircuit().GetPath(pulse::BagValveMaskPath::ReservoirToBag);
  m_BagToValve = m_data.GetCircuits().GetBagValveMaskCircuit().GetPath(pulse::BagValveMaskPath::BagToValve);
  m_ValveToFilter = m_data.GetCircuits().GetBagValveMaskCircuit().GetPath(pulse::BagValveMaskPath::ValveToFilter);
  m_FilterToConnection = m_data.GetCircuits().GetBagValveMaskCircuit().GetPath(pulse::BagValveMaskPath::FilterToConnection);
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

    //Now set it on the compartment
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

  //Reset the substance quantities
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
/// Connect to the patient via the mask, a tube, or no connection
///
/// \param  enumBagValveMaskConnection
/// Connectoin type : Mask, tube, or off
/// 
/// \details
/// If the enum is set to mask, then the mask is applied to the face
/// If the enum is set to tube, then the BVM is connected to the tube
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
    Error("Connection failed : Cannot apply bag valve mask mask if patient is intubated.");
  else if (c == eBagValveMask_Connection::Tube && m_data.GetIntubation() == eSwitch::Off)
    Error("Connection failed : Cannot apply bag valve mask to tube if patient is not intubated.");
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
/// Initializes gas volumes and volume fractions supplied by the bag valve mask depending on the airway mode
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
        Info("Bag Valve Mask has been disconnected due to an intubation.");
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
        Info("Bag Valve Mask has been disconnected removal of intubation.");
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
/// Bag Valve Mask preprocess function
///
/// \details
/// Applies all the settings and calculates the instantaneous driver value.
//--------------------------------------------------------------------------------------------------
void BagValveMask::PreProcess()
{
  if (GetConnection() == eBagValveMask_Connection::Off && !m_data.GetActions().GetEquipmentActions().HasBagValveMaskConfiguration())
  {
    if (m_data.GetActions().GetEquipmentActions().HasBagValveMaskAutomated())
    {
      /// \error Error: The Bag Valve Mask must be connected (i.e., connection type configured) to run the Squeeze action.
      Error("The Bag Valve Mask must be connected (i.e., connection type configured) to run the automated action.");
      m_data.GetActions().GetEquipmentActions().RemoveBagValveMaskAutomated();
    }
    if (m_data.GetActions().GetEquipmentActions().HasBagValveMaskInstantaneous())
    {
      /// \error Error: The Bag Valve Mask must be connected (i.e., connection type configured) to run the Instantaneous action.
      Error("The Bag Valve Mask must be connected (i.e., connection type configured) to run the Instantaneous action.");
      m_data.GetActions().GetEquipmentActions().RemoveBagValveMaskInstantaneous();
    }
    if (m_data.GetActions().GetEquipmentActions().HasBagValveMaskSqueeze())
    {
      /// \error Error: The Bag Valve Mask must be connected (i.e., connection type configured) to run the Squeeze action.
      Error("The Bag Valve Mask must be connected (i.e., connection type configured) to run the Squeeze action.");
      m_data.GetActions().GetEquipmentActions().RemoveBagValveMaskSqueeze();
    }
    return;
  }

  if (m_data.GetActions().GetEquipmentActions().HasBagValveMaskConfiguration())
  {
    ProcessConfiguration(m_data.GetActions().GetEquipmentActions().GetBagValveMaskConfiguration(), m_data.GetSubstances());
    m_data.GetActions().GetEquipmentActions().RemoveBagValveMaskConfiguration();

    SetConnection();
  }
  // BVM is being turned off, remove any bvm actions and stop our preprocess
  if (GetConnection() == eBagValveMask_Connection::Off)
  {
    m_CurrentBreathState = eBreathState::Exhale;
    m_CurrentPeriodTime_s = 0.0;

    m_data.GetActions().GetEquipmentActions().RemoveBagValveMaskAutomated();
    m_data.GetActions().GetEquipmentActions().RemoveBagValveMaskInstantaneous();
    m_data.GetActions().GetEquipmentActions().RemoveBagValveMaskSqueeze();
    return;
  }

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

  m_BreathFrequency = nullptr;
  m_InspiratoryExpiratoryRatio = nullptr;
  m_SqueezePressure = nullptr;
  m_SqueezeVolume = nullptr;
  m_ExpiratoryPeriod = nullptr;
  m_InspiratoryPeriod = nullptr;

  if (m_data.GetActions().GetEquipmentActions().HasBagValveMaskSqueeze())
  {
    if (m_data.GetActions().GetEquipmentActions().GetBagValveMaskSqueeze().HasSqueezePressure())
      m_SqueezePressure = &m_data.GetActions().GetEquipmentActions().GetBagValveMaskSqueeze().GetSqueezePressure();
    if (m_data.GetActions().GetEquipmentActions().GetBagValveMaskSqueeze().HasSqueezeVolume())
      m_SqueezeVolume = &m_data.GetActions().GetEquipmentActions().GetBagValveMaskSqueeze().GetSqueezeVolume();
    if (m_data.GetActions().GetEquipmentActions().GetBagValveMaskSqueeze().HasExpiratoryPeriod())
      m_ExpiratoryPeriod = &m_data.GetActions().GetEquipmentActions().GetBagValveMaskSqueeze().GetExpiratoryPeriod();
    if (m_data.GetActions().GetEquipmentActions().GetBagValveMaskSqueeze().HasExpiratoryPeriod())
      m_InspiratoryPeriod = &m_data.GetActions().GetEquipmentActions().GetBagValveMaskSqueeze().GetInspiratoryPeriod();
  }
  else if (m_data.GetActions().GetEquipmentActions().HasBagValveMaskAutomated())
  {
    if (m_data.GetActions().GetEquipmentActions().GetBagValveMaskAutomated().HasBreathFrequency())
      m_BreathFrequency = &m_data.GetActions().GetEquipmentActions().GetBagValveMaskAutomated().GetBreathFrequency();
    if (m_data.GetActions().GetEquipmentActions().GetBagValveMaskAutomated().HasInspiratoryExpiratoryRatio())
      m_InspiratoryExpiratoryRatio = &m_data.GetActions().GetEquipmentActions().GetBagValveMaskAutomated().GetInspiratoryExpiratoryRatio();
    if (m_data.GetActions().GetEquipmentActions().GetBagValveMaskAutomated().HasSqueezePressure())
      m_SqueezePressure = &m_data.GetActions().GetEquipmentActions().GetBagValveMaskAutomated().GetSqueezePressure();
    if (m_data.GetActions().GetEquipmentActions().GetBagValveMaskAutomated().HasSqueezeVolume())
      m_SqueezeVolume = &m_data.GetActions().GetEquipmentActions().GetBagValveMaskAutomated().GetSqueezeVolume();
  }
  else
  {
    //Hold this pressure until an action is called or the BVM is turned off
    m_SqueezePressure_cmH2O = 0.0;
    if (HasValvePositiveEndExpiredPressure())
    {
      m_SqueezePressure_cmH2O = GetValvePositiveEndExpiredPressure(PressureUnit::cmH2O);
    }
    m_SqueezeFlow_L_Per_s = SEScalar::dNaN();

    SetSqeezeDriver();
    return;
  }

  CalculateInspiration();
  CalculateExpiration();
  SetSqeezeDriver();
  SetResistances();
  SetVolumes();

  ///\todo We should add the ability to manually handle source flow and reservoir volume like the NonRebreatherMask.
  ///      This would entail adding something like ReservoirSourceFlow and ReservoirVolume (default = 2.5 L).
  ///      If the volume goes to zero, ambient substance values should be used

  m_CurrentPeriodTime_s += m_data.GetTimeStep_s();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Bag Valve Mask process function
///
/// \details
/// The current Pulse implementation has no specific process functionality for the bag valve mask.
/// Bag Valve Mask processing is currently done in the Respiratory System with the combined circuit methodology.
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
/// Bag Valve Mask postprocess function
///
/// \details
/// The substance volumes and the volume fractions are updated for all of the nodes in the bag valve mask
/// circuit during post process. 
/// The volumes are updated based on the previously calculated nodal analysis.
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

  bool stateChange = false;
  if (!std::isnan(m_SqueezePressure_cmH2O))
  {
    if (m_ReservoirToBag->HasNextFlowSource())
    {
      stateChange = true;
      m_ReservoirToBag->RemoveFlowSource();
    }
    if (!m_ReservoirToBag->HasNextPressureSource())
    {
      stateChange = true;
      m_ReservoirToBag->GetPressureSourceBaseline().SetValue(0, PressureUnit::cmH2O);
    }
    m_ReservoirToBag->GetNextPressureSource().SetValue(m_SqueezePressure_cmH2O, PressureUnit::cmH2O);
  }
  else if (!std::isnan(m_SqueezeFlow_L_Per_s))
  {
    if (m_ReservoirToBag->HasNextPressureSource())
    {
      stateChange = true;
      m_ReservoirToBag->RemovePressureSource();
    }
    if (!m_ReservoirToBag->HasNextFlowSource())
    {
      stateChange = true;
      m_ReservoirToBag->GetFlowSourceBaseline().SetValue(0, VolumePerTimeUnit::L_Per_s);
    }
    m_ReservoirToBag->GetNextFlowSource().SetValue(m_SqueezeFlow_L_Per_s, VolumePerTimeUnit::L_Per_s);
  }
  if(stateChange)
    m_data.GetCircuits().GetRespiratoryAndBagValveMaskCircuit().StateChange();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Determine the instantaneous squeeze pressure during inspiration.
//--------------------------------------------------------------------------------------------------
void BagValveMask::CalculateInspiration()
{
  if (m_CurrentBreathState != eBreathState::Inhale)
  {
    return;
  }

  // Check trigger
  // Give defaults, if not user defined
  double breathFrequency_Per_s = 12.0 / 60.0; //12 bpm
  if (m_BreathFrequency != nullptr)
  {
    breathFrequency_Per_s = m_BreathFrequency->GetValue(FrequencyUnit::Per_s);
  }

  double totalBreathTime_s = 1.0 / breathFrequency_Per_s;

  double inspiratoryExpiratoryRatio = 0.5; //Default
  if (m_InspiratoryExpiratoryRatio != nullptr)
  {
    inspiratoryExpiratoryRatio = m_InspiratoryExpiratoryRatio->GetValue();
  }

  double inspirationPeriod_s = inspiratoryExpiratoryRatio * totalBreathTime_s / (1.0 + inspiratoryExpiratoryRatio);

  if (m_InspiratoryPeriod != nullptr)
  {
    inspirationPeriod_s = m_InspiratoryPeriod->GetValue(TimeUnit::s);
  }

  if (m_CurrentPeriodTime_s >= inspirationPeriod_s)
  {
    CycleMode();
    return;
  }

  // Calculate source - constant pressure or flow during inspiration phase
  if (m_SqueezePressure != nullptr)
  {
    m_SqueezePressure_cmH2O = m_SqueezePressure->GetValue(PressureUnit::cmH2O);
    m_SqueezeFlow_L_Per_s = SEScalar::dNaN();
  }
  else if (m_SqueezeVolume != nullptr)
  {
    m_SqueezeFlow_L_Per_s = m_SqueezeVolume->GetValue(VolumeUnit::L) / inspirationPeriod_s;
    m_SqueezePressure_cmH2O = SEScalar::dNaN();
  }
  else
  {
    // Set a default squeeze volume that attempts to acheive the ideal tidal volume (7 mL/kg(ideal))
    double idealBodyWeight_kg = m_data.GetInitialPatient().GetIdealBodyWeight(MassUnit::kg);
    double tidalVolume_mL = 7.0 * idealBodyWeight_kg;
    m_SqueezeFlow_L_Per_s = (tidalVolume_mL / 1000.0) / inspirationPeriod_s;
    m_SqueezePressure_cmH2O = SEScalar::dNaN();
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Determine the instantaneous squeeze pressure during expiration.
//--------------------------------------------------------------------------------------------------
void BagValveMask::CalculateExpiration()
{
  if (m_CurrentBreathState != eBreathState::Exhale)
  {
    return;
  }

  // Check trigger
  // Give defaults, if not user defined
  double breathFrequency_Per_s = 12.0 / 60.0; //12 bpm
  if (m_BreathFrequency != nullptr)
  {
    breathFrequency_Per_s = m_BreathFrequency->GetValue(FrequencyUnit::Per_s);
  }

  double totalBreathTime_s = 1.0 / breathFrequency_Per_s;

  double inspiratoryExpiratoryRatio = 0.5; //Default
  if (m_InspiratoryExpiratoryRatio != nullptr)
  {
    inspiratoryExpiratoryRatio = m_InspiratoryExpiratoryRatio->GetValue();
  }

  double inspirationPeriod_s = inspiratoryExpiratoryRatio * totalBreathTime_s / (1.0 + inspiratoryExpiratoryRatio);
  double expirationPeriod_s = totalBreathTime_s - inspirationPeriod_s;

  if (m_ExpiratoryPeriod != nullptr)
  {
    expirationPeriod_s = m_ExpiratoryPeriod->GetValue(TimeUnit::s);
  }

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
/// Inhale to exhale or exhale to inhale.
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

    //End of breath, so remove the squeeze action
    if (m_data.GetActions().GetEquipmentActions().HasBagValveMaskSqueeze())
    {
      m_data.GetActions().GetEquipmentActions().RemoveBagValveMaskSqueeze();
    }
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Set Bag Valve Mask circuit resistances.
///
/// \details
/// If no values are specified, defaults will be used.
//--------------------------------------------------------------------------------------------------
void BagValveMask::SetResistances()
{
  if (HasBagResistance())
  {
    m_BagToValve->GetNextResistance().Set(GetBagResistance());
  }

  if (HasValveResistance())
  {
    m_ValveToFilter->GetNextResistance().Set(GetValveResistance());
  }

  if (HasFilterResistance())
  {
    m_FilterToConnection->GetNextResistance().Set(GetFilterResistance());
  }

  //Aaron - Add SealResistance for pulse::BagValveMaskPath::ConnectionToEnvironment
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Set Bag Valve Mask node/compartment volumes.
///
/// \details
/// If no values are specified, defaults will be used.
//--------------------------------------------------------------------------------------------------
void BagValveMask::SetVolumes()
{
  if (HasFilterVolume())
  {
    m_Filter->GetNextVolume().Set(GetFilterVolume());
  }
  else
  {
    //Volume doesn't reset to baseline like path elements
    m_Filter->GetNextVolume().Set(m_Filter->GetVolumeBaseline());
  }
}
