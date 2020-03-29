/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class PulseEngine;
PULSE_BIND_DECL(State)


class PULSE_DECL PBPulseState
{
public:

  static void Load(const PULSE_BIND::StateData& src, PulseEngine& dst, const SEScalarTime* simTime, const SEEngineConfiguration* config);
  static PULSE_BIND::StateData* Unload(const PulseEngine& src);
  static bool Serialize(const PULSE_BIND::StateData& src, PulseEngine& dst, const SEScalarTime* simTime, const SEEngineConfiguration* config);
  static bool Serialize(const PulseEngine& src, PULSE_BIND::StateData& dst);

  static bool SerializeToString(const PulseEngine& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const PulseEngine& src, const std::string& filename, SerializationFormat m);
  static bool SerializeFromString(const std::string& src, PulseEngine& dst, SerializationFormat m, const SEScalarTime* simTime, const SEEngineConfiguration* config);
  static bool SerializeFromFile(const std::string& filename, PulseEngine& dst, SerializationFormat m, const SEScalarTime* simTime, const SEEngineConfiguration* config);

};
