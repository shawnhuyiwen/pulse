/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

/**
 * @brief Generic class for handling the %Pulse stabilization methodology
 */
class PULSE_DECL PulseSystem
{
public:
  PulseSystem(PulseData& data) : m_data(data) {}
  virtual ~PulseSystem() = default;
protected:
  /** @brief - Default system values to their homeostatic values */
  virtual void Initialize() { SetUp(); }// NOT called when loading a state
  virtual void SetUp() = 0; // Called after Initialize if stablizing, or after serialization if loading in a state

  /// Notify systems that steady state has been achieved
  virtual void AtSteadyState() = 0;
  virtual void PreProcess() = 0;
  // This is where any circuits would be solved or graphs transported
  virtual void Process(bool solve_and_transport=true) = 0;
  virtual void PostProcess(bool solve_and_transport=true) = 0;

  PulseData& m_data;
};
