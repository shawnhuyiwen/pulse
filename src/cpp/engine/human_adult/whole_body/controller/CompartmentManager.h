/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/controller/CompartmentManager.h"
#include "engine/human_adult/whole_body/Engine.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace pulse { namespace human_adult_whole_body
{
  class PULSE_DECL CompartmentManager : public pulse::CompartmentManager
  {
    friend class EngineTest;
  public:
    CompartmentManager(pulse::Data& data) : pulse::CompartmentManager(data) {}
    virtual ~CompartmentManager() = default;
  };
END_NAMESPACE_EX
