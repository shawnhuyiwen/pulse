/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "EngineHowTo.h"

// Include the various types you will be using in your code
#include "engine/SEEngineTracker.h"
#include "scenario/SEScenario.h"
#include "engine/SEPatientConfiguration.h"
#include "engine/SEAdvanceTime.h"
#include "engine/SEDataRequestManager.h"
#include "properties/SEScalarTime.h"

//--------------------------------------------------------------------------------------------------
/// \brief
/// Creating an engine based on a scenario file
///
/// \details
/// Read a scenario file and pull things out for your use case
//--------------------------------------------------------------------------------------------------
void HowToScenarioBase()
{
  // Create our engine
  std::unique_ptr<PhysiologyEngine> pe = CreatePulseEngine();
  pe->GetLogger()->SetLogFile("./test_results/HowTo_ScenarioBase.log");
  pe->GetLogger()->Info("HowTo_ScenarioBase");
  
  
  //Let's read the scenario we want to base this engine on
  SEScenario sce(pe->GetLogger());
  sce.SerializeFromFile("YourScenario.json");

  if (sce.HasEngineStateFile())
  {
    if (!pe->SerializeFromFile("./states/StandardMale@0s.json"))
    {
      pe->GetLogger()->Error("Could not load state, check the error");
      return;
    }
  }
  else if (sce.HasPatientConfiguration())
  {
    if (!pe->InitializeEngine(sce.GetPatientConfiguration()))
    {
      pe->GetLogger()->Error("Could not load patient configuration, check the error");
      return;
    }
  }

  // Make a copy of the data requests, note this clears out data requests from the engine
  // This will clear out the data requests if any exist in the DataRequestManager
  pe->GetEngineTracker()->GetDataRequestManager().SerializeToFile("YourDataRequestsFile.json");

  // NOTE : You can just make a DataRequests file that holds only data requests
  // And serialize that in instead of a sceanrio file, if all you want is a consistent
  // This will clear out any requests already in the object
  pe->GetEngineTracker()->GetDataRequestManager().SerializeFromFile("YourDataRequestsFile.json", pe->GetSubstanceManager());
  // Don't need to delete drData as obj is wrapped in a unique_ptr
 

  if (!pe->GetEngineTracker()->GetDataRequestManager().HasResultsFilename())
    pe->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("./ResultsFileName.csv");

  // Let's request data do be tracked that is in the scenario  
  HowToTracker tracker(*pe);
  const SEAdvanceTime* adv;
  // Now run the scenario actions
  for (const SEAction* a : sce.GetActions())
  {
    // We want the tracker to process an advance time action so it will write each time step of data to our track file
    adv = dynamic_cast<const SEAdvanceTime*>(a);
    if (adv != nullptr)
      tracker.AdvanceModelTime(adv->GetTime(TimeUnit::s));// you could just do pe->AdvanceModelTime without tracking timesteps
    else
      pe->ProcessAction(*a);
  }

  // At this point your engine is where you want it to be
  // You could read in a new scenario and run it's actions 
  // or programatically add actions as your applications sees fit

}