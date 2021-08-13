/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/physiology/HepaticModel.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace HUMAN_ADULT_WHOLE_BODY
{
  class PULSE_DECL HepaticModel : public PULSE_ENGINE::HepaticModel
  {
    friend class EngineTest;
  public:
    HepaticModel(PULSE_ENGINE::Data& data) : PULSE_ENGINE::HepaticModel(data) {}
    virtual ~HepaticModel() = default;
  };
}
