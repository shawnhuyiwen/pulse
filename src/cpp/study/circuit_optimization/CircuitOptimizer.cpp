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
#include "Eigen/Dense"

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

  bool CircuitOptimizer::ConvergeToHemodynamicsTargets(size_t maxLoops, std::vector<SEValidationTarget>& targets)
  {
    // TODO
    // Depending on what circuit we are trying to optimize
    // We should remove modifiers unrelated to the circuit we are optimizing
    // Currently GenerateData only runs the CV circuit, so only those modifiers should be used
    // Currently we only have CV modifiers in the config
    
    // Start with the default modifiers
    PulseConfiguration cfg(GetLogger());
    // Set all modifiers to 1.0
    for (auto& [name, modifier] : cfg.GetModifiers())
      modifier.value = 1.0;

    bool converged;
    for (unsigned int i = 0; i < maxLoops; ++i)// Maximum loops
    {
      // 1. Generate data with current modifiers
      if (!GenerateHemodynamicsData(cfg, targets))
      {
        Fatal("Error generating data");
        return false;
      }
      // 2. Look at the error of each target
      //    (I am just making up convergence criteria)
      converged = true;
      for (SEValidationTarget& vt : targets)
      {
        if (vt.GetError() > 10.0)// Just a guess here...
        {
          converged = false;
          break;
        }
      }
      
      // 3. Test Convergence Criteria
      if (converged)
      {
        Info("We have successfully tuned the circuit!");
        // TODO Print out the winning modifiers
        return true;
      }

      // 4. We did not converge, so compute a new modifier set to test
      if (!ComputeNewModifiers(cfg, targets))
        return false;

      Info("Finished Modifier Loop " + std::to_string(i+1));
    }

    Error("Exceeded maximum number of loops");
    return false;
  }

  bool CircuitOptimizer::GenerateHemodynamicsData(PulseConfiguration& cfg, std::vector<SEValidationTarget>& targets)
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
      // Reset the computed data in our targets
      vt.GetData().clear();
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
  bool CircuitOptimizer::ComputeNewModifiers(PulseConfiguration& cfg, std::vector<SEValidationTarget>& targets)
  {
    // TODO: No bounding constraint has been imposed yet.

    auto nX = cfg.GetModifiers().size();
    auto nY = targets.size();
    double stepSize = 0.001; // magic number, could be too large/small for different inputs

    // Diff/Error/Y matrix for output targets
    Eigen::MatrixXd diffY(nY, 1);
    for (size_t i = 0; i < targets.size(); i++)
      diffY.coeffRef(i, 0) = targets[i].GetError();

    // Jacobian matrix
    Eigen::MatrixXd Jacobian(nY, nX);

    // Compute entries in Jacobian matrix
    // Individually step each modifier and compute
    size_t i = 0;
    for (auto& [name, modifier] : cfg.GetModifiers())
    {
      // Step one modifier and compute
      double val = modifier.value; // cache it
      modifier.value = val + stepSize;
      if (!GenerateHemodynamicsData(cfg, targets)) // TODO this should be a function pointer
      {
        Fatal("Error generating data");
        return false;
      }

      // Put all target errors for this one modifier step in our matrix
      int j = 0;
      for (SEValidationTarget& vt : targets)
      {
        Jacobian.coeffRef(j, i)  = (vt.GetError() - diffY.coeffRef(j, 0)) / stepSize;
        j++;
      }

      // Reset the modifier from the cache
      modifier.value = val;
      i++;
    }

    // Least squares fitting using uniform weights.
    // Assume number of targets is equal or greater than the number of input variables/modifiers,
    // otherwise J^T * J is non-invertible, and we will need to use other solution method (e.g. null-space method)
    Eigen::MatrixXd deltaX = (Jacobian.transpose() * Jacobian).inverse() * (Jacobian.transpose() * diffY);

    // Put the new modifier set into the config
    i = 0;
    for (auto& [name, modifier] : cfg.GetModifiers())
    {
      double val = modifier.value;
      modifier.value = val + (0.1 * deltaX.coeff(i, 0));
      i++;
    }
    return true;
  }
}
