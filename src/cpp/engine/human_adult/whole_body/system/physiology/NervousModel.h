/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/physiology/NervousModel.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace HUMAN_ADULT_WHOLE_BODY
{
  class PULSE_DECL NervousModel : public PULSE_ENGINE::NervousModel
  {
    friend class EngineTest;
  public:
    NervousModel(PULSE_ENGINE::Data& data) :PULSE_ENGINE::NervousModel(data) {}
    virtual ~NervousModel() = default;
  };
}
