/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/Systems.h"
#include "engine/common/system/equipment/BagValveMaskModel.h"

namespace HUMAN_ADULT_WHOLE_BODY
{
  class PULSE_DECL BagValveMaskModel : public PULSE_ENGINE::BagValveMaskModel
  {
    friend class EngineTest;
  public:
    BagValveMaskModel(PULSE_ENGINE::Data& data) : PULSE_ENGINE::BagValveMaskModel(data) {}
    virtual ~BagValveMaskModel() = default;
  };
}
