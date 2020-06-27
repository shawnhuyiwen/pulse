/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
PUSH_PROTO_WARNINGS()
#include "pulse/cdm/bind/AnesthesiaMachine.pb.h"
POP_PROTO_WARNINGS()
#include "system/equipment/anesthesia_machine/SEAnesthesiaMachine.h"

const std::string& eAnesthesiaMachine_OxygenSource_Name(eAnesthesiaMachine_OxygenSource m)
{
  return CDM_BIND::AnesthesiaMachineData::eOxygenSource_Name((CDM_BIND::AnesthesiaMachineData::eOxygenSource)m);
}

const std::string& eAnesthesiaMachine_PrimaryGas_Name(eAnesthesiaMachine_PrimaryGas m)
{
  return CDM_BIND::AnesthesiaMachineData::ePrimaryGas_Name((CDM_BIND::AnesthesiaMachineData::ePrimaryGas)m);
}

const std::string& eAnesthesiaMachine_Connection_Name(eAnesthesiaMachine_Connection m)
{
  return CDM_BIND::AnesthesiaMachineData::eConnection_Name((CDM_BIND::AnesthesiaMachineData::eConnection)m);
}
