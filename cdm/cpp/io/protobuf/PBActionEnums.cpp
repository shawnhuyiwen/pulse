/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "bind/cpp/cdm/ActionEnums.pb.h"
#include "engine/SESerializeState.h"

const std::string& eSerialization_Type_Name(eSerialization_Type m)
{
  return cdm::eSerialization_Type_Name((cdm::eSerialization_Type)m);
}