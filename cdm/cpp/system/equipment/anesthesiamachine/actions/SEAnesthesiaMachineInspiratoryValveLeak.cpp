/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineInspiratoryValveLeak.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBAnesthesiaMachineActions.h"

SEAnesthesiaMachineInspiratoryValveLeak::SEAnesthesiaMachineInspiratoryValveLeak() : SEAnesthesiaMachineAction()
{
  m_Severity = nullptr;
}

SEAnesthesiaMachineInspiratoryValveLeak::~SEAnesthesiaMachineInspiratoryValveLeak()
{
  Clear();
}

void SEAnesthesiaMachineInspiratoryValveLeak::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  SAFE_DELETE(m_Severity);
}

void SEAnesthesiaMachineInspiratoryValveLeak::Copy(const SEAnesthesiaMachineInspiratoryValveLeak& src)
{// Using Bindings to make a copy
  PBAnesthesiaMachineAction::Copy(src, *this);
}

bool SEAnesthesiaMachineInspiratoryValveLeak::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid() && HasSeverity();
}

bool SEAnesthesiaMachineInspiratoryValveLeak::IsActive() const
{
  return HasSeverity() ? !m_Severity->IsZero() : false;
}

bool SEAnesthesiaMachineInspiratoryValveLeak::HasSeverity() const
{
  return m_Severity != nullptr;
}
SEScalar0To1& SEAnesthesiaMachineInspiratoryValveLeak::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
double SEAnesthesiaMachineInspiratoryValveLeak::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SEAnesthesiaMachineInspiratoryValveLeak::ToString(std::ostream &str) const
{
  str << "Anesthesia Machine Action : Inspiratory Valve Leak";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tSeverity :"; HasSeverity() ? str << *m_Severity : str << "NaN";
  str << std::flush;
}