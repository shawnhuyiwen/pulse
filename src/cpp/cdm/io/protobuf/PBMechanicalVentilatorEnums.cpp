/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
PUSH_PROTO_WARNINGS
#include "pulse/cdm/bind/MechanicalVentilator.pb.h"
POP_PROTO_WARNINGS
#include "cdm/system/equipment/mechanical_ventilator/SEMechanicalVentilatorSettings.h"

const std::string& eMechanicalVentilator_Connection_Name(eMechanicalVentilator_Connection c)
{
  return CDM_BIND::MechanicalVentilatorSettingsData::eConnection_Name((CDM_BIND::MechanicalVentilatorSettingsData::eConnection)c);
}

const std::string& eMechanicalVentilator_DriverWaveform_Name(eMechanicalVentilator_DriverWaveform w)
{
  return CDM_BIND::MechanicalVentilatorSettingsData::eDriverWaveform_Name((CDM_BIND::MechanicalVentilatorSettingsData::eDriverWaveform)w);
}
