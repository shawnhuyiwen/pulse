/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "EngineTest.h"

PulseEngineTest::PulseEngineTest(Logger* logger) : Loggable(logger)
{
  FillFunctionMap();
}

PulseEngineTest::~PulseEngineTest()
{
}

bool PulseEngineTest::RunTest(const std::string& testName, const std::string& sOutputDirectory)
{
  try
  {
    //If you find the test name, run it
    if (bgeMap.find(testName) != bgeMap.end())
    {
      testFunction func = bgeMap.at(testName);
      (this->*func)(sOutputDirectory);
      return true;
    }
    else
    {
      //Try to find the function with "Test" appended; run it if found
      if (bgeMap.find(testName + "Test") != bgeMap.end())
      {
        testFunction func = bgeMap.at(testName + "Test");
        (this->*func)(sOutputDirectory);
        return true;
      }

      m_ss << "Could not find " << testName << " or " << testName << "Test in PulseEngineTest." << std::endl;
      Error(m_ss);
      return false;
    }
  }
  catch (CommonDataModelException& ex)
  {
    Error(ex.what());
  }
  catch (std::exception& ex)
  {
    Error(ex.what());
  }
  catch (...)
  {
    Error("Exception!!");
  }
  return false;
}

void PulseEngineTest::FillFunctionMap()
{
  //A few functions have overloads, so we have to use the pointer clarify which one we want
  testFunction cardioCTTest = &PulseEngineTest::CardiovascularCircuitAndTransportTest;
  testFunction cardioBGTest = &PulseEngineTest::CardiovascularBloodGasesTest;
  testFunction cardioTuneCircuitTest = &PulseEngineTest::TuneCardiovascularCircuitTest;
  testFunction respCTTest = &PulseEngineTest::RespiratoryCircuitAndTransportTest;
  testFunction anesthCTTest = &PulseEngineTest::AnesthesiaMachineCircuitAndTransportTest;
  testFunction ventCTTest = &PulseEngineTest::MechanicalVentilatorCircuitAndTransportTest;


  bgeMap.insert(std::make_pair("ReadScenarios", &PulseEngineTest::ReadScenarios));
  bgeMap.insert(std::make_pair("ReuseEngine", &PulseEngineTest::ReuseEngine));

  //Fill a map that ties unit test names to their actual functions
  bgeMap.insert(std::make_pair("CardiovascularCircuitAndTransportTest", cardioCTTest));
  bgeMap.insert(std::make_pair("CardiovascularAndRenalCircuitAndTransportTest", &PulseEngineTest::CardiovascularAndRenalCircuitAndTransportTest));
  bgeMap.insert(std::make_pair("CardiovascularAndTissueCircuitAndTransportTest", &PulseEngineTest::CardiovascularAndTissueCircuitAndTransportTest));
  bgeMap.insert(std::make_pair("CardiovascularAndCerebrospinalFluidCircuitAndTransportTest", &PulseEngineTest::CardiovascularAndCerebrospinalFluidCircuitAndTransportTest));
  bgeMap.insert(std::make_pair("FullCardiovascularCircuitAndTransportTest", &PulseEngineTest::FullCardiovascularCircuitAndTransportTest));
  bgeMap.insert(std::make_pair("CardiovascularBloodGasesTest", cardioBGTest));
  bgeMap.insert(std::make_pair("TuneCardiovascularCircuitTest", cardioTuneCircuitTest));
  bgeMap.insert(std::make_pair("CardiovascularCircuitScaleTests", &PulseEngineTest::CardiovascularCircuitScaleTests));

  bgeMap.insert(std::make_pair("RenalCircuitAndTransportTest", &PulseEngineTest::RenalCircuitAndTransportTest));
  bgeMap.insert(std::make_pair("RenalTGFFeedbackTest", &PulseEngineTest::RenalTGFFeedbackTest));
  bgeMap.insert(std::make_pair("RenalTGFandUPRFeedbackTest", &PulseEngineTest::RenalTGFandUPRFeedbackTest));
  bgeMap.insert(std::make_pair("RenalSecretionTest", &PulseEngineTest::RenalSecretionTest));
  bgeMap.insert(std::make_pair("RenalUrinateTest", &PulseEngineTest::RenalUrinateTest));

  bgeMap.insert(std::make_pair("RespiratoryCircuitAndTransportTest", respCTTest));
  bgeMap.insert(std::make_pair("RespiratoryDriverTest", &PulseEngineTest::RespiratoryDriverTest));

  bgeMap.insert(std::make_pair("AnesthesiaMachineCircuitAndTransportTest", anesthCTTest));
  bgeMap.insert(std::make_pair("RespiratoryWithAnesthesiaMachineCircuitAndTransportTest", &PulseEngineTest::RespiratoryWithAnesthesiaMachineCircuitAndTransportTest));

  bgeMap.insert(std::make_pair("MechanicalVentilatorCircuitAndTransportTest", ventCTTest));
  bgeMap.insert(std::make_pair("RespiratoryWithMechanicalVentilatorCircuitAndTransportTest", &PulseEngineTest::RespiratoryWithMechanicalVentilatorCircuitAndTransportTest));

  bgeMap.insert(std::make_pair("RespiratoryWithInhalerCircuitAndTransportTest", &PulseEngineTest::RespiratoryWithInhalerCircuitAndTransportTest));

  bgeMap.insert(std::make_pair("RespiratoryWithMechanicalVentilationCircuitAndTransportTest", &PulseEngineTest::RespiratoryWithMechanicalVentilationCircuitAndTransportTest));

  bgeMap.insert(std::make_pair("InternalTemperatureVariableBMRCircuitTest", &PulseEngineTest::InternalTemperatureVariableBMRCircuitTest));
  bgeMap.insert(std::make_pair("InternalTemperatureVariableSkinCircuitTest", &PulseEngineTest::InternalTemperatureVariableSkinCircuitTest));
  bgeMap.insert(std::make_pair("InternalTemperatureVariableCoreCircuitTest", &PulseEngineTest::InternalTemperatureVariableCoreCircuitTest));
  bgeMap.insert(std::make_pair("EnvironmentVariableTemperatureCircuitTest", &PulseEngineTest::EnvironmentVariableTemperatureCircuitTest));
  bgeMap.insert(std::make_pair("CombinedInternalAndEnvironmentVariableBMRandTemperatureCircuitTest", &PulseEngineTest::CombinedInternalAndEnvironmentVariableBMRandTemperatureCircuitTest));
  bgeMap.insert(std::make_pair("CombinedInternalAndEnvironmentSkinTempDropCircuitTest", &PulseEngineTest::CombinedInternalAndEnvironmentSkinTempDropCircuitTest));
  bgeMap.insert(std::make_pair("EnvironmentISO7730ComparisonTest", &PulseEngineTest::EnvironmentISO7730ComparisonTest));

  bgeMap.insert(std::make_pair("DiffusionClearanceExcretionTests", &PulseEngineTest::DiffusionClearanceExcretionTests));
  bgeMap.insert(std::make_pair("AlveolarCarbonDioxideDiffusionTest", &PulseEngineTest::AlveolarCarbonDioxideDiffusionTest));
  bgeMap.insert(std::make_pair("AlveolarOxygenDiffusionTest", &PulseEngineTest::AlveolarOxygenDiffusionTest));
  bgeMap.insert(std::make_pair("InstantPlusSimpleDiffusionTest", &PulseEngineTest::InstantPlusSimpleDiffusionTest));
  bgeMap.insert(std::make_pair("SimpleDiffusionTwoCompartmentTest", &PulseEngineTest::SimpleDiffusionTwoCompartmentTest));
  bgeMap.insert(std::make_pair("SimpleDiffusionFourCompartmentTest", &PulseEngineTest::SimpleDiffusionFourCompartmentTest));
  bgeMap.insert(std::make_pair("SimpleDiffusionHierarchyTest", &PulseEngineTest::SimpleDiffusionHierarchyTest));
  bgeMap.insert(std::make_pair("FacilitatedDiffusionTest", &PulseEngineTest::FacilitatedDiffusionTest));

  bgeMap.insert(std::make_pair("BrainInjuryTest", &PulseEngineTest::BrainInjuryTest));

  bgeMap.insert(std::make_pair("ConditionCombinations", &PulseEngineTest::ConditionCombinations));
  bgeMap.insert(std::make_pair("MultiEngineTest", &PulseEngineTest::MultiEngineTest));
  bgeMap.insert(std::make_pair("SerializationTest", &PulseEngineTest::SerializationTest));
  bgeMap.insert(std::make_pair("SolverSpeedTest", &PulseEngineTest::SolverSpeedTest));

  bgeMap.insert(std::make_pair("AcidBaseMathTest", &PulseEngineTest::AcidBaseMathTest));
  bgeMap.insert(std::make_pair("AcidBaseFeedbackTest", &PulseEngineTest::AcidBaseFeedbackTest));
  bgeMap.insert(std::make_pair("AcidBaseLimitsTest", &PulseEngineTest::AcidBaseLimitsTest));
  bgeMap.insert(std::make_pair("AcidBaseExtremeTest", &PulseEngineTest::AcidBaseExtremeTest));
  bgeMap.insert(std::make_pair("AcidBaseBloodGasTests", &PulseEngineTest::AcidBaseBloodGasTests));

  bgeMap.insert(std::make_pair("AerosolTest", &PulseEngineTest::AerosolTest));

  bgeMap.insert(std::make_pair("FourCompartmentTestSimple", &PulseEngineTest::FourCompartmentTestSimple));
  bgeMap.insert(std::make_pair("AcidBaseFourCompartmentTest", &PulseEngineTest::AcidBaseFourCompartmentTest));
  bgeMap.insert(std::make_pair("FiveCompartmentTestWithDiffusion", &PulseEngineTest::FiveCompartmentTestWithDiffusion));
  bgeMap.insert(std::make_pair("AcidBaseFourCompartmentTestWithProductionConsumption", &PulseEngineTest::AcidBaseFourCompartmentTestWithProductionConsumption));
  bgeMap.insert(std::make_pair("AcidBaseFiveCompartmentTestWithDiffusion", &PulseEngineTest::AcidBaseFiveCompartmentTestWithDiffusion));
  bgeMap.insert(std::make_pair("AcidBaseFiveCompartmentTestWithProductionConsumptionAndDiffusion", &PulseEngineTest::AcidBaseFiveCompartmentTestWithProductionConsumptionAndDiffusion));
  
}
