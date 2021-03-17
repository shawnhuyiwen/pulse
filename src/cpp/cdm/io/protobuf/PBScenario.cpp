/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
PUSH_PROTO_WARNINGS()
#include "pulse/cdm/bind/Scenario.pb.h"
POP_PROTO_WARNINGS()
#include "io/protobuf/PBScenario.h"
#include "io/protobuf/PBActions.h"
#include "io/protobuf/PBEngine.h"
#include "io/protobuf/PBUtils.h"
#include "scenario/SEScenario.h"
#include "scenario/SEScenarioExec.h"
#include "utils/FileUtils.h"

void PBScenario::Load(const CDM_BIND::ScenarioData& src, SEScenario& dst)
{
  dst.Clear();
  PBScenario::Serialize(src, dst);
}
void PBScenario::Serialize(const CDM_BIND::ScenarioData& src, SEScenario& dst)
{
  dst.SetName(src.name());
  dst.SetDescription(src.description());

  if(!src.enginestatefile().empty())
    dst.SetEngineStateFile(src.enginestatefile());
  if (src.has_patientconfiguration())
    PBEngine::Load(src.patientconfiguration(), dst.GetPatientConfiguration(), *dst.m_SubMgr);

  if (src.has_datarequestmanager())
    PBEngine::Load(src.datarequestmanager(), dst.GetDataRequestManager(), *dst.m_SubMgr);

  for (int i = 0; i < src.anyaction_size(); i++)
  {
    SEAction* a = PBAction::Load(src.anyaction()[i], *dst.m_SubMgr);
    if(a != nullptr)
      dst.m_Actions.push_back(a);
  }
}

CDM_BIND::ScenarioData* PBScenario::Unload(const SEScenario& src)
{
  CDM_BIND::ScenarioData* dst = new CDM_BIND::ScenarioData();
  PBScenario::Serialize(src, *dst);
  return dst;
}
void PBScenario::Serialize(const SEScenario& src, CDM_BIND::ScenarioData& dst)
{
  dst.set_name(src.m_Name);
  dst.set_description(src.m_Description);

  if (src.HasEngineStateFile())
    dst.set_enginestatefile(src.m_EngineStateFile);
  else if (src.HasPatientConfiguration())
    dst.set_allocated_patientconfiguration(PBEngine::Unload(*src.m_PatientConfiguration));

  dst.set_allocated_datarequestmanager(PBEngine::Unload(*src.m_DataRequestMgr));

  for (const SEAction* a : src.m_Actions)
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*a));
}
void PBScenario::Copy(const SEScenario& src, SEScenario& dst)
{
  dst.Clear();
  CDM_BIND::ScenarioData data;
  PBScenario::Serialize(src, data);
  PBScenario::Serialize(data, dst);
}

bool PBScenario::SerializeToString(const SEScenario& src, std::string& output, SerializationFormat m)
{
  CDM_BIND::ScenarioData data;
  PBScenario::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBScenario::SerializeToFile(const SEScenario& src, const std::string& filename)
{
  CDM_BIND::ScenarioData data;
  PBScenario::Serialize(src, data);
  return PBUtils::SerializeToFile(data, filename, src.GetLogger());
}
bool PBScenario::SerializeFromString(const std::string& src, SEScenario& dst, SerializationFormat m)
{
  CDM_BIND::ScenarioData data;
  if (!PBUtils::SerializeFromString(src, data, m, dst.GetLogger()))
    return false;
  PBScenario::Load(data, dst);
  return true;
}
bool PBScenario::SerializeFromFile(const std::string& filename, SEScenario& dst)
{
  CDM_BIND::ScenarioData data;
  if (!PBUtils::SerializeFromFile(filename, data, dst.GetLogger()))
    return false;
  PBScenario::Load(data, dst);
  return true;
}


void PBScenario::Load(const CDM_BIND::ScenarioExecData& src, SEScenarioExec& dst)
{
  dst.Clear();
  PBScenario::Serialize(src, dst);
}
void PBScenario::Serialize(const CDM_BIND::ScenarioExecData& src, SEScenarioExec& dst)
{
  dst.SetLogToConsole((eSwitch)src.logtoconsole());
  dst.SetLogFilename(src.logfilename());
  dst.SetDataRequestCSVFilename(src.datarequestcsvfilename());
  dst.SetDataRootDirectory(src.datarootdirectory());

  dst.SetSerializationDirectory(src.serializationdirectory());
  dst.SetAutoSerializeFilename(src.autoserializefilename());
  dst.SetAutoSerializeAfterActions((eSwitch)src.autoserializeafteractions());
  dst.SetAutoSerializePeriod_s(src.autoserializeperiod_s());
  dst.SetTimeStampSerializedStates((eSwitch)src.timestampserializedstates());
  dst.SetReloadSerializedState((eSwitch)src.reloadserializedstate());

  switch (src.EngineConfiguration_case())
  {
  case CDM_BIND::ScenarioExecData::EngineConfigurationCase::kEngineConfigurationContent:
  {
    dst.SetEngineConfigurationContent(src.engineconfigurationcontent());
    break;
  }
  case CDM_BIND::ScenarioExecData::EngineConfigurationCase::kEngineConfigurationFilename:
  {
    dst.SetEngineConfigurationFilename(src.engineconfigurationfilename());
    break;
  }
  }

  switch (src.Scenario_case())
  {
  case CDM_BIND::ScenarioExecData::ScenarioCase::kScenarioContent:
  {
    dst.SetScenarioContent(src.scenariocontent());
    break;
  }
  case CDM_BIND::ScenarioExecData::ScenarioCase::kScenarioFilename:
  {
    dst.SetScenarioFilename(src.scenariofilename());
    break;
  }
  }

  dst.SetContentFormat((SerializationFormat)src.contentformat());
}

CDM_BIND::ScenarioExecData* PBScenario::Unload(const SEScenarioExec& src)
{
  CDM_BIND::ScenarioExecData* dst = new CDM_BIND::ScenarioExecData();
  PBScenario::Serialize(src, *dst);
  return dst;
}
void PBScenario::Serialize(const SEScenarioExec& src, CDM_BIND::ScenarioExecData& dst)
{
  dst.set_logtoconsole((CDM_BIND::eSwitch)src.GetLogToConsole());
  dst.set_logfilename(src.GetLogFilename());
  dst.set_datarequestcsvfilename(src.GetDataRequestCSVFilename());
  dst.set_datarootdirectory(src.GetDataRootDirectory());

  dst.set_serializationdirectory(src.GetSerializationDirectory());
  dst.set_autoserializefilename(src.GetAutoSerializeFilename());
  dst.set_autoserializeafteractions((CDM_BIND::eSwitch)src.GetAutoSerializeAfterActions());
  dst.set_autoserializeperiod_s(src.GetAutoSerializePeriod_s());
  dst.set_timestampserializedstates((CDM_BIND::eSwitch)src.GetTimeStampSerializedStates());
  dst.set_reloadserializedstate((CDM_BIND::eSwitch)src.GetReloadSerializedState());

  if (!src.GetEngineConfigurationContent().empty())
    dst.set_engineconfigurationcontent(src.GetEngineConfigurationContent());
  else if (!src.GetEngineConfigurationFilename().empty())
    dst.set_engineconfigurationfilename(src.GetEngineConfigurationFilename());

  if (!src.GetEngineConfigurationContent().empty())
    dst.set_engineconfigurationcontent(src.GetEngineConfigurationContent());
  else if (!src.GetEngineConfigurationFilename().empty())
    dst.set_engineconfigurationfilename(src.GetEngineConfigurationFilename());

  dst.set_contentformat((CDM_BIND::eSerializationFormat)src.GetContentFormat());
}

bool PBScenario::SerializeToString(const SEScenarioExec& src, std::string& output, SerializationFormat m, Logger* logger)
{
  CDM_BIND::ScenarioExecData data;
  PBScenario::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, logger);
}
bool PBScenario::SerializeFromString(const std::string& src, SEScenarioExec& dst, SerializationFormat m, Logger* logger)
{
  CDM_BIND::ScenarioExecData data;
  if (!PBUtils::SerializeFromString(src, data, m, logger))
    return false;
  PBScenario::Load(data, dst);
  return true;
}