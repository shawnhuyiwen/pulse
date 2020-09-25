/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "engine/SEDynamicStabilizationEngineConvergence.h"
#include "engine/SEDynamicStabilizationPropertyConvergence.h"
#include "engine/SEDataRequest.h"
#include "engine/SEDataRequestManager.h"
#include "properties/SEScalarTime.h"

SEDynamicStabilizationEngineConvergence::SEDynamicStabilizationEngineConvergence(Logger *logger) : Loggable(logger)
{
  m_DataRequestMgr = new SEDataRequestManager(logger);
  m_ConvergenceTime = new SEScalarTime();
  m_MinimumReactionTime = new SEScalarTime();
  m_MaximumAllowedStabilizationTime = new SEScalarTime();
  Clear();
}

SEDynamicStabilizationEngineConvergence::~SEDynamicStabilizationEngineConvergence()
{
  Clear();
  SAFE_DELETE(m_DataRequestMgr)
  SAFE_DELETE(m_ConvergenceTime)
  SAFE_DELETE(m_MinimumReactionTime)
  SAFE_DELETE(m_MaximumAllowedStabilizationTime)
}

void SEDynamicStabilizationEngineConvergence::Clear()
{
  m_ConvergenceTime->SetValue(0, TimeUnit::s);
  m_MinimumReactionTime->SetValue(0, TimeUnit::s);
  m_MaximumAllowedStabilizationTime->SetValue(0, TimeUnit::s);
  DELETE_VECTOR(m_PropertyConvergence);
}

SEDynamicStabilizationPropertyConvergence& SEDynamicStabilizationEngineConvergence::CreatePropertyConvergence(SEDataRequest& dr, double percentError)
{
  SEDynamicStabilizationPropertyConvergence* p = new SEDynamicStabilizationPropertyConvergence(dr, GetLogger());
  p->m_Error = percentError;
  m_PropertyConvergence.push_back(p);
  return *p;
}

const std::vector<SEDynamicStabilizationPropertyConvergence*>& SEDynamicStabilizationEngineConvergence::GetPropertyConvergence() const
{
  return m_PropertyConvergence;
}

SEScalarTime& SEDynamicStabilizationEngineConvergence::GetConvergenceTime()
{ 
  return *m_ConvergenceTime;
}
double SEDynamicStabilizationEngineConvergence::GetConvergenceTime(const TimeUnit& unit) const
{ 
  return m_ConvergenceTime->GetValue(unit);
}

SEScalarTime& SEDynamicStabilizationEngineConvergence::GetMinimumReactionTime() 
{ 
  return *m_MinimumReactionTime;
}
double SEDynamicStabilizationEngineConvergence::GetMinimumReactionTime(const TimeUnit& unit) const
{ 
  return m_MinimumReactionTime->GetValue(unit);
}

SEScalarTime& SEDynamicStabilizationEngineConvergence::GetMaximumAllowedStabilizationTime()
{
  return *m_MaximumAllowedStabilizationTime;
}
double SEDynamicStabilizationEngineConvergence::GetMaximumAllowedStabilizationTime(const TimeUnit& unit) const
{
  return m_MaximumAllowedStabilizationTime->GetValue(unit);
}
