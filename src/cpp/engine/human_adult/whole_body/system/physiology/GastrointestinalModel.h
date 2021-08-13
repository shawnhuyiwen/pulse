/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/physiology/GastrointestinalModel.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace HUMAN_ADULT_WHOLE_BODY
{
  class PULSE_DECL GastrointestinalModel : public PULSE_ENGINE::GastrointestinalModel
  {
    friend class EngineTest;
  public:
    GastrointestinalModel(PULSE_ENGINE::Data& data) : PULSE_ENGINE::GastrointestinalModel(data) {}
    virtual ~GastrointestinalModel() = default;
  };
}
