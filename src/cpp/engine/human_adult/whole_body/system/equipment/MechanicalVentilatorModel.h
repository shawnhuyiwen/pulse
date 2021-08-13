/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/equipment/MechanicalVentilatorModel.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace HUMAN_ADULT_WHOLE_BODY
{
  class PULSE_DECL MechanicalVentilatorModel : public PULSE_ENGINE::MechanicalVentilatorModel
  {
    friend class EngineTest;
  public:
    MechanicalVentilatorModel(PULSE_ENGINE::Data& data) : PULSE_ENGINE::MechanicalVentilatorModel(data) {}
    virtual ~MechanicalVentilatorModel() = default;
  };
}
