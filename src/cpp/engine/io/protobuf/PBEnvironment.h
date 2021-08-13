/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

PULSE_BIND_DECL(Environment)
#include "engine/common/system/environment/EnvironmentModel.h"

namespace PULSE_ENGINE
{
  class PULSE_DECL PBEnvironment
  {
  public:

    static void Load(const PULSE_BIND::EnvironmentData& src, EnvironmentModel& dst);
    static PULSE_BIND::EnvironmentData* Unload(const EnvironmentModel& src);
    static void Serialize(const PULSE_BIND::EnvironmentData& src, EnvironmentModel& dst);
    static void Serialize(const EnvironmentModel& src, PULSE_BIND::EnvironmentData& dst);
  };
}