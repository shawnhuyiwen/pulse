/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/environment/EnvironmentModel.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace pulse { namespace human_adult_whole_body
{
  class PULSE_DECL EnvironmentModel : public pulse::EnvironmentModel
  {
    friend class EngineTest;
  public:
    EnvironmentModel(pulse::Data& data) : pulse::EnvironmentModel(data) {}
    virtual ~EnvironmentModel() = default;
  };
END_NAMESPACE_EX
