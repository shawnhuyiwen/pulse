/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/equipment/ElectroCardioGramModel.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace pulse { namespace human_adult_whole_body
{
  class PULSE_DECL ElectroCardioGramModel : public pulse::ElectroCardioGramModel
  {
    friend class EngineTest;
  public:
    ElectroCardioGramModel(pulse::Data& data) : pulse::ElectroCardioGramModel(data) {}
    virtual ~ElectroCardioGramModel() = default;
  };
END_NAMESPACE_EX
