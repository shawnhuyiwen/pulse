/* Distributed under the Apache License, Version 2.0.
See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEForcedPause.h"
#include "properties/SEScalarTime.h"
#include "io/protobuf/PBPatientActions.h"

SEForcedPause::SEForcedPause(Logger* logger) : SEConsciousRespirationCommand(logger)
{
  m_Period = nullptr;
}

SEForcedPause::~SEForcedPause()
{
  SAFE_DELETE(m_Period);
}

void SEForcedPause::Clear()
{
  SEConsciousRespirationCommand::Clear();
  INVALIDATE_PROPERTY(m_Period);
}

void SEForcedPause::Copy(const SEForcedPause& src)
{
  PBPatientAction::Copy(src, *this);
}

bool SEForcedPause::IsValid() const
{
  return SEConsciousRespirationCommand::IsValid() && HasPeriod();
}

bool SEForcedPause::IsActive() const
{
  return SEConsciousRespirationCommand::IsActive();
}

bool SEForcedPause::HasPeriod() const
{
  return m_Period == nullptr ? false : m_Period->IsValid();
}
SEScalarTime& SEForcedPause::GetPeriod()
{
  if (m_Period == nullptr)
    m_Period = new SEScalarTime();
  return *m_Period;
}
double SEForcedPause::GetPeriod(const TimeUnit& unit) const
{
  if (m_Period == nullptr)
    return SEScalar::dNaN();
  return m_Period->GetValue(unit);
}

void SEForcedPause::ToString(std::ostream &str) const
{
  str << "Breath Hold";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tPeriod: "; HasPeriod() ? str << *m_Period : str << "NaN";
  str << std::flush;
}