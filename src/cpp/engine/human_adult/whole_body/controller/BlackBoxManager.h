/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/controller/BlackBoxManager.h"
#include "engine/human_adult/whole_body/Engine.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace HUMAN_ADULT_WHOLE_BODY
{
  class PULSE_DECL BlackBoxManager : public PULSE_ENGINE::BlackBoxManager
  {
    friend class EngineTest;
  public:
    BlackBoxManager(PULSE_ENGINE::Controller& c) : PULSE_ENGINE::BlackBoxManager(c) {}
    virtual ~BlackBoxManager() = default;
  };
}
