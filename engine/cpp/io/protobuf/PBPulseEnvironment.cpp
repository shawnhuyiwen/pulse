/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/PBPulseEnvironment.h"
#include "io/protobuf/PBEnvironment.h"
#include "bind/cpp/pulse/PulseEnvironment.pb.h"
#include "environment/Environment.h"

using namespace pulse::proto;

void PBPulseEnvironment::Load(const EnvironmentData& src, Environment& dst)
{
  PBPulseEnvironment::Serialize(src, dst);
  dst.SetUp();
}
void PBPulseEnvironment::Serialize(const EnvironmentData& src, Environment& dst)
{
  PBEnvironment::Serialize(src.common(), dst);
  dst.m_PatientEquivalentDiameter_m = src.patientequivalentdiameter_m();
}
EnvironmentData* PBPulseEnvironment::Unload(const Environment& src)
{
  EnvironmentData* dst = new EnvironmentData();
  PBPulseEnvironment::Serialize(src, *dst);
  return dst;
}
void PBPulseEnvironment::Serialize(const Environment& src, EnvironmentData& dst)
{
  PBEnvironment::Serialize(src, *dst.mutable_common());
  dst.set_patientequivalentdiameter_m(src.m_PatientEquivalentDiameter_m);
}
