/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/physiology/TissueModel.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace HUMAN_ADULT_WHOLE_BODY
{
  class PULSE_DECL TissueModel : public PULSE_ENGINE::TissueModel
  {
    friend class EngineTest;
  public:
    TissueModel(PULSE_ENGINE::Data& data) : PULSE_ENGINE::TissueModel(data) {}
    virtual ~TissueModel() = default;
  };
}
