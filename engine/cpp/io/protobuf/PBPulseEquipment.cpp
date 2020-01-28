/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/PBPulseEquipment.h"
#include "io/protobuf/PBAnesthesiaMachine.h"
#include "io/protobuf/PBElectroCardioGram.h"
#include "io/protobuf/PBInhaler.h"
#include "pulse/engine/bind/PulseEquipment.pb.h"
#include "equipment/AnesthesiaMachine.h"
#include "equipment/ECG.h"
#include "equipment/Inhaler.h"
#include "system/equipment/anesthesiamachine/SEAnesthesiaMachine.h"
#include "system/equipment/electrocardiogram/SEElectroCardioGram.h"
#include "system/equipment/electrocardiogram/SEElectroCardioGramWaveformInterpolator.h"
#include "system/equipment/inhaler/SEInhaler.h"

void PBPulseEquipment::Load(const PULSE_BIND::AnesthesiaMachineData& src, AnesthesiaMachine& dst)
{
  PBPulseEquipment::Serialize(src, dst);
  dst.SetUp();
}
void PBPulseEquipment::Serialize(const PULSE_BIND::AnesthesiaMachineData& src, AnesthesiaMachine& dst)
{
  PBAnesthesiaMachine::Serialize(src.common(), dst);
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
  PBPulseEquipment::Serialize(src, dst);
  dst.SetUp();
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
  PBPulseEquipment::Serialize(src, dst);
  dst.SetUp();
}
void PBPulseEquipment::Serialize(const PULSE_BIND::InhalerData& src, Inhaler& dst)
{
  PBInhaler::Serialize(src.common(), dst);
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
