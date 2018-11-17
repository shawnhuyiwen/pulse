/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "bind/cpp/cdm/AnesthesiaMachineEnums.pb.h"
#include "system/equipment/anesthesiamachine/SEAnesthesiaMachine.h"

const std::string& eAnesthesiaMachine_Event_Name(eAnesthesiaMachine_Event m)
{
  return cdm::eAnesthesiaMachine_Event_Name((cdm::eAnesthesiaMachine_Event)m);
}

const std::string& eAnesthesiaMachine_OxygenSource_Name(eAnesthesiaMachine_OxygenSource m)
{
  return cdm::eAnesthesiaMachine_OxygenSource_Name((cdm::eAnesthesiaMachine_OxygenSource)m);
}

const std::string& eAnesthesiaMachine_PrimaryGas_Name(eAnesthesiaMachine_PrimaryGas m)
{
  return cdm::eAnesthesiaMachine_PrimaryGas_Name((cdm::eAnesthesiaMachine_PrimaryGas)m);
}

const std::string& eAnesthesiaMachine_Connection_Name(eAnesthesiaMachine_Connection m)
{
  return cdm::eAnesthesiaMachine_Connection_Name((cdm::eAnesthesiaMachine_Connection)m);
}
