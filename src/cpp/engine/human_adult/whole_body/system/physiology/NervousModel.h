/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/physiology/NervousModel.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace pulse { namespace human_adult_whole_body
{
  class PULSE_DECL NervousModel : public pulse::NervousModel
  {
    friend class EngineTest;
  public:
    NervousModel(pulse::Data& data) :pulse::NervousModel(data) {}
    virtual ~NervousModel() = default;
  };
END_NAMESPACE_EX
