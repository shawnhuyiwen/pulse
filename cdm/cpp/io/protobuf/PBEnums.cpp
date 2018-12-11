/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "bind/cpp/cdm/Enums.pb.h"

const std::string& eSide_Name(eSide m)
{
  return cdm::eSide_Name((cdm::eSide)m);
}

const std::string& eGate_Name(eGate m)
{
  return cdm::eGate_Name((cdm::eGate)m);
}

const std::string& eSwitch_Name(eSwitch m)
{
  return cdm::eSwitch_Name((cdm::eSwitch)m);
}

const std::string& eCharge_Name(eCharge m)
{
  return cdm::eCharge_Name((cdm::eCharge)m);
}