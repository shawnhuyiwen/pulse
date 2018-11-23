/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "substance/SESubstance.h"
#include "substance/SESubstancePhysicochemicals.h"
#include "bind/cpp/cdm/SubstanceEnums.pb.h"

const std::string& eSubstance_State_Name(eSubstance_State m)
{
  return cdm::eSubstance_State_Name((cdm::eSubstance_State)m);
}
const std::string& eSubstance_IonicState_Name(eSubstance_IonicState m)
{
  return cdm::eSubstance_IonicState_Name((cdm::eSubstance_IonicState)m);
}
const std::string& eSubstance_BindingProtein_Name(eSubstance_BindingProtein m)
{
  return cdm::eSubstance_BindingProtein_Name((cdm::eSubstance_BindingProtein)m);
}
