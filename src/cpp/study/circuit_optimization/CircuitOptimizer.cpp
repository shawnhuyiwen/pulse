/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "CircuitOptimizer.h"

#include "engine/human_adult/hemodynamics/Engine.h"
#include "engine/common/controller/CircuitManager.h"

#include "cdm/engine/SEEventManager.h"
#include "cdm/engine/SEEngineTracker.h"
#include "cdm/engine/SEDataRequestManager.h"
#include "cdm/engine/SEPatientConfiguration.h"
#include "cdm/utils/DataTrack.h"

#include "Eigen/Dense"

namespace pulse::study::circuit_optimization
{

  class MyEventHandler : public SEEventHandler
  {
  public:
    MyEventHandler() : SEEventHandler() {}
   
    
  };

  CircuitOptimizer::CircuitOptimizer(Logger* logger) : Loggable(logger), m_SubMgr(logger)
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

  bool CircuitOptimizer::ConvergeToHemodynamicsTargets(size_t maxLoops, std::vector<SEValidationTarget*>& targets)
  {
    // Start with the default modifiers
    PulseConfiguration cfg(GetLogger());
    // Which modifier set do we want to start with?
    cfg.Initialize(); // Use the current defaults, or
    //cfg.SerializeFromFile("modifiesr.json", m_SubMgr);// Use a file we previously generated

    // Ye Han, This is how you write a modifier set you want to save on disk
    // delete this section when you grok it
    //for (auto& i : cfg.GetModifiers())
    //  i.second = 0.123;
    //WriteModifiers(cfg,"modifiesr.json");

    int nTarget = (int)targets.size();
    Info("Number of modifiers: " + std::to_string(cfg.GetModifiers().size()));
    Info("Number of targets: " + std::to_string(nTarget));
    for (auto& [name, modifier] : cfg.GetModifiers())
      Info("Default modifier " + name + ": " + std::to_string(modifier.value));

    bool converged;
    std::string check;
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
      Info("Checking simulation results...[Expected] [Value] [Error]");
      converged = true;
      int nFail = 0;
      double errorNorm  = 0;
      for (SEValidationTarget* vt : targets)
      {
        check = "PASS ";
        if (vt->GetError() > 10.0)// Just a guess here...
        {
          check = "FAIL ";
          nFail++;
          converged = false;
        }
        Info(check + vt->GetCompartmentName() + " " + vt->GetPropertyName() +
          " [" + pulse::cdm::to_string(vt->GetRangeMin()) + ", " + pulse::cdm::to_string(vt->GetRangeMax()) + "] " +
          pulse::cdm::to_string(vt->GetValue()) + " " + pulse::cdm::to_string(vt->GetError()) + "%");
        errorNorm += vt->GetError() * vt->GetError();
      }
      Info("Total error norm (l2): " + std::to_string(errorNorm) +
        " Passed: " + std::to_string(nTarget - nFail) + " Failed: " + std::to_string(nFail));
      
      // 3. Test Convergence Criteria
      if (converged)
      {
        Info("We have successfully tuned the circuit!");
        WriteModifiers(cfg, "PulseConfig.json");
        return true;
      }

      // 4. We did not converge, so compute a new modifier set to test
      if (!ComputeNewModifiers(cfg, targets))
        return false;

      Info("Finished Modifier Loop " + std::to_string(i+1));
    }

    // Check final results
    if (!GenerateHemodynamicsData(cfg, targets))
    {
      Fatal("Error generating data");
      return false;
    }
    Info("Checking simulation results...[Expected] [Value] [Error]");
    converged = true;
    int nFail = 0;
    double errorNorm  = 0;
    for (SEValidationTarget* vt : targets)
    {
      check = "PASS ";
      if (vt->GetError() > 10.0)// Just a guess here...
      {
        check = "FAIL ";
        converged = false;
        nFail++;
      }
      Info(check + vt->GetCompartmentName() + " " + vt->GetPropertyName() +
           " [" + pulse::cdm::to_string(vt->GetRangeMin()) + ", " + pulse::cdm::to_string(vt->GetRangeMax()) + "] " +
           pulse::cdm::to_string(vt->GetValue()) + " " + pulse::cdm::to_string(vt->GetError()) + "%");
      errorNorm += vt->GetError() * vt->GetError();
    }
    Info("Final total error norm (l2): " + std::to_string(errorNorm) +
         " Passed: " + std::to_string(nTarget - nFail) + " Failed: " + std::to_string(nFail));
    return false;
  }

  bool CircuitOptimizer::GenerateHemodynamicsData(PulseConfiguration& cfg, std::vector<SEValidationTarget*>& targets)
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
    std::map<SEValidationTarget*, SEValidationTarget*> t2e;
    engine.GetEngineTracker()->SetTrackMode(TrackMode::Dynamic); // No file needed
    SEDataRequestManager& drMgr = engine.GetEngineTracker()->GetDataRequestManager();
    for (SEValidationTarget* vt : targets)
    {
      // Reset the computed data in our targets
      vt->GetData().clear();
      // Cache off the mapping of our target to this engine
      t2e[vt] = &drMgr.CopyValidationTarget(*vt);
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
      for (SEValidationTarget* vt : targets)
      {
        if (m_StartOfCardiacCycle)
          vt->GetData().clear();
        vt->GetData().push_back(engine.GetEngineTracker()->GetValue(*t2e[vt]));
      }
      m_StartOfCardiacCycle = false;
    }
    for (SEValidationTarget* vt : targets)
    {
      if (!vt->ComputeError())
      {
        Error("Check target dataset");
        return false;
      }
    }
    Info("Finished simulation " +std::to_string(++m_SimulationNum));

    return true;
  }
  bool CircuitOptimizer::ComputeNewModifiers(PulseConfiguration& cfg, std::vector<SEValidationTarget*>& targets)
  {
    // TODO: No bounding constraint has been imposed yet.

    // Manually choose modifiers and targets
    std::vector<std::string> modifiers;
    modifiers.emplace_back(pulse::CardiovascularPath::Aorta3ToAorta1);
    modifiers.emplace_back(pulse::CardiovascularPath::VenaCavaToGround);
//    modifiers.emplace_back(pulse::CardiovascularPath::Aorta1ToLargeIntestine);
//    modifiers.emplace_back(pulse::CardiovascularPath::Aorta1ToLeftKidney1); // doesn't change anything solely, causing stabilization issue jointly

    auto nX = (int)modifiers.size();
    auto nY = (int)targets.size();
    double finiteDiferenceStepSize = 0.001; // step size for finite differencing
    double stepRatio = 0.1; // step ratio for gradient descent

    // Diff/Error/Y matrix for output targets
    Eigen::MatrixXd diffY(nY, 1);
    for (size_t i = 0; i < targets.size(); i++)
      diffY.coeffRef(i, 0) = targets[i]->GetError();

    // Jacobian matrix
    Eigen::MatrixXd Jacobian(nY, nX);

    // Compute entries in Jacobian matrix
    // Individually step each modifier and compute
    size_t i = 0;
    for (auto& name : modifiers)
    {
      // Step one modifier and compute
      double val = cfg.GetModifiers()[name].value; // cache it
      cfg.GetModifiers()[name].value = val + finiteDiferenceStepSize;
      if (!GenerateHemodynamicsData(cfg, targets)) // TODO this should be a function pointer
      {
        Fatal("Error generating data");
        return false;
      }

      // Put all target errors for this one modifier step in our matrix
      int j = 0;
      for (SEValidationTarget* vt : targets)
      {
        Jacobian.coeffRef(j, i)  = (vt->GetError() - diffY.coeffRef(j, 0)) / finiteDiferenceStepSize;
        j++;
      }

      // Reset the modifier from the cache
      cfg.GetModifiers()[name].value = val;
      i++;
    }

    // Least squares fitting using uniform weights.
    // Assume number of targets is equal or greater than the number of input variables/modifiers,
    // otherwise J^T * J is non-invertible, and we will need to use other solution method (e.g. null-space method)
    Eigen::MatrixXd deltaX = (Jacobian.transpose() * Jacobian).inverse() * (Jacobian.transpose() * diffY);

    // Put the new modifier set into the config
    i = 0;
    for (auto& name : modifiers)
    {
      double val = cfg.GetModifiers()[name].value;
      cfg.GetModifiers()[name].value = val - (stepRatio * deltaX.coeff(i, 0));
      i++;
    }
    return true;
  }

  void CircuitOptimizer::WriteModifiers(const PulseConfiguration& cfg, const std::string& filename)
  {
    PulseConfiguration out;
    // We only want the modifiers from the given cfg
    out.GetModifiers() = cfg.GetModifiers();
    out.SerializeToFile(filename);
  }
}
