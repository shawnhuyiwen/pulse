/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "CircuitOptimizer.h"

#include "engine/human_adult/hemodynamics/Engine.h"

#include "cdm/engine/SEEventManager.h"
#include "cdm/engine/SEEngineTracker.h"
#include "cdm/engine/SEDataRequestManager.h"
#include "cdm/engine/SEPatientConfiguration.h"

#include "cdm/utils/DataTrack.h"
#include "cdm/utils/GeneralMath.h"

namespace pulse::study::circuit_optimization
{

  class MyEventHandler : public SEEventHandler
  {
  public:
    MyEventHandler() : SEEventHandler() {}
   
    
  };

  CircuitOptimizer::CircuitOptimizer(Logger* logger) : Loggable(logger)
  {
 
  }
  CircuitOptimizer::~CircuitOptimizer()
  {
    
  }

  void CircuitOptimizer::HandleEvent(eEvent type, bool active, const SEScalarTime*)
  {
    if (type == eEvent::StartOfCardiacCycle && active)
    {
      m_CardiacCycle++;
      if (m_CardiacCycle != m_StopAtCycle)
      {
        m_StartOfCardiacCycle = true;
      }
    }
  }

  bool CircuitOptimizer::GenerateData(PulseConfiguration& cfg, std::vector<SEValidationTarget>& targets)
  {
    pulse::human_adult_hemodynamics::Engine engine(GetLogger());

    SEPatientConfiguration pCfg(engine.GetLogger());
    pCfg.SetPatientFile("./patients/StandardMale.json");

    if (!engine.SetConfigurationOverride(&cfg))
    {
      Error("Unable to override configuration");
      return false;
    }

    if (!engine.InitializeEngine(pCfg))
    {
      Error("Unable to initialize engine");
      return false;
    }

    // Listen for Cardiac Cycles
    engine.GetEventManager().ForwardEvents(this);

    // Setup Data Requests
    std::map<SEDataRequest*, SEDataRequest*> t2e;
    engine.GetEngineTracker()->SetTrackMode(TrackMode::Dynamic); // No file needed
    SEDataRequestManager& drMgr = engine.GetEngineTracker()->GetDataRequestManager();
    for (SEValidationTarget& vt : targets)
    {
      // Cache off the mapping of our target to this engine
      t2e[vt.GetDataRequest()] = &drMgr.CopyDataRequest(*vt.GetDataRequest());
    }

    // Advance for 5 Cardiac Cycles
    m_CardiacCycle = 0;
    m_StartOfCardiacCycle = false;
    while (m_CardiacCycle < m_StopAtCycle)
    {
      if (!engine.AdvanceModelTime())
      {
        Error("Unable to advance time");
        return false;
      }
      engine.GetEngineTracker()->TrackData(engine.GetSimulationTime(TimeUnit::s));
      for (SEValidationTarget& vt : targets)
      {
        if (m_StartOfCardiacCycle)
          vt.GetData().clear();
        vt.GetData().push_back(engine.GetEngineTracker()->GetValue(*t2e[vt.GetDataRequest()]));
      }
      m_StartOfCardiacCycle = false;
    }
    for (SEValidationTarget& vt : targets)
    {
      if (!vt.ComputeError())
      {
        Error("Check target dataset");
        return false;
      }
    }
    Info("Finished simulation " +std::to_string(++m_SimulationNum));

    return true;
  }
}
