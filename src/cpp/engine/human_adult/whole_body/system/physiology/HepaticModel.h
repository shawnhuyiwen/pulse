/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/physiology/HepaticModel.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace pulse { namespace human_adult_whole_body
{
  class PULSE_DECL HepaticModel : public pulse::HepaticModel
  {
    friend class EngineTest;
  public:
    HepaticModel(pulse::Data& data) : pulse::HepaticModel(data) {}
    virtual ~HepaticModel() = default;
  };
END_NAMESPACE_EX
