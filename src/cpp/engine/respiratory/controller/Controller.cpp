/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "PulsePhysiologyEngine.h"
#include "engine/respiratory/controller/Controller.h"
#include "engine/respiratory/controller/Compartments.h"
// Common Pulse Objects
#include "PulseConfiguration.h"
#include "cpm/controller/BlackBoxes.h"
#include "cpm/controller/Circuits.h"
#include "cpm/controller/Substances.h"
#include "cpm/environment/Environment.h"
#include "cpm/equipment/MechanicalVentilator.h"
#include "cpm/physiology/Respiratory.h"
// Common Data Model Objects
#include "cdm/engine/SEActionManager.h"
#include "cdm/engine/SEConditionManager.h"
#include "cdm/engine/SEPatientConfiguration.h"
#include "cdm/engine/SEEngineTracker.h"
#include "cdm/patient/SEPatient.h"

class FatalListner : public LoggerForward
{
public:
  FatalListner(SEEventManager& mgr, SEScalarTime& ct) : m_Events(mgr), m_CurrentTime(ct) {};
  ~FatalListner() = default;

  virtual void ForwardDebug(const std::string& msg, const std::string& origin) { }
  virtual void ForwardInfo(const std::string& msg, const std::string& origin) { }
  virtual void ForwardWarning(const std::string& msg, const std::string& origin) { }
  virtual void ForwardError(const std::string& msg, const std::string& origin) { }
  virtual void ForwardFatal(const std::string& msg, const std::string& origin)
  {
    m_Events.SetEvent(eEvent::IrreversibleState, true, m_CurrentTime);
  }

protected:
  SEEventManager& m_Events;
  SEScalarTime& m_CurrentTime;
};

Controller::Controller(Logger* logger) : PulseController(logger)
{
  SetConfigurationOverride(nullptr);
}

bool Controller::IsAirwayModeSupported(eAirwayMode mode)
{
  // Only support two modes
  return (mode == eAirwayMode::Free || mode == eAirwayMode::MechanicalVentilation);
}

void Controller::Allocate()
{
  // Create common objects we will use
  m_Substances = new PulseSubstances(*this);

  m_InitialPatient = new SEPatient(GetLogger());
  m_CurrentPatient = new SEPatient(GetLogger());

  m_Config = new PulseConfiguration(GetLogger());
  m_Config->Initialize("");//Setup defaults that don't need files on disk

  m_Actions = new SEActionManager(*m_Substances);
  m_Conditions = new SEConditionManager(GetLogger());

  m_Environment = new Environment(*this);

  m_RespiratorySystem = new Respiratory(*this);

  m_MechanicalVentilator = new MechanicalVentilator(*this);

  m_EventManager = new SEEventManager(GetLogger());

  // Create our derived objects
  m_BlackBoxes = new PulseBlackBoxes(*this);
  m_Compartments = new Compartments(*this);
  m_Circuits = new PulseCircuits(*this);

  m_LogForward = new FatalListner(*m_EventManager, m_CurrentTime);
  m_Logger->AddForward(m_LogForward);

  SetupTracker();
}

bool Controller::Stabilize(const SEPatientConfiguration& patient_configuration)
{
  if (patient_configuration.HasConditions() && !patient_configuration.GetConditions()->IsEmpty())
      Warning("Provided conditions will be ignored, this engine does not need to stabilize");

  return true;// Nothing to do here, we are good to go!
}

void Controller::InitializeSystems()
{
  Info("Initializing Systems");
  m_RespiratorySystem->Clear();
  m_MechanicalVentilator->Clear();

  m_RespiratorySystem->Initialize();
  m_MechanicalVentilator->Initialize();
}

void Controller::AtSteadyState(EngineState state)
{
  m_State = state;

  m_MechanicalVentilator->AtSteadyState();
  m_RespiratorySystem->AtSteadyState();
}

void Controller::PreProcess()
{
  m_MechanicalVentilator->PreProcess();
  m_RespiratorySystem->PreProcess();
}

void Controller::Process()
{
  m_MechanicalVentilator->Process();
  m_RespiratorySystem->Process();
}

void Controller::PostProcess()
{
  m_MechanicalVentilator->PostProcess();
  m_RespiratorySystem->PostProcess();
}
