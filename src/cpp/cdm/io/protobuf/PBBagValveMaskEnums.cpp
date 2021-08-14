/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDataModel.h"
PUSH_PROTO_WARNINGS
#include "pulse/cdm/bind/BagValveMask.pb.h"
POP_PROTO_WARNINGS
#include "cdm/system/equipment/bag_valve_mask/SEBagValveMask.h"

const std::string& eBagValveMask_Connection_Name(eBagValveMask_Connection c)
{
  return CDM_BIND::BagValveMaskData::eConnection_Name((CDM_BIND::BagValveMaskData::eConnection)c);
}
