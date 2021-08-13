/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/physiology/CardiovascularModel.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace HUMAN_ADULT_WHOLE_BODY
{
  class PULSE_DECL CardiovascularModel : public PULSE_ENGINE::CardiovascularModel
  {
    friend class EngineTest;
  public:
    CardiovascularModel(PULSE_ENGINE::Data& data) : PULSE_ENGINE::CardiovascularModel(data) {}
    virtual ~CardiovascularModel() = default;
  };
}
