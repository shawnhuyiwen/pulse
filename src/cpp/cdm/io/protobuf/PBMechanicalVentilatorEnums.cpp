/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
PUSH_PROTO_WARNINGS()
#include "pulse/cdm/bind/MechanicalVentilator.pb.h"
POP_PROTO_WARNINGS()
#include "system/equipment/mechanical_ventilator/SEMechanicalVentilator.h"

const std::string& eMechanicalVentilator_Connection_Name(eMechanicalVentilator_Connection c)
{
  return CDM_BIND::MechanicalVentilatorData::eConnection_Name((CDM_BIND::MechanicalVentilatorData::eConnection)c);
}

const std::string& eMechanicalVentilator_DriverWaveform_Name(eMechanicalVentilator_DriverWaveform w)
{
  return CDM_BIND::MechanicalVentilatorData::eDriverWaveform_Name((CDM_BIND::MechanicalVentilatorData::eDriverWaveform)w);
}
