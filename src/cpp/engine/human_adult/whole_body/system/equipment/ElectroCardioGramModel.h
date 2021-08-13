/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/equipment/ElectroCardioGramModel.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace HUMAN_ADULT_WHOLE_BODY
{
  class PULSE_DECL ElectroCardioGramModel : public PULSE_ENGINE::ElectroCardioGramModel
  {
    friend class EngineTest;
  public:
    ElectroCardioGramModel(PULSE_ENGINE::Data& data) : PULSE_ENGINE::ElectroCardioGramModel(data) {}
    virtual ~ElectroCardioGramModel() = default;
  };
}
