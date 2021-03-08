/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "EngineTest.h"
#include "controller/Controller.h"
#include "utils/FileUtils.h"

#include "engine/SETimedStabilization.h"
#include "engine/SEEngineTracker.h"
#include "engine/SEDataRequestManager.h"
#include "engine/SEPatientConfiguration.h"

#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"

#include "patient/actions/SEConsciousRespiration.h"
#include "patient/actions/SEForcedExhale.h"
#include "patient/actions/SEForcedInhale.h"
#include "patient/actions/SEUseInhaler.h"
#include "patient/actions/SEForcedPause.h"
#include "patient/actions/SESubstanceBolus.h"

#include "system/equipment/anesthesia_machine/SEAnesthesiaMachine.h"
#include "system/equipment/anesthesia_machine/SEAnesthesiaMachineOxygenBottle.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineConfiguration.h"
#include "system/equipment/inhaler/SEInhaler.h"
#include "system/equipment/inhaler/actions/SEInhalerConfiguration.h"

#include "properties/SEScalar0To1.h"
#include "properties/SEScalarElectricPotential.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarHeatResistanceArea.h"
#include "properties/SEScalarLengthPerTime.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarTemperature.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEFunctionVolumeVsTime.h"
#include "properties/SEScalarPower.h"

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

void PulseEngineTest::InhalerState(PhysiologyEngine* pc, HowToTracker& tracker)
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
  pc->SerializeToString(state,BINARY);
  now.SetValue(pc->GetSimulationTime(TimeUnit::s), TimeUnit::s);
  pc->SerializeFromString(state,BINARY);
  pc->SetSimulationTime(now);

  // Change the results file
  pc->GetLogger()->SetLogFile("InhalerSerialization.log");
  std::remove("InhalerSerializationResults.csv");
  pc->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("InhalerSerializationResults.csv");

  tracker.AdvanceModelTime(145);
}

void PulseEngineTest::InjectSuccsState(PhysiologyEngine* pc, HowToTracker& tracker, const SESubstance& succs)
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
  amConfig.GetConfiguration().SetConnection(eAnesthesiaMachine_Connection::Mask);
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

void PulseEngineTest::SerializationTest(const std::string& sTestDirectory)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> pc = CreatePulseEngine();
  pc->GetLogger()->SetLogFile("SerializationTestSetup.log");
  HowToTracker tracker(*pc);

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
      pc->GetLogger()->ResetLogFile("BasicStandardResults.log");
      pc->GetEngineTrack()->RequestData(tracker.m_Requests, "BasicStandardResults.csv");
      if (!pc->InitializeEngine("StandardMale.json"))
      {
        std::cerr << "Could not load initialize engine, check the error" << std::endl;
        return;
      }
      tracker.AdvanceModelTime(120);
    }*/

    // Gen Basic Standard State
    /*{
      pc->GetLogger()->ResetLogFile("BasicStandardStateSetupResults.log");
      pc->GetEngineTrack()->RequestData(tracker.m_Requests, "BasicStandardStateSetupResults.csv");
      if (!pc->InitializeEngine("StandardMale.json"))
      {
        std::cerr << "Could not load initialize engine, check the error" << std::endl;
        return;
      }
      tracker.AdvanceModelTime(60);
      pc->SaveState("./BasicStandardState@60s.json");
    }*/

    // Run Basic Standard State
    {
      pc->GetLogger()->SetLogFile("BasicStandardStateResults.log");
      pc->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("BasicStandardStateResults.csv");
      pc->SerializeFromFile("./BasicStandardState@60s.json");
      tracker.AdvanceModelTime(60);
    }
  }
  // Several Options to choose how to set up our engine before we save and load
  if(false)
  {
    pc->GetEngineTracker()->GetDataRequestManager().CreateSubstanceDataRequest("Albuterol", "MassInBody", MassUnit::ug);
    pc->GetEngineTracker()->GetDataRequestManager().CreateSubstanceDataRequest("Albuterol", "PlasmaConcentration", MassPerVolumeUnit::ug_Per_mL);
    pc->GetEngineTracker()->GetDataRequestManager().CreateSubstanceDataRequest("Albuterol", "RemainingSystemicMassCleared", MassUnit::ug);
    InhalerState(pc.get(), tracker);
  }
  else if (false)
  {
    pc->GetEngineTracker()->GetDataRequestManager().CreateSubstanceDataRequest("Succinylcholine", "MassInBody", MassUnit::ug);
    pc->GetEngineTracker()->GetDataRequestManager().CreateSubstanceDataRequest("Succinylcholine", "PlasmaConcentration", MassPerVolumeUnit::ug_Per_mL);
    pc->GetEngineTracker()->GetDataRequestManager().CreateSubstanceDataRequest("Succinylcholine", "RemainingSystemicMassCleared", MassUnit::ug);
    pc->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest("BrainVasculature", "Succinylcholine", "Concentration", MassPerVolumeUnit::ug_Per_mL);
    pc->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest("BrainTissueExtracellular", "Succinylcholine", "Concentration", MassPerVolumeUnit::ug_Per_mL);
    InjectSuccsState(pc.get(), tracker, *pc->GetSubstanceManager().GetSubstance("Succinylcholine"));
  }
  pc->SerializeToFile("./FinalEngineState.json");
}
