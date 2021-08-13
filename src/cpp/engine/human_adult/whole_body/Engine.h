/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once

#include "engine/common/Engine.h"
#define HUMAN_ADULT_WHOLE_BODY pulse::engine::human_adult::whole_body

namespace HUMAN_ADULT_WHOLE_BODY
{
  //--------------------------------------------------------------------------------------------------
  /// @brief
  /// This is the implementation of the PhysiologyEngine interface for the this engines.
  /// @details
  /// It contains the necessary execution calls, patient customization calls, insult and intervention 
  /// calls as well as assessment calls for obtaining the results. During engine execution a log files
  /// is generated containing information, warning and error data.
  //--------------------------------------------------------------------------------------------------
  class PULSE_DECL Engine : public PULSE_ENGINE::Engine
  {
  public:
    Engine(Logger* logger = nullptr);
    virtual ~Engine() = default;
  protected:
    virtual void AllocateController() const override;
  };
}
