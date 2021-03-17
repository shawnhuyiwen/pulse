/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class PulseScenario;
PULSE_BIND_DECL(Scenario)

class PULSE_DECL PBPulse
{
public:

  static void Load(const PULSE_BIND::ScenarioData& src, PulseScenario& dst);
  static PULSE_BIND::ScenarioData* Unload(const PulseScenario& src);
  static void Serialize(const PULSE_BIND::ScenarioData& src, PulseScenario& dst);
  static void Serialize(const PulseScenario& src, PULSE_BIND::ScenarioData& dst);
  static void Copy(const PulseScenario& src, PulseScenario& dst);

  static bool SerializeToString(const PulseScenario& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const PulseScenario& src, const std::string& filename);
  static bool SerializeFromString(const std::string& src, PulseScenario& dst, SerializationFormat m);
  static bool SerializeFromFile(const std::string& filename, PulseScenario& dst);
};