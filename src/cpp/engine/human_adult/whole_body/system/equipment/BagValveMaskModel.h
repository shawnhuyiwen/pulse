/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/Systems.h"
#include "engine/common/system/equipment/BagValveMaskModel.h"

namespace pulse { namespace human_adult_whole_body
{
  class PULSE_DECL BagValveMaskModel : public pulse::BagValveMaskModel
  {
    friend class EngineTest;
  public:
    BagValveMaskModel(pulse::Data& data) : pulse::BagValveMaskModel(data) {}
    virtual ~BagValveMaskModel() = default;
  };
END_NAMESPACE_EX
