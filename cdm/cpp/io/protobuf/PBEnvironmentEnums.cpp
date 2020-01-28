/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "bind/Environment.pb.h"
#include "system/environment/SEEnvironmentalConditions.h"

const std::string& eSurroundingType_Name(eSurroundingType m)
{
  return CDM_BIND::EnvironmentalConditionsData::eSurroundingType_Name((CDM_BIND::EnvironmentalConditionsData::eSurroundingType)m);
}