/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

PULSE_BIND_DECL(Scenario)
#include "PulseScenario.h"

namespace pulse
{
  class PULSE_DECL PBScenario
  {
  public:

    static void Load(const PULSE_BIND::ScenarioData& src, PulseScenario& dst);
    static PULSE_BIND::ScenarioData* Unload(const PulseScenario& src);
    static void Serialize(const PULSE_BIND::ScenarioData& src, PulseScenario& dst);
    static void Serialize(const PulseScenario& src, PULSE_BIND::ScenarioData& dst);
    static void Copy(const PulseScenario& src, PulseScenario& dst);

    static bool SerializeToString(const PulseScenario& src, std::string& output, eSerializationFormat m);
    static bool SerializeToFile(const PulseScenario& src, const std::string& filename);
    static bool SerializeFromString(const std::string& src, PulseScenario& dst, eSerializationFormat m);
    static bool SerializeFromFile(const std::string& filename, PulseScenario& dst);
  };
}