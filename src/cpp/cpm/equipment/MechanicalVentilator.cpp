/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/


#include "stdafx.h"
#include "equipment/MechanicalVentilator.h"
#include "controller/Controller.h"
#include "controller/Substances.h"
#include "controller/Circuits.h"
#include "controller/Compartments.h"
#include "PulseConfiguration.h"
#include "system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorConfiguration.h"
#include "engine/SEActionManager.h"
#include "engine/SEEquipmentActionCollection.h"
#include "engine/SEPatientActionCollection.h"
#include "engine/SEEventManager.h"
#include "circuit/fluid/SEFluidCircuit.h"
#include "compartment/fluid/SEGasCompartment.h"
#include "compartment/substances/SEGasSubstanceQuantity.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceFraction.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarFrequency.h"
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

MechanicalVentilator::MechanicalVentilator(PulseData& data) : SEMechanicalVentilator(data.GetSubstances()), m_data(data)
{
  Clear();
}

MechanicalVentilator::~MechanicalVentilator ()
{
  Clear();
}

void MechanicalVentilator::Clear()
{
  SEMechanicalVentilator::Clear();
  m_Environment = nullptr;
  m_Ventilator = nullptr;
  m_EnvironmentToVentilator = nullptr;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes system properties to valid homeostatic values.
//--------------------------------------------------------------------------------------------------
void MechanicalVentilator::Initialize()
{
  PulseSystem::Initialize();
  SetConnection(eMechanicalVentilator_Connection::Off);
  m_Inhaling = true;
  m_CurrentBreathingCycleTime_s = 0.0;
  StateChange();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes parameters for the mechanical ventilator class
///
/// \details
/// Initializes member variables and system level values on the common data model.
//--------------------------------------------------------------------------------------------------
void MechanicalVentilator::SetUp()
{
  m_dt_s = m_data.GetTimeStep().GetValue(TimeUnit::s);

  // Compartments
  m_Environment = m_data.GetCompartments().GetGasCompartment(pulse::EnvironmentCompartment::Ambient);
  m_Ventilator = m_data.GetCompartments().GetGasCompartment(pulse::MechanicalVentilatorCompartment::MechanicalVentilator);

  // Paths
  m_EnvironmentToVentilator = m_data.GetCircuits().GetMechanicalVentilatorCircuit().GetPath(pulse::MechanicalVentilatorPath::EnvironmentToVentilator);
}

void MechanicalVentilator::StateChange()
{
  SetConnection();

  if(m_data.GetAirwayMode() != eAirwayMode::MechanicalVentilator)
  { 
    return;
  }

  if (GetControl() != eMechanicalVentilator_Control::P_CMV)
  {
    //Only one option for now
    //jbw - error for unsupported
    SetControl(eMechanicalVentilator_Control::P_CMV);
  }

  if (GetDriverWaveform() != eMechanicalVentilator_DriverWaveform::Square)
  {
    //Only one option for now
    //jbw - error for unsupported
    SetDriverWaveform(eMechanicalVentilator_DriverWaveform::Square);
  }

  if (!HasPositiveEndExpiredPressure())
  {
    //jbw - fatal - required
  }

  if (!HasPeakInspiratoryPressure())
  {
    //jbw - fatal - required
  }

  //jbw - add checks to alert when things are overriden
  if (HasRespiratoryRate())
  {
    GetBreathPeriod().SetValue(1.0 / GetRespiratoryRate(FrequencyUnit::Per_s), TimeUnit::s);
  }
  else if (HasBreathPeriod())
  {
    GetRespiratoryRate().SetValue(1.0 / GetBreathPeriod(TimeUnit::min), FrequencyUnit::Per_min);
  }
  else if (HasInspiratoryPeriod() && HasExpiratoryPeriod())
  {
    double totalPeriod_s = GetInspiratoryPeriod(TimeUnit::s) + GetExpiratoryPeriod(TimeUnit::s);
    GetBreathPeriod().SetValue(totalPeriod_s, TimeUnit::s);
    GetRespiratoryRate().SetValue(60.0 / totalPeriod_s, FrequencyUnit::Per_min);
  }
  else
  {
    //jbw - fatal - need one of these
  }

  if (HasInspiratoryExpiratoryRatio())
  {
    double IERatio = GetInspiratoryExpiratoryRatio().GetValue();
    double totalPeriod_s = GetBreathPeriod(TimeUnit::s);
    double inspiratoryPeriod_s = IERatio * totalPeriod_s / (1 + IERatio);
    GetInspiratoryPeriod().SetValue(inspiratoryPeriod_s, TimeUnit::s);
    GetExpiratoryPeriod().SetValue(totalPeriod_s - inspiratoryPeriod_s, TimeUnit::s);
  }
  else if (HasInspiratoryPeriod())
  {
    double inspiratoryPeriod_s = GetInspiratoryPeriod(TimeUnit::s);
    double totalPeriod_s = GetBreathPeriod(TimeUnit::s);
    double expiratoryPeriod_s = totalPeriod_s - inspiratoryPeriod_s;
    GetExpiratoryPeriod().SetValue(expiratoryPeriod_s, TimeUnit::s);
    GetInspiratoryExpiratoryRatio().SetValue(inspiratoryPeriod_s / expiratoryPeriod_s);
  }
  else if (HasExpiratoryPeriod())
  {
    double expiratoryPeriod_s = GetInspiratoryPeriod(TimeUnit::s);
    double totalPeriod_s = GetBreathPeriod(TimeUnit::s);
    double inspiratoryPeriod_s = totalPeriod_s - expiratoryPeriod_s;
    GetInspiratoryPeriod().SetValue(inspiratoryPeriod_s, TimeUnit::s);
    GetInspiratoryExpiratoryRatio().SetValue(inspiratoryPeriod_s / expiratoryPeriod_s);
  }
  else
  {
    //jbw - fatal - need one of these
  }

  m_Inhaling = true;
  m_CurrentBreathingCycleTime_s = 0.0;

  // If you have one substance, make sure its Oxygen and add the standard CO2 and N2 to fill the difference

  //Set the substance volume fractions ********************************************
  std::vector<SESubstanceFraction*> gasFractions = GetFractionInspiredGases();

  //Reset the substance quantities at the connection
  for (SEGasSubstanceQuantity* subQ : m_Ventilator->GetSubstanceQuantities())
    subQ->SetToZero();

  //Start by setting everything to ambient
  for (auto s : m_Environment->GetSubstanceQuantities())
  {
    m_Ventilator->GetSubstanceQuantity(s->GetSubstance())->GetVolumeFraction().Set(s->GetVolumeFraction());
  }

  double totalFractionDefined = 0.0;
  //Has fractions defined
  for (auto f : gasFractions)
  {
    SESubstance& sub = f->GetSubstance();
    double fraction = f->GetFractionAmount().GetValue();
    totalFractionDefined += fraction;

    //Do this, just in case it's something new
    m_data.GetSubstances().AddActiveSubstance(sub);

    //Now set it on the connection compartment
    //It has a infinate volume, so this will keep the same volume fraction no matter what's going on around it
    m_Ventilator->GetSubstanceQuantity(sub)->GetVolumeFraction().SetValue(fraction);
  }

  //Add or remove Nitrogen to balance
  double gasFractionDiff = 1.0 - totalFractionDefined;
  double currentN2Fraction = m_Ventilator->GetSubstanceQuantity(m_data.GetSubstances().GetN2())->GetVolumeFraction().GetValue();
  if (currentN2Fraction + gasFractionDiff < 0.0)
  {
    //jbw - error - not enough N2 to balance
  }
  m_Ventilator->GetSubstanceQuantity(m_data.GetSubstances().GetN2())->GetVolumeFraction().SetValue(currentN2Fraction + gasFractionDiff);


  //jbw - add aerosols

  ////Set the aerosol concentrations ********************************************
  //std::vector<SESubstanceConcentration*> liquidConcentrations = mv->GetAerosols();

  ////Reset the substance quantities at the connection
  //for (SELiquidSubstanceQuantity* subQ : m_MechanicalVentilationAerosolConnection->GetSubstanceQuantities())
  //  subQ->SetToZero();

  //if (!liquidConcentrations.empty())
  //{
  //  //Has fractions defined
  //  for (auto f : liquidConcentrations)
  //  {
  //    SESubstance& sub = f->GetSubstance();
  //    SEScalarMassPerVolume concentration = f->GetConcentration();

  //    //Do this, just in case it's something new
  //    m_data.GetSubstances().AddActiveSubstance(sub);

  //    //Now set it on the connection compartment
  //    //It has infinite volume, so this will keep the same volume fraction no matter what's going on around it
  //    m_MechanicalVentilationAerosolConnection->GetSubstanceQuantity(sub)->GetConcentration().Set(concentration);
  //  }
  //}
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Connect to the patient via the ventilator mask, an endotracheal tube, or no connection
///
/// \param  enumMechanicalVentilatorConnection
/// Connectoin type : Mask, tube, or off
/// 
/// \details
/// If the enum is set to mask, then the mask is applied to the face
/// If the enum is set to tube, then the ventilator is connected to the tube
/// If the enum is set to off, the airway mode is set to free.
//--------------------------------------------------------------------------------------------------
void MechanicalVentilator::SetConnection(eMechanicalVentilator_Connection c)
{
  if (m_Connection == c)
    return; // No Change
  // Update the Pulse airway mode when this changes
  SEMechanicalVentilator::SetConnection(c);
  if (c == eMechanicalVentilator_Connection::Mask && m_data.GetIntubation() == eSwitch::Off)
  {
    m_data.SetAirwayMode(eAirwayMode::MechanicalVentilator);
    return;
  }
  else if (c == eMechanicalVentilator_Connection::Tube && m_data.GetIntubation() == eSwitch::On)
  {
    m_data.SetAirwayMode(eAirwayMode::MechanicalVentilator);
    return;
  }
  else if (c == eMechanicalVentilator_Connection::Mask && m_data.GetIntubation() == eSwitch::On)
    Error("Connection failed : Cannot apply mechanical ventilator mask if patient is intubated.");
  else if (c == eMechanicalVentilator_Connection::Tube && m_data.GetIntubation() == eSwitch::Off)
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
void MechanicalVentilator::InvalidateConnection()
{
  // Set airway mode to free
  m_data.SetAirwayMode(eAirwayMode::Free);
  // THEN invalidate
  m_Connection = eMechanicalVentilator_Connection::Off;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes gas volumes and volume fractions supplied by the mechanical ventilator depending on the airway mode
///
/// \details
/// The gas volumes and volume fractions are initialized and updated based on the airway mode (mask, free, or tube)
/// and the volume associated with each airway mode.
//--------------------------------------------------------------------------------------------------
void MechanicalVentilator::SetConnection()
{
  switch (m_data.GetAirwayMode())
  {
  case eAirwayMode::Free:
    //Basically a full leak to ground
    // TODO jbw
    break;
  case eAirwayMode::MechanicalVentilator:
    if (m_Connection == eMechanicalVentilator_Connection::Mask)
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
    else if (m_Connection == eMechanicalVentilator_Connection::Tube)
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
/// Checks the mechanical ventilator settings to ensure all are within the acceptable ranges. 
/// Processes all equipment failures.
/// Verifies the oxygen source.
/// Processes any failures with the oxygen source. 
/// Calculates the gas inlet volume fractions based on equipment failures and gas sources.
//--------------------------------------------------------------------------------------------------
void MechanicalVentilator::PreProcess()
{
  if (m_data.GetActions().GetEquipmentActions().HasMechanicalVentilatorConfiguration())
  {
    ProcessConfiguration(*m_data.GetActions().GetEquipmentActions().GetMechanicalVentilatorConfiguration());
    m_data.GetActions().GetEquipmentActions().RemoveMechanicalVentilatorConfiguration();
  }
  //Do nothing if the ventilator is off and not initialized
  if (GetConnection() == eMechanicalVentilator_Connection::Off)
  {
    m_Inhaling = true;
    m_CurrentBreathingCycleTime_s = 0.0;
    return;
  }
  
  SetConnection();
  CalculateCyclePhase();
  CalculateVentilatorPressure();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Mechanical ventilator process function
///
/// \details
/// The current Pulse implementation has no specific process functionality for the mechanical ventilator.
/// Mechanical ventilator processing is currently done in the Respiratory System with the combined circuit methodology.
//--------------------------------------------------------------------------------------------------
void MechanicalVentilator::Process()
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
void MechanicalVentilator::PostProcess()
{

}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculates respiration cycle
///
/// \details
/// The inspiratory and expiratory phase times are calculated based on
/// a pre-set respiration rate and inspiration-expiration ratio parameters. These parameters are selected as
/// input parameters for the anesthesia machine configurations.
//--------------------------------------------------------------------------------------------------
void MechanicalVentilator::CalculateCyclePhase()
{
  //Determine where we are in the cycle
  m_CurrentBreathingCycleTime_s += m_dt_s;
  if (m_CurrentBreathingCycleTime_s > GetBreathPeriod(TimeUnit::s)) //End of the cycle
  {
    m_CurrentBreathingCycleTime_s = 0.0;
  }

  if (m_CurrentBreathingCycleTime_s < GetInspiratoryPeriod(TimeUnit::s)) //Inspiration
  {
    m_Inhaling = true;
  }
  else //Expiration
  {
    m_Inhaling = false;
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculates ventilator pressure
///
/// \details
/// The Anesthesia machine employs pressure-control ventilation mode. This method calculates the 
/// control ventilator pressure that drives the gas flow in the breathing circle. During inspiration,
/// the ventilator pressure is set to pre-defined constant value to serve as an input pressure source.
/// This causes gas to flow into the inspiratory limb path. The pressure is dropped to much low pressure during the expiration 
/// phase to allow gas return to the ventilator.
//--------------------------------------------------------------------------------------------------
void MechanicalVentilator::CalculateVentilatorPressure()
{
  //Calculate the driver pressure
  double dDriverPressure = 0.0;
  if (m_Inhaling)
  {
    dDriverPressure = GetPeakInspiratoryPressure(PressureUnit::cmH2O);
  }
  else
  {
    //Exhaling
    dDriverPressure = GetPositiveEndExpiredPressure(PressureUnit::cmH2O);
  }
  m_EnvironmentToVentilator->GetNextPressureSource().SetValue(dDriverPressure, PressureUnit::cmH2O);
}
