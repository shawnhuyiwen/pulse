/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "bind/Actions.pb.h"
#include "engine/SESerializeState.h"

const std::string& eSerialization_Type_Name(eSerialization_Type m)
{
  return CDM_BIND::SerializeStateData::eType_Name((CDM_BIND::SerializeStateData::eType)m);
}