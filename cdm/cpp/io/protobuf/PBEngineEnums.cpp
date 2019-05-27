/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "bind/cpp/cdm/Engine.pb.h"
#include "engine/SEDataRequest.h"

const std::string& eDataRequest_Category_Name(eDataRequest_Category m)
{
  return cdm::DataRequestData::eCategory_Name((cdm::DataRequestData::eCategory)m);
}

const std::string& eDecimalFormat_Type_Name(eDecimalFormat_Type m)
{
  return cdm::DecimalFormatData::eType_Name((cdm::DecimalFormatData::eType)m);
}
