/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "PulseEngine.h"
#include "PulseConfiguration.h"

#include "SEValidationTarget.h"

namespace pulse::study::circuit_optimization
{
  class CircuitOptimizer : public Loggable, protected SEEventHandler
  {
  public:
    CircuitOptimizer(Logger* logger=nullptr);
    virtual ~CircuitOptimizer();

    bool GenerateData(PulseConfiguration& cfg, std::vector<SEValidationTarget>& targets);
    std::map<std::string, double> ComputeNewModifiers(PulseConfiguration& cfg,
                                                      std::vector<SEValidationTarget>& targets,
                                                      std::map<std::string, double>& currentModifiers);

    virtual void HandleEvent(eEvent type, bool active, const SEScalarTime* time = nullptr) override;

  protected:
    unsigned int m_SimulationNum = 0;
    unsigned int m_StopAtCycle = 5;
    unsigned int m_CardiacCycle = 0;
    bool         m_StartOfCardiacCycle = false;
  };
}
