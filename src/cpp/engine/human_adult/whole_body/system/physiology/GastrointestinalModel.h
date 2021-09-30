/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/physiology/GastrointestinalModel.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace pulse { namespace human_adult_whole_body
{
  class PULSE_DECL GastrointestinalModel : public pulse::GastrointestinalModel
  {
    friend class EngineTest;
  public:
    GastrointestinalModel(pulse::Data& data) : pulse::GastrointestinalModel(data) {}
    virtual ~GastrointestinalModel() = default;
  };
END_NAMESPACE_EX
