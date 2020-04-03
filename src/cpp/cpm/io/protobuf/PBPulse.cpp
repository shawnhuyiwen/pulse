/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
PUSH_PROTO_WARNINGS()
#include "pulse/cpm/bind/Pulse.pb.h"
POP_PROTO_WARNINGS()
#include "io/protobuf/PBPulse.h"
#include "io/protobuf/PBPulseConfiguration.h"
#include "io/protobuf/PBScenario.h"
#include "io/protobuf/PBUtils.h"
#include "PulseScenario.h"
#include "utils/FileUtils.h"

void PBPulse::Load(const PULSE_BIND::ScenarioData& src, PulseScenario& dst)
{
  PBPulse::Serialize(src, dst);
}
void PBPulse::Serialize(const PULSE_BIND::ScenarioData& src, PulseScenario& dst)
{
  dst.Clear();
  PBScenario::Serialize(src.scenario(), dst);

  if (src.has_configuration())
    PBPulseConfiguration::Load(src.configuration(), dst.GetConfiguration());
}
PULSE_BIND::ScenarioData* PBPulse::Unload(const PulseScenario& src)
{
  PULSE_BIND::ScenarioData* dst = new PULSE_BIND::ScenarioData();
  PBPulse::Serialize(src, *dst);
  return dst;
}
void PBPulse::Serialize(const PulseScenario& src, PULSE_BIND::ScenarioData& dst)
{
  PBScenario::Serialize(src, *dst.mutable_scenario());
  if (src.HasConfiguration())
    dst.set_allocated_configuration(PBPulseConfiguration::Unload(*src.GetConfiguration()));
}

bool PBPulse::SerializeToString(const PulseScenario& src, std::string& output, SerializationFormat m)
{
  PULSE_BIND::ScenarioData data;
  PBPulse::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBPulse::SerializeToFile(const PulseScenario& src, const std::string& filename, SerializationFormat m)
{
  PULSE_BIND::ScenarioData data;
  PBPulse::Serialize(src, data);
  std::string content;
  PBPulse::SerializeToString(src, content, m);
  return WriteFile(content, filename, m);
}

bool PBPulse::SerializeFromString(const std::string& src, PulseScenario& dst, SerializationFormat m)
{
  PULSE_BIND::ScenarioData data;
  dst.GetLogger()->Info("Loading scenario...");
  if (!PBUtils::SerializeFromString(src, data, m, dst.GetLogger()))
  {
    dst.GetLogger()->Info("Attempting to resolve errors...");
    // Try our base class
    CDM_BIND::ScenarioData cdm_data;
    if (!PBUtils::SerializeFromString(src, cdm_data, m, dst.GetLogger()))
      return false;
    dst.GetLogger()->Info("Successfully loaded scenario as a base SEScenario");
    PBScenario::Load(cdm_data, dst);
    return true;
  }
  PBPulse::Load(data, dst);
  dst.GetLogger()->Info("Successfully loaded scenario");
  return true;
}
bool PBPulse::SerializeFromFile(const std::string& filename, PulseScenario& dst, SerializationFormat m)
{
  std::string content = ReadFile(filename, m);
  if (content.empty())
    return false;
  return PBPulse::SerializeFromString(content, dst, m);
}