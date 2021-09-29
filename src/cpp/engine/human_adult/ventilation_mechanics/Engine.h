/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once

#include "engine/human_adult/whole_body/Engine.h"

namespace pulse { namespace human_adult_ventilation_mechanics
{
  //--------------------------------------------------------------------------------------------------
  /// @brief
  /// This is the implementation of the PhysiologyEngine interface for the this engines.
  /// @details
  /// It contains the necessary execution calls, patient customization calls, insult and intervention 
  /// calls as well as assessment calls for obtaining the results. During engine execution a log files
  /// is generated containing information, warning and error data.
  //--------------------------------------------------------------------------------------------------
  class PULSE_DECL Engine : public pulse::human_adult_whole_body::Engine
  {
  public:
    Engine(Logger* logger=nullptr);
    virtual ~Engine() = default;
  protected:
    virtual void AllocateController() const override;
  };
END_NAMESPACE_EX
