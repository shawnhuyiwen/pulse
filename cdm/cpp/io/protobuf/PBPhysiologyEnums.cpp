/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "bind/Physiology.pb.h"
#include "system/physiology/SECardiovascularSystem.h"

const std::string& eHeartRhythm_Name(eHeartRhythm m)
{
  return CDM_BIND::eHeartRhythm_Name((CDM_BIND::eHeartRhythm)m);
}