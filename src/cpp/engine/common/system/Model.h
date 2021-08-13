/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/controller/Controller.h"

namespace PULSE_ENGINE
{
  class Data;
  /**
   * @brief Generic class for handling the %Pulse stabilization methodology
   */
  class PULSE_DECL Model
  {
  public:
    Model(Data& data) : m_data(data) {}
    virtual ~Model() = default;

    /** @brief - Default system values to their homeostatic values */
    virtual void Initialize() { SetUp(); }// NOT called when loading a state
    virtual void SetUp() = 0; // Called after Initialize if stablizing, or after serialization if loading in a state

    /// Notify systems that steady state has been achieved
    virtual void AtSteadyState() = 0;
    virtual void PreProcess() = 0;
    // This is where any circuits would be solved or graphs transported
    virtual void Process(bool solve_and_transport = true) = 0;
    virtual void PostProcess(bool solve_and_transport = true) = 0;

  protected:
    Data& m_data;
  };
}
