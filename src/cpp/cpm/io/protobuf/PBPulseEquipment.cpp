/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
PUSH_PROTO_WARNINGS()
#include "pulse/cpm/bind/PulseEquipment.pb.h"
POP_PROTO_WARNINGS()
#include "io/protobuf/PBPulseEquipment.h"
#include "io/protobuf/PBAnesthesiaMachine.h"
#include "io/protobuf/PBElectroCardioGram.h"
#include "io/protobuf/PBInhaler.h"
#include "io/protobuf/PBMechanicalVentilator.h"
#include "equipment/AnesthesiaMachine.h"
#include "equipment/ECG.h"
#include "equipment/Inhaler.h"
#include "equipment/MechanicalVentilator.h"
#include "system/equipment/anesthesia_machine/SEAnesthesiaMachine.h"
#include "system/equipment/electrocardiogram/SEElectroCardioGram.h"
#include "system/equipment/electrocardiogram/SEElectroCardioGramWaveformInterpolator.h"
#include "system/equipment/inhaler/SEInhaler.h"

void PBPulseEquipment::Load(const PULSE_BIND::AnesthesiaMachineData& src, AnesthesiaMachine& dst)
{
  dst.Clear();
  dst.SetUp();
  PBPulseEquipment::Serialize(src, dst);
  dst.StateChange();
}
void PBPulseEquipment::Serialize(const PULSE_BIND::AnesthesiaMachineData& src, AnesthesiaMachine& dst)
{
  PBAnesthesiaMachine::Serialize(src.common(), dst, (SESubstanceManager&)dst.m_data.GetSubstances());
  dst.m_inhaling = src.inhaling();
  dst.m_currentbreathingCycleTime_s = src.currentbreathingcycletime_s();
  dst.m_inspirationTime_s = src.inspirationtime_s();
  dst.m_O2InletVolumeFraction = src.oxygeninletvolumefraction();
  dst.m_totalBreathingCycleTime_s = src.totalbreathingcycletime_s();
}
PULSE_BIND::AnesthesiaMachineData* PBPulseEquipment::Unload(const AnesthesiaMachine& src)
{
  PULSE_BIND::AnesthesiaMachineData* dst = new PULSE_BIND::AnesthesiaMachineData();
  PBPulseEquipment::Serialize(src, *dst);
  return dst;
}
void PBPulseEquipment::Serialize(const AnesthesiaMachine& src, PULSE_BIND::AnesthesiaMachineData& dst)
{
  PBAnesthesiaMachine::Serialize(src, *dst.mutable_common());
  dst.set_inhaling(src.m_inhaling);
  dst.set_currentbreathingcycletime_s(src.m_currentbreathingCycleTime_s);
  dst.set_inspirationtime_s(src.m_inspirationTime_s);
  dst.set_oxygeninletvolumefraction(src.m_O2InletVolumeFraction);
  dst.set_totalbreathingcycletime_s(src.m_totalBreathingCycleTime_s);
}

void PBPulseEquipment::Load(const PULSE_BIND::ElectroCardioGramData& src, ECG& dst)
{
  dst.Clear();
  dst.SetUp();
  PBPulseEquipment::Serialize(src, dst);
}
void PBPulseEquipment::Serialize(const PULSE_BIND::ElectroCardioGramData& src, ECG& dst)
{
  PBElectroCardioGram::Serialize(src.common(), dst);
  dst.m_heartRhythmTime_s = src.heartrythmtime_s();
  dst.m_heartRhythmPeriod_s = src.heartrythmperiod_s();
  PBElectroCardioGram::Load(src.waveforms(), *dst.m_interpolator);
  dst.m_interpolator->SetLeadElectricPotential(eElectroCardioGram_WaveformLead::Lead3, dst.GetLead3ElectricPotential());
}
PULSE_BIND::ElectroCardioGramData* PBPulseEquipment::Unload(const ECG& src)
{
  PULSE_BIND::ElectroCardioGramData* dst = new PULSE_BIND::ElectroCardioGramData();
  PBPulseEquipment::Serialize(src, *dst);
  return dst;
}
void PBPulseEquipment::Serialize(const ECG& src, PULSE_BIND::ElectroCardioGramData& dst)
{
  PBElectroCardioGram::Serialize(src, *dst.mutable_common());
  dst.set_heartrythmtime_s(src.m_heartRhythmTime_s);
  dst.set_heartrythmperiod_s(src.m_heartRhythmPeriod_s);
  dst.set_allocated_waveforms(PBElectroCardioGram::Unload(*src.m_interpolator));
}

void PBPulseEquipment::Load(const PULSE_BIND::InhalerData& src, Inhaler& dst)
{
  dst.Clear();
  dst.SetUp();
  PBPulseEquipment::Serialize(src, dst);
  dst.StateChange();
}
void PBPulseEquipment::Serialize(const PULSE_BIND::InhalerData& src, Inhaler& dst)
{
  PBInhaler::Serialize(src.common(), dst, (SESubstanceManager&)dst.m_data.GetSubstances());
}
PULSE_BIND::InhalerData* PBPulseEquipment::Unload(const Inhaler& src)
{
  PULSE_BIND::InhalerData* dst = new PULSE_BIND::InhalerData();
  PBPulseEquipment::Serialize(src, *dst);
  return dst;
}
void PBPulseEquipment::Serialize(const Inhaler& src, PULSE_BIND::InhalerData& dst)
{
  PBInhaler::Serialize(src, *dst.mutable_common());
}

void PBPulseEquipment::Load(const PULSE_BIND::MechanicalVentilatorData& src, MechanicalVentilator& dst)
{
  dst.Clear();
  dst.SetUp();
  PBPulseEquipment::Serialize(src, dst);
  dst.StateChange();
}
void PBPulseEquipment::Serialize(const PULSE_BIND::MechanicalVentilatorData& src, MechanicalVentilator& dst)
{
  PBMechanicalVentilator::Serialize(src.common(), dst, (SESubstanceManager&)dst.m_data.GetSubstances());
  dst.m_CurrentBreathState = (eBreathState)src.currentbreathstate();
  dst.m_CurrentInspiratoryVolume_L = src.currentinspiratoryvolume_l();
  dst.m_CurrentPeriodTime_s = src.currentperiodtime_s();
  dst.m_DriverFlow_L_Per_s = src.driverflow_l_per_s();
  dst.m_DriverPressure_cmH2O = src.driverpressure_cmh2o();
}
PULSE_BIND::MechanicalVentilatorData* PBPulseEquipment::Unload(const MechanicalVentilator& src)
{
  PULSE_BIND::MechanicalVentilatorData* dst = new PULSE_BIND::MechanicalVentilatorData();
  PBPulseEquipment::Serialize(src, *dst);
  return dst;
}
void PBPulseEquipment::Serialize(const MechanicalVentilator& src, PULSE_BIND::MechanicalVentilatorData& dst)
{
  PBMechanicalVentilator::Serialize(src, *dst.mutable_common());
  dst.set_currentbreathstate((CDM_BIND::eBreathState)src.m_CurrentBreathState);
  dst.set_currentinspiratoryvolume_l(src.m_CurrentInspiratoryVolume_L);
  dst.set_currentperiodtime_s(src.m_CurrentPeriodTime_s);
  dst.set_driverflow_l_per_s(src.m_DriverFlow_L_Per_s);
  dst.set_driverpressure_cmh2o(src.m_DriverPressure_cmH2O);
}
