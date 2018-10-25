/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/anesthesiamachine/actions/SEInspiratoryValveLeak.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBAnesthesiaMachineActions.h"

SEInspiratoryValveLeak::SEInspiratoryValveLeak() : SEAnesthesiaMachineAction()
{
  m_Severity = nullptr;
}

SEInspiratoryValveLeak::~SEInspiratoryValveLeak()
{
  Clear();
}

void SEInspiratoryValveLeak::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  SAFE_DELETE(m_Severity);
}

void SEInspiratoryValveLeak::Copy(const SEInspiratoryValveLeak& src)
{// Using Bindings to make a copy
  PBAnesthesiaMachineAction::Copy(src, *this);
}

bool SEInspiratoryValveLeak::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid() && HasSeverity();
}

bool SEInspiratoryValveLeak::IsActive() const
{
  return HasSeverity() ? !m_Severity->IsZero() : false;
}

bool SEInspiratoryValveLeak::HasSeverity() const
{
  return m_Severity != nullptr;
}
SEScalar0To1& SEInspiratoryValveLeak::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
double SEInspiratoryValveLeak::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SEInspiratoryValveLeak::ToString(std::ostream &str) const
{
  str << "Anesthesia Machine Action : Inspiratory Valve Leak";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tSeverity :"; HasSeverity() ? str << *m_Severity : str << "NaN";
  str << std::flush;
}