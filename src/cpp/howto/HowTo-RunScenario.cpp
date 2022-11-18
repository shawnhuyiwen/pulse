/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "EngineHowTo.h"
#include "PulseEngine.h"
#include "PulseScenarioExec.h"

// Include the various types you will be using in your code
#include "cdm/scenario/SEScenario.h"
#include "cdm/scenario/SEScenarioExec.h"
#include "cdm/engine/SEPatientConfiguration.h"
#include "cdm/engine/SEDataRequestManager.h"
#include "cdm/engine/SEAdvanceTime.h"
#include "cdm/compartment/SECompartmentManager.h"
#include "cdm/engine/SEAdvanceHandler.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/io/protobuf/PBEngine.h"


//--------------------------------------------------------------------------------------------------
/// \brief
/// A class used to handle any specific logic you may want to do each time step
///
/// \details
/// This method will be called at the end of EACH time step of the engine
/// The SEScenarioExecutor will process the advance time actions in a scenario and 
/// step the engine, calling this method each time step
//--------------------------------------------------------------------------------------------------
class MyCustomExec : public SEAdvanceHandler
{
public:
  MyCustomExec() : SEAdvanceHandler(false) { } // Nix callbacks during stabilization
  void OnAdvance(double time_s)
  {
    // you are given the current scenairo time and the engine, so you can do what ever you want
  }
};

//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage of creating and running a scenario
///
/// \details
//--------------------------------------------------------------------------------------------------
void HowToRunScenario()
{
  Logger logger("./test_results/howto/HowTo-RunScenario.cpp/HowToRunScenario.log");

  // Let's make a scenario (you could just point the executor to a scenario json file on disk as well)
  SEScenario sce(&logger);
  sce.SetName("HowToRunScenario");
  sce.SetDescription("Simple Scenario to demonstrate building a scenario by the CDM API");
  sce.GetPatientConfiguration().SetPatientFile("StandardMale.json");
  // Note you can set an Engine state, or create your own SEPatient object (see HowTo-CreateAPatient)
  // When filling out a data request, units are optional
  // The units will be set to whatever units the engine uses.
  // NOTE: The scenario makes it's own copy of these requests
  // Once you set it, any changes will not be reflected in the scenario
  // You can reuse this object for future actions
  std::vector<SEDataRequest*> drs;
  drs.push_back(&sce.GetDataRequestManager().CreatePhysiologyDataRequest("HeartRate",FrequencyUnit::Per_min));
  drs.push_back(&sce.GetDataRequestManager().CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit::Per_min));
  drs.push_back(&sce.GetDataRequestManager().CreatePhysiologyDataRequest("TotalLungVolume", VolumeUnit::mL));

  // Save data requests to a file
  std::string drFile = "./test_results/howto/HowTo-RunScenario.cpp/HowToRunScenarioDataRequests.json";
  if (!PBEngine::SerializeToFile(drs, drFile))
  {
    std::cout << "Failed to serialize data requests" << std::endl;
  }

  // NOTE: the scenario will make it's own copy of this action
  // Once you set it, any changes will not be reflected in the scenario
  // You can reuse this object for future actions
  SEAdvanceTime adv;
  adv.GetTime().SetValue(2, TimeUnit::min);
  sce.AddAction(adv);

  std::string json;
  PulseScenarioExec execOpts(&logger);
  sce.SerializeToString(json, eSerializationFormat::JSON);
  std::cout << json << std::endl;
  execOpts.SetScenarioContent(json);
  execOpts.Execute();

  // Create new scenario that uses data requests saved to file
  SEScenario sce2(&logger);
  sce2.SetName("HowToRunScenario2");
  sce2.SetDescription("Simple Scenario to demonstrate creating data requests via a file");
  sce2.GetPatientConfiguration().SetPatientFile("StandardMale.json");
  sce2.GetDataRequestFiles().push_back(drFile);
  sce2.AddAction(adv);
  std::string json2;
  sce2.SerializeToString(json2, eSerializationFormat::JSON);
  std::cout << json2 << std::endl;
  execOpts.SetScenarioContent(json2);
  execOpts.Execute();

}
