/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "PulsePhysiologyEngine.h"
#include "cpm/controller/Controller.h"

/**
* @brief Manages and controls execution of all data/systems in an engine
*/
class PULSE_DECL Controller : public PulseController
{
  friend class RespiratoryEngine;
  friend class RespiratoryEngineTest;
protected:// Create via PulseEngine
  Controller(Logger* logger = nullptr);
public:
  virtual ~Controller() = default;

  virtual bool IsAirwayModeSupported(eAirwayMode mode) override;

  virtual bool CreateCircuitsAndCompartments() override;

protected:
  virtual void Allocate() override;
  virtual bool Stabilize(const SEPatientConfiguration& patient_configuration) override;

  virtual void InitializeSystems() override;
  // Notify systems that steady state has been achieved
  virtual void AtSteadyState(EngineState state) override;
  virtual void PreProcess() override;
  virtual void Process() override;
  virtual void PostProcess() override;
};
