/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "EngineTest.h"

#include "blackbox/SEBlackBoxManager.h"
#include "blackbox/fluid/SELiquidBlackBox.h"
#include "engine/SEEngineTracker.h"
#include "engine/SEDataRequestManager.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarTime.h"
#include "utils/DataTrack.h"
#include "utils/FileUtils.h"
#include "utils/TimingProfile.h"

SELiquidBlackBox* SetupBBDataRequests(PhysiologyEngine& pulse, const std::string& csvFilename)
{
  SELiquidBlackBox* bb = pulse.GetBlackBoxes().GetLiquidBlackBox(pulse::VascularCompartment::Aorta, pulse::VascularCompartment::RightLeg);

  pulse.GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("HeartRate", FrequencyUnit::Per_min);
  pulse.GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("BloodVolume", VolumeUnit::mL);
  pulse.GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("CardiacOutput", VolumePerTimeUnit::mL_Per_min);
  pulse.GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("MeanArterialPressure", PressureUnit::mmHg);
  pulse.GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("SystolicArterialPressure", PressureUnit::mmHg);
  pulse.GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("DiastolicArterialPressure", PressureUnit::mmHg);

  pulse.GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::VascularCompartment::Aorta, "Pressure", PressureUnit::mmHg);
  pulse.GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::VascularCompartment::Aorta, "InFlow", VolumePerTimeUnit::mL_Per_s);
  pulse.GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::VascularCompartment::Aorta, "OutFlow", VolumePerTimeUnit::mL_Per_s);
  pulse.GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::VascularCompartment::Aorta, "Volume", VolumeUnit::mL);

  pulse.GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::VascularCompartment::RightLeg, "Pressure", PressureUnit::mmHg);
  pulse.GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::VascularCompartment::RightLeg, "InFlow", VolumePerTimeUnit::mL_Per_s);
  pulse.GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::VascularCompartment::RightLeg, "OutFlow", VolumePerTimeUnit::mL_Per_s);
  pulse.GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::VascularCompartment::RightLeg, "Volume", VolumeUnit::mL);

  pulse.GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::VascularCompartment::VenaCava, "Pressure", PressureUnit::mmHg);
  pulse.GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::VascularCompartment::VenaCava, "InFlow", VolumePerTimeUnit::mL_Per_s);
  pulse.GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::VascularCompartment::VenaCava, "OutFlow", VolumePerTimeUnit::mL_Per_s);
  pulse.GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::VascularCompartment::VenaCava, "Volume", VolumeUnit::mL);

  pulse.GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(bb->GetCompartment().GetName(), "Pressure", PressureUnit::mmHg);
  pulse.GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(bb->GetCompartment().GetName(), "InFlow", VolumePerTimeUnit::mL_Per_s);
  pulse.GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(bb->GetCompartment().GetName(), "OutFlow", VolumePerTimeUnit::mL_Per_s);
  pulse.GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(bb->GetCompartment().GetName(), "Volume", VolumeUnit::mL);

  pulse.GetEngineTracker()->GetDataRequestManager().SetResultsFilename(csvFilename);

  return bb;
}

void PulseEngineTest::EmptyBlackBoxTest(const std::string& outputDir)
{
  std::unique_ptr<PhysiologyEngine> pulse = CreatePulseEngine(m_Logger);
  Info("--------EmptyBlackBoxTest--------");

  if (!pulse->SerializeFromFile("./states/StandardMale@0s.json"))
  {
    pulse->GetLogger()->Error("Could not load state, check the error");
    Error("Could not load state, check the error");
    return;
  }

  SetupBBDataRequests(*pulse, outputDir + "/EmptyBlackBoxTest.csv");

  // Run for two mins
  TimingProfile profile;
  profile.Start("eBB");
  for (size_t i=0; i<120/pulse->GetTimeStep(TimeUnit::s); i++)
  {
    if (!pulse->AdvanceModelTime())
    {
      Error("Unable to advance time");
      return;
    }
    pulse->GetEngineTracker()->TrackData(pulse->GetSimulationTime(TimeUnit::s));
    if (i == 3000)
      Info("It took " + std::to_string(profile.GetElapsedTime_s("eBB")) + "(s) to simulate 60s");
  }
  Info("It took " + std::to_string(profile.GetElapsedTime_s("eBB")) + "(s) to simulate 60s");
  profile.Stop("eBB");
}

void PulseEngineTest::ImposeFlowBlackBoxTest(const std::string& outputDir)
{
  std::unique_ptr<PhysiologyEngine> pulse = CreatePulseEngine(m_Logger);
  Info("--------ImposeFlowBlackBoxTest--------");

  if (!pulse->SerializeFromFile("./states/StandardMale@0s.json"))
  {
    pulse->GetLogger()->Error("Could not load state, check the error");
    Error("Could not load state, check the error");
    return;
  }

  SELiquidBlackBox* bb = SetupBBDataRequests(*pulse, outputDir + "/ImposeFlowBlackBoxTest.csv");

  // Run for two mins
  TimingProfile profile;
  profile.Start("BB");
  for (size_t i = 0; i < 120 / pulse->GetTimeStep(TimeUnit::s); i++)
  {
    bb->ImposeSourceFlux(2.1, VolumePerTimeUnit::mL_Per_s);
    bb->ImposeTargetFlux(2.1, VolumePerTimeUnit::mL_Per_s);

    if (!pulse->AdvanceModelTime())
    {
      Error("Unable to advance time");
      return;
    }
    pulse->GetEngineTracker()->TrackData(pulse->GetSimulationTime(TimeUnit::s));
    if (i == 3000)
      Info("It took " + std::to_string(profile.GetElapsedTime_s("BB")) + "(s) to simulate 60s");
  }
  Info("It took " + std::to_string(profile.GetElapsedTime_s("BB")) + "(s) to simulate 60s");
  profile.Stop("BB");
}

void PulseEngineTest::ImposePressureAndFlowBlackBoxTest(const std::string& outputDir)
{
  std::unique_ptr<PhysiologyEngine> pulse = CreatePulseEngine(m_Logger);
  Info("--------ImposePressureAndFlowBlackBoxTest--------");

  if (!pulse->SerializeFromFile("./states/StandardMale@0s.json"))
  {
    pulse->GetLogger()->Error("Could not load state, check the error");
    Error("Could not load state, check the error");
    return;
  }

  SELiquidBlackBox* bb = SetupBBDataRequests(*pulse, outputDir + "/ImposePressureAndFlowBlackBoxTest.csv");

  double resistance_mmHg_s_Per_mL = 0.1;
  
  double dampenFraction = 0.01;

  // Run for two mins
  TimingProfile profile;
  profile.Start("BB");
  for (size_t i = 0; i < 120 / pulse->GetTimeStep(TimeUnit::s); i++)
  {
    // Calculate a change due to a small resistance
    //It will only change half as much as it wants to each time step to ensure it's critically damped and doesn't overshoot

    double inletFlow_mL_Per_s = bb->GetSourceFlux(VolumePerTimeUnit::mL_Per_s);
    double outletPressure_mmHg = bb->GetTargetPotential(PressureUnit::mmHg);

    double inletPressure_mmHg = bb->GetSourcePotential(PressureUnit::mmHg);
    double outletFlow_mL_Per_s = bb->GetTargetFlux(VolumePerTimeUnit::mL_Per_s);

    double nextInletPressure_mmhg = inletFlow_mL_Per_s * resistance_mmHg_s_Per_mL + outletPressure_mmHg;
    double next_OutletFlow_mL_Per_s = (nextInletPressure_mmhg - outletPressure_mmHg) / resistance_mmHg_s_Per_mL;

    double inletPressureChange_mmhg = (nextInletPressure_mmhg - inletPressure_mmHg) * dampenFraction;
    double outletFlowChange_mL_Per_s = (next_OutletFlow_mL_Per_s - outletFlow_mL_Per_s) * dampenFraction;

    bb->ImposeSourcePotential(inletPressure_mmHg + inletPressureChange_mmhg, PressureUnit::mmHg);
    bb->ImposeTargetFlux(outletFlow_mL_Per_s + outletFlowChange_mL_Per_s, VolumePerTimeUnit::mL_Per_s);

    if (!pulse->AdvanceModelTime())
    {
      Error("Unable to advance time");
      return;
    }
    pulse->GetEngineTracker()->TrackData(pulse->GetSimulationTime(TimeUnit::s));
    if (i == 3000)
      Info("It took " + std::to_string(profile.GetElapsedTime_s("BB")) + "(s) to simulate 60s");
  }
  Info("It took " + std::to_string(profile.GetElapsedTime_s("BB")) + "(s) to simulate 60s");
  profile.Stop("BB");
}
