/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/equipment/MechanicalVentilatorModel.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace pulse { namespace human_adult_whole_body
{
  class PULSE_DECL MechanicalVentilatorModel : public pulse::MechanicalVentilatorModel
  {
    friend class EngineTest;
  public:
    MechanicalVentilatorModel(pulse::Data& data) : pulse::MechanicalVentilatorModel(data) {}
    virtual ~MechanicalVentilatorModel() = default;
  };
END_NAMESPACE_EX
