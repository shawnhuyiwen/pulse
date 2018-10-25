/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/PBEngine.h"
#include "io/protobuf/PBScenario.h"
#include "io/protobuf/PBProperties.h"
#include "io/protobuf/PBUtils.h"
#include "bind/cdm/Engine.pb.h"
#include "engine/SEAutoSerialization.h"
#include "engine/SEDynamicStabilization.h"
#include "engine/SEDynamicStabilizationEngineConvergence.h"
#include "engine/SEDynamicStabilizationPropertyConvergence.h"
#include "engine/SETimedStabilization.h"
#include "scenario/SEDataRequest.h"
#include "scenario/SEDataRequestManager.h"
#include "properties/SEScalarTime.h"
#include "utils/FileUtils.h"

void PBEngine::Load(const cdm::AutoSerializationData& src, SEAutoSerialization& dst)
{
  PBEngine::Serialize(src, dst);
}
void PBEngine::Serialize(const cdm::AutoSerializationData& src, SEAutoSerialization& dst)
{
  dst.Clear();
  if (src.has_period())
    PBProperty::Load(src.period(), dst.GetPeriod());
  if (src.periodtimestamps() != cdm::eSwitch::NullSwitch)
    dst.SetPeriodTimeStamps((eSwitch)src.periodtimestamps());
  if (src.afteractions() != cdm::eSwitch::NullSwitch)
    dst.SetAfterActions((eSwitch)src.afteractions());
  if (src.reloadstate() != cdm::eSwitch::NullSwitch)
    dst.SetReloadState((eSwitch)src.reloadstate());
  dst.SetDirectory(src.directory());
  dst.SetFileName(src.filename());
}
cdm::AutoSerializationData* PBEngine::Unload(const SEAutoSerialization& src)
{
  cdm::AutoSerializationData *dst = new cdm::AutoSerializationData();
  PBEngine::Serialize(src, *dst);
  return dst;
}
void PBEngine::Serialize(const SEAutoSerialization& src, cdm::AutoSerializationData& dst)
{
  if (src.HasPeriod())
    dst.set_allocated_period(PBProperty::Unload(*src.m_Period));
  dst.set_periodtimestamps((cdm::eSwitch)src.m_PeriodTimeStamps);
  dst.set_afteractions((cdm::eSwitch)src.m_AfterActions);
  dst.set_reloadstate((cdm::eSwitch)src.m_ReloadState);
  if (src.HasDirectory())
    dst.set_directory(src.m_Directory);
  if (src.HasFileName())
    dst.set_filename(src.m_FileName);
}

void PBEngine::Load(const cdm::DynamicStabilizationData& src, SEDynamicStabilization& dst)
{
  PBEngine::Serialize(src, dst);
}
void PBEngine::Serialize(const cdm::DynamicStabilizationData& src, SEDynamicStabilization& dst)
{
  dst.Clear();
  if (src.trackingstabilization() != cdm::eSwitch::NullSwitch)
    dst.TrackStabilization((eSwitch)src.trackingstabilization());
  if (src.has_restingconvergence())
    PBEngine::Load(src.restingconvergence(), dst.GetRestingConvergence());
  if (src.has_feedbackconvergence())
    PBEngine::Load(src.feedbackconvergence(), dst.GetFeedbackConvergence());

  for (auto itr : src.conditionconvergence())
  {
    SEDynamicStabilizationEngineConvergence* c = new SEDynamicStabilizationEngineConvergence(dst.GetLogger());
    PBEngine::Load(itr.second, *c);
    dst.m_ConditionConvergence[itr.first] = c;
  }
}
cdm::DynamicStabilizationData* PBEngine::Unload(const SEDynamicStabilization& src)
{
  cdm::DynamicStabilizationData* dst(new cdm::DynamicStabilizationData());
  PBEngine::Serialize(src, *dst);
  return dst;
}
void PBEngine::Serialize(const SEDynamicStabilization& src, cdm::DynamicStabilizationData& dst)
{
  dst.set_trackingstabilization((cdm::eSwitch)src.m_TrackingStabilization);
  dst.set_allocated_restingconvergence(PBEngine::Unload(*src.m_RestingConvergence));
  if (src.HasFeedbackConvergence())
    dst.set_allocated_feedbackconvergence(PBEngine::Unload(*src.m_FeedbackConvergence));
  for (auto &c : src.m_ConditionConvergence)
  {
    cdm::DynamicStabilizationEngineConvergenceData* cData = PBEngine::Unload(*c.second);
    (*dst.mutable_conditionconvergence())[c.first] = *cData;
    delete cData;
  }
}


void PBEngine::Load(const cdm::DynamicStabilizationEngineConvergenceData& src, SEDynamicStabilizationEngineConvergence& dst)
{
  PBEngine::Serialize(src, dst);
}
void PBEngine::Serialize(const cdm::DynamicStabilizationEngineConvergenceData& src, SEDynamicStabilizationEngineConvergence& dst)
{
  dst.Clear();
  // TODO Warn if these are not provided
  if (src.has_convergencetime())
    PBProperty::Load(src.convergencetime(), dst.GetConvergenceTime());
  if (src.has_minimumreactiontime())
    PBProperty::Load(src.minimumreactiontime(), dst.GetMinimumReactionTime());
  if (src.has_maximumallowedstabilizationtime())
    PBProperty::Load(src.maximumallowedstabilizationtime(), dst.GetMaximumAllowedStabilizationTime());
  for (int i = 0; i < src.propertyconvergence_size(); i++)
  {
    const cdm::DynamicStabilizationPropertyConvergenceData& pcData = src.propertyconvergence(i);
    if (pcData.has_datarequest())
    {
      SEDataRequest& dr = dst.m_DataRequestMgr->CreateDataRequest((eDataRequest_Category)pcData.datarequest().category());
      PBScenario::Load(pcData.datarequest(), dr);
      dst.CreatePropertyConvergence(dr, pcData.percentdifference());
    }
    else
    {
      dst.Error("Ignoring property convergence that has no data request");
    }
  }
}
cdm::DynamicStabilizationEngineConvergenceData* PBEngine::Unload(const SEDynamicStabilizationEngineConvergence& src)
{
  cdm::DynamicStabilizationEngineConvergenceData* dst = new cdm::DynamicStabilizationEngineConvergenceData();
  PBEngine::Serialize(src, *dst);
  return dst;
}
void PBEngine::Serialize(const SEDynamicStabilizationEngineConvergence& src, cdm::DynamicStabilizationEngineConvergenceData& dst)
{
  dst.set_allocated_convergencetime(PBProperty::Unload(*src.m_ConvergenceTime));
  dst.set_allocated_minimumreactiontime(PBProperty::Unload(*src.m_MinimumReactionTime));
  dst.set_allocated_maximumallowedstabilizationtime(PBProperty::Unload(*src.m_MaximumAllowedStabilizationTime));
  for (auto pc : src.m_PropertyConvergence)
  {
    cdm::DynamicStabilizationPropertyConvergenceData* pcData = dst.mutable_propertyconvergence()->Add();
    pcData->set_percentdifference(pc->m_Error);
    pcData->set_allocated_datarequest(PBScenario::Unload(pc->m_DataRequest));
  }
}

void PBEngine::Load(const cdm::TimedStabilizationData& src, SETimedStabilization& dst)
{
  PBEngine::Serialize(src, dst);
}
void PBEngine::Serialize(const cdm::TimedStabilizationData& src, SETimedStabilization& dst)
{
  dst.Clear();
  if (src.trackingstabilization() != cdm::eSwitch::NullSwitch)
    dst.TrackStabilization((eSwitch)src.trackingstabilization());
  if (src.has_restingstabilizationtime())
    PBProperty::Load(src.restingstabilizationtime(), dst.GetRestingStabilizationTime());
  if (src.has_feedbackstabilizationtime())
    PBProperty::Load(src.feedbackstabilizationtime(), dst.GetFeedbackStabilizationTime());
  for (auto itr : src.conditionstabilization())
  {
    SEScalarTime* time = new SEScalarTime();
    PBProperty::Load(itr.second, *time);
    dst.m_ConditionTimes[itr.first] = time;
  }
}
cdm::TimedStabilizationData* PBEngine::Unload(const SETimedStabilization& src)
{
  cdm::TimedStabilizationData* dst(new cdm::TimedStabilizationData());
  PBEngine::Serialize(src, *dst);
  return dst;
}
void PBEngine::Serialize(const SETimedStabilization& src, cdm::TimedStabilizationData& dst)
{
  dst.set_trackingstabilization((cdm::eSwitch)src.m_TrackingStabilization);
  dst.set_allocated_restingstabilizationtime(PBProperty::Unload(*src.m_RestingStabilizationTime));
  if (src.HasFeedbackStabilizationTime())
    dst.set_allocated_feedbackstabilizationtime(PBProperty::Unload(*src.m_FeedbackStabilizationTime));
  for (auto cc : src.m_ConditionTimes)
  {
    if (cc.second == nullptr)
      continue;
    cdm::ScalarTimeData* time = PBProperty::Unload(*cc.second);
    (*dst.mutable_conditionstabilization())[cc.first] = *time;
    delete time;
  }
}

bool PBEngine::SerializeToString(const SEDynamicStabilization& src, std::string& output, SerializationMode m)
{
  cdm::DynamicStabilizationData data;
  PBEngine::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m);
}
bool PBEngine::SerializeToFile(const SEDynamicStabilization& src, const std::string& filename, SerializationMode m)
{
  cdm::DynamicStabilizationData data;
  PBEngine::Serialize(src, data);
  std::string content;
  PBEngine::SerializeToString(src, content, m);
  return WriteFile(content, filename, m);
}
bool PBEngine::SerializeFromString(const std::string& src, SEDynamicStabilization& dst, SerializationMode m)
{
  cdm::DynamicStabilizationData data;
  if (!PBUtils::SerializeFromString(src, data, m))
    return false;
  PBEngine::Load(data, dst);
  return true;
}
bool PBEngine::SerializeFromFile(const std::string& filename, SEDynamicStabilization& dst, SerializationMode m)
{
  std::string content = ReadFile(filename, m);
  if (content.empty())
    return false;
  return PBEngine::SerializeFromString(content, dst, m);
}

bool PBEngine::SerializeToString(const SETimedStabilization& src, std::string& output, SerializationMode m)
{
  cdm::TimedStabilizationData data;
  PBEngine::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m);
}
bool PBEngine::SerializeToFile(const SETimedStabilization& src, const std::string& filename, SerializationMode m)
{
  cdm::TimedStabilizationData data;
  PBEngine::Serialize(src, data);
  std::string content;
  PBEngine::SerializeToString(src, content, m);
  return WriteFile(content, filename, m);
}
bool PBEngine::SerializeFromString(const std::string& src, SETimedStabilization& dst, SerializationMode m)
{
  cdm::TimedStabilizationData data;
  if (!PBUtils::SerializeFromString(src, data, m))
    return false;
  PBEngine::Load(data, dst);
  return true;
}
bool PBEngine::SerializeFromFile(const std::string& filename, SETimedStabilization& dst, SerializationMode m)
{
  std::string content = ReadFile(filename, m);
  if (content.empty())
    return false;
  return PBEngine::SerializeFromString(content, dst, m);
}

