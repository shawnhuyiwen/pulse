/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "bind/cdm/EngineEnums.pb.h"
#include "engine/SEDataRequest.h"

const std::string& eDataRequest_Category_Name(eDataRequest_Category m)
{
  return cdm::eDataRequest_Category_Name((cdm::eDataRequest_Category)m);
}

const std::string& eDecimalFormat_Type_Name(eDecimalFormat_Type m)
{
  return cdm::eDecimalFormat_Type_Name((cdm::eDecimalFormat_Type)m);
}
