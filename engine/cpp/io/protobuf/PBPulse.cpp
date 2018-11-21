/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/PBPulse.h"
#include "io/protobuf/PBPulseConfiguration.h"
#include "io/protobuf/PBScenario.h"
#include "io/protobuf/PBUtils.h"
#include "bind/cpp/pulse/Pulse.pb.h"
#include "PulseScenario.h"
#include "utils/FileUtils.h"

void PBPulse::Load(const pulse::proto::ScenarioData& src, PulseScenario& dst)
{
  PBPulse::Serialize(src, dst);
}
void PBPulse::Serialize(const pulse::proto::ScenarioData& src, PulseScenario& dst)
{
  dst.Clear();
  PBScenario::Serialize(src.scenario(), dst);

  if (src.has_configuration())
    PBPulseConfiguration::Load(src.configuration(), dst.GetConfiguration());
}
pulse::proto::ScenarioData* PBPulse::Unload(const PulseScenario& src)
{
  pulse::proto::ScenarioData* dst = new pulse::proto::ScenarioData();
  PBPulse::Serialize(src, *dst);
  return dst;
}
void PBPulse::Serialize(const PulseScenario& src, pulse::proto::ScenarioData& dst)
{
  PBScenario::Serialize(src, *dst.mutable_scenario());
  if (src.HasConfiguration())
    dst.set_allocated_configuration(PBPulseConfiguration::Unload(*src.GetConfiguration()));
}

bool PBPulse::SerializeToString(const PulseScenario& src, std::string& output, SerializationFormat m)
{
  pulse::proto::ScenarioData data;
  PBPulse::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m);
}
bool PBPulse::SerializeToFile(const PulseScenario& src, const std::string& filename, SerializationFormat m)
{
  pulse::proto::ScenarioData data;
  PBPulse::Serialize(src, data);
  std::string content;
  PBPulse::SerializeToString(src, content, m);
  return WriteFile(content, filename, m);
}

bool PBPulse::SerializeFromString(const std::string& src, PulseScenario& dst, SerializationFormat m)
{
  pulse::proto::ScenarioData data;
  if (!PBUtils::SerializeFromString(src, data, m))
  {
    dst.GetLogger()->Info("Trying to load with base SEScenario");
    // Try our base class
    cdm::ScenarioData cdm_data;
    if (!PBUtils::SerializeFromString(src, cdm_data, m))
      return false;
    dst.GetLogger()->Info("Successfully loaded Scenario as base SEScenario");
    PBScenario::Load(cdm_data, dst);
    return true;
  }
  PBPulse::Load(data, dst);
  return true;
}
bool PBPulse::SerializeFromFile(const std::string& filename, PulseScenario& dst, SerializationFormat m)
{
  std::string content = ReadFile(filename, m);
  if (content.empty())
    return false;
  return PBPulse::SerializeFromString(content, dst, m);
}