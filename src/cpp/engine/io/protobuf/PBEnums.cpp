/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "engine/CommonDefs.h"
#include "engine/common/controller/Controller.h"
PUSH_PROTO_WARNINGS
#include "pulse/engine/bind/Enums.pb.h"
POP_PROTO_WARNINGS

namespace PULSE_ENGINE
{
  const std::string& eAirwayMode_Name(eAirwayMode m)
  {
    return PULSE_BIND::eAirwayMode_Name((PULSE_BIND::eAirwayMode)m);
  }
}
