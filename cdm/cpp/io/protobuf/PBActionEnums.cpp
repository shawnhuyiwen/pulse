/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "bind/cpp/cdm/Actions.pb.h"
#include "engine/SESerializeState.h"

const std::string& eSerialization_Type_Name(eSerialization_Type m)
{
  return cdm::SerializeStateData::eType_Name((cdm::SerializeStateData::eType)m);
}