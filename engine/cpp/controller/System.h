/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "PulsePhysiologySystems.h"

/**
 * @brief Generic class for handling the %Pulse stabilization methodology
 */
class PULSE_DECL PulseSystem
{
protected:
  friend class PulseEngine;

  /** @brief - Default system values to their homeostatic values */
  virtual void Initialize() { SetUp(); }// NOT called when loading a state
  virtual void SetUp() = 0; // Called after Initialize if stablizing, or after serialization if loading in a state

  /// Notify systems that steady state has been achieved
  virtual void AtSteadyState() {};
  virtual void PreProcess() = 0;
  virtual void Process() = 0;
  virtual void PostProcess() = 0;
};
