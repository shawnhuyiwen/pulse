/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEForcedInhale.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarTime.h"
#include "io/protobuf/PBPatientActions.h"

SEForcedInhale::SEForcedInhale() : SEConsciousRespirationCommand()
{
  m_InspiratoryCapacityFraction = nullptr;
  m_Period = nullptr;
}

SEForcedInhale::~SEForcedInhale()
{
  Clear();
}

void SEForcedInhale::Clear()
{
  SEConsciousRespirationCommand::Clear();
  SAFE_DELETE(m_InspiratoryCapacityFraction);
  SAFE_DELETE(m_Period);
}

void SEForcedInhale::Copy(const SEForcedInhale& src)
{
  PBPatientAction::Copy(src, *this);
}

bool SEForcedInhale::IsValid() const
{
  return SEConsciousRespirationCommand::IsValid() && HasInspiratoryCapacityFraction() && HasPeriod();
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

bool SEForcedInhale::HasPeriod() const
{
  return m_Period == nullptr ? false : m_Period->IsValid();
}
SEScalarTime& SEForcedInhale::GetPeriod()
{
  if (m_Period == nullptr)
    m_Period = new SEScalarTime();
  return *m_Period;
}
double SEForcedInhale::GetPeriod(const TimeUnit& unit) const
{
  if (m_Period == nullptr)
    return SEScalar::dNaN();
  return m_Period->GetValue(unit);
}

void SEForcedInhale::ToString(std::ostream &str) const
{
  str << "Forced Inhale";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tInspiratoryCapacityFraction: "; HasInspiratoryCapacityFraction() ? str << *m_InspiratoryCapacityFraction : str << "NaN";
  str << "\n\tPeriod: "; HasPeriod() ? str << *m_Period : str << "NaN";
  str << std::flush;
}