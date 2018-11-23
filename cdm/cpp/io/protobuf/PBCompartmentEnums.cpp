/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "compartment/SECompartmentManager.h"
#include "bind/cpp/cdm/CompartmentEnums.pb.h"

const std::string& eCompartment_Type_Name(eCompartment_Type m)
{
  return cdm::eCompartment_Type_Name((cdm::eCompartment_Type)m);
}
