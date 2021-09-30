/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/physiology/DrugModel.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace pulse { namespace human_adult_whole_body
{
  class PULSE_DECL DrugModel : public pulse::DrugModel
  {
    friend class EngineTest;
  public:
    DrugModel(pulse::Data& data) : pulse::DrugModel(data) {}
    virtual ~DrugModel() = default;
  };
END_NAMESPACE_EX
