/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "EngineHowTo.h"

#include <memory>

// Include the various types you will be using in your code
#include "PulsePhysiologyEnginePool.h"
#include "engine/SEAdvanceTime.h"
#include "patient/actions/SEHemorrhage.h"
#include "system/physiology/SECardiovascularSystem.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarVolume.h"

#include "utils/TimingProfile.h"

//--------------------------------------------------------------------------------------------------
/// \brief
/// Creating many engines to be run in a thread pool
///
//--------------------------------------------------------------------------------------------------
void HowToPulseEnginePool()
{
  size_t numEngine = 5;
  TimingProfile profiler;
  double sim_time_s = 60;

  SEPhysiologyEnginePool pool;
  pool.GetLogger()->LogToConsole(true);
  pool.GetLogger()->SetLogFile("./test_results/HowToPulseEndingPool/Pool.log");

  pool.Info("Creating a pool with " + std::to_string(pool.GetWorkerCount()) + " workers");
  pool.Info("Creating a pool with " + std::to_string(numEngine) + " engines");
  for (size_t i=0; i<numEngine; i++)
  {
    SEPhysiologyEnginePoolEngine* e = pool.CreateEngine(i);
    e->EngineInitialization.SetStateFilename("./states/StandardMale@0s.json");
    e->EngineInitialization.SetLogFilename("./test_results/HowToPulseEndingPool/PoolEngine"+std::to_string(i)+".log");
  }

  // Only retuns false is ALL engines do not initialize
  // A warning will be logged if any engine could not initialize
  if (!pool.InitializeEngines())
  {
    pool.Fatal("No engines were able to initialize");
    return;
  }

  // There are 2 ways to add actions to engines
  // You can just call Process actions on the pool engine directly
  // The process will occur on this thread
  // ProcessAction just copies the action
  // This is very fast, and fine for all actions execpt SEAdvanceTime
  // SEAdvanceTime actions advance the engine in the ProcessAction call!
  // Since you are using a pool, you want to advance the engine in a thread
  // So add those to the SEPhysiologyEnginePoolEngine Action vector
  // Actions in this vector are processed in a pool thread

  // Add a hemorrhage to one engine on this thread
  SEHemorrhage h;
  h.GetSeverity().SetValue(0.8);
  h.SetType(eHemorrhage_Type::External);
  h.SetCompartment(pulse::VascularCompartment::RightLeg);
  pool.GetEngine(0)->Engine->ProcessAction(h);
  // Advance some time on an engine, in the thread pool
  SEAdvanceTime adv;
  adv.GetTime().SetValue(15, TimeUnit::s);
  pool.GetEngine(0)->Actions.push_back(&adv);
  pool.ProcessActions();


  profiler.Start("s");
  pool.AdvanceModelTime(sim_time_s, TimeUnit::s);
  double elapsed_time_s = profiler.GetElapsedTime_s("s");
  pool.Info("It took " + std::to_string(elapsed_time_s) + "s to simulate " + std::to_string(sim_time_s)+"s");
  pool.Info("Engine 0 Blood Volume(mL) :"+std::to_string(pool.GetEngine(0)->Engine->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL)));
  pool.Info("Engine 1 Blood Volume(mL) :" + std::to_string(pool.GetEngine(1)->Engine->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL)));
}
