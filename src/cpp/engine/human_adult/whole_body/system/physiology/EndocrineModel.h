/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/physiology/EndocrineModel.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace pulse { namespace human_adult_whole_body
{
  class PULSE_DECL EndocrineModel : public pulse::EndocrineModel
  {
    friend class EngineTest;
  public:
    EndocrineModel(pulse::Data& data) : pulse::EndocrineModel(data) {}
    virtual ~EndocrineModel() = default;
  };
END_NAMESPACE_EX
