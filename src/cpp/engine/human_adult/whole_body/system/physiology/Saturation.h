/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/system/physiology/Saturation.h"

namespace pulse { namespace human_adult_whole_body
{
  class PULSE_DECL SaturationCalculator : public pulse::SaturationCalculator
  {
  public:
    SaturationCalculator(pulse::Data& data) : pulse::SaturationCalculator(data) {}
    virtual ~SaturationCalculator() = default;
  };
END_NAMESPACE_EX
