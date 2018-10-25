/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class PulseEngine;
namespace pulse { namespace proto { class StateData; } }


class PULSE_DECL PBPulseState
{
public:

  static void Load(const pulse::proto::StateData& src, PulseEngine& dst, const SEScalarTime* simTime, const SEEngineConfiguration* config);
  static pulse::proto::StateData* Unload(const PulseEngine& src);
  static bool Serialize(const pulse::proto::StateData& src, PulseEngine& dst, const SEScalarTime* simTime, const SEEngineConfiguration* config);
  static bool Serialize(const PulseEngine& src, pulse::proto::StateData& dst);

  static bool SerializeToString(const PulseEngine& src, std::string& output, SerializationMode m);
  static bool SerializeToFile(const PulseEngine& src, const std::string& filename, SerializationMode m);
  static bool SerializeFromString(const std::string& src, PulseEngine& dst, SerializationMode m, const SEScalarTime* simTime, const SEEngineConfiguration* config);
  static bool SerializeFromFile(const std::string& filename, PulseEngine& dst, SerializationMode m, const SEScalarTime* simTime, const SEEngineConfiguration* config);

};
