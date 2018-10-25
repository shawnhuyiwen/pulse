/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL2(AutoSerialization)
CDM_BIND_DECL2(DynamicStabilization)
CDM_BIND_DECL2(DynamicStabilizationEngineConvergence)
CDM_BIND_DECL2(TimedStabilization)

class CDM_DECL PBEngine
{
public:

  static void Load(const cdm::AutoSerializationData& src, SEAutoSerialization& dst);
  static cdm::AutoSerializationData* Unload(const SEAutoSerialization& src);
  static void Serialize(const cdm::AutoSerializationData& src, SEAutoSerialization& dst);
  static void Serialize(const SEAutoSerialization& src, cdm::AutoSerializationData& dst);

  static void Load(const cdm::DynamicStabilizationData& src, SEDynamicStabilization& dst);
  static cdm::DynamicStabilizationData* Unload(const SEDynamicStabilization& src);
  static void Serialize(const cdm::DynamicStabilizationData& src, SEDynamicStabilization& dst);
  static void Serialize(const SEDynamicStabilization& src, cdm::DynamicStabilizationData& dst);

  static bool SerializeToString(const SEDynamicStabilization& src, std::string& output, SerializationMode m);
  static bool SerializeToFile(const SEDynamicStabilization& src, const std::string& filename, SerializationMode);
  static bool SerializeFromString(const std::string& src, SEDynamicStabilization& dst, SerializationMode m);
  static bool SerializeFromFile(const std::string& filename, SEDynamicStabilization& dst, SerializationMode m);

  static void Load(const cdm::DynamicStabilizationEngineConvergenceData& src, SEDynamicStabilizationEngineConvergence& dst);
  static cdm::DynamicStabilizationEngineConvergenceData* Unload(const SEDynamicStabilizationEngineConvergence& src);
  static void Serialize(const cdm::DynamicStabilizationEngineConvergenceData& src, SEDynamicStabilizationEngineConvergence& dst);
  static void Serialize(const SEDynamicStabilizationEngineConvergence& src, cdm::DynamicStabilizationEngineConvergenceData& dst);

  static void Load(const cdm::TimedStabilizationData& src, SETimedStabilization& dst);
  static cdm::TimedStabilizationData* Unload(const SETimedStabilization& src);
  static void Serialize(const cdm::TimedStabilizationData& src, SETimedStabilization& dst);
  static void Serialize(const SETimedStabilization& src, cdm::TimedStabilizationData& dst);

  static bool SerializeToString(const SETimedStabilization& src, std::string& output, SerializationMode m);
  static bool SerializeToFile(const SETimedStabilization& src, const std::string& filename, SerializationMode m);
  static bool SerializeFromString(const std::string& src, SETimedStabilization& dst, SerializationMode m);
  static bool SerializeFromFile(const std::string& filename, SETimedStabilization& dst, SerializationMode m);
};