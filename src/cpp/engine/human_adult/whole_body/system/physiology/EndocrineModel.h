/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/physiology/EndocrineModel.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace HUMAN_ADULT_WHOLE_BODY
{
  class PULSE_DECL EndocrineModel : public PULSE_ENGINE::EndocrineModel
  {
    friend class EngineTest;
  public:
    EndocrineModel(PULSE_ENGINE::Data& data) : PULSE_ENGINE::EndocrineModel(data) {}
    virtual ~EndocrineModel() = default;
  };
}
