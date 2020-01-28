/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "substance/SESubstance.h"
#include "substance/SESubstancePhysicochemicals.h"
#include "bind/Substance.pb.h"

const std::string& eSubstance_State_Name(eSubstance_State m)
{
  return CDM_BIND::SubstanceData::eState_Name((CDM_BIND::SubstanceData::eState)m);
}
const std::string& eSubstance_IonicState_Name(eSubstance_IonicState m)
{
  return CDM_BIND::SubstancePhysicochemicalsData::eIonicState_Name((CDM_BIND::SubstancePhysicochemicalsData::eIonicState)m);
}
const std::string& eSubstance_BindingProtein_Name(eSubstance_BindingProtein m)
{
  return CDM_BIND::SubstancePhysicochemicalsData::eBindingProtein_Name((CDM_BIND::SubstancePhysicochemicalsData::eBindingProtein)m);
}
