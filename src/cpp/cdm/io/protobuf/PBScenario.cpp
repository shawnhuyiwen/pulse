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

  if (src.has_starttype())
  {
    if (src.starttype().has_patientconfiguration())
      PBEngine::Load(src.starttype().patientconfiguration(), dst.GetPatientConfiguration(), dst.m_SubMgr);
    else
    {
      dst.SetEngineStateFile(src.starttype().enginestatefile());
    }
  }

  if (src.has_datarequestmanager())
    PBEngine::Load(src.datarequestmanager(), dst.GetDataRequestManager(), dst.m_SubMgr);

  for (int i = 0; i < src.anyaction_size(); i++)
  {
    SEAction* a = PBAction::Load(src.anyaction()[i], dst.m_SubMgr);
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
  {
    dst.mutable_starttype()->set_enginestatefile(src.m_EngineStateFile);
  }
  else if (src.HasPatientConfiguration())
    dst.mutable_starttype()->set_allocated_patientconfiguration(PBEngine::Unload(*src.m_PatientConfiguration));

  dst.set_allocated_datarequestmanager(PBEngine::Unload(*src.m_DataRequestMgr));

  for (const SEAction* a : src.m_Actions)
    dst.mutable_anyaction()->AddAllocated(PBAction::Unload(*a));
}

bool PBScenario::SerializeToString(const SEScenario& src, std::string& output, SerializationFormat m)
{
  CDM_BIND::ScenarioData data;
  PBScenario::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBScenario::SerializeToFile(const SEScenario& src, const std::string& filename, SerializationFormat m)
{
  CDM_BIND::ScenarioData data;
  PBScenario::Serialize(src, data);
  std::string content;
  PBScenario::SerializeToString(src, content, m);
  return WriteFile(content, filename, m);
}
bool PBScenario::SerializeFromString(const std::string& src, SEScenario& dst, SerializationFormat m)
{
  CDM_BIND::ScenarioData data;
  if (!PBUtils::SerializeFromString(src, data, m, dst.GetLogger()))
    return false;
  PBScenario::Load(data, dst);
  return true;
}
bool PBScenario::SerializeFromFile(const std::string& filename, SEScenario& dst, SerializationFormat m)
{
  std::string content = ReadFile(filename, m);
  if (content.empty())
    return false;
  return PBScenario::SerializeFromString(content, dst, m);
}