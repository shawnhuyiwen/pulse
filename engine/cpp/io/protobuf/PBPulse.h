/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class PulseScenario;
namespace pulse { namespace proto { class ScenarioData; } }


class PULSE_DECL PBPulse
{
public:

  static void Load(const pulse::proto::ScenarioData& src, PulseScenario& dst);
  static pulse::proto::ScenarioData* Unload(const PulseScenario& src);
  static void Serialize(const pulse::proto::ScenarioData& src, PulseScenario& dst);
  static void Serialize(const PulseScenario& src, pulse::proto::ScenarioData& dst);

  static bool SerializeToString(const PulseScenario& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const PulseScenario& src, const std::string& filename, SerializationFormat m);
  static bool SerializeFromString(const std::string& src, PulseScenario& dst, SerializationFormat m);
  static bool SerializeFromFile(const std::string& filename, PulseScenario& dst, SerializationFormat m);
};