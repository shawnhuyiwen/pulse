/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/anesthesiamachine/actions/SETubeCuffLeak.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBAnesthesiaMachineActions.h"

SETubeCuffLeak::SETubeCuffLeak() : SEAnesthesiaMachineAction()
{
  m_Severity = nullptr;
}

SETubeCuffLeak::~SETubeCuffLeak()
{
  Clear();
}

void SETubeCuffLeak::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  SAFE_DELETE(m_Severity);
}

void SETubeCuffLeak::Copy(const SETubeCuffLeak& src)
{// Using Bindings to make a copy
  PBAnesthesiaMachineAction::Copy(src, *this);
}

bool SETubeCuffLeak::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid() && HasSeverity();
}

bool SETubeCuffLeak::IsActive() const
{
  return HasSeverity() ? !m_Severity->IsZero() : false;
}

bool SETubeCuffLeak::HasSeverity() const
{
  return m_Severity != nullptr;
}
SEScalar0To1& SETubeCuffLeak::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
double SETubeCuffLeak::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SETubeCuffLeak::ToString(std::ostream &str) const
{
  str << "Anesthesia Machine Action : Tube Cuff Leak";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tSeverity :"; HasSeverity() ? str << *m_Severity : str << "NaN";
  str << std::flush;
}