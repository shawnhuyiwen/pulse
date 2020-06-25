/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "EngineTest.h"
#include "CommonDataModel.h"
#include "utils/testing/SETestReport.h"
#include "utils/testing/SETestCase.h"
#include "utils/testing/SETestSuite.h"
#include "patient/conditions/SEChronicAnemia.h"
#include "engine/SEPatientConfiguration.h"
#include "engine/SEConditionManager.h"

#include "properties/SEScalar0To1.h"
#include "properties/SEScalar0To1.h"
#include "utils/GeneralMath.h"

void PulseEngineTest::ConditionCombinations(const std::string& rptDirectory)
{
  Logger log("ConditionsPermutationsReport.log");

  std::unique_ptr<PhysiologyEngine> physEng = CreatePulseEngine(&log);
  SEPatientConfiguration pc(&log);

  std::vector<SECondition*> conditions;
  SEChronicAnemia cAnem;
  cAnem.GetReductionFactor().SetValue(0.1);
  conditions.push_back(&cAnem);

  SEPatientConfiguration sceConfig(&log);
  pc.SetPatientFile("StandardMale.json");

  std::vector<int> conditionSwitches;
  for (unsigned int i = 0; i < conditions.size(); i++)
    conditionSwitches.push_back(1);
  // Conditions will either be on or off when we run
  // Now calculate all the permutations with our conditions
  std::vector<std::vector<int>> permutations;
  GeneralMath::Combinations(conditionSwitches, permutations);
  std::stringstream ss;

  SETestReport testReport(&log);
  SETestSuite& testSuite = testReport.CreateTestSuite();
  testSuite.SetName("ConditionPermutations");

  //for (int i = 0; i < 3; i++)
  {
    // Step severity from 0.1 to 0.5 to 1.0
    for (std::vector<int> combo : permutations)
    {
      ss.clear();
      sceConfig.GetConditions().Clear();
      for (unsigned int c = 0; c < conditions.size(); c++)
      {
        if (combo[c] == 1)
        {
          sceConfig.GetConditions().Copy(*conditions[c], physEng->GetSubstanceManager());
          ss << conditions[c]->GetName() << "-";
        }
      }
      if (ss.str().empty())
         ss << "NoConditions";
      SETestCase& testCase = testSuite.CreateTestCase();
      log.Info(ss);
      if (!physEng->InitializeEngine(sceConfig))
      {
        testCase.AddFailure("Unable to stabilize condition permutation");
      }  
    }
  }
  testReport.SerializeToFile("ConditionsPermutationsReport.json");

}