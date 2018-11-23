/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "bind/cpp/cdm/EnvironmentEnums.pb.h"
#include "system/environment/SEEnvironmentalConditions.h"

const std::string& eSurroundingType_Name(eSurroundingType m)
{
  return cdm::eEnvironment_SurroundingType_Name((cdm::eEnvironment_SurroundingType)m);
}