/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"

#include "pulse/engine/bind/Pulse.pb.h"
#include "pulse/engine/bind/PulseState.pb.h"
#include "controller/Controller.h"

const std::string& eAirwayMode_Name(eAirwayMode m)
{
  return PULSE_BIND::eAirwayMode_Name((PULSE_BIND::eAirwayMode)m);
}