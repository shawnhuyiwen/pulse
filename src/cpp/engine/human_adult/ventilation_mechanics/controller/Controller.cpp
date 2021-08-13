/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "engine/human_adult/ventilation_mechanics/controller/Controller.h"
#include "engine/human_adult/ventilation_mechanics/controller/CompartmentManager.h"
#include "engine/human_adult/whole_body/controller/CircuitManager.h"
#include "engine/human_adult/whole_body/controller/SubstanceManager.h"
#include "engine/human_adult/whole_body/system/environment/EnvironmentModel.h"
#include "engine/human_adult/whole_body/system/equipment/MechanicalVentilatorModel.h"
#include "engine/human_adult/whole_body/system/physiology/RespiratoryModel.h"

#include "cdm/engine/SEActionManager.h"
#include "cdm/engine/SEConditionManager.h"
#include "cdm/engine/SEEventManager.h"
#include "cdm/engine/SEPatientConfiguration.h"
#include "cdm/engine/SEEngineTracker.h"
#include "cdm/patient/SEPatient.h"

namespace pmc = PULSE_ENGINE;
namespace hawb = HUMAN_ADULT_WHOLE_BODY;

namespace HUMAN_ADULT_VENT_MECH
{
  Controller::Controller(Logger* logger) : hawb::Controller(logger)
  {

  }

  // Serialization is not supported
  bool Controller::SerializeFromFile(const std::string& filename)
  {
    Error("Serialization is not supported in this engine");
    return false;
  }
  bool Controller::SerializeToFile(const std::string& filename) const
  {
    Error("Serialization is not supported in this engine");
    return false;
  }

  bool Controller::SerializeFromString(const std::string& src, SerializationFormat m)
  {
    Error("Serialization is not supported in this engine");
    return false;
  }
  bool Controller::SerializeToString(std::string& output, SerializationFormat m) const
  {
    Error("Serialization is not supported in this engine");
    return false;
  }

  bool Controller::GetPatientAssessment(SEPatientAssessment& assessment) const
  {
    Error("No assesments are supported in this engine");
    return false;
  }

  bool Controller::IsAirwayModeSupported(pmc::eAirwayMode mode)
  {
    // Only support two modes
    return (mode == pmc::eAirwayMode::Free || mode == pmc::eAirwayMode::MechanicalVentilator);
  }

  void Controller::Allocate()
  {
    // Create common objects we will use
    m_Substances = new pmc::SubstanceManager(*this);

    m_InitialPatient = new SEPatient(GetLogger());
    m_CurrentPatient = new SEPatient(GetLogger());

    m_Config = new pmc::PulseConfiguration(GetLogger());
    m_Config->Initialize("");//Setup defaults that don't need files on disk

    m_Actions = new SEActionManager(*m_Substances);
    m_Conditions = new SEConditionManager(GetLogger());

    m_EnvironmentModel = new pmc::EnvironmentModel(*this);

    m_RespiratoryModel = new pmc::RespiratoryModel(*this);

    m_MechanicalVentilatorModel = new pmc::MechanicalVentilatorModel(*this);

    m_EventManager = new SEEventManager(GetLogger());

    // Create our derived objects
    m_BlackBoxes = new pmc::BlackBoxManager(*this);
    m_Compartments = new CompartmentManager(*this);
    m_Circuits = new pmc::CircuitManager(*this);

    m_LogForward = new pmc::FatalListner(*m_EventManager, m_CurrentTime);
    m_Logger->AddForward(m_LogForward);

    SetupTracker();
  }

  bool Controller::Stabilize(const SEPatientConfiguration& patient_configuration)
  {
    if (patient_configuration.HasConditions() && !patient_configuration.GetConditions()->IsEmpty())
      Warning("Provided conditions will be ignored, this engine does not need to stabilize");

    return true;// Nothing to do here, we are good to go!
  }

  void Controller::InitializeModels()
  {
    Info("Initializing Models");
    m_RespiratoryModel->Clear();
    m_MechanicalVentilatorModel->Clear();

    m_RespiratoryModel->Initialize();
    m_MechanicalVentilatorModel->Initialize();
  }

  void Controller::AtSteadyState(pmc::EngineState state)
  {
    m_State = state;

    m_MechanicalVentilatorModel->AtSteadyState();
    m_RespiratoryModel->AtSteadyState();
  }

  void Controller::PreProcess()
  {
    m_MechanicalVentilatorModel->PreProcess();
    m_RespiratoryModel->PreProcess();
  }

  void Controller::Process()
  {
    m_MechanicalVentilatorModel->Process();
    m_RespiratoryModel->Process();
  }

  void Controller::PostProcess()
  {
    m_MechanicalVentilatorModel->PostProcess();
    m_RespiratoryModel->PostProcess();
  }
}
