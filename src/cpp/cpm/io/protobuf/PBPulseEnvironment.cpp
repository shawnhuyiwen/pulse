/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
PUSH_PROTO_WARNINGS()
#include "pulse/cpm/bind/PulseEnvironment.pb.h"
POP_PROTO_WARNINGS()
#include "io/protobuf/PBPulseEnvironment.h"
#include "io/protobuf/PBEnvironment.h"
#include "environment/Environment.h"

void PBPulseEnvironment::Load(const PULSE_BIND::EnvironmentData& src, Environment& dst)
{
  dst.Clear();
  dst.SetUp();
  PBPulseEnvironment::Serialize(src, dst);
  dst.StateChange();
}
void PBPulseEnvironment::Serialize(const PULSE_BIND::EnvironmentData& src, Environment& dst)
{
  PBEnvironment::Serialize(src.common(), dst, (SESubstanceManager&)dst.m_data.GetSubstances());
  dst.m_PatientEquivalentDiameter_m = src.patientequivalentdiameter_m();
}
PULSE_BIND::EnvironmentData* PBPulseEnvironment::Unload(const Environment& src)
{
  PULSE_BIND::EnvironmentData* dst = new PULSE_BIND::EnvironmentData();
  PBPulseEnvironment::Serialize(src, *dst);
  return dst;
}
void PBPulseEnvironment::Serialize(const Environment& src, PULSE_BIND::EnvironmentData& dst)
{
  PBEnvironment::Serialize(src, *dst.mutable_common());
  dst.set_patientequivalentdiameter_m(src.m_PatientEquivalentDiameter_m);
}
