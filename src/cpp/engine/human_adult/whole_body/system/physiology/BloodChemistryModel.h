/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/physiology/BloodChemistryModel.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace pulse { namespace human_adult_whole_body
{
  class PULSE_DECL BloodChemistryModel : public pulse::BloodChemistryModel
  {
    friend class EngineTest;
  public:
    BloodChemistryModel(pulse::Data& data) : pulse::BloodChemistryModel(data) {}
    virtual ~BloodChemistryModel() = default;
  };
END_NAMESPACE_EX
