/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/physiology/RenalModel.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace pulse { namespace human_adult_whole_body
{
  class PULSE_DECL RenalModel : public pulse::RenalModel
  {
    friend class EngineTest;
  public:
    RenalModel(pulse::Data& data) : pulse::RenalModel(data) {}
    virtual ~RenalModel() = default;
  };
END_NAMESPACE_EX
