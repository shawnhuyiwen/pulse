/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/physiology/CardiovascularModel.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace pulse { namespace human_adult_whole_body
{
  class PULSE_DECL CardiovascularModel : public pulse::CardiovascularModel
  {
    friend class EngineTest;
  public:
    CardiovascularModel(pulse::Data& data) : pulse::CardiovascularModel(data) {}
    virtual ~CardiovascularModel() = default;
  };
END_NAMESPACE_EX
