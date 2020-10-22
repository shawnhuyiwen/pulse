/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/conditions/SEChronicRenalStenosis.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBPatientConditions.h"

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

void SEChronicRenalStenosis::ToString(std::ostream &str) const
{
  str << "Patient Condition : Chronic Renal Stenosis";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tLeft Kidney Occlusion 0To1: "; HasLeftKidneySeverity() ? str << *m_LeftKidneySeverity : str << "NaN";
  str << "\n\tRight Kidney Occlusion 0To1: "; HasRightKidneySeverity() ? str << *m_RightKidneySeverity : str << "NaN";
  str << std::flush;
}