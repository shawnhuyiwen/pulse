/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "engine/human_adult/whole_body/test/EngineTest.h"

#include "cdm/utils/testing/SETestReport.h"
#include "cdm/utils/testing/SETestCase.h"
#include "cdm/utils/testing/SETestSuite.h"
#include "cdm/patient/conditions/SEChronicAnemia.h"
#include "cdm/engine/SEPatientConfiguration.h"
#include "cdm/engine/SEConditionManager.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/utils/GeneralMath.h"

namespace pulse { namespace human_adult_whole_body
{
  void EngineTest::ConditionCombinations(const std::string& /*rptDirectory*/)
  {
    Logger log("ConditionsPermutationsReport.log");

    Engine pe;
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
            sceConfig.GetConditions().Copy(*conditions[c], pe.GetSubstanceManager());
            ss << conditions[c]->GetName() << "-";
          }
        }
        if (ss.str().empty())
          ss << "NoConditions";
        SETestCase& testCase = testSuite.CreateTestCase();
        log.Info(ss);
        if (!pe.InitializeEngine(sceConfig))
        {
          testCase.AddFailure("Unable to stabilize condition permutation");
        }
      }
    }
    testReport.SerializeToFile("ConditionsPermutationsReport.json");
  }
END_NAMESPACE_EX
