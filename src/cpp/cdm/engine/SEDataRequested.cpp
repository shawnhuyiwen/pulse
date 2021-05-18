/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "engine/SEDataRequested.h"
#include "engine/SEDataRequestManager.h"
#include "engine/SEEngineTracker.h"
#include "engine/SEEventManager.h"
#include "engine/SEPatientConfiguration.h"
#include "substance/SESubstanceManager.h"
#include "io/protobuf/PBEngine.h"
#include "utils/DataTrack.h"

SEDataRequested::SEDataRequested()
{
  m_KeepLogMessages = false;
  m_KeepEventChanges = false;
  Clear();
}

SEDataRequested::~SEDataRequested()
{

}

void SEDataRequested::SetEngine(const PhysiologyEngine& engine)
{
  m_Engine = &engine;
}

bool SEDataRequested::SerializeToString(std::string& dst, SerializationFormat m) const
{
  return PBEngine::SerializeToString(*this, dst, m);
}
bool SEDataRequested::SerializeToString(const std::vector<SEDataRequested*>& src, std::string& dst, SerializationFormat m)
{
  return PBEngine::SerializeToString(src, dst, m);
}

void SEDataRequested::Clear()
{
  m_ID = -1;
  m_IsActive = false;
  m_Values.clear();
  m_EventChanges.clear();
  m_LogMessages.Clear();
}

int SEDataRequested::GetID() const
{
  return m_ID;
}
void SEDataRequested::SetID(int id)
{
  m_ID = id;
}

bool SEDataRequested::IsActive() const
{
  return m_IsActive;
}
void SEDataRequested::SetIsActive(bool b)
{
  m_IsActive = b;
}

void SEDataRequested::PullDataRequested()
{
  if (!m_Engine)
    return;
  m_Values.clear();
  double currentTime_s = m_Engine->GetSimulationTime(TimeUnit::s);
  m_Values.push_back(currentTime_s);
  m_Engine->GetEngineTracker()->TrackData(currentTime_s);
  size_t length = m_Engine->GetEngineTracker()->GetDataTrack().NumTracks() + 1;
  for (size_t i = 1; i < length; i++)
    m_Values.push_back(m_Engine->GetEngineTracker()->GetDataTrack().GetProbe(i - 1));
}
void SEDataRequested::ClearDataRequested()
{
  m_Values.clear();
  m_EventChanges.clear();
  m_LogMessages.Clear();
}

const std::vector<double>& SEDataRequested::GetValues() const
{
  return m_Values;
}

const std::vector<SEEventChange>& SEDataRequested::GetEventChanges() const
{
  return m_EventChanges;
}
void SEDataRequested::HandleEvent(eEvent type, bool active, const SEScalarTime* time)
{
  if (m_KeepEventChanges)
    m_EventChanges.push_back({ type, active, time });
  if (type == eEvent::IrreversibleState && active)
    m_IsActive = false;
}

const LogMessages& SEDataRequested::GetLogMessages() const
{
  return m_LogMessages;
}

void SEDataRequested::ForwardDebug(const std::string& msg, const std::string& origin)
{
  if (m_KeepLogMessages)
    m_LogMessages.debug_msgs.push_back(msg + origin);
}
void SEDataRequested::ForwardInfo(const std::string& msg, const std::string& origin)
{
  if (m_KeepLogMessages)
    m_LogMessages.info_msgs.push_back(msg + origin);
}
void SEDataRequested::ForwardWarning(const std::string& msg, const std::string& origin)
{
  if (m_KeepLogMessages)
    m_LogMessages.warning_msgs.push_back(msg + origin);
}
void SEDataRequested::ForwardError(const std::string& msg, const std::string& origin)
{
  m_IsActive = false;
  if (m_KeepLogMessages)
    m_LogMessages.error_msgs.push_back(msg + origin);
}
void SEDataRequested::ForwardFatal(const std::string& msg, const std::string& origin)
{
  m_IsActive = false;
  if (m_KeepLogMessages)
    m_LogMessages.fatal_msgs.push_back(msg + origin);
}
