/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
PUSH_PROTO_WARNINGS
#include "pulse/cdm/bind/Physiology.pb.h"
POP_PROTO_WARNINGS
#include "cdm/system/physiology/SECardiovascularSystem.h"
#include "cdm/system/physiology/SERespiratorySystem.h"

const std::string& eHeartRhythm_Name(eHeartRhythm m)
{
  return CDM_BIND::eHeartRhythm_Name((CDM_BIND::eHeartRhythm)m);
}

const std::string& eBreathState_Name(eBreathState m)
{
  return CDM_BIND::eBreathState_Name((CDM_BIND::eBreathState)m);
}