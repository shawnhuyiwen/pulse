/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
PUSH_PROTO_WARNINGS
#include "pulse/cdm/bind/Environment.pb.h"
POP_PROTO_WARNINGS
#include "cdm/system/environment/SEEnvironmentalConditions.h"

const std::string& eSurroundingType_Name(eSurroundingType m)
{
  return CDM_BIND::EnvironmentalConditionsData::eSurroundingType_Name((CDM_BIND::EnvironmentalConditionsData::eSurroundingType)m);
}