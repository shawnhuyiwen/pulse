/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/physiology/DrugModel.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace HUMAN_ADULT_WHOLE_BODY
{
  class PULSE_DECL DrugModel : public PULSE_ENGINE::DrugModel
  {
    friend class EngineTest;
  public:
    DrugModel(PULSE_ENGINE::Data& data) : PULSE_ENGINE::DrugModel(data) {}
    virtual ~DrugModel() = default;
  };
}
