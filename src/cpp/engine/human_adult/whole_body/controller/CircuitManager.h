/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/controller/CircuitManager.h"
#include "engine/human_adult/whole_body/Engine.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace pulse { namespace human_adult_whole_body
{
  class PULSE_DECL CircuitManager : public pulse::CircuitManager
  {
    friend class EngineTest;
  public:
    CircuitManager(pulse::Data& data) : pulse::CircuitManager(data) {}
    virtual ~CircuitManager() = default;
  };
END_NAMESPACE_EX
