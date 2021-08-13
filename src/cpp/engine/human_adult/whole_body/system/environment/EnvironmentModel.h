/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/environment/EnvironmentModel.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace HUMAN_ADULT_WHOLE_BODY
{
  class PULSE_DECL EnvironmentModel : public PULSE_ENGINE::EnvironmentModel
  {
    friend class EngineTest;
  public:
    EnvironmentModel(PULSE_ENGINE::Data& data) : PULSE_ENGINE::EnvironmentModel(data) {}
    virtual ~EnvironmentModel() = default;
  };
}
