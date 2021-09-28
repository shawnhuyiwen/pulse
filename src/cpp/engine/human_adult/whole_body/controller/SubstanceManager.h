/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/controller/SubstanceManager.h"
#include "engine/human_adult/whole_body/Engine.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace pulse { namespace human_adult_whole_body
{
  class PULSE_DECL SubstanceManager : public pulse::SubstanceManager
  {
    friend class EngineTest;
  public:
    SubstanceManager(pulse::Data& data) : pulse::SubstanceManager(data) {}
    virtual ~SubstanceManager() = default;
  };
END_NAMESPACE_EX
