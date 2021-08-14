/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "PulseConfiguration.h"
PULSE_BIND_DECL(Configuration)

namespace PULSE_ENGINE
{
  class PULSE_DECL PBConfiguration
  {
  public:

    static void Load(const PULSE_BIND::ConfigurationData& src, PulseConfiguration& dst, SESubstanceManager& subMgr);
    static PULSE_BIND::ConfigurationData* Unload(const PulseConfiguration& src);
    static void Serialize(const PULSE_BIND::ConfigurationData& src, PulseConfiguration& dst, SESubstanceManager& subMgr, bool merge = false);
    static void Serialize(const PulseConfiguration& src, PULSE_BIND::ConfigurationData& dst);
    static void Merge(const PulseConfiguration& src, PulseConfiguration& dst, SESubstanceManager& subMgr);

    static bool SerializeToString(const PulseConfiguration& src, std::string& output, eSerializationFormat m);
    static bool SerializeToFile(const PulseConfiguration& src, const std::string& filename);
    static bool SerializeFromString(const std::string& src, PulseConfiguration& dst, eSerializationFormat m, SESubstanceManager& subMgr);
    static bool SerializeFromFile(const std::string& filename, PulseConfiguration& dst, SESubstanceManager& subMgr);
  };
}