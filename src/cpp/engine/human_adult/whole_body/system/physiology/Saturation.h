/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/physiology/Saturation.h"

namespace HUMAN_ADULT_WHOLE_BODY
{
  class PULSE_DECL SaturationCalculator : public PULSE_ENGINE::SaturationCalculator
  {
  public:
    SaturationCalculator(PULSE_ENGINE::Data& data) : PULSE_ENGINE::SaturationCalculator(data) {}
    virtual ~SaturationCalculator() = default;
  };
}
