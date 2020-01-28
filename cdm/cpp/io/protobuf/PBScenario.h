/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL2(Scenario)

class CDM_DECL PBScenario
{
public:

  static void Load(const CDM_BIND::ScenarioData& src, SEScenario& dst);
  static CDM_BIND::ScenarioData* Unload(const SEScenario& src);
  static void Serialize(const CDM_BIND::ScenarioData& src, SEScenario& dst);
  static void Serialize(const SEScenario& src, CDM_BIND::ScenarioData& dst);

  static bool SerializeToString(const SEScenario& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const SEScenario& src, const std::string& filename, SerializationFormat m);
  static bool SerializeFromString(const std::string& src, SEScenario& dst, SerializationFormat m);
  static bool SerializeFromFile(const std::string& filename, SEScenario& dst, SerializationFormat m);

};