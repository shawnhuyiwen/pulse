/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
PUSH_PROTO_WARNINGS
#include "pulse/cdm/bind/MechanicalVentilator.pb.h"
#include "pulse/cdm/bind/MechanicalVentilatorActions.pb.h"
POP_PROTO_WARNINGS
#include "cdm/system/equipment/mechanical_ventilator/SEMechanicalVentilatorSettings.h"
#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorPressureControl.h"
#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorVolumeControl.h"

const std::string& eMechanicalVentilator_DriverWaveform_Name(eMechanicalVentilator_DriverWaveform w)
{
  return CDM_BIND::MechanicalVentilatorSettingsData::eDriverWaveform_Name((CDM_BIND::MechanicalVentilatorSettingsData::eDriverWaveform)w);
}

const std::string& eMechanicalVentilator_PressureControlMode_Name(eMechanicalVentilator_PressureControlMode m)
{
  return CDM_BIND::MechanicalVentilatorPressureControlData::eMode_Name((CDM_BIND::MechanicalVentilatorPressureControlData::eMode)m);
}

const std::string& eMechanicalVentilator_VolumeControlMode_Name(eMechanicalVentilator_VolumeControlMode m)
{
  return CDM_BIND::MechanicalVentilatorVolumeControlData::eMode_Name((CDM_BIND::MechanicalVentilatorVolumeControlData::eMode)m);
}
