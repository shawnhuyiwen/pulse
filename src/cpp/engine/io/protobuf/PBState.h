/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/common/controller/Controller.h"
PULSE_BIND_DECL(State)

namespace PULSE_ENGINE
{
  class PULSE_DECL PBState
  {
  public:

    static void Load(const PULSE_BIND::StateData& src, Controller& dst, const SEEngineConfiguration* config = nullptr);
    static PULSE_BIND::StateData* Unload(const Controller& src);
    static bool Serialize(const PULSE_BIND::StateData& src, Controller& dst, const SEEngineConfiguration* config = nullptr);
    static bool Serialize(const Controller& src, PULSE_BIND::StateData& dst);

    static bool SerializeFromFile(const std::string& filename, Controller& dst, const SEEngineConfiguration* config = nullptr);
    static bool SerializeToFile(const Controller& src, const std::string& filename);

    static bool SerializeFromString(const std::string& src, Controller& dst, SerializationFormat m, const SEEngineConfiguration* config = nullptr);
    static bool SerializeToString(const Controller& src, std::string& output, SerializationFormat m);
  };
}
