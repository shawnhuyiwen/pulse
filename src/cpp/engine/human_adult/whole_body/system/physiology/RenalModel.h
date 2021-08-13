/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/physiology/RenalModel.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace HUMAN_ADULT_WHOLE_BODY
{
  class PULSE_DECL RenalModel : public PULSE_ENGINE::RenalModel
  {
    friend class EngineTest;
  public:
    RenalModel(PULSE_ENGINE::Data& data) : PULSE_ENGINE::RenalModel(data) {}
    virtual ~RenalModel() = default;
  };
}
