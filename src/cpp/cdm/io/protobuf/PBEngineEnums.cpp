/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
PUSH_PROTO_WARNINGS()
#include "pulse/cdm/bind/Engine.pb.h"
POP_PROTO_WARNINGS()
#include "engine/SEDataRequest.h"

const std::string& eDataRequest_Category_Name(eDataRequest_Category m)
{
  return CDM_BIND::DataRequestData::eCategory_Name((CDM_BIND::DataRequestData::eCategory)m);
}

const std::string& eDecimalFormat_Type_Name(eDecimalFormat_Type m)
{
  return CDM_BIND::DecimalFormatData::eType_Name((CDM_BIND::DecimalFormatData::eType)m);
}
