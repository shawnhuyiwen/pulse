/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/equipment/AnesthesiaMachineModel.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace pulse { namespace human_adult_whole_body
{
  class PULSE_DECL AnesthesiaMachineModel :public pulse::AnesthesiaMachineModel
  {
    friend class EngineTest;
  public:
    AnesthesiaMachineModel(pulse::Data& data) : pulse::AnesthesiaMachineModel(data) {}
    virtual ~AnesthesiaMachineModel() = default;
  };
END_NAMESPACE_EX
