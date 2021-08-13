/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/physiology/EnergyModel.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace HUMAN_ADULT_WHOLE_BODY
{
  class PULSE_DECL EnergyModel : public PULSE_ENGINE::EnergyModel
  {
    friend class EngineTest;
  public:
    EnergyModel(PULSE_ENGINE::Data& data) : PULSE_ENGINE::EnergyModel(data) {}
    virtual ~EnergyModel() = default;
  };
}
