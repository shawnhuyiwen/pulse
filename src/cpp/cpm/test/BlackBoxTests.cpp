/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "EngineTest.h"

#include "PulseConfiguration.h"
#include "blackbox/SEBlackBoxManager.h"
#include "blackbox/fluid/SELiquidBlackBox.h"
#include "engine/SEEngineTracker.h"
#include "engine/SEDataRequestManager.h"
#include "compartment/SECompartmentManager.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarTime.h"
#include "utils/DataTrack.h"
#include "utils/FileUtils.h"
#include "utils/TimingProfile.h"

struct BlackBoxes
{
  enum class locations { AORTA = 0, VENACAVA = 1, BOTH = 2 };
  BlackBoxes(locations l = locations::AORTA) { at = l; }

  locations at;
  SELiquidBlackBox* aortaToRightLeg = nullptr;
  SELiquidBlackBox* rightLegToVenaCava = nullptr;
};
bool SetupBBDataRequests(BlackBoxes& bbz, PhysiologyEngine& pulse, const std::string& csvFilename)
{
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

  if (bbz.at == BlackBoxes::locations::AORTA || bbz.at == BlackBoxes::locations::BOTH)
  {
    bbz.aortaToRightLeg = pulse.GetBlackBoxes().GetLiquidBlackBox(pulse::VascularCompartment::Aorta, pulse::VascularCompartment::RightLeg);
    if (bbz.aortaToRightLeg == nullptr)
      return false;
  
    bbz.aortaToRightLeg->GetCompartment().GetVolume().SetValue(10, VolumeUnit::mL);
    pulse.GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(bbz.aortaToRightLeg->GetName(), "Pressure", PressureUnit::mmHg);
    pulse.GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(bbz.aortaToRightLeg->GetName(), "InFlow", VolumePerTimeUnit::mL_Per_s);
    pulse.GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(bbz.aortaToRightLeg->GetName(), "OutFlow", VolumePerTimeUnit::mL_Per_s);
    pulse.GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(bbz.aortaToRightLeg->GetName(), "Volume", VolumeUnit::mL);
  }
  if (bbz.at == BlackBoxes::locations::VENACAVA || bbz.at == BlackBoxes::locations::BOTH)
  {
    bbz.rightLegToVenaCava = pulse.GetBlackBoxes().GetLiquidBlackBox(pulse::VascularCompartment::RightLeg, pulse::VascularCompartment::VenaCava);
    if (bbz.rightLegToVenaCava == nullptr)
      return false;

    bbz.rightLegToVenaCava->GetCompartment().GetVolume().SetValue(10, VolumeUnit::mL);
    pulse.GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(bbz.rightLegToVenaCava->GetName(), "Pressure", PressureUnit::mmHg);
    pulse.GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(bbz.rightLegToVenaCava->GetName(), "InFlow", VolumePerTimeUnit::mL_Per_s);
    pulse.GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(bbz.rightLegToVenaCava->GetName(), "OutFlow", VolumePerTimeUnit::mL_Per_s);
    pulse.GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(bbz.rightLegToVenaCava->GetName(), "Volume", VolumeUnit::mL);
  }

  pulse.GetEngineTracker()->GetDataRequestManager().SetResultsFilename(csvFilename);

  return true;
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

  BlackBoxes bbz;
  if(!SetupBBDataRequests(bbz, *pulse, outputDir+"/EmptyBlackBoxTest.csv"))
  {
    pulse->GetLogger()->Error("Could not create black boxes");
    Error("Could not create black boxes");
    return;
  }

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

  PulseConfiguration config;
  config.AllowDynamicTimeStep(eSwitch::On);
  pulse->SetConfigurationOverride(&config);

  if (!pulse->SerializeFromFile("./states/StandardMale@0s.json"))
  {
    pulse->GetLogger()->Error("Could not load state, check the error");
    Error("Could not load state, check the error");
    return;
  }


  BlackBoxes bbz(BlackBoxes::locations::VENACAVA);
  if(!SetupBBDataRequests(bbz, *pulse, outputDir+"/ImposeFlowBlackBoxTest.csv"))
  {
    pulse->GetLogger()->Error("Could not create black boxes");
    Error("Could not create black boxes");
    return;
  }

  double aortaToRightLegInflow = 2.0;
  double aortaToRightLegOutflow = 1.5;

  double rightLegToVenaCavaInflow = -1.5;
  double rightLegToVenaCavaOutflow = 1.5;

  // Run for two mins
  TimingProfile profile;
  profile.Start("BB");
  for (size_t i = 0; i < 120 / pulse->GetTimeStep(TimeUnit::s); i++)
  {
    if (bbz.aortaToRightLeg != nullptr)
    {
      Info("Current Aorta/Right Leg Source Node Potential : " + std::to_string(bbz.aortaToRightLeg->GetSourcePotential(PressureUnit::mmHg)) + " mmHg");
      Info("Current Aorta/Right Leg Source Compartment Potential : " + std::to_string(bbz.aortaToRightLeg->GetSourceCompartment().GetPressure(PressureUnit::mmHg)) + " mmHg");
      Info("Current Aorta/Right Leg Source Path Flux : " + std::to_string(bbz.aortaToRightLeg->GetSourceFlux(VolumePerTimeUnit::mL_Per_s)) + " mL/s");
      Info("Imposing " + bbz.aortaToRightLeg->GetName() + " source flow, to " + std::to_string(aortaToRightLegInflow) + " mL/s");
      bbz.aortaToRightLeg->ImposeSourceFlux(aortaToRightLegInflow, VolumePerTimeUnit::mL_Per_s);
      Info("New Aorta/Right Leg Source Path Flux : " + std::to_string(bbz.aortaToRightLeg->GetSourceFlux(VolumePerTimeUnit::mL_Per_s)) + " mL/s\n");

      Info("Current Aorta/Right Leg Target Node Potential : " + std::to_string(bbz.aortaToRightLeg->GetTargetPotential(PressureUnit::mmHg)) + " mmHg");
      Info("Current Aorta/Right Leg Target Compartment Potential : " + std::to_string(bbz.aortaToRightLeg->GetTargetCompartment().GetPressure(PressureUnit::mmHg)) + " mmHg");
      Info("Current Aorta/Right Leg Target Path Flux : " + std::to_string(bbz.aortaToRightLeg->GetTargetFlux(VolumePerTimeUnit::mL_Per_s)) + " mL/s");
      Info("Imposing " + bbz.aortaToRightLeg->GetName() + " target flow, to " + std::to_string(aortaToRightLegOutflow) + " mL/s");
      bbz.aortaToRightLeg->ImposeTargetFlux(aortaToRightLegOutflow, VolumePerTimeUnit::mL_Per_s);
      Info("New Aorta/Right Leg Target Path Flux : " + std::to_string(bbz.aortaToRightLeg->GetTargetFlux(VolumePerTimeUnit::mL_Per_s)) + " mL/s\n");
    }

    if (bbz.rightLegToVenaCava != nullptr)
    {
      Info("Current Right Leg/Vena Cava Source Node Potential : " + std::to_string(bbz.rightLegToVenaCava->GetSourcePotential(PressureUnit::mmHg)) + " mmHg");
      Info("Current Right Leg/Vena Cava Source Compartment Potential : " + std::to_string(bbz.rightLegToVenaCava->GetSourceCompartment().GetPressure(PressureUnit::mmHg)) + " mmHg");
      Info("Current Right Leg/Vena Cava Source Path Flux : " + std::to_string(bbz.rightLegToVenaCava->GetSourceFlux(VolumePerTimeUnit::mL_Per_s)) + " mL/s");
      Info("Imposing " + bbz.rightLegToVenaCava->GetName() + " source flow, to " + std::to_string(rightLegToVenaCavaInflow) + " mL/s");
      bbz.rightLegToVenaCava->ImposeSourceFlux(rightLegToVenaCavaInflow, VolumePerTimeUnit::mL_Per_s);
      Info("New Vena Right Leg/Cava Source Flux : " + std::to_string(bbz.rightLegToVenaCava->GetSourceFlux(VolumePerTimeUnit::mL_Per_s)) + " mL/s\n");

      Info("Current Right Leg/Vena Cava Target Node Potential : " + std::to_string(bbz.rightLegToVenaCava->GetTargetPotential(PressureUnit::mmHg)) + " mmHg");
      Info("Current Right Leg/Vena Cava Target Compartment Potential : " + std::to_string(bbz.rightLegToVenaCava->GetTargetCompartment().GetPressure(PressureUnit::mmHg)) + " mmHg");
      Info("Current Right Leg/Vena Cava Target Path Flux : " + std::to_string(bbz.rightLegToVenaCava->GetTargetFlux(VolumePerTimeUnit::mL_Per_s)) + " mL/s");
      Info("Imposing " + bbz.rightLegToVenaCava->GetName() + " target flow, to " + std::to_string(rightLegToVenaCavaOutflow) + " mL/s");
      bbz.rightLegToVenaCava->ImposeTargetFlux(rightLegToVenaCavaOutflow, VolumePerTimeUnit::mL_Per_s);
      Info("New Right Leg/Vena Cava Target Path Flux : " + std::to_string(bbz.rightLegToVenaCava->GetTargetFlux(VolumePerTimeUnit::mL_Per_s)) + " mL/s\n");
    }

    if (!pulse->AdvanceModelTime(0.0002, TimeUnit::s))
    {
      Error("Unable to advance time");
      return;
    }
    Info("--------------- Advance Time ---------------\n");

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

  BlackBoxes bbz;
  if (!SetupBBDataRequests(bbz, *pulse, outputDir + "/ImposePressureAndFlowBlackBoxTest.csv"))
  {
    pulse->GetLogger()->Error("Could not create black boxes");
    Error("Could not create black boxes");
    return;
  }

  double resistance_mmHg_s_Per_mL = 0.1;
  
  double dampenFraction = 0.01;

  // Run for two mins
  TimingProfile profile;
  profile.Start("BB");
  for (size_t i = 0; i < 120 / pulse->GetTimeStep(TimeUnit::s); i++)
  {
    // Calculate a change due to a small resistance
    //It will only change half as much as it wants to each time step to ensure it's critically damped and doesn't overshoot

    double inletFlow_mL_Per_s = bbz.aortaToRightLeg->GetSourceFlux(VolumePerTimeUnit::mL_Per_s);
    double outletPressure_mmHg = bbz.aortaToRightLeg->GetTargetPotential(PressureUnit::mmHg);

    double inletPressure_mmHg = bbz.aortaToRightLeg->GetSourcePotential(PressureUnit::mmHg);
    double outletFlow_mL_Per_s = bbz.aortaToRightLeg->GetTargetFlux(VolumePerTimeUnit::mL_Per_s);

    double nextInletPressure_mmhg = inletFlow_mL_Per_s * resistance_mmHg_s_Per_mL + outletPressure_mmHg;
    double next_OutletFlow_mL_Per_s = (nextInletPressure_mmhg - outletPressure_mmHg) / resistance_mmHg_s_Per_mL;

    double inletPressureChange_mmhg = (nextInletPressure_mmhg - inletPressure_mmHg) * dampenFraction;
    double outletFlowChange_mL_Per_s = (next_OutletFlow_mL_Per_s - outletFlow_mL_Per_s) * dampenFraction;

    bbz.aortaToRightLeg->ImposeSourcePotential(inletPressure_mmHg + inletPressureChange_mmhg, PressureUnit::mmHg);
    bbz.aortaToRightLeg->ImposeTargetFlux(outletFlow_mL_Per_s + outletFlowChange_mL_Per_s, VolumePerTimeUnit::mL_Per_s);

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
