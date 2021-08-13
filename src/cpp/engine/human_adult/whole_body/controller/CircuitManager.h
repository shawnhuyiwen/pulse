/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/controller/CircuitManager.h"
#include "engine/human_adult/whole_body/Engine.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace HUMAN_ADULT_WHOLE_BODY
{
  class PULSE_DECL CircuitManager : public PULSE_ENGINE::CircuitManager
  {
    friend class EngineTest;
  public:
    CircuitManager(PULSE_ENGINE::Data& data) : PULSE_ENGINE::CircuitManager(data) {}
    virtual ~CircuitManager() = default;
  };
}
