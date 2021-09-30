/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/equipment/InhalerModel.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace pulse { namespace human_adult_whole_body
{
  class PULSE_DECL InhalerModel : public pulse::InhalerModel
  {
    friend class EngineTest;
  public:
    InhalerModel(pulse::Data& data) : pulse::InhalerModel(data) {}
    virtual ~InhalerModel() = default;
  };
END_NAMESPACE_EX
