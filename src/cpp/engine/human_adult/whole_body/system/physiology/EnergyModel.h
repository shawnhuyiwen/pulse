/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/physiology/EnergyModel.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace pulse { namespace human_adult_whole_body
{
  class PULSE_DECL EnergyModel : public pulse::EnergyModel
  {
    friend class EngineTest;
  public:
    EnergyModel(pulse::Data& data) : pulse::EnergyModel(data) {}
    virtual ~EnergyModel() = default;
  };
END_NAMESPACE_EX
