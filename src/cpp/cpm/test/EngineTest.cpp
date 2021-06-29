/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "EngineTest.h"

PulseEngineTest::PulseEngineTest(Logger* logger) : SETestManager(logger)
{

}

PulseEngineTest::~PulseEngineTest()
{
}

bool PulseEngineTest::RunTest(const std::string& testName, const std::string& sOutputDirectory)
{
  try
  {
    //If you find the test name, run it
    if (testMap.find(testName) != testMap.end())
    {
      testFunction func = testMap.at(testName);
      (this->*func)(sOutputDirectory);
      return true;
    }
    else
    {
      //Try to find the function with "Test" appended; run it if found
      if (testMap.find(testName + "Test") != testMap.end())
      {
        testFunction func = testMap.at(testName + "Test");
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


  testMap.insert(std::make_pair("ReadScenarios", &PulseEngineTest::ReadScenarios));
  testMap.insert(std::make_pair("ReuseEngine", &PulseEngineTest::ReuseEngine));

  //Fill a map that ties unit test names to their actual functions
  testMap.insert(std::make_pair("CardiovascularCircuitAndTransportTest", cardioCTTest));
  testMap.insert(std::make_pair("CardiovascularAndRenalCircuitAndTransportTest", &PulseEngineTest::CardiovascularAndRenalCircuitAndTransportTest));
  testMap.insert(std::make_pair("CardiovascularAndTissueCircuitAndTransportTest", &PulseEngineTest::CardiovascularAndTissueCircuitAndTransportTest));
  testMap.insert(std::make_pair("CardiovascularAndCerebrospinalFluidCircuitAndTransportTest", &PulseEngineTest::CardiovascularAndCerebrospinalFluidCircuitAndTransportTest));
  testMap.insert(std::make_pair("FullCardiovascularCircuitAndTransportTest", &PulseEngineTest::FullCardiovascularCircuitAndTransportTest));
  testMap.insert(std::make_pair("CardiovascularBloodGasesTest", cardioBGTest));
  testMap.insert(std::make_pair("TuneCardiovascularCircuitTest", cardioTuneCircuitTest));
  testMap.insert(std::make_pair("CardiovascularCircuitScaleTests", &PulseEngineTest::CardiovascularCircuitScaleTests));

  testMap.insert(std::make_pair("RenalCircuitAndTransportTest", &PulseEngineTest::RenalCircuitAndTransportTest));
  testMap.insert(std::make_pair("RenalTGFFeedbackTest", &PulseEngineTest::RenalTGFFeedbackTest));
  testMap.insert(std::make_pair("RenalTGFandUPRFeedbackTest", &PulseEngineTest::RenalTGFandUPRFeedbackTest));
  testMap.insert(std::make_pair("RenalSecretionTest", &PulseEngineTest::RenalSecretionTest));
  testMap.insert(std::make_pair("RenalUrinateTest", &PulseEngineTest::RenalUrinateTest));

  testMap.insert(std::make_pair("RespiratoryCircuitAndTransportTest", respCTTest));
  testMap.insert(std::make_pair("RespiratoryDriverTest", &PulseEngineTest::RespiratoryDriverTest));

  testMap.insert(std::make_pair("AnesthesiaMachineCircuitAndTransportTest", anesthCTTest));
  testMap.insert(std::make_pair("RespiratoryWithAnesthesiaMachineCircuitAndTransportTest", &PulseEngineTest::RespiratoryWithAnesthesiaMachineCircuitAndTransportTest));

  testMap.insert(std::make_pair("MechanicalVentilatorCircuitAndTransportTest", ventCTTest));
  testMap.insert(std::make_pair("RespiratoryWithMechanicalVentilatorCircuitAndTransportTest", &PulseEngineTest::RespiratoryWithMechanicalVentilatorCircuitAndTransportTest));

  testMap.insert(std::make_pair("RespiratoryWithInhalerCircuitAndTransportTest", &PulseEngineTest::RespiratoryWithInhalerCircuitAndTransportTest));

  testMap.insert(std::make_pair("RespiratoryWithMechanicalVentilationCircuitAndTransportTest", &PulseEngineTest::RespiratoryWithMechanicalVentilationCircuitAndTransportTest));

  testMap.insert(std::make_pair("InternalTemperatureVariableBMRCircuitTest", &PulseEngineTest::InternalTemperatureVariableBMRCircuitTest));
  testMap.insert(std::make_pair("InternalTemperatureVariableSkinCircuitTest", &PulseEngineTest::InternalTemperatureVariableSkinCircuitTest));
  testMap.insert(std::make_pair("InternalTemperatureVariableCoreCircuitTest", &PulseEngineTest::InternalTemperatureVariableCoreCircuitTest));
  testMap.insert(std::make_pair("EnvironmentVariableTemperatureCircuitTest", &PulseEngineTest::EnvironmentVariableTemperatureCircuitTest));
  testMap.insert(std::make_pair("CombinedInternalAndEnvironmentVariableBMRandTemperatureCircuitTest", &PulseEngineTest::CombinedInternalAndEnvironmentVariableBMRandTemperatureCircuitTest));
  testMap.insert(std::make_pair("CombinedInternalAndEnvironmentSkinTempDropCircuitTest", &PulseEngineTest::CombinedInternalAndEnvironmentSkinTempDropCircuitTest));
  testMap.insert(std::make_pair("EnvironmentISO7730ComparisonTest", &PulseEngineTest::EnvironmentISO7730ComparisonTest));

  testMap.insert(std::make_pair("DiffusionClearanceExcretionTests", &PulseEngineTest::DiffusionClearanceExcretionTests));
  testMap.insert(std::make_pair("AlveolarCarbonDioxideDiffusionTest", &PulseEngineTest::AlveolarCarbonDioxideDiffusionTest));
  testMap.insert(std::make_pair("AlveolarOxygenDiffusionTest", &PulseEngineTest::AlveolarOxygenDiffusionTest));
  testMap.insert(std::make_pair("InstantPlusSimpleDiffusionTest", &PulseEngineTest::InstantPlusSimpleDiffusionTest));
  testMap.insert(std::make_pair("SimpleDiffusionTwoCompartmentTest", &PulseEngineTest::SimpleDiffusionTwoCompartmentTest));
  testMap.insert(std::make_pair("SimpleDiffusionFourCompartmentTest", &PulseEngineTest::SimpleDiffusionFourCompartmentTest));
  testMap.insert(std::make_pair("SimpleDiffusionHierarchyTest", &PulseEngineTest::SimpleDiffusionHierarchyTest));
  testMap.insert(std::make_pair("FacilitatedDiffusionTest", &PulseEngineTest::FacilitatedDiffusionTest));

  testMap.insert(std::make_pair("BrainInjuryTest", &PulseEngineTest::BrainInjuryTest));

  testMap.insert(std::make_pair("ConditionCombinations", &PulseEngineTest::ConditionCombinations));
  testMap.insert(std::make_pair("MultiEngineTest", &PulseEngineTest::MultiEngineTest));
  testMap.insert(std::make_pair("SerializationTest", &PulseEngineTest::SerializationTest));
  testMap.insert(std::make_pair("SolverSpeedTest", &PulseEngineTest::SolverSpeedTest));

  testMap.insert(std::make_pair("AcidBaseMathTest", &PulseEngineTest::AcidBaseMathTest));
  testMap.insert(std::make_pair("AcidBaseFeedbackTest", &PulseEngineTest::AcidBaseFeedbackTest));
  testMap.insert(std::make_pair("AcidBaseLimitsTest", &PulseEngineTest::AcidBaseLimitsTest));
  testMap.insert(std::make_pair("AcidBaseExtremeTest", &PulseEngineTest::AcidBaseExtremeTest));
  testMap.insert(std::make_pair("AcidBaseBloodGasTests", &PulseEngineTest::AcidBaseBloodGasTests));

  testMap.insert(std::make_pair("AerosolTest", &PulseEngineTest::AerosolTest));

  testMap.insert(std::make_pair("FourCompartmentTestSimple", &PulseEngineTest::FourCompartmentTestSimple));
  testMap.insert(std::make_pair("AcidBaseFourCompartmentTest", &PulseEngineTest::AcidBaseFourCompartmentTest));
  testMap.insert(std::make_pair("FiveCompartmentTestWithDiffusion", &PulseEngineTest::FiveCompartmentTestWithDiffusion));
  testMap.insert(std::make_pair("AcidBaseFourCompartmentTestWithProductionConsumption", &PulseEngineTest::AcidBaseFourCompartmentTestWithProductionConsumption));
  testMap.insert(std::make_pair("AcidBaseFiveCompartmentTestWithDiffusion", &PulseEngineTest::AcidBaseFiveCompartmentTestWithDiffusion));
  testMap.insert(std::make_pair("AcidBaseFiveCompartmentTestWithProductionConsumptionAndDiffusion", &PulseEngineTest::AcidBaseFiveCompartmentTestWithProductionConsumptionAndDiffusion));
  
}
