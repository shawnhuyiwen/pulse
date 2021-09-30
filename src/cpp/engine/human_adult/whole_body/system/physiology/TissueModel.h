/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/physiology/TissueModel.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace pulse { namespace human_adult_whole_body
{
  class PULSE_DECL TissueModel : public pulse::TissueModel
  {
    friend class EngineTest;
  public:
    TissueModel(pulse::Data& data) : pulse::TissueModel(data) {}
    virtual ~TissueModel() = default;
  };
END_NAMESPACE_EX
