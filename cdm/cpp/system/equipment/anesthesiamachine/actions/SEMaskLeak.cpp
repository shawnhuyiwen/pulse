/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/anesthesiamachine/actions/SEMaskLeak.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBAnesthesiaMachineActions.h"

SEMaskLeak::SEMaskLeak() : SEAnesthesiaMachineAction()
{
  m_Severity = nullptr;
}

SEMaskLeak::~SEMaskLeak()
{
  Clear();
}

void SEMaskLeak::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  SAFE_DELETE(m_Severity);
}

void SEMaskLeak::Copy(const SEMaskLeak& src)
{// Using Bindings to make a copy
  PBAnesthesiaMachineAction::Copy(src, *this);
}

bool SEMaskLeak::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid() && HasSeverity();
}

bool SEMaskLeak::IsActive() const
{
  return HasSeverity() ? !m_Severity->IsZero() : false;
}

bool SEMaskLeak::HasSeverity() const
{
  return m_Severity != nullptr;
}
SEScalar0To1& SEMaskLeak::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
double SEMaskLeak::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SEMaskLeak::ToString(std::ostream &str) const
{
  str << "Anesthesia Machine Action : Mask Leak";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tSeverity :"; HasSeverity() ? str << *m_Severity : str << "NaN";
  str << std::flush;
}