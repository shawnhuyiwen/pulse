/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "PulseConfiguration.h"
#include "engine/human_adult/whole_body/controller/Controller.h"
#include "engine/human_adult/whole_body/controller/BlackBoxManager.h"
#include "engine/human_adult/whole_body/controller/CircuitManager.h"
#include "engine/human_adult/whole_body/controller/CompartmentManager.h"
#include "engine/human_adult/whole_body/controller/SubstanceManager.h"
#include "engine/human_adult/whole_body/system/environment/EnvironmentModel.h"
#include "engine/human_adult/whole_body/system/equipment/AnesthesiaMachineModel.h"
#include "engine/human_adult/whole_body/system/equipment/BagValveMaskModel.h"
#include "engine/human_adult/whole_body/system/equipment/ElectroCardioGramModel.h"
#include "engine/human_adult/whole_body/system/equipment/InhalerModel.h"
#include "engine/human_adult/whole_body/system/equipment/MechanicalVentilatorModel.h"
#include "engine/human_adult/whole_body/system/physiology/BloodChemistryModel.h"
#include "engine/human_adult/whole_body/system/physiology/CardiovascularModel.h"
#include "engine/human_adult/whole_body/system/physiology/DrugModel.h"
#include "engine/human_adult/whole_body/system/physiology/EndocrineModel.h"
#include "engine/human_adult/whole_body/system/physiology/EnergyModel.h"
#include "engine/human_adult/whole_body/system/physiology/GastrointestinalModel.h"
#include "engine/human_adult/whole_body/system/physiology/HepaticModel.h"
#include "engine/human_adult/whole_body/system/physiology/NervousModel.h"
#include "engine/human_adult/whole_body/system/physiology/RenalModel.h"
#include "engine/human_adult/whole_body/system/physiology/RespiratoryModel.h"
#include "engine/human_adult/whole_body/system/physiology/Saturation.h"
#include "engine/human_adult/whole_body/system/physiology/TissueModel.h"

#include "cdm/engine/SEActionManager.h"
#include "cdm/engine/SEConditionManager.h"
#include "cdm/patient/SEPatient.h"
#include "cdm/patient/assessments/SEArterialBloodGasTest.h"
#include "cdm/patient/assessments/SECompleteBloodCount.h"
#include "cdm/patient/assessments/SEComprehensiveMetabolicPanel.h"
#include "cdm/patient/assessments/SEPulmonaryFunctionTest.h"
#include "cdm/patient/assessments/SEUrinalysis.h"

namespace pmc = PULSE_ENGINE;

namespace HUMAN_ADULT_WHOLE_BODY
{
  Controller::Controller(Logger* logger) : pmc::Controller(logger)
  {

  }

  // I am pretty sure we will want different states per engine
  bool Controller::SerializeFromFile(const std::string& filename)
  {
    return pmc::Controller::SerializeFromFile(filename);
  }
  bool Controller::SerializeToFile(const std::string& filename) const
  {
    return pmc::Controller::SerializeToFile(filename);
  }

  bool Controller::SerializeFromString(const std::string& src, eSerializationFormat m)
  {
    return pmc::Controller::SerializeFromString(src, m);
  }
  bool Controller::SerializeToString(std::string& output, eSerializationFormat m) const
  {
    return pmc::Controller::SerializeToString(output, m);
  }

  void Controller::Allocate()
  {
    m_Stabilizer = new pmc::StabilizationController(*this);

    m_Substances = new SubstanceManager(*this);

    m_InitialPatient = new SEPatient(GetLogger());
    m_CurrentPatient = new SEPatient(GetLogger());

    m_Config = new pmc::PulseConfiguration(GetLogger());
    m_Config->Initialize("");//Setup defaults that don't need files on disk

    m_SaturationCalculator = new pmc::SaturationCalculator(*this);

    m_Actions = new SEActionManager(*m_Substances);
    m_Conditions = new SEConditionManager(GetLogger());

    m_BloodChemistryModel = new BloodChemistryModel(*this);
    m_CardiovascularModel = new CardiovascularModel(*this);
    m_EndocrineModel = new EndocrineModel(*this);
    m_EnergyModel = new EnergyModel(*this);
    m_GastrointestinalModel = new GastrointestinalModel(*this);
    m_HepaticModel = new HepaticModel(*this);
    m_NervousModel = new NervousModel(*this);
    m_RenalModel = new RenalModel(*this);
    m_RespiratoryModel = new RespiratoryModel(*this);
    m_DrugModel = new DrugModel(*this);
    m_TissueModel = new TissueModel(*this);

    m_EnvironmentModel = new EnvironmentModel(*this);

    m_AnesthesiaMachineModel = new AnesthesiaMachineModel(*this);
    m_BagValveMaskModel = new BagValveMaskModel(*this);
    m_ElectroCardioGramModel = new ElectroCardioGramModel(*this);
    m_InhalerModel = new InhalerModel(*this);
    m_MechanicalVentilatorModel = new MechanicalVentilatorModel(*this);

    m_EventManager = new SEEventManager(GetLogger());

    m_Compartments = new CompartmentManager(*this);
    m_BlackBoxes = new BlackBoxManager(*this);

    m_Circuits = new CircuitManager(*this);

    m_LogForward = new pmc::FatalListner(*m_EventManager, m_CurrentTime);
    m_Logger->AddForward(m_LogForward);

    SetupTracker();
  }

  void Controller::InitializeModels()
  {
    // Environment is cleared and initialized in circuit setup

    // Clear everything before initializing
    m_CardiovascularModel->Clear();
    m_RespiratoryModel->Clear();
    m_AnesthesiaMachineModel->Clear();
    m_BagValveMaskModel->Clear();
    m_MechanicalVentilatorModel->Clear();
    m_GastrointestinalModel->Clear();
    m_HepaticModel->Clear();
    m_RenalModel->Clear();
    m_NervousModel->Clear();
    m_EndocrineModel->Clear();
    m_DrugModel->Clear();
    m_EnergyModel->Clear();
    m_BloodChemistryModel->Clear();
    m_TissueModel->Clear();
    m_ElectroCardioGramModel->Clear();
    m_InhalerModel->Clear();

    Info("Initializing Models");
    m_CardiovascularModel->Initialize();
    m_RespiratoryModel->Initialize();
    m_AnesthesiaMachineModel->Initialize();
    m_BagValveMaskModel->Initialize();
    m_MechanicalVentilatorModel->Initialize();
    m_GastrointestinalModel->Initialize();
    m_HepaticModel->Initialize();
    m_RenalModel->Initialize();
    m_NervousModel->Initialize();
    m_EndocrineModel->Initialize();
    m_DrugModel->Initialize();
    m_EnergyModel->Initialize();
    m_BloodChemistryModel->Initialize();
    m_TissueModel->Initialize(); // Depends on some parameters that Blood Chemistry initializes,needs to be after
    m_ElectroCardioGramModel->Initialize();
    m_InhalerModel->Initialize();
  }

  void Controller::AtSteadyState(pmc::EngineState state)
  {
    m_State = state;
    m_EnvironmentModel->AtSteadyState();
    m_CardiovascularModel->AtSteadyState();
    m_InhalerModel->AtSteadyState();
    m_RespiratoryModel->AtSteadyState();
    m_AnesthesiaMachineModel->AtSteadyState();
    m_BagValveMaskModel->AtSteadyState();
    m_MechanicalVentilatorModel->AtSteadyState();
    m_GastrointestinalModel->AtSteadyState();
    m_HepaticModel->AtSteadyState();
    m_RenalModel->AtSteadyState();
    m_NervousModel->AtSteadyState();
    m_EnergyModel->AtSteadyState();
    m_EndocrineModel->AtSteadyState();
    m_DrugModel->AtSteadyState();
    m_TissueModel->AtSteadyState();
    m_BloodChemistryModel->AtSteadyState();
    m_ElectroCardioGramModel->AtSteadyState();
  }

  void Controller::PreProcess()
  {
    m_EnvironmentModel->PreProcess();
    m_CardiovascularModel->PreProcess();
    m_InhalerModel->PreProcess();
    m_RespiratoryModel->PreProcess();
    m_AnesthesiaMachineModel->PreProcess();
    m_BagValveMaskModel->PreProcess();
    m_MechanicalVentilatorModel->PreProcess();
    m_GastrointestinalModel->PreProcess();
    m_HepaticModel->PreProcess();
    m_RenalModel->PreProcess();
    m_NervousModel->PreProcess();
    m_EnergyModel->PreProcess();
    m_EndocrineModel->PreProcess();
    m_DrugModel->PreProcess();
    m_TissueModel->PreProcess();
    m_BloodChemistryModel->PreProcess();
    m_ElectroCardioGramModel->PreProcess();
  }
  void Controller::Process()
  {
    m_EnvironmentModel->Process();
    m_CardiovascularModel->Process();
    m_InhalerModel->Process();
    m_RespiratoryModel->Process();
    m_AnesthesiaMachineModel->Process();
    m_BagValveMaskModel->Process();
    m_MechanicalVentilatorModel->Process();
    m_GastrointestinalModel->Process();
    m_HepaticModel->Process();
    m_RenalModel->Process();
    m_NervousModel->Process();
    m_EnergyModel->Process();
    m_EndocrineModel->Process();
    m_DrugModel->Process();
    m_TissueModel->Process();
    m_BloodChemistryModel->Process();
    m_ElectroCardioGramModel->Process();
  }
  void Controller::PostProcess()
  {
    m_EnvironmentModel->PostProcess();
    m_CardiovascularModel->PostProcess();
    m_InhalerModel->PostProcess();
    m_RespiratoryModel->PostProcess();
    m_AnesthesiaMachineModel->PostProcess();
    m_BagValveMaskModel->PostProcess();
    m_MechanicalVentilatorModel->PostProcess();
    m_GastrointestinalModel->PostProcess();
    m_HepaticModel->PostProcess();
    m_RenalModel->PostProcess();
    m_NervousModel->PostProcess();
    m_EnergyModel->PostProcess();
    m_EndocrineModel->PostProcess();
    m_DrugModel->PostProcess();
    m_TissueModel->PostProcess();
    m_BloodChemistryModel->PostProcess();
    m_ElectroCardioGramModel->PostProcess();
  }

  bool Controller::GetPatientAssessment(SEPatientAssessment& assessment) const
  {
    if (!IsReady())
      return false;

    SEArterialBloodGasTest* abg = dynamic_cast<SEArterialBloodGasTest*>(&assessment);
    if (abg != nullptr)
      return m_BloodChemistryModel->CalculateArterialBloodGasTest(*abg);

    SECompleteBloodCount* cbc = dynamic_cast<SECompleteBloodCount*>(&assessment);
    if (cbc != nullptr)
      return m_BloodChemistryModel->CalculateCompleteBloodCount(*cbc);

    SEComprehensiveMetabolicPanel* cmp = dynamic_cast<SEComprehensiveMetabolicPanel*>(&assessment);
    if (cmp != nullptr)
      return m_BloodChemistryModel->CalculateComprehensiveMetabolicPanel(*cmp);

    SEPulmonaryFunctionTest* pft = dynamic_cast<SEPulmonaryFunctionTest*>(&assessment);
    if (pft != nullptr)
      return m_RespiratoryModel->CalculatePulmonaryFunctionTest(*pft);

    SEUrinalysis* u = dynamic_cast<SEUrinalysis*>(&assessment);
    if (u != nullptr)
    {
      return m_RenalModel->CalculateUrinalysis(*u);
    }

    Error("Unsupported patient assessment");
    return false;
  }
}
