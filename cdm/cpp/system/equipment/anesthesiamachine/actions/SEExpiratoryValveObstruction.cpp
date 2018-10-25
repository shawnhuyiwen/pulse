/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/anesthesiamachine/actions/SEExpiratoryValveObstruction.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBAnesthesiaMachineActions.h"

SEExpiratoryValveObstruction::SEExpiratoryValveObstruction() : SEAnesthesiaMachineAction()
{
  m_Severity = nullptr;
}

SEExpiratoryValveObstruction::~SEExpiratoryValveObstruction()
{
  Clear();
}

void SEExpiratoryValveObstruction::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  SAFE_DELETE(m_Severity);
}

void SEExpiratoryValveObstruction::Copy(const SEExpiratoryValveObstruction& src)
{// Using Bindings to make a copy
  PBAnesthesiaMachineAction::Copy(src, *this);
}

bool SEExpiratoryValveObstruction::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid() && HasSeverity();
}

bool SEExpiratoryValveObstruction::IsActive() const
{
  return HasSeverity() ? !m_Severity->IsZero() : false;
}

bool SEExpiratoryValveObstruction::HasSeverity() const
{
  return m_Severity != nullptr;
}
SEScalar0To1& SEExpiratoryValveObstruction::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
double SEExpiratoryValveObstruction::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SEExpiratoryValveObstruction::ToString(std::ostream &str) const
{
  str << "Anesthesia Machine Action : Expiratory Valve Obstruction";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tSeverity :"; HasSeverity() ? str << *m_Severity : str << "NaN";
  str << std::flush;
}