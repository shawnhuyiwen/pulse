/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class PulseConfiguration;
namespace pulse { namespace proto { class ConfigurationData; } }

class PULSE_DECL PBPulseConfiguration
{
public:

  static void Load(const pulse::proto::ConfigurationData& src, PulseConfiguration& dst);
  static pulse::proto::ConfigurationData* Unload(const PulseConfiguration& src);
  static void Serialize(const pulse::proto::ConfigurationData& src, PulseConfiguration& dst, bool merge=false);
  static void Serialize(const PulseConfiguration& src, pulse::proto::ConfigurationData& dst);
  static void Merge(const PulseConfiguration& src, PulseConfiguration& dst);

  static bool SerializeToString(const PulseConfiguration& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const PulseConfiguration& src, const std::string& filename, SerializationFormat m);
  static bool SerializeFromString(const std::string& src, PulseConfiguration& dst, SerializationFormat m);
  static bool SerializeFromFile(const std::string& filename, PulseConfiguration& dst, SerializationFormat m);
};