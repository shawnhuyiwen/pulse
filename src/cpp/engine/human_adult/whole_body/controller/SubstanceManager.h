/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/controller/SubstanceManager.h"
#include "engine/human_adult/whole_body/Engine.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace HUMAN_ADULT_WHOLE_BODY
{
  class PULSE_DECL SubstanceManager : public PULSE_ENGINE::SubstanceManager
  {
    friend class EngineTest;
  public:
    SubstanceManager(PULSE_ENGINE::Data& data) : PULSE_ENGINE::SubstanceManager(data) {}
    virtual ~SubstanceManager() = default;
  };
}
