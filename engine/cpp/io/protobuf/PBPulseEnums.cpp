/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"

#include "bind/cpp/pulse/Pulse.pb.h"
#include "bind/cpp/pulse/PulseEnums.pb.h"
#include "controller/Controller.h"

const std::string& eAirwayMode_Name(eAirwayMode m)
{
  return pulse::proto::eAirwayMode_Name((pulse::proto::eAirwayMode)m);
}