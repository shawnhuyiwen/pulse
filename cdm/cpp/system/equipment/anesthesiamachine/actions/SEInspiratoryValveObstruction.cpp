/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/anesthesiamachine/actions/SEInspiratoryValveObstruction.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBAnesthesiaMachineActions.h"

SEInspiratoryValveObstruction::SEInspiratoryValveObstruction() : SEAnesthesiaMachineAction()
{
  m_Severity = nullptr;
}

SEInspiratoryValveObstruction::~SEInspiratoryValveObstruction()
{
  Clear();
}

void SEInspiratoryValveObstruction::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  SAFE_DELETE(m_Severity);
}

void SEInspiratoryValveObstruction::Copy(const SEInspiratoryValveObstruction& src)
{// Using Bindings to make a copy
  PBAnesthesiaMachineAction::Copy(src, *this);
}

bool SEInspiratoryValveObstruction::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid() && HasSeverity();
}

bool SEInspiratoryValveObstruction::IsActive() const
{
  return HasSeverity() ? !m_Severity->IsZero() : false;
}

bool SEInspiratoryValveObstruction::HasSeverity() const
{
  return m_Severity != nullptr;
}
SEScalar0To1& SEInspiratoryValveObstruction::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
double SEInspiratoryValveObstruction::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SEInspiratoryValveObstruction::ToString(std::ostream &str) const
{
  str << "Anesthesia Machine Action : Inspiratory Valve Obstruction";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tSeverity :"; HasSeverity() ? str << *m_Severity : str << "NaN";
  str << std::flush;
}