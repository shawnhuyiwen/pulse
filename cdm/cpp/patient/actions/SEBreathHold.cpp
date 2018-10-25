/* Distributed under the Apache License, Version 2.0.
See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEBreathHold.h"
#include "properties/SEScalarTime.h"
#include "io/protobuf/PBPatientActions.h"

SEBreathHold::SEBreathHold() : SEConsciousRespirationCommand()
{
  m_Period = nullptr;
}

SEBreathHold::~SEBreathHold()
{
  Clear();
}

void SEBreathHold::Clear()
{
  SEConsciousRespirationCommand::Clear();
  SAFE_DELETE(m_Period);
}

void SEBreathHold::Copy(const SEBreathHold& src)
{
  PBPatientAction::Copy(src, *this);
}

bool SEBreathHold::IsValid() const
{
  return SEConsciousRespirationCommand::IsValid() && HasPeriod();
}

bool SEBreathHold::IsActive() const
{
  return SEConsciousRespirationCommand::IsActive();
}

bool SEBreathHold::HasPeriod() const
{
  return m_Period == nullptr ? false : m_Period->IsValid();
}
SEScalarTime& SEBreathHold::GetPeriod()
{
  if (m_Period == nullptr)
    m_Period = new SEScalarTime();
  return *m_Period;
}
double SEBreathHold::GetPeriod(const TimeUnit& unit) const
{
  if (m_Period == nullptr)
    return SEScalar::dNaN();
  return m_Period->GetValue(unit);
}

void SEBreathHold::ToString(std::ostream &str) const
{
  str << "Breath Hold";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tPeriod: "; HasPeriod() ? str << *m_Period : str << "NaN";
  str << std::flush;
}