/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
PUSH_PROTO_WARNINGS()
#include "io/protobuf/PBBlackBox.h"
#include "pulse/cdm/bind/BlackBox.pb.h"
POP_PROTO_WARNINGS()
#include "blackbox/SEBlackBox.h"

const std::string& eBlackBox_Property_Type_Name(eBlackBox_Property_Type m)
{
  return CDM_BIND::BlackBoxPropertyData::eType_Name((CDM_BIND::BlackBoxPropertyData::eType)m);
}

