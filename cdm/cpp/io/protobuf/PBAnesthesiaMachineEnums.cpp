/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "bind/cpp/cdm/AnesthesiaMachine.pb.h"
#include "system/equipment/anesthesiamachine/SEAnesthesiaMachine.h"

const std::string& eAnesthesiaMachine_Event_Name(eAnesthesiaMachine_Event m)
{
  return cdm::eAnesthesiaMachine_Event_Name((cdm::eAnesthesiaMachine_Event)m);
}

const std::string& eAnesthesiaMachine_OxygenSource_Name(eAnesthesiaMachine_OxygenSource m)
{
  return cdm::AnesthesiaMachineData::eOxygenSource_Name((cdm::AnesthesiaMachineData::eOxygenSource)m);
}

const std::string& eAnesthesiaMachine_PrimaryGas_Name(eAnesthesiaMachine_PrimaryGas m)
{
  return cdm::AnesthesiaMachineData::ePrimaryGas_Name((cdm::AnesthesiaMachineData::ePrimaryGas)m);
}

const std::string& eAnesthesiaMachine_Connection_Name(eAnesthesiaMachine_Connection m)
{
  return cdm::AnesthesiaMachineData::eConnection_Name((cdm::AnesthesiaMachineData::eConnection)m);
}
