/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/anesthesiamachine/actions/SEExpiratoryValveLeak.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBAnesthesiaMachineActions.h"

SEExpiratoryValveLeak::SEExpiratoryValveLeak() : SEAnesthesiaMachineAction()
{
  m_Severity = nullptr;
}

SEExpiratoryValveLeak::~SEExpiratoryValveLeak()
{
  Clear();
}

void SEExpiratoryValveLeak::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  SAFE_DELETE(m_Severity);
}

void SEExpiratoryValveLeak::Copy(const SEExpiratoryValveLeak& src)
{// Using Bindings to make a copy
  PBAnesthesiaMachineAction::Copy(src, *this);
}

bool SEExpiratoryValveLeak::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid() && HasSeverity();
}

bool SEExpiratoryValveLeak::IsActive() const
{
  return HasSeverity() ? !m_Severity->IsZero() : false;
}

bool SEExpiratoryValveLeak::HasSeverity() const
{
  return m_Severity != nullptr;
}
SEScalar0To1& SEExpiratoryValveLeak::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
double SEExpiratoryValveLeak::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SEExpiratoryValveLeak::ToString(std::ostream &str) const
{
  str << "Anesthesia Machine Action : Expiratory Valve Leak";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tSeverity :"; HasSeverity() ? str << *m_Severity : str << "NaN";
  str << std::flush;
}