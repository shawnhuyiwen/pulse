/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/equipment/InhalerModel.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace HUMAN_ADULT_WHOLE_BODY
{
  class PULSE_DECL InhalerModel : public PULSE_ENGINE::InhalerModel
  {
    friend class EngineTest;
  public:
    InhalerModel(PULSE_ENGINE::Data& data) : PULSE_ENGINE::InhalerModel(data) {}
    virtual ~InhalerModel() = default;
  };
}
