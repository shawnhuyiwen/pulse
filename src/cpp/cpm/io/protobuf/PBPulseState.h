/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class PulseController;
PULSE_BIND_DECL(State)


class PULSE_DECL PBPulseState
{
public:

  static void Load(const PULSE_BIND::StateData& src, PulseController& dst, const SEScalarTime* simTime, const SEEngineConfiguration* config);
  static PULSE_BIND::StateData* Unload(const PulseController& src);
  static bool Serialize(const PULSE_BIND::StateData& src, PulseController& dst, const SEScalarTime* simTime, const SEEngineConfiguration* config);
  static bool Serialize(const PulseController& src, PULSE_BIND::StateData& dst);

  static bool SerializeToString(const PulseController& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const PulseController& src, const std::string& filename, SerializationFormat m);
  static bool SerializeFromString(const std::string& src, PulseController& dst, SerializationFormat m, const SEScalarTime* simTime, const SEEngineConfiguration* config);
  static bool SerializeFromFile(const std::string& filename, PulseController& dst, SerializationFormat m, const SEScalarTime* simTime, const SEEngineConfiguration* config);

};
