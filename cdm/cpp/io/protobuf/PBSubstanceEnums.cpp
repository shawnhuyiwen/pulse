/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "substance/SESubstance.h"
#include "substance/SESubstancePhysicochemicals.h"
#include "bind/cpp/cdm/Substance.pb.h"

const std::string& eSubstance_State_Name(eSubstance_State m)
{
  return cdm::SubstanceData::eState_Name((cdm::SubstanceData::eState)m);
}
const std::string& eSubstance_IonicState_Name(eSubstance_IonicState m)
{
  return cdm::SubstancePhysicochemicalsData::eIonicState_Name((cdm::SubstancePhysicochemicalsData::eIonicState)m);
}
const std::string& eSubstance_BindingProtein_Name(eSubstance_BindingProtein m)
{
  return cdm::SubstancePhysicochemicalsData::eBindingProtein_Name((cdm::SubstancePhysicochemicalsData::eBindingProtein)m);
}
