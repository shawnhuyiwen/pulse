/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/controller/CompartmentManager.h"
#include "engine/human_adult/whole_body/Engine.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace HUMAN_ADULT_WHOLE_BODY
{
  class PULSE_DECL CompartmentManager : public PULSE_ENGINE::CompartmentManager
  {
    friend class EngineTest;
  public:
    CompartmentManager(PULSE_ENGINE::Data& data) : PULSE_ENGINE::CompartmentManager(data) {}
    virtual ~CompartmentManager() = default;
  };
}
