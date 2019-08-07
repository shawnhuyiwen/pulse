/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "bind/cpp/cdm/Physiology.pb.h"
#include "system/physiology/SECardiovascularSystem.h"

const std::string& eHeartRhythm_Name(eHeartRhythm m)
{
  return cdm::eHeartRhythm_Name((cdm::eHeartRhythm)m);
}