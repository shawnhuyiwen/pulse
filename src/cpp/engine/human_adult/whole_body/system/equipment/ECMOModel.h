/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/equipment/ECMOModel.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace pulse { namespace human_adult_whole_body
{
  class PULSE_DECL ECMOModel : public pulse::ECMOModel
  {
    friend class EngineTest;
  public:
    ECMOModel(pulse::Data& data) : pulse::ECMOModel(data) {}
    virtual ~ECMOModel() = default;
  };
END_NAMESPACE_EX
