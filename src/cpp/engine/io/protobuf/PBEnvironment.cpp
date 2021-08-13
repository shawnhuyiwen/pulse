/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "engine/common/CommonDefs.h"
PUSH_PROTO_WARNINGS
#include "pulse/engine/bind/Environment.pb.h"
POP_PROTO_WARNINGS
#include "engine/io/protobuf/PBEnvironment.h"
#include "cdm/io/protobuf/PBEnvironment.h"

namespace PULSE_ENGINE
{
  void PBEnvironment::Load(const PULSE_BIND::EnvironmentData& src, EnvironmentModel& dst)
  {
    dst.Clear();
    dst.SetUp();
    PBEnvironment::Serialize(src, dst);
    dst.StateChange();
  }
  void PBEnvironment::Serialize(const PULSE_BIND::EnvironmentData& src, EnvironmentModel& dst)
  {
    ::PBEnvironment::Serialize(src.common(), dst, (SESubstanceManager&)dst.m_data.GetSubstances());
    dst.m_PatientEquivalentDiameter_m = src.patientequivalentdiameter_m();
  }
  PULSE_BIND::EnvironmentData* PBEnvironment::Unload(const EnvironmentModel& src)
  {
    PULSE_BIND::EnvironmentData* dst = new PULSE_BIND::EnvironmentData();
    PBEnvironment::Serialize(src, *dst);
    return dst;
  }
  void PBEnvironment::Serialize(const EnvironmentModel& src, PULSE_BIND::EnvironmentData& dst)
  {
    ::PBEnvironment::Serialize(src, *dst.mutable_common());
    dst.set_patientequivalentdiameter_m(src.m_PatientEquivalentDiameter_m);
  }
}
