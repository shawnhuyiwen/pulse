/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
PUSH_PROTO_WARNINGS()
#include "pulse/cdm/bind/Compartment.pb.h"
POP_PROTO_WARNINGS()
#include "compartment/SECompartmentManager.h"

const std::string& eCompartment_Type_Name(eCompartment_Type m)
{
  return CDM_BIND::CompartmentData::eType_Name((CDM_BIND::CompartmentData::eType)m);
}
