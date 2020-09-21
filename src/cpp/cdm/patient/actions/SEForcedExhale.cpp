/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEForcedExhale.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarTime.h"
#include "io/protobuf/PBPatientActions.h"

SEForcedExhale::SEForcedExhale(Logger* logger) : SEConsciousRespirationCommand(logger)
{
  m_ExpiratoryReserveVolumeFraction = nullptr;
  m_ExhalePeriod = nullptr;
  m_HoldPeriod = nullptr;
  m_ReleasePeriod = nullptr;
}

SEForcedExhale::~SEForcedExhale()
{
  SAFE_DELETE(m_ExpiratoryReserveVolumeFraction);
  SAFE_DELETE(m_ExhalePeriod);
  SAFE_DELETE(m_HoldPeriod);
  SAFE_DELETE(m_ReleasePeriod);
}

void SEForcedExhale::Clear()
{
  SEConsciousRespirationCommand::Clear();
  INVALIDATE_PROPERTY(m_ExpiratoryReserveVolumeFraction);
  INVALIDATE_PROPERTY(m_ExhalePeriod);
  INVALIDATE_PROPERTY(m_HoldPeriod);
  INVALIDATE_PROPERTY(m_ReleasePeriod);
}

void SEForcedExhale::Copy(const SEForcedExhale& src)
{
  PBPatientAction::Copy(src, *this);
}

bool SEForcedExhale::IsValid() const
{
  return SEConsciousRespirationCommand::IsValid() && HasExpiratoryReserveVolumeFraction() && HasExhalePeriod();
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

bool SEForcedExhale::HasExhalePeriod() const
{
  return m_ExhalePeriod == nullptr ? false : m_ExhalePeriod->IsValid();
}
SEScalarTime& SEForcedExhale::GetExhalePeriod()
{
  if (m_ExhalePeriod == nullptr)
    m_ExhalePeriod = new SEScalarTime();
  return *m_ExhalePeriod;
}
double SEForcedExhale::GetExhalePeriod(const TimeUnit& unit) const
{
  if (m_ExhalePeriod == nullptr)
    return SEScalar::dNaN();
  return m_ExhalePeriod->GetValue(unit);
}

bool SEForcedExhale::HasHoldPeriod() const
{
  return m_HoldPeriod == nullptr ? false : m_HoldPeriod->IsValid();
}
SEScalarTime& SEForcedExhale::GetHoldPeriod()
{
  if (m_HoldPeriod == nullptr)
    m_HoldPeriod = new SEScalarTime();
  return *m_HoldPeriod;
}
double SEForcedExhale::GetHoldPeriod(const TimeUnit& unit) const
{
  if (m_HoldPeriod == nullptr)
    return SEScalar::dNaN();
  return m_HoldPeriod->GetValue(unit);
}

bool SEForcedExhale::HasReleasePeriod() const
{
  return m_ReleasePeriod == nullptr ? false : m_ReleasePeriod->IsValid();
}
SEScalarTime& SEForcedExhale::GetReleasePeriod()
{
  if (m_ReleasePeriod == nullptr)
    m_ReleasePeriod = new SEScalarTime();
  return *m_ReleasePeriod;
}
double SEForcedExhale::GetReleasePeriod(const TimeUnit& unit) const
{
  if (m_ReleasePeriod == nullptr)
    return SEScalar::dNaN();
  return m_ReleasePeriod->GetValue(unit);
}

void SEForcedExhale::ToString(std::ostream &str) const
{
  str << "Forced Exhale";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tExpiratoryReserveVolumeFraction: "; HasExpiratoryReserveVolumeFraction() ? str << *m_ExpiratoryReserveVolumeFraction : str << "NaN";
  str << "\n\tExhalePeriod: "; HasExhalePeriod() ? str << *m_ExhalePeriod : str << "NaN";
  str << "\n\tHoldPeriod: "; HasHoldPeriod() ? str << *m_HoldPeriod : str << "NaN";
  str << "\n\tReleasePeriod: "; HasReleasePeriod() ? str << *m_ReleasePeriod : str << "NaN";
  str << std::flush;
}