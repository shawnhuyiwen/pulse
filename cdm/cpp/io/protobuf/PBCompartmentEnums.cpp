/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "compartment/SECompartmentManager.h"
#include "bind/Compartment.pb.h"

const std::string& eCompartment_Type_Name(eCompartment_Type m)
{
  return CDM_BIND::CompartmentData::eType_Name((CDM_BIND::CompartmentData::eType)m);
}
