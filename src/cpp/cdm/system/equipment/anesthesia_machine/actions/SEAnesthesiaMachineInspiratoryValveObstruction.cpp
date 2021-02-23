/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineInspiratoryValveObstruction.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBEquipmentActions.h"

SEAnesthesiaMachineInspiratoryValveObstruction::SEAnesthesiaMachineInspiratoryValveObstruction(Logger* logger) : SEAnesthesiaMachineAction(logger)
{
  m_Severity = nullptr;
}

SEAnesthesiaMachineInspiratoryValveObstruction::~SEAnesthesiaMachineInspiratoryValveObstruction()
{
  SAFE_DELETE(m_Severity);
}

void SEAnesthesiaMachineInspiratoryValveObstruction::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  INVALIDATE_PROPERTY(m_Severity);
}

void SEAnesthesiaMachineInspiratoryValveObstruction::Copy(const SEAnesthesiaMachineInspiratoryValveObstruction& src, bool preserveState)
{// Using Bindings to make a copy
  PBEquipmentAction::Copy(src, *this);
}

bool SEAnesthesiaMachineInspiratoryValveObstruction::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid() && HasSeverity();
}

bool SEAnesthesiaMachineInspiratoryValveObstruction::IsActive() const
{
  if (!SEAnesthesiaMachineAction::IsActive())
    return false;
  return HasSeverity() ? !m_Severity->IsZero() : false;
}
void SEAnesthesiaMachineInspiratoryValveObstruction::Deactivate()
{
  SEAnesthesiaMachineAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEAnesthesiaMachineInspiratoryValveObstruction::GetScalar(const std::string& name)
{
  if (name.compare("Severity") == 0)
    return &GetSeverity();
  return nullptr;
}

bool SEAnesthesiaMachineInspiratoryValveObstruction::HasSeverity() const
{
  return m_Severity != nullptr;
}
SEScalar0To1& SEAnesthesiaMachineInspiratoryValveObstruction::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
double SEAnesthesiaMachineInspiratoryValveObstruction::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SEAnesthesiaMachineInspiratoryValveObstruction::ToString(std::ostream &str) const
{
  str << "Anesthesia Machine Action : Inspiratory Valve Obstruction";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tSeverity :"; HasSeverity() ? str << *m_Severity : str << "NaN";
  str << std::flush;
}