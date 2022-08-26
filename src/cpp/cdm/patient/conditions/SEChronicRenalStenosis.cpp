/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/conditions/SEChronicRenalStenosis.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/io/protobuf/PBPatientConditions.h"

SEChronicRenalStenosis::SEChronicRenalStenosis(Logger* logger) : SEPatientCondition(logger)
{
  m_LeftKidneySeverity = nullptr;
  m_RightKidneySeverity = nullptr;
}

SEChronicRenalStenosis::~SEChronicRenalStenosis()
{
  Clear();
  SAFE_DELETE(m_LeftKidneySeverity);
  SAFE_DELETE(m_RightKidneySeverity);
}

void SEChronicRenalStenosis::Clear()
{
  SEPatientCondition::Clear();
  INVALIDATE_PROPERTY(m_LeftKidneySeverity);
  INVALIDATE_PROPERTY(m_RightKidneySeverity);
}

void SEChronicRenalStenosis::Copy(const SEChronicRenalStenosis& src)
{
  PBPatientCondition::Copy(src, *this);
}

bool SEChronicRenalStenosis::IsValid() const
{
  return (HasLeftKidneySeverity() || HasRightKidneySeverity());
}
bool SEChronicRenalStenosis::IsActive() const
{
  if (!IsValid())
    return false;
  if (GetLeftKidneySeverity() > 0)
    return true;
  if (GetRightKidneySeverity() > 0)
    return true;
  return false;
}

bool SEChronicRenalStenosis::HasLeftKidneySeverity() const
{
  return m_LeftKidneySeverity == nullptr ? false : m_LeftKidneySeverity->IsValid();
}
SEScalar0To1& SEChronicRenalStenosis::GetLeftKidneySeverity()
{
  if (m_LeftKidneySeverity == nullptr)
    m_LeftKidneySeverity = new SEScalar0To1();
  return *m_LeftKidneySeverity;
}
double SEChronicRenalStenosis::GetLeftKidneySeverity() const
{
  if (m_LeftKidneySeverity == nullptr)
    return SEScalar::dNaN();
  return m_LeftKidneySeverity->GetValue();
}

bool SEChronicRenalStenosis::HasRightKidneySeverity() const
{
  return m_RightKidneySeverity == nullptr ? false : m_RightKidneySeverity->IsValid();
}
SEScalar0To1& SEChronicRenalStenosis::GetRightKidneySeverity()
{
  if (m_RightKidneySeverity == nullptr)
    m_RightKidneySeverity = new SEScalar0To1();
  return *m_RightKidneySeverity;
}
double SEChronicRenalStenosis::GetRightKidneySeverity() const
{
  if (m_RightKidneySeverity == nullptr)
    return SEScalar::dNaN();
  return m_RightKidneySeverity->GetValue();
}
