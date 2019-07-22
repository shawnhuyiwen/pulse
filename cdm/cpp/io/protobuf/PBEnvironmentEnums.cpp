/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "bind/cpp/cdm/Environment.pb.h"
#include "system/environment/SEEnvironmentalConditions.h"

const std::string& eSurroundingType_Name(eSurroundingType m)
{
  return cdm::EnvironmentalConditionsData::eSurroundingType_Name((cdm::EnvironmentalConditionsData::eSurroundingType)m);
}