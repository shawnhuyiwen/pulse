/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "engine/human_adult/whole_body/test/EngineTest.h"
#include "engine/human_adult/whole_body/Engine.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

#include "cdm/engine/SETimedStabilization.h"
#include "cdm/engine/SEEngineTracker.h"
#include "cdm/engine/SEDataRequestManager.h"
#include "cdm/engine/SEPatientConfiguration.h"
#include "cdm/substance/SESubstance.h"
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/patient/actions/SEConsciousRespiration.h"
#include "cdm/patient/actions/SEForcedExhale.h"
#include "cdm/patient/actions/SEForcedInhale.h"
#include "cdm/patient/actions/SEUseInhaler.h"
#include "cdm/patient/actions/SEForcedPause.h"
#include "cdm/patient/actions/SESubstanceBolus.h"
#include "cdm/system/equipment/anesthesia_machine/SEAnesthesiaMachine.h"
#include "cdm/system/equipment/anesthesia_machine/SEAnesthesiaMachineOxygenBottle.h"
#include "cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineConfiguration.h"
#include "cdm/system/equipment/inhaler/SEInhaler.h"
#include "cdm/system/equipment/inhaler/actions/SEInhalerConfiguration.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarElectricPotential.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarHeatResistanceArea.h"
#include "cdm/properties/SEScalarLengthPerTime.h"
#include "cdm/properties/SEScalarMass.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarTemperature.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/properties/SEFunctionVolumeVsTime.h"
#include "cdm/properties/SEScalarPower.h"
#include "cdm/utils/FileUtils.h"

class HowToTracker
{
private:
  double m_dT_s;  // Cached Engine Time Step
  PhysiologyEngine& m_Engine;
public:
  HowToTracker(PhysiologyEngine& engine) : m_Engine(engine)
  {
    m_dT_s = m_Engine.GetTimeStep(TimeUnit::s);
  }
  ~HowToTracker(){}

  // This class will operate on seconds
  void AdvanceModelTime(double time_s)
  {
    int count = static_cast<int>(time_s / m_dT_s);
    for (int i = 0; i <= count; i++)
    {
      m_Engine.AdvanceModelTime();  // Compute 1 time step

      // Pull Track will pull data from the engine and append it to the file
      m_Engine.GetEngineTracker()->TrackData(m_Engine.GetSimulationTime(TimeUnit::s));
    }
  }
};

namespace hawb = HUMAN_ADULT_WHOLE_BODY;

namespace HUMAN_ADULT_WHOLE_BODY
{
  void EngineTest::InhalerState(PhysiologyEngine* pc, HowToTracker& tracker)
  {
    pc->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("InhalerResults.csv");
    SEPatientConfiguration pconfig;
    pconfig.SetPatientFile("StandardMale.json");
    if (!pc->InitializeEngine(pconfig))
    {
      std::cerr << "Could not load initialize engine, check the error" << std::endl;
      return;
    }

    tracker.AdvanceModelTime(30);

    SEInhalerConfiguration config(pc->GetLogger());
    config.GetConfiguration().SetSubstance(pc->GetSubstanceManager().GetSubstance("Albuterol"));
    config.GetConfiguration().GetMeteredDose().SetValue(90.0, MassUnit::ug);
    config.GetConfiguration().GetNozzleLoss().SetValue(0.040);
    pc->ProcessAction(config);

    SEConsciousRespiration cResp;

    SEForcedExhale& exhale = cResp.AddForcedExhale();
    exhale.GetExpiratoryReserveVolumeFraction().SetValue(1.0);
    exhale.GetExhalePeriod().SetValue(3.0, TimeUnit::s);

    SEForcedInhale& inhale = cResp.AddForcedInhale();
    inhale.GetInspiratoryCapacityFraction().SetValue(1.0);
    inhale.GetInhalePeriod().SetValue(5.0, TimeUnit::s);

    cResp.AddUseInhaler();

    SEForcedPause& pause = cResp.AddForcedPause();
    pause.GetPeriod().SetValue(10, TimeUnit::s);

    pc->ProcessAction(cResp);
    tracker.AdvanceModelTime(5);

    SEScalarTime now;// Make sure to tell the engine that we are at the same time
    // Save and Load the Engine State
    pc->GetLogger()->Info("Serializing");
    std::string state;
    pc->SerializeToString(state, eSerializationFormat::BINARY);
    now.SetValue(pc->GetSimulationTime(TimeUnit::s), TimeUnit::s);
    pc->SerializeFromString(state, eSerializationFormat::BINARY);
    pc->SetSimulationTime(now);

    // Change the results file
    pc->GetLogger()->SetLogFile("InhalerSerialization.log");
    std::remove("InhalerSerializationResults.csv");
    pc->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("InhalerSerializationResults.csv");

    tracker.AdvanceModelTime(145);
  }

  void EngineTest::InjectSuccsState(PhysiologyEngine* pc, HowToTracker& tracker, const SESubstance& succs)
  {
    pc->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("InjectSuccsResults.csv");
    SEPatientConfiguration pconfig;
    pconfig.SetPatientFile("StandardMale.json");
    if (!pc->InitializeEngine(pconfig))
    {
      std::cerr << "Could not load initialize engine, check the error" << std::endl;
      return;
    }

    SESubstanceBolus injection(succs);
    injection.SetAdminRoute(eSubstanceAdministration_Route::Intravenous);
    injection.GetConcentration().SetValue(4820, MassPerVolumeUnit::ug_Per_mL);
    injection.GetDose().SetValue(30, VolumeUnit::mL);
    pc->ProcessAction(injection);
    pc->AdvanceModelTime(1, TimeUnit::s);// Not tracking

    SEScalarTime now;// Make sure to tell the engine that we are at the same time

    // Change our results file name
    pc->GetLogger()->SetLogFile("InjectSuccsSerialization.log");
    std::remove("InjectSuccsSerialization.csv");
    pc->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("InjectSuccsSerialization.csv");

    // Save and Load the Engine State
    pc->SerializeToFile("./MidBolusState.json");
    now.SetValue(pc->GetSimulationTime(TimeUnit::s), TimeUnit::s);
    pc->SerializeFromFile("./MidBolusState.json");
    pc->SetSimulationTime(now);

    tracker.AdvanceModelTime(15);

    SEAnesthesiaMachineConfiguration amConfig(pc->GetLogger());
    amConfig.GetConfiguration().SetConnection(eSwitch::On);
    amConfig.GetConfiguration().GetInletFlow().SetValue(5, VolumePerTimeUnit::L_Per_min);
    amConfig.GetConfiguration().GetInspiratoryExpiratoryRatio().SetValue(0.5);
    amConfig.GetConfiguration().GetOxygenFraction().SetValue(0.4);
    amConfig.GetConfiguration().SetOxygenSource(eAnesthesiaMachine_OxygenSource::Wall);
    amConfig.GetConfiguration().GetPositiveEndExpiredPressure().SetValue(3.0, PressureUnit::cmH2O);
    amConfig.GetConfiguration().SetPrimaryGas(eAnesthesiaMachine_PrimaryGas::Nitrogen);
    amConfig.GetConfiguration().GetReliefValvePressure().SetValue(20, PressureUnit::cmH2O);
    amConfig.GetConfiguration().GetRespiratoryRate().SetValue(16.0, FrequencyUnit::Per_min);
    amConfig.GetConfiguration().GetPeakInspiratoryPressure().SetValue(10, PressureUnit::cmH2O);
    amConfig.GetConfiguration().GetOxygenBottleOne().GetVolume().SetValue(660, VolumeUnit::L);
    amConfig.GetConfiguration().GetOxygenBottleTwo().GetVolume().SetValue(660, VolumeUnit::L);
    pc->ProcessAction(amConfig);
    tracker.AdvanceModelTime(5);

    pc->SerializeToFile("./AnesthesiaMachineState.json");
    now.SetValue(pc->GetSimulationTime(TimeUnit::s), TimeUnit::s);
    pc->SerializeFromFile("./AnesthesiaMachineState.json");
    pc->SetSimulationTime(now);

    tracker.AdvanceModelTime(40);
  }

  void EngineTest::SerializationTest(const std::string& sTestDirectory)
  {
    // Create the engine and load the patient
    Engine pe;
    pe.GetLogger()->SetLogFile("SerializationTestSetup.log");
    HowToTracker tracker(pe);

    /*
      SESubstanceDataRequest* subRequest;


      SECompartmentDataRequest* cmptRequest;
      cmptRequest = new SECompartmentDataRequest();
      cmptRequest->Set("Aorta", CDM::enumCompartmentType::Liquid, *O2, "PartialPressure", PressureUnit::mmHg);
      tracker.m_Requests.push_back(cmptRequest);
      cmptRequest = new SECompartmentDataRequest();
      cmptRequest->Set("Carina", CDM::enumCompartmentType::Gas, *O2, "PartialPressure", PressureUnit::cmH2O);
      tracker.m_Requests.push_back(cmptRequest);
      cmptRequest = new SECompartmentDataRequest();
      cmptRequest->Set("Aorta", CDM::enumCompartmentType::Liquid, *CO2, "PartialPressure", PressureUnit::mmHg);
      tracker.m_Requests.push_back(cmptRequest);
      cmptRequest = new SECompartmentDataRequest();
      cmptRequest->Set("Carina", CDM::enumCompartmentType::Gas, *CO2, "PartialPressure", PressureUnit::cmH2O);
      tracker.m_Requests.push_back(cmptRequest);


      SEPhysiologySystemDataRequest* physRequest;
      physRequest = new SEPhysiologySystemDataRequest();
      physRequest->Set("ArterialPressure", PressureUnit::mmHg);
      tracker.m_Requests.push_back(physRequest);
      physRequest = new SEPhysiologySystemDataRequest();
      physRequest->Set("BloodVolume", VolumeUnit::mL);
      tracker.m_Requests.push_back(physRequest);
      physRequest = new SEPhysiologySystemDataRequest();
      physRequest->Set("CarbonDioxideProductionRate", VolumePerTimeUnit::mL_Per_min);
      tracker.m_Requests.push_back(physRequest);
      physRequest = new SEPhysiologySystemDataRequest();
      physRequest->Set("CarbonDioxideSaturation");
      tracker.m_Requests.push_back(physRequest);
      physRequest = new SEPhysiologySystemDataRequest();
      physRequest->Set("CardiacOutput", VolumePerTimeUnit::mL_Per_min);
      tracker.m_Requests.push_back(physRequest);
      physRequest = new SEPhysiologySystemDataRequest();
      physRequest->Set("CentralVenousPressure", PressureUnit::mmHg);
      tracker.m_Requests.push_back(physRequest);
      physRequest = new SEPhysiologySystemDataRequest();
      physRequest->Set("DiastolicArterialPressure", PressureUnit::mmHg);
      tracker.m_Requests.push_back(physRequest);
      physRequest = new SEPhysiologySystemDataRequest();
      physRequest->Set("GlomerularFiltrationRate", VolumePerTimeUnit::mL_Per_day);
      tracker.m_Requests.push_back(physRequest);
      physRequest = new SEPhysiologySystemDataRequest();
      physRequest->Set("HeartRate", FrequencyUnit::Per_min);
      tracker.m_Requests.push_back(physRequest);
      physRequest = new SEPhysiologySystemDataRequest();
      physRequest->Set("HeartStrokeVolume", VolumeUnit::mL);
      tracker.m_Requests.push_back(physRequest);
      physRequest = new SEPhysiologySystemDataRequest();
      physRequest->Set("LeftAfferentArterioleResistance", FlowResistanceUnit::mmHg_min_Per_mL);
      tracker.m_Requests.push_back(physRequest);
      physRequest = new SEPhysiologySystemDataRequest();
      physRequest->Set("MeanArterialPressure", PressureUnit::mmHg);
      tracker.m_Requests.push_back(physRequest);
      physRequest = new SEPhysiologySystemDataRequest();
      physRequest->Set("NeuromuscularBlockLevel");
      tracker.m_Requests.push_back(physRequest);
      physRequest = new SEPhysiologySystemDataRequest();
      physRequest->Set("OxygenConsumptionRate", VolumePerTimeUnit::mL_Per_min);
      tracker.m_Requests.push_back(physRequest);
      physRequest = new SEPhysiologySystemDataRequest();
      physRequest->Set("OxygenSaturation");
      tracker.m_Requests.push_back(physRequest);
      physRequest = new SEPhysiologySystemDataRequest();
      physRequest->Set("RenalBloodFlow",VolumePerTimeUnit::mL_Per_min);
      tracker.m_Requests.push_back(physRequest);
      physRequest = new SEPhysiologySystemDataRequest();
      physRequest->Set("RespirationRate", FrequencyUnit::Per_min);
      tracker.m_Requests.push_back(physRequest);
      physRequest = new SEPhysiologySystemDataRequest();
      physRequest->Set("SystolicArterialPressure", PressureUnit::mmHg);
      tracker.m_Requests.push_back(physRequest);
      physRequest = new SEPhysiologySystemDataRequest();
      physRequest->Set("TotalAlveolarVentilation", VolumePerTimeUnit::L_Per_min);
      tracker.m_Requests.push_back(physRequest);
      physRequest = new SEPhysiologySystemDataRequest();
      physRequest->Set("TotalLungVolume", VolumeUnit::mL);
      tracker.m_Requests.push_back(physRequest);
      physRequest = new SEPhysiologySystemDataRequest();
      physRequest->Set("TranspulmonaryPressure", PressureUnit::cmH2O);
      tracker.m_Requests.push_back(physRequest);
      SEEquipmentSystemDataRequest* equipRequest = new SEEquipmentSystemDataRequest();
      equipRequest->Set("Lead3ElectricPotential", ElectricPotentialUnit::mV);
      tracker.m_Requests.push_back(equipRequest);
    */


    {// Basic Standard    

      // Gen Basic Standard Baseline
     /* {
        pe.GetLogger()->ResetLogFile("BasicStandardResults.log");
        pe.GetEngineTrack()->RequestData(tracker.m_Requests, "BasicStandardResults.csv");
        if (!pe.InitializeEngine("StandardMale.json"))
        {
          std::cerr << "Could not load initialize engine, check the error" << std::endl;
          return;
        }
        tracker.AdvanceModelTime(120);
      }*/

      // Gen Basic Standard State
      /*{
        pe.GetLogger()->ResetLogFile("BasicStandardStateSetupResults.log");
        pe.GetEngineTrack()->RequestData(tracker.m_Requests, "BasicStandardStateSetupResults.csv");
        if (!pe.InitializeEngine("StandardMale.json"))
        {
          std::cerr << "Could not load initialize engine, check the error" << std::endl;
          return;
        }
        tracker.AdvanceModelTime(60);
        pe.SaveState("./BasicStandardState@60s.json");
      }*/

      // Run Basic Standard State
      {
        pe.GetLogger()->SetLogFile("BasicStandardStateResults.log");
        pe.GetEngineTracker()->GetDataRequestManager().SetResultsFilename("BasicStandardStateResults.csv");
        pe.SerializeFromFile("./BasicStandardState@60s.json");
        tracker.AdvanceModelTime(60);
      }
    }
    // Several Options to choose how to set up our engine before we save and load
    if (false)
    {
      pe.GetEngineTracker()->GetDataRequestManager().CreateSubstanceDataRequest("Albuterol", "MassInBody", MassUnit::ug);
      pe.GetEngineTracker()->GetDataRequestManager().CreateSubstanceDataRequest("Albuterol", "PlasmaConcentration", MassPerVolumeUnit::ug_Per_mL);
      pe.GetEngineTracker()->GetDataRequestManager().CreateSubstanceDataRequest("Albuterol", "RemainingSystemicMassCleared", MassUnit::ug);
      InhalerState(&pe, tracker);
    }
    else if (false)
    {
      pe.GetEngineTracker()->GetDataRequestManager().CreateSubstanceDataRequest("Succinylcholine", "MassInBody", MassUnit::ug);
      pe.GetEngineTracker()->GetDataRequestManager().CreateSubstanceDataRequest("Succinylcholine", "PlasmaConcentration", MassPerVolumeUnit::ug_Per_mL);
      pe.GetEngineTracker()->GetDataRequestManager().CreateSubstanceDataRequest("Succinylcholine", "RemainingSystemicMassCleared", MassUnit::ug);
      pe.GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest("BrainVasculature", "Succinylcholine", "Concentration", MassPerVolumeUnit::ug_Per_mL);
      pe.GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest("BrainTissueExtracellular", "Succinylcholine", "Concentration", MassPerVolumeUnit::ug_Per_mL);
      InjectSuccsState(&pe, tracker, *pe.GetSubstanceManager().GetSubstance("Succinylcholine"));
    }
    pe.SerializeToFile("./FinalEngineState.json");
  }
}
