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
  std::map<std::string, double> CircuitOptimizer::ComputeNewModifiers(PulseConfiguration& cfg,
                                                                      std::vector<SEValidationTarget>& currentTargets,
                                                                      std::map<std::string, double>& currentModifiers)
  {
    // TODO: No bounding constraint has been imposed yet.

    auto nX = currentModifiers.size();
    auto nY = currentTargets.size();
    double stepSize = 0.001; // magic number, could be too large/small for different inputs

    // Diff/Error/Y matrix for output targets
    Eigen::MatrixXd diffY(nY, 1);
    for (int i = 0; i < currentTargets.size(); i++)
    {
      diffY.coeffRef(i, 0) = currentTargets[i].GetError();
    }

    // Jacobian matrix
    Eigen::MatrixXd Jacobian(nY, nX);

    // Temporary newTargets for finite differencing
    std::vector<SEValidationTarget> newTargets;
    newTargets.reserve(currentTargets.size());
    for (auto target : currentTargets)
    {
      newTargets.push_back(target); // TODO: works?
    }

    // Compute entries in Jacobian matrix
    int i = 0;
    for (auto const & [key, val] : currentModifiers)
    {
      cfg.GetModifiers()[key] = val + stepSize;
      if (!this->GenerateData(cfg, newTargets))
      {
        this->m_Logger->Fatal("Error generating data");
        break;
      }

      int j = 0;
      for (SEValidationTarget& vt : newTargets)
      {
        Jacobian.coeffRef(j, i)  = (vt.GetError() - diffY.coeffRef(j, 0)) / stepSize;
        j++;
      }

      // Reset
      cfg.GetModifiers()[key] = val;
      i++;
    }

    // Least squares fitting using uniform weights.
    // Assume number of targets is equal or greater than the number of input variables/modifiers,
    // otherwise J^T * J is non-invertible, and we will need to use other solution method (e.g. null-space method)
    Eigen::MatrixXd deltaX = (Jacobian.transpose() * Jacobian).inverse() * (Jacobian.transpose() * diffY);

    // Create new modifiers
    std::map<std::string, double> newModifiers;
    i = 0;
    for (auto const & [key, val] : currentModifiers)
    {
      newModifiers[key] = val + 0.1 * deltaX.coeff(i,0);
      i++;
    }
    return newModifiers;
  }
}
