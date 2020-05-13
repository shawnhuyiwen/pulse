/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
PUSH_PROTO_WARNINGS()
#include "pulse/cdm/bind/Actions.pb.h"
POP_PROTO_WARNINGS()
#include "engine/SESerializeState.h"

const std::string& eSerialization_Type_Name(eSerialization_Type m)
{
  return CDM_BIND::SerializeStateData::eType_Name((CDM_BIND::SerializeStateData::eType)m);
}