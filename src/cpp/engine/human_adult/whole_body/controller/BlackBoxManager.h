/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/controller/BlackBoxManager.h"
#include "engine/human_adult/whole_body/Engine.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace pulse { namespace human_adult_whole_body
{
  class PULSE_DECL BlackBoxManager : public pulse::BlackBoxManager
  {
    friend class EngineTest;
  public:
    BlackBoxManager(pulse::Controller& c) : pulse::BlackBoxManager(c) {}
    virtual ~BlackBoxManager() = default;
  };
END_NAMESPACE_EX
