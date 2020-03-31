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

MechanicalVentilator::MechanicalVentilator(PulseController& data) : SEMechanicalVentilator(data.GetSubstances()), m_data(data)
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
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes system properties to valid homeostatic values.
//--------------------------------------------------------------------------------------------------
void MechanicalVentilator::Initialize()
{
  PulseSystem::Initialize();

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
  
}

void MechanicalVentilator::StateChange()
{
  // TODO (jbw)
  // Make sure you have RR or BreathPeriod and I/E ratio
  // OR I and E Periods
  // If you have one substance, make sure its Oxygen and add the standard CO2 and N2 to fill the difference
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
   
    return;
  }
  SetConnection();
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
