/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
PUSH_PROTO_WARNINGS
#include "pulse/cdm/bind/ECMO.pb.h"
#include "pulse/cdm/bind/ECMOActions.pb.h"
POP_PROTO_WARNINGS
#include "cdm/system/equipment/ecmo/SEECMOSettings.h"

const std::string& eECMO_DriverWaveform_Name(eECMO_DriverWaveform w)
{
  return CDM_BIND::ECMOSettingsData::eDriverWaveform_Name((CDM_BIND::ECMOSettingsData::eDriverWaveform)w);
}
