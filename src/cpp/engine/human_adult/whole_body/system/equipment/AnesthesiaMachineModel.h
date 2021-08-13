/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/equipment/AnesthesiaMachineModel.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace HUMAN_ADULT_WHOLE_BODY
{
  class PULSE_DECL AnesthesiaMachineModel :public PULSE_ENGINE::AnesthesiaMachineModel
  {
    friend class EngineTest;
  public:
    AnesthesiaMachineModel(PULSE_ENGINE::Data& data) : PULSE_ENGINE::AnesthesiaMachineModel(data) {}
    virtual ~AnesthesiaMachineModel() = default;
  };
}
