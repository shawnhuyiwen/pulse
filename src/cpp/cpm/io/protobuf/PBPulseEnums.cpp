/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
PUSH_PROTO_WARNINGS()
#include "pulse/cpm/bind/Pulse.pb.h"
#include "pulse/cpm/bind/PulseState.pb.h"
POP_PROTO_WARNINGS()
#include "controller/Controller.h"

const std::string& eAirwayMode_Name(eAirwayMode m)
{
  return PULSE_BIND::eAirwayMode_Name((PULSE_BIND::eAirwayMode)m);
}
