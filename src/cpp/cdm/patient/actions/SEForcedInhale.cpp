/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEForcedInhale.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarTime.h"
#include "io/protobuf/PBPatientActions.h"

SEForcedInhale::SEForcedInhale(Logger* logger) : SEConsciousRespirationCommand(logger)
{
  m_InspiratoryCapacityFraction = nullptr;
  m_InhalePeriod = nullptr;
  m_HoldPeriod = nullptr;
  m_ReleasePeriod = nullptr;
}

SEForcedInhale::~SEForcedInhale()
{
  SAFE_DELETE(m_InspiratoryCapacityFraction);
  SAFE_DELETE(m_InhalePeriod);
  SAFE_DELETE(m_HoldPeriod);
  SAFE_DELETE(m_ReleasePeriod);
}

void SEForcedInhale::Clear()
{
  SEConsciousRespirationCommand::Clear();
  INVALIDATE_PROPERTY(m_InspiratoryCapacityFraction);
  INVALIDATE_PROPERTY(m_InhalePeriod);
  INVALIDATE_PROPERTY(m_HoldPeriod);
  INVALIDATE_PROPERTY(m_ReleasePeriod);
}

void SEForcedInhale::Copy(const SEForcedInhale& src)
{
  PBPatientAction::Copy(src, *this);
}

bool SEForcedInhale::IsValid() const
{
  return SEConsciousRespirationCommand::IsValid() && HasInspiratoryCapacityFraction() && HasInhalePeriod();
}

bool SEForcedInhale::IsActive() const
{
  return SEConsciousRespirationCommand::IsActive();
}

bool SEForcedInhale::HasInspiratoryCapacityFraction() const
{
  return m_InspiratoryCapacityFraction == nullptr ? false : m_InspiratoryCapacityFraction->IsValid();
}
SEScalar0To1& SEForcedInhale::GetInspiratoryCapacityFraction()
{
  if (m_InspiratoryCapacityFraction == nullptr)
    m_InspiratoryCapacityFraction = new SEScalar0To1();
  return *m_InspiratoryCapacityFraction;
}
double SEForcedInhale::GetInspiratoryCapacityFraction() const
{
  if (m_InspiratoryCapacityFraction == nullptr)
    return SEScalar::dNaN();
  return m_InspiratoryCapacityFraction->GetValue();
}

bool SEForcedInhale::HasInhalePeriod() const
{
  return m_InhalePeriod == nullptr ? false : m_InhalePeriod->IsValid();
}
SEScalarTime& SEForcedInhale::GetInhalePeriod()
{
  if (m_InhalePeriod == nullptr)
    m_InhalePeriod = new SEScalarTime();
  return *m_InhalePeriod;
}
double SEForcedInhale::GetInhalePeriod(const TimeUnit& unit) const
{
  if (m_InhalePeriod == nullptr)
    return SEScalar::dNaN();
  return m_InhalePeriod->GetValue(unit);
}

bool SEForcedInhale::HasHoldPeriod() const
{
  return m_HoldPeriod == nullptr ? false : m_HoldPeriod->IsValid();
}
SEScalarTime& SEForcedInhale::GetHoldPeriod()
{
  if (m_HoldPeriod == nullptr)
    m_HoldPeriod = new SEScalarTime();
  return *m_HoldPeriod;
}
double SEForcedInhale::GetHoldPeriod(const TimeUnit& unit) const
{
  if (m_HoldPeriod == nullptr)
    return SEScalar::dNaN();
  return m_HoldPeriod->GetValue(unit);
}

bool SEForcedInhale::HasReleasePeriod() const
{
  return m_ReleasePeriod == nullptr ? false : m_ReleasePeriod->IsValid();
}
SEScalarTime& SEForcedInhale::GetReleasePeriod()
{
  if (m_ReleasePeriod == nullptr)
    m_ReleasePeriod = new SEScalarTime();
  return *m_ReleasePeriod;
}
double SEForcedInhale::GetReleasePeriod(const TimeUnit& unit) const
{
  if (m_ReleasePeriod == nullptr)
    return SEScalar::dNaN();
  return m_ReleasePeriod->GetValue(unit);
}

void SEForcedInhale::ToString(std::ostream &str) const
{
  str << "Forced Inhale";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tInspiratoryCapacityFraction: "; HasInspiratoryCapacityFraction() ? str << *m_InspiratoryCapacityFraction : str << "NaN";
  str << "\n\tInhalePeriod: "; HasInhalePeriod() ? str << *m_InhalePeriod : str << "NaN";
  str << "\n\tHoldPeriod: "; HasHoldPeriod() ? str << *m_HoldPeriod : str << "NaN";
  str << "\n\tReleasePeriod: "; HasReleasePeriod() ? str << *m_ReleasePeriod : str << "NaN";
  str << std::flush;
}