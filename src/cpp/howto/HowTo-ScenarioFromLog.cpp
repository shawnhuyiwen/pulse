/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "EngineHowTo.h"
#include "PulseEngine.h"

// Include the various types you will be using in your code
#include "cdm/engine/SEEngineTracker.h"
#include "cdm/scenario/SEScenario.h"
#include "cdm/scenario/SEScenarioUtils.h"

//--------------------------------------------------------------------------------------------------
/// \brief
/// Creating a scenario based on log file
///
/// \details
/// Read a log file to re-create the executed scenario
//--------------------------------------------------------------------------------------------------
void HowToScenarioFromLog()
{
  // Create our engine
  std::unique_ptr<PhysiologyEngine> pe = CreatePulseEngine();
  pe->GetLogger()->SetLogFile("./test_results/HowTo_ScenarioFromLog.log");
  pe->GetLogger()->Info("HowTo_ScenarioFromLog");
  
  // Create scenario from log file
  SEScenario sce(pe->GetLogger());
  std::string logFile = "./test_results/pulse-cpr-bvm.log";
  if (!SEScenarioUtils::GenerateScenarioFromLog(logFile, sce, pe->GetLogger()))
  {
    pe->GetLogger()->Error("Could not generate scenario from file, check the error");
    return;
  }
}