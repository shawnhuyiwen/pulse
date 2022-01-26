/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "engine/common/controller/Controller.h"
#include "engine/common/system/environment/EnvironmentModel.h"
#include "engine/common/system/equipment/AnesthesiaMachineModel.h"
#include "engine/common/system/equipment/BagValveMaskModel.h"
#include "engine/common/system/equipment/ElectroCardioGramModel.h"
#include "engine/common/system/equipment/InhalerModel.h"
#include "engine/common/system/equipment/MechanicalVentilatorModel.h"
#include "engine/common/system/physiology/BloodChemistryModel.h"
#include "engine/common/system/physiology/CardiovascularModel.h"
#include "engine/common/system/physiology/DrugModel.h"
#include "engine/common/system/physiology/EndocrineModel.h"
#include "engine/common/system/physiology/EnergyModel.h"
#include "engine/common/system/physiology/GastrointestinalModel.h"
#include "engine/common/system/physiology/HepaticModel.h"
#include "engine/common/system/physiology/NervousModel.h"
#include "engine/common/system/physiology/RenalModel.h"
#include "engine/common/system/physiology/RespiratoryModel.h"
#include "engine/common/system/physiology/Saturation.h"
#include "engine/common/system/physiology/TissueModel.h"
#include "engine/io/protobuf/PBState.h"

#include "cdm/engine/SEPatientActionCollection.h"
#include "cdm/engine/SEPatientConfiguration.h"
#include "cdm/engine/SEConditionManager.h"
#include "cdm/engine/SEActionManager.h"
#include "cdm/engine/SEEngineTracker.h"
#include "cdm/engine/SEDataRequestManager.h"
#include "cdm/engine/SEAdvanceTime.h"
#include "cdm/engine/SESerializeState.h"
#include "cdm/engine/SEOverrides.h"
#include "cdm/engine/SEEventManager.h"
#include "cdm/engine/SEAdvanceHandler.h"
#include "cdm/engine/SEEngineStabilization.h"
#include "cdm/patient/SEPatient.h"
#include "cdm/patient/actions/SEIntubation.h"
#include "cdm/patient/actions/SEPatientAssessmentRequest.h"
#include "cdm/patient/assessments/SEArterialBloodGasTest.h"
#include "cdm/patient/assessments/SECompleteBloodCount.h"
#include "cdm/patient/assessments/SEComprehensiveMetabolicPanel.h"
#include "cdm/patient/assessments/SEPulmonaryFunctionTest.h"
#include "cdm/patient/assessments/SEUrinalysis.h"
#include "cdm/utils/FileUtils.h"

namespace pulse
{
  Data::Data(Logger* logger) : Loggable(logger)
  {
    m_State = EngineState::NotReady;
    m_AirwayMode = eAirwayMode::Free;
    m_Intubation = eSwitch::Off;
    m_EngineTrack = nullptr;
    m_AdvanceHandler = nullptr;

    m_CurrentTime.SetValue(0, TimeUnit::s);
    m_SimulationTime.SetValue(0, TimeUnit::s);
    m_SpareAdvanceTime_s = 0;

    m_Logger->SetLogTime(&m_SimulationTime);
  }
  Data::~Data()
  {
    SAFE_DELETE(m_EngineTrack);
  }

  void Data::AdvanceCallback(double time_s)
  {
    if (m_AdvanceHandler)
    {
      if (time_s >= 0 || m_AdvanceHandler->OnForStabilization())
        m_AdvanceHandler->OnAdvance(time_s);
    }
  }

  void Data::SetAirwayMode(eAirwayMode mode)
  {
    if (mode == m_AirwayMode)
      return;// do nazing!
    if (mode != eAirwayMode::Free && m_AirwayMode != eAirwayMode::Free)
      throw CommonDataModelException("Can only change airway mode from the Free mode, Disable other equipment first.");
    if (!IsAirwayModeSupported(mode))
      throw CommonDataModelException("Unsupported airwaymode : " + eAirwayMode_Name(mode));
    if (m_Intubation == eSwitch::On)
    {
      if(m_AirwayMode == eAirwayMode::Inhaler)
        throw CommonDataModelException("Cannot apply inhaler if intubated.");
    }
    m_Compartments->UpdateAirwayGraph();
    m_AirwayMode = mode;
    std::stringstream ss;
    ss << "Airway Mode : " << eAirwayMode_Name(m_AirwayMode);
    Info(ss);
  }

  void Data::SetupTracker()
  {
    m_EngineTrack = new SEEngineTracker(*m_CurrentPatient, *m_Actions, *m_Substances, *m_Compartments, m_Logger);
    if (m_BloodChemistryModel)
      m_EngineTrack->AddSystem(*m_BloodChemistryModel);
    if (m_CardiovascularModel)
      m_EngineTrack->AddSystem(*m_CardiovascularModel);
    if (m_EndocrineModel)
      m_EngineTrack->AddSystem(*m_EndocrineModel);
    if (m_EnergyModel)
      m_EngineTrack->AddSystem(*m_EnergyModel);
    if (m_GastrointestinalModel)
      m_EngineTrack->AddSystem(*m_GastrointestinalModel);
    if (m_HepaticModel)
      m_EngineTrack->AddSystem(*m_HepaticModel);
    if (m_NervousModel)
      m_EngineTrack->AddSystem(*m_NervousModel);
    if (m_RenalModel)
      m_EngineTrack->AddSystem(*m_RenalModel);
    if (m_RespiratoryModel)
      m_EngineTrack->AddSystem(*m_RespiratoryModel);
    if (m_DrugModel)
      m_EngineTrack->AddSystem(*m_DrugModel);
    if (m_TissueModel)
      m_EngineTrack->AddSystem(*m_TissueModel);
    if (m_EnvironmentModel)
      m_EngineTrack->AddSystem(*m_EnvironmentModel);
    if (m_AnesthesiaMachineModel)
      m_EngineTrack->AddSystem(*m_AnesthesiaMachineModel);
    if (m_BagValveMaskModel)
      m_EngineTrack->AddSystem(*m_BagValveMaskModel);
    if (m_ElectroCardioGramModel)
      m_EngineTrack->AddSystem(*m_ElectroCardioGramModel);
    if (m_InhalerModel)
      m_EngineTrack->AddSystem(*m_InhalerModel);
    if (m_MechanicalVentilatorModel)
      m_EngineTrack->AddSystem(*m_MechanicalVentilatorModel);
  }

  SEEngineTracker& Data::GetEngineTracker() const { return *m_EngineTrack; }
  DataTrack& Data::GetDataTrack() const { return m_EngineTrack->GetDataTrack(); }
  SaturationCalculator& Data::GetSaturationCalculator() const { return *m_SaturationCalculator; }

  SubstanceManager& Data::GetSubstances() const { return *m_Substances; }

  const SEPatient& Data::GetInitialPatient() const { return *m_InitialPatient; }
  SEPatient& Data::GetCurrentPatient() const { return *m_CurrentPatient; }

  bool Data::HasBloodChemistry() const { return m_BloodChemistryModel != nullptr; }
  SEBloodChemistrySystem& Data::GetBloodChemistry() const { return *m_BloodChemistryModel; }
  bool Data::HasCardiovascular() const { return m_CardiovascularModel != nullptr; }
  SECardiovascularSystem& Data::GetCardiovascular() const { return *m_CardiovascularModel; }
  bool Data::HasDrugs() const { return m_DrugModel != nullptr; }
  SEDrugSystem& Data::GetDrugs() const { return *m_DrugModel; }
  bool Data::HasEndocrine() const { return m_EndocrineModel != nullptr; }
  SEEndocrineSystem& Data::GetEndocrine() const { return *m_EndocrineModel; }
  bool Data::HasEnergy() const { return m_EnergyModel != nullptr; }
  SEEnergySystem& Data::GetEnergy() const { return *m_EnergyModel; }
  bool Data::HasGastrointestinal() const { return m_GastrointestinalModel != nullptr; }
  SEGastrointestinalSystem& Data::GetGastrointestinal() const { return *m_GastrointestinalModel; }
  bool Data::HasHepatic() const { return m_HepaticModel != nullptr; }
  SEHepaticSystem& Data::GetHepatic() const { return *m_HepaticModel; }
  bool Data::HasNervous() const { return m_NervousModel != nullptr; }
  SENervousSystem& Data::GetNervous() const { return *m_NervousModel; }
  bool Data::HasRenal() const { return m_RenalModel != nullptr; }
  SERenalSystem& Data::GetRenal() const { return *m_RenalModel; }
  bool Data::HasRespiratory() const { return m_RespiratoryModel != nullptr; }
  SERespiratorySystem& Data::GetRespiratory() const { return *m_RespiratoryModel; }
  bool Data::HasTissue() const { return m_TissueModel != nullptr; }
  SETissueSystem& Data::GetTissue() const { return *m_TissueModel; }

  bool Data::HasEnvironment() const { return m_EnvironmentModel != nullptr; }
  SEEnvironment& Data::GetEnvironment() const { return *m_EnvironmentModel; }
  bool Data::HasAnesthesiaMachine() const { return m_AnesthesiaMachineModel != nullptr; }
  SEAnesthesiaMachine& Data::GetAnesthesiaMachine() const { return *m_AnesthesiaMachineModel; }
  bool Data::HasBagValveMask() const { return m_BagValveMaskModel != nullptr; }
  SEBagValveMask& Data::GetBagValveMask() const { return *m_BagValveMaskModel; }
  bool Data::HasECG() const { return m_ElectroCardioGramModel != nullptr; }
  SEElectroCardioGram& Data::GetECG() const { return *m_ElectroCardioGramModel; }
  bool Data::HasInhaler() const { return m_InhalerModel != nullptr; }
  SEInhaler& Data::GetInhaler() const { return *m_InhalerModel; }
  bool Data::HasMechanicalVentilator() const { return m_MechanicalVentilatorModel != nullptr; }
  SEMechanicalVentilator& Data::GetMechanicalVentilator() const { return *m_MechanicalVentilatorModel; }

  SEActionManager& Data::GetActions() const { return *m_Actions; }

  SEConditionManager& Data::GetConditions() const { return *m_Conditions; }

  SEEventManager& Data::GetEvents() const { return *m_EventManager; }

  CircuitManager& Data::GetCircuits() const { return *m_Circuits; }

  CompartmentManager& Data::GetCompartments() const { return *m_Compartments; }

  BlackBoxManager& Data::GetBlackBoxes() const { return *m_BlackBoxes; }

  const PulseConfiguration& Data::GetConfiguration() const { return *m_Config; }

  const SEScalarTime& Data::GetEngineTime() const { return m_CurrentTime; }
  const SEScalarTime& Data::GetSimulationTime() const { return m_SimulationTime; }
  const SEScalarTime& Data::GetTimeStep() const { return m_Config->GetTimeStep(); }
  double Data::GetTimeStep_s() const { return GetTimeStep().GetValue(TimeUnit::s); }

  bool Data::HasOverride() const { return m_ScalarOverrides.size() > 0; }
  const std::vector<SEScalarProperty>& Data::GetOverrides() const { return m_ScalarOverrides; }

  Controller::Controller(Logger* logger) : Data(logger)
  {
    m_ConfigOverride = nullptr;
    m_Logger->LogToConsole(true);
  }
  Controller::~Controller()
  {
    m_Logger->RemoveForward(m_LogForward);
    SAFE_DELETE(m_LogForward);

    SAFE_DELETE(m_Stabilizer);

    SAFE_DELETE(m_Substances);

    SAFE_DELETE(m_InitialPatient);
    SAFE_DELETE(m_CurrentPatient);

    SAFE_DELETE(m_Config);

    SAFE_DELETE(m_SaturationCalculator);

    SAFE_DELETE(m_Actions);
    SAFE_DELETE(m_Conditions);

    SAFE_DELETE(m_BloodChemistryModel);
    SAFE_DELETE(m_CardiovascularModel);
    SAFE_DELETE(m_EndocrineModel);
    SAFE_DELETE(m_EnergyModel);
    SAFE_DELETE(m_GastrointestinalModel);
    SAFE_DELETE(m_HepaticModel);
    SAFE_DELETE(m_NervousModel);
    SAFE_DELETE(m_RenalModel);
    SAFE_DELETE(m_RespiratoryModel);
    SAFE_DELETE(m_DrugModel);
    SAFE_DELETE(m_TissueModel);

    SAFE_DELETE(m_EnvironmentModel);

    SAFE_DELETE(m_AnesthesiaMachineModel);
    SAFE_DELETE(m_BagValveMaskModel);
    SAFE_DELETE(m_ElectroCardioGramModel);
    SAFE_DELETE(m_InhalerModel);
    SAFE_DELETE(m_MechanicalVentilatorModel);

    SAFE_DELETE(m_EventManager);

    SAFE_DELETE(m_Compartments);
    SAFE_DELETE(m_BlackBoxes);

    SAFE_DELETE(m_Circuits);
  }

  bool Controller::SetConfigurationOverride(const SEEngineConfiguration* config)
  {
    if (config != nullptr)
    {
      m_ConfigOverride = dynamic_cast<const PulseConfiguration*>(config);
      if (m_ConfigOverride == nullptr)
      {
        Error("Configuration provided is not a Pulse Configuration Object");
        return false;
      }
    }
    else
      m_ConfigOverride = nullptr;
    return true;
  }

  bool Controller::SerializeFromFile(const std::string& filename)
  {
    return PBState::SerializeFromFile(filename, *this, m_ConfigOverride);
  }
  bool Controller::SerializeToFile(const std::string& filename) const
  {
    return PBState::SerializeToFile(*this, filename);
  }

  bool Controller::SerializeFromString(const std::string& src, eSerializationFormat m)
  {
    return PBState::SerializeFromString(src, *this, m);
  }
  bool Controller::SerializeToString(std::string& output, eSerializationFormat m) const
  {
    return PBState::SerializeToString(*this, output, m);
  }

  bool Controller::InitializeEngine(const std::string& patient_configuration, eSerializationFormat m)
  {
    SEPatientConfiguration pc(GetLogger());
    pc.SerializeFromString(patient_configuration, m, *m_Substances);
    return InitializeEngine(pc);
  }

  bool Controller::InitializeEngine(const SEPatientConfiguration& patient_configuration)
  {
    m_State = EngineState::NotReady;

    m_SpareAdvanceTime_s = 0;
    m_AirwayMode = eAirwayMode::Free;
    m_Intubation = eSwitch::Off;
    m_CurrentTime.SetValue(0, TimeUnit::s);
    m_SimulationTime.SetValue(0, TimeUnit::s);
    m_Logger->SetLogTime(&m_SimulationTime);

    Info("Looking for files in " + patient_configuration.GetDataRoot());
    m_DataDir = patient_configuration.GetDataRoot();

    m_EngineTrack->ResetFile();
    m_Config->Initialize();
    m_ScalarOverrides.clear();
    m_State = EngineState::Initialization;
    if (patient_configuration.HasOverride())
    {
      for (auto& so : patient_configuration.GetScalarOverrides())
        m_ScalarOverrides.push_back(so);
    }
    if (patient_configuration.HasPatient())
    {
      if (!Initialize(*patient_configuration.GetPatient()))
        return false;
    }
    else if (patient_configuration.HasPatientFile())
    {
      SEPatient patient(m_Logger);
      std::string pFile = patient_configuration.GetPatientFile();
      if (!FileExists(pFile))
      {
        if (FileExists(m_DataDir + pFile))
        {
          pFile = m_DataDir + pFile;
        }
        else if (FileExists(m_DataDir + "/patients/" + pFile))
        {
          pFile = m_DataDir + "/patients/" + pFile;
        }
      }
      if (!patient.SerializeFromFile(pFile))// TODO Support all serialization formats
        return false;
      if (!Initialize(patient))
        return false;
    }
    else
      return false;
    InitializeModels();
    AdvanceCallback(-1);

    // We don't capture events during initialization
    SEEventHandler* event_handler = m_EventManager->GetEventHandler();
    m_EventManager->ForwardEvents(nullptr);

    if (!Stabilize(patient_configuration))
    {
      Error("Pulse needs stabilization criteria, none provided in configuration file");
      return false;
    }

    // Don't allow any changes to Quantity/Potential/Flux values directly
    // Use Quantity/Potential/Flux Sources
    m_Circuits->SetReadOnly(true);

    AtSteadyState(EngineState::Active);
    if (!m_Config->GetStabilization()->IsTrackingStabilization())
      m_SimulationTime.SetValue(0, TimeUnit::s);
    // Hook up the handlers (Note events will still be in the log)
    m_EventManager->ForwardEvents(event_handler);

    return true;
  }

  bool Controller::Initialize(SEPatient const& patient)
  {
    Info("Configuring patient");
    if (!SetupPatient(patient))
      return false;

    Info("Resetting Substances");
    m_Substances->LoadSubstanceDirectory(m_DataDir);

    // Clear all substances and reload the original data
    // This clears out all engine specific data stored in the substance
    // This will preserve the pointer to the substance, but not any pointers
    // to any substance child objects, those will need to be fixed up, if they exist

    Info("Initializing Configuration");
    m_Config->Initialize(m_DataDir, m_Substances); // Reset to Defaults

    // Now, Let's see if there is anything to merge into our base configuration
    Info("Merging OnDisk Configuration");
    PulseConfiguration cFile(GetLogger());
    cFile.SerializeFromFile("Configuration.json", *m_Substances);
    m_Config->Merge(cFile, *m_Substances);

    // Now, override anything with a configuration provided by the user or scenario
    if (m_ConfigOverride != nullptr)
    {
      Info("Merging Provided Configuration");
      m_Config->Merge(*m_ConfigOverride, *m_Substances);
    }

    if (!m_Config->IsPDEnabled())
      Info("PD IS DISABLED!!!!");

    // Now we can check the config
    if (m_Config->IsWritingPatientBaselineFile())
    {
      std::string stableDir = m_DataDir + "/stable/";
      MakeDirectory(stableDir.c_str());
      m_CurrentPatient->SerializeToFile(stableDir + m_CurrentPatient->GetName() + ".json");
    }

    m_Actions->Clear();
    m_Conditions->Clear();
    m_EventManager->Clear();

    // This will also Initialize the environment
    // Due to needing the initial environment values for circuits to construct properly
    Info("Creating Circuits and Compartments");
    CreateCircuitsAndCompartments();

    // Appy any prestabilization overrides
    if (m_Config->HasInitialOverrides())
    {
      ProcessAction(m_Config->GetInitialOverrides());
      OverrideCircuits();// Override any circuit values
    }

    m_Substances->InitializeSubstances();
    if (m_RespiratoryModel && m_EnvironmentModel)
    {
      Info("Initializing Gas Substances");
      m_Substances->InitializeGasCompartments();
    }
    if (m_SaturationCalculator && m_CardiovascularModel)
    {
      Info("Initializing Liquid Substances");
      m_SaturationCalculator->Setup();
      m_Substances->InitializeLiquidCompartmentGases();
      m_Substances->InitializeLiquidCompartmentNonGases();
    }
    return true;
  }

  bool Controller::Stabilize(const SEPatientConfiguration& patient_configuration)
  {
    // Stabilize the engine to a resting state (with a standard meal and environment)
    if (!m_Config->HasStabilization())
    {
      Error("Pulse needs stabilization criteria, none provided in configuration file");
      return false;
    }

    m_State = EngineState::InitialStabilization;
    if (!m_Config->GetStabilization()->StabilizeRestingState(*m_Stabilizer))
      return false;

    // Copy any changes to the current patient to the initial patient
    m_InitialPatient->Copy(*m_CurrentPatient);

    // We need to copy conditions here, so models can prepare for them in their AtSteadyState method
    if (patient_configuration.HasConditions())
      m_Conditions->Copy(*patient_configuration.GetConditions(), *m_Substances);
    AtSteadyState(EngineState::AtInitialStableState);// This will peek at conditions

    m_State = EngineState::SecondaryStabilization;
    // Apply conditions and anything else to the physiology
    // now that it's steady with provided patient, environment, and feedback
    if (!m_Conditions->IsEmpty())
    {// Now restabilize the patient with any conditions that were applied
     // Push conditions into condition manager
      if (!m_Config->GetStabilization()->StabilizeConditions(*m_Stabilizer, *m_Conditions))
        return false;
    }
    AtSteadyState(EngineState::AtSecondaryStableState);
    return true;
  }

  void Controller::SetSimulationTime(const SEScalarTime& time)
  {
    m_SimulationTime.Set(time);
  }

  bool Controller::IsReady() const
  {
    if (m_State == EngineState::Fatal)
      return false;
    if (m_State == EngineState::NotReady)
    {
      Error("Engine is not ready to process, Initialize the engine or Load a state.");
      return false;
    }
    return true;
  }

  void Controller::CheckIntubation()
  {
    if (m_Actions->GetPatientActions().HasIntubation())
    {
      if (m_Intubation == eSwitch::Off)
      {
        Info("Intubating Patient");
        m_Intubation = eSwitch::On;
      }
      return;
    }
    if (m_Intubation == eSwitch::On)
    {
      Info("Extubating Patient");
      m_Intubation = eSwitch::Off;
    }
  }

  bool Controller::AdvanceModelTime()
  {
    if (!IsReady())
      return false;

    CheckIntubation();
    PreProcess();
    Process();
    PostProcess();

    if (m_EventManager->IsEventActive(eEvent::IrreversibleState))
    {
      m_State = EngineState::Fatal;
      return false;
    }

    m_EventManager->UpdateEvents(m_Config->GetTimeStep());
    m_CurrentTime.Increment(m_Config->GetTimeStep());
    m_SimulationTime.Increment(m_Config->GetTimeStep());

    if (m_AdvanceHandler)
      m_AdvanceHandler->OnAdvance(m_CurrentTime.GetValue(TimeUnit::s));

    // TODO Figure out a way to track what overrides were used and which were not
    m_ScalarOverrides.clear();

    return true;
  }

  bool Controller::AdvanceModelTime(double time, const TimeUnit& unit)
  {
    if (m_Config->AllowDynamicTimeStep() == eSwitch::On)
    {
      m_Config->GetTimeStep().SetValue(time, unit);
      return AdvanceModelTime();
    }
    else
    {
      double time_s = Convert(time, unit, TimeUnit::s) + m_SpareAdvanceTime_s;
      int count = (int)(time_s / GetTimeStep_s());
      for (int i = 0; i < count; i++)
        if (!AdvanceModelTime())
          return false;
      m_SpareAdvanceTime_s = time_s - (count * GetTimeStep_s());
    }
    return true;
  }

  bool Controller::ProcessAction(const SEAction& action)
  {
    // We can setup overrides before the engine is ready
    const SEOverrides* overrides = dynamic_cast<const SEOverrides*>(&action);
    if (overrides != nullptr)
    {
      for (auto& so : overrides->GetScalarProperties())
      {
        m_ScalarOverrides.push_back(so);
        Info("Overriding " + so.name + " with " + cdm::to_string(so.value) + " " + so.unit);
      }
      return true;
    }

    if (!IsReady())
      return false;
    m_ss << "[Action] " << m_SimulationTime << ", " << action;
    Info(m_ss);

    const SEAdvanceTime* adv = dynamic_cast<const SEAdvanceTime*>(&action);
    if (adv != nullptr)
      return AdvanceModelTime(adv->GetTime(TimeUnit::s), TimeUnit::s);

    const SESerializeState* serialize = dynamic_cast<const SESerializeState*>(&action);
    if (serialize != nullptr)
    {
      if (serialize->GetType() == eSerialization_Type::Save)
      {
        if (serialize->HasFilename())
        {
          SerializeToFile(serialize->GetFilename());
        }
        else
        {
          std::stringstream ss;
          MakeDirectory("./states");
          ss << "./states/" << m_InitialPatient->GetName() << "@" << GetSimulationTime().GetValue(TimeUnit::s) << "s.pbb";
          Info("Saving " + ss.str());
          SerializeToFile(ss.str());
          ss.str(""); ss.clear();
          ss << "./states/" << m_InitialPatient->GetName() << "@" << GetSimulationTime().GetValue(TimeUnit::s) << "s.json";
          Info("Saving " + ss.str());
          SerializeToFile(ss.str());
          // Debug code to make sure things are consistent
          //SerializeFomFile(ss.str());
          //SerializeToFile("./states/AfterSave.json);
        }
      }
      else
        return SerializeFromFile(serialize->GetFilename());
      return true;
    }

    const SEPatientAssessmentRequest* patientAss = dynamic_cast<const SEPatientAssessmentRequest*>(&action);
    if (patientAss != nullptr)
    {
      switch (patientAss->GetType())
      {
      case ePatientAssessment_Type::ArterialBloodGasTest:
      {
        SEArterialBloodGasTest abg(m_Logger);
        if (GetPatientAssessment(abg))
        {
          std::string abgFile = GetEngineTracker().GetDataRequestManager().GetResultFilename();
          if (abgFile.empty())
            abgFile = "ArterialBloodGasTest";
          m_ss << "ABG@" << GetSimulationTime().GetValue(TimeUnit::s) << "s";
          abgFile = Replace(abgFile, "Results", m_ss.str());
          abgFile = Replace(abgFile, ".csv", ".json");
          m_ss << "ArterialBloodGasTest@" << GetSimulationTime().GetValue(TimeUnit::s) << "s.json";
          abg.SerializeToFile(abgFile);
        }
        break;
      }

      case ePatientAssessment_Type::CompleteBloodCount:
      {
        SECompleteBloodCount cbc(m_Logger);
        if (GetPatientAssessment(cbc))
        {
          std::string cbcFile = GetEngineTracker().GetDataRequestManager().GetResultFilename();
          if (cbcFile.empty())
            cbcFile = "CompleteBloodCount";
          m_ss << "CBC@" << GetSimulationTime().GetValue(TimeUnit::s) << "s";
          cbcFile = Replace(cbcFile, "Results", m_ss.str());
          cbcFile = Replace(cbcFile, ".csv", ".json");
          m_ss << "CompleteBloodCount@" << GetSimulationTime().GetValue(TimeUnit::s) << "s.json";
          cbc.SerializeToFile(cbcFile);
        }
        break;
      }

      case ePatientAssessment_Type::ComprehensiveMetabolicPanel:
      {
        SEComprehensiveMetabolicPanel mp(m_Logger);
        if (GetPatientAssessment(mp))
        {
          std::string mpFile = GetEngineTracker().GetDataRequestManager().GetResultFilename();
          if (mpFile.empty())
            mpFile = "ComprehensiveMetabolicPanel";
          m_ss << "CMP@" << GetSimulationTime().GetValue(TimeUnit::s) << "s";
          mpFile = Replace(mpFile, "Results", m_ss.str());
          mpFile = Replace(mpFile, ".csv", ".json");
          m_ss << "ComprehensiveMetabolicPanel@" << GetSimulationTime().GetValue(TimeUnit::s) << "s.json";
          mp.SerializeToFile(mpFile);
        }
        break;
      }

      case ePatientAssessment_Type::PulmonaryFunctionTest:
      {
        SEPulmonaryFunctionTest pft(m_Logger);
        if (GetPatientAssessment(pft))
        {
          // Write out the Assessement
          std::string pftFile = GetEngineTracker().GetDataRequestManager().GetResultFilename();
          if (pftFile.empty())
            pftFile = "PulmonaryFunctionTest";
          m_ss << "PFT@" << GetSimulationTime().GetValue(TimeUnit::s) << "s";
          pftFile = Replace(pftFile, "Results", m_ss.str());
          pftFile = Replace(pftFile, ".csv", ".json");
          m_ss << "PulmonaryFunctionTest@" << GetSimulationTime().GetValue(TimeUnit::s) << "s.json";
          pft.SerializeToFile(pftFile);
        }
        break;
      }
      case ePatientAssessment_Type::Urinalysis:
      {
        SEUrinalysis upan(m_Logger);
        if (GetPatientAssessment(upan))
        {
          std::string upanFile = GetEngineTracker().GetDataRequestManager().GetResultFilename();
          if (upanFile.empty())
            upanFile = "Urinalysis";
          m_ss << "Urinalysis@" << GetSimulationTime().GetValue(TimeUnit::s) << "s";
          upanFile = Replace(upanFile, "Results", m_ss.str());
          upanFile = Replace(upanFile, ".csv", ".json");
          m_ss << "Urinalysis@" << GetSimulationTime().GetValue(TimeUnit::s) << "s.json";
          upan.SerializeToFile(upanFile);
        }
        break;
      }

      default:
      {
        m_ss << "Unsupported assessment request " << ePatientAssessment_Type_Name(patientAss->GetType());
        Error(m_ss);
        return false;
      }
      }
      return true;
    }

    return GetActions().ProcessAction(action);
  }
}