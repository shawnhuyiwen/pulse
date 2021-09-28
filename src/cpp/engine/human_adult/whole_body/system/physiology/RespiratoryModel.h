/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/physiology/RespiratoryModel.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace pulse { namespace human_adult_whole_body
{
  class PULSE_DECL RespiratoryModel : public pulse::RespiratoryModel
  {
    friend class EngineTest;
  public:
    RespiratoryModel(pulse::Data& data) : pulse::RespiratoryModel(data) {}
    virtual ~RespiratoryModel() = default;
  };
END_NAMESPACE_EX
