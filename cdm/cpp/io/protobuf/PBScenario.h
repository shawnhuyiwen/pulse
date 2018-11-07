/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL2(Scenario)

class CDM_DECL PBScenario
{
public:

  static void Load(const cdm::ScenarioData& src, SEScenario& dst);
  static cdm::ScenarioData* Unload(const SEScenario& src);
  static void Serialize(const cdm::ScenarioData& src, SEScenario& dst);
  static void Serialize(const SEScenario& src, cdm::ScenarioData& dst);

  static bool SerializeToString(const SEScenario& src, std::string& output, SerializationMode m);
  static bool SerializeToFile(const SEScenario& src, const std::string& filename, SerializationMode m);
  static bool SerializeFromString(const std::string& src, SEScenario& dst, SerializationMode m);
  static bool SerializeFromFile(const std::string& filename, SEScenario& dst, SerializationMode m);

};