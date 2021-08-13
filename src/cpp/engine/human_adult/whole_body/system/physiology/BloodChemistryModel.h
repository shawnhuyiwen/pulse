/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/physiology/BloodChemistryModel.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace HUMAN_ADULT_WHOLE_BODY
{
  class PULSE_DECL BloodChemistryModel : public PULSE_ENGINE::BloodChemistryModel
  {
    friend class EngineTest;
  public:
    BloodChemistryModel(PULSE_ENGINE::Data& data) : PULSE_ENGINE::BloodChemistryModel(data) {}
    virtual ~BloodChemistryModel() = default;
  };
}
