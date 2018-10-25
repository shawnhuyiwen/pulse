/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEForcedExhale.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarTime.h"
#include "io/protobuf/PBPatientActions.h"

SEForcedExhale::SEForcedExhale() : SEConsciousRespirationCommand()
{
  m_ExpiratoryReserveVolumeFraction = nullptr;
  m_Period = nullptr;
}

SEForcedExhale::~SEForcedExhale()
{
  Clear();
}

void SEForcedExhale::Clear()
{
  SEConsciousRespirationCommand::Clear();
  SAFE_DELETE(m_ExpiratoryReserveVolumeFraction);
  SAFE_DELETE(m_Period);
}

void SEForcedExhale::Copy(const SEForcedExhale& src)
{
  PBPatientAction::Copy(src, *this);
}

bool SEForcedExhale::IsValid() const
{
  return SEConsciousRespirationCommand::IsValid() && HasExpiratoryReserveVolumeFraction() && HasPeriod();
}

bool SEForcedExhale::IsActive() const
{
  return SEConsciousRespirationCommand::IsActive();
}

bool SEForcedExhale::HasExpiratoryReserveVolumeFraction() const
{
  return m_ExpiratoryReserveVolumeFraction == nullptr ? false : m_ExpiratoryReserveVolumeFraction->IsValid();
}
SEScalar0To1& SEForcedExhale::GetExpiratoryReserveVolumeFraction()
{
  if (m_ExpiratoryReserveVolumeFraction == nullptr)
    m_ExpiratoryReserveVolumeFraction = new SEScalar0To1();
  return *m_ExpiratoryReserveVolumeFraction;
}
double SEForcedExhale::GetExpiratoryReserveVolumeFraction() const
{
  if (m_ExpiratoryReserveVolumeFraction == nullptr)
    return SEScalar::dNaN();
  return m_ExpiratoryReserveVolumeFraction->GetValue();
}

bool SEForcedExhale::HasPeriod() const
{
  return m_Period == nullptr ? false : m_Period->IsValid();
}
SEScalarTime& SEForcedExhale::GetPeriod()
{
  if (m_Period == nullptr)
    m_Period = new SEScalarTime();
  return *m_Period;
}
double SEForcedExhale::GetPeriod(const TimeUnit& unit) const
{
  if (m_Period == nullptr)
    return SEScalar::dNaN();
  return m_Period->GetValue(unit);
}

void SEForcedExhale::ToString(std::ostream &str) const
{
  str << "Forced Exhale";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tExpiratoryReserveVolumeFraction: "; HasExpiratoryReserveVolumeFraction() ? str << *m_ExpiratoryReserveVolumeFraction : str << "NaN";
  str << "\n\tPeriod: "; HasPeriod() ? str << *m_Period : str << "NaN";
  str << std::flush;
}