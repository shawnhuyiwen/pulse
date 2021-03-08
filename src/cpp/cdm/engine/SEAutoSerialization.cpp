/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "engine/SEAutoSerialization.h"
#include "properties/SEScalarTime.h"

SEAutoSerialization::SEAutoSerialization(Logger* logger) : Loggable(logger)
{
  m_Period = nullptr;
  m_PeriodTimeStamps = eSwitch::Off;
  m_AfterActions = eSwitch::Off;
  m_ReloadState  = eSwitch::Off;
  m_Directory    = "";
  m_FileName     = "";
}

SEAutoSerialization::~SEAutoSerialization()
{
  SAFE_DELETE(m_Period);
  m_PeriodTimeStamps = eSwitch::Off;
  m_AfterActions = eSwitch::Off;
  m_ReloadState = eSwitch::Off;
  m_Directory = "";
  m_FileName = "";
}

void SEAutoSerialization::Clear()
{
  INVALIDATE_PROPERTY(m_Period);
  m_PeriodTimeStamps = eSwitch::Off;
  m_AfterActions = eSwitch::Off;
  m_ReloadState = eSwitch::Off;
  m_Directory = "";
  m_FileName = "";
}

bool SEAutoSerialization::IsValid() const
{
  if (HasPeriod() && m_Period->IsNegative())
    return false;
  else if (m_AfterActions == eSwitch::Off)
    return false;
  if (!HasDirectory())
    return false;
  if (!HasFileName())
    return false;
  return true;
}

bool SEAutoSerialization::HasPeriod() const
{
  return m_Period == nullptr ? false : m_Period->IsValid();
}
SEScalarTime& SEAutoSerialization::GetPeriod()
{
  if (m_Period == nullptr)
    m_Period = new SEScalarTime();
  return *m_Period;
}
double SEAutoSerialization::GetPeriod(const TimeUnit& unit) const
{
  if (m_Period == nullptr)
    return SEScalar::dNaN();
  return m_Period->GetValue(unit);
}

eSwitch SEAutoSerialization::GetPeriodTimeStamps() const
{
  return m_PeriodTimeStamps;
}
void SEAutoSerialization::SetPeriodTimeStamps(eSwitch v)
{
  m_PeriodTimeStamps = (v == eSwitch::NullSwitch) ? eSwitch::Off : v;
}

eSwitch SEAutoSerialization::GetAfterActions() const
{
  return m_AfterActions;
}
void SEAutoSerialization::SetAfterActions(eSwitch v)
{
  m_AfterActions = (v == eSwitch::NullSwitch) ? eSwitch::Off : v;
}

eSwitch SEAutoSerialization::GetReloadState() const
{
  return m_ReloadState;
}
void SEAutoSerialization::SetReloadState(eSwitch v)
{
  m_ReloadState = (v == eSwitch::NullSwitch) ? eSwitch::Off : v;
}

std::string SEAutoSerialization::GetDirectory() const
{
  return m_Directory;
}
void SEAutoSerialization::SetDirectory(const std::string& dir)
{
  m_Directory = dir;
}
bool SEAutoSerialization::HasDirectory() const
{
  return m_Directory.empty() ? false : true;
}
void SEAutoSerialization::InvalidateDirectory()
{
  m_Directory = "";
}

std::string SEAutoSerialization::GetFileName() const
{
  return m_FileName;
}
void SEAutoSerialization::SetFileName(const std::string& name)
{
  m_FileName = name;
}
bool SEAutoSerialization::HasFileName() const
{
  return m_FileName.empty() ? false : true;
}
void SEAutoSerialization::InvalidateFileName()
{
  m_FileName = "";
}