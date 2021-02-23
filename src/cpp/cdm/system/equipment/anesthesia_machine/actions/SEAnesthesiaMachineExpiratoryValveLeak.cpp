/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineExpiratoryValveLeak.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBEquipmentActions.h"

SEAnesthesiaMachineExpiratoryValveLeak::SEAnesthesiaMachineExpiratoryValveLeak(Logger* logger) : SEAnesthesiaMachineAction(logger)
{
  m_Severity = nullptr;
}

SEAnesthesiaMachineExpiratoryValveLeak::~SEAnesthesiaMachineExpiratoryValveLeak()
{
  SAFE_DELETE(m_Severity);
}

void SEAnesthesiaMachineExpiratoryValveLeak::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  INVALIDATE_PROPERTY(m_Severity);
}

void SEAnesthesiaMachineExpiratoryValveLeak::Copy(const SEAnesthesiaMachineExpiratoryValveLeak& src, bool preserveState)
{// Using Bindings to make a copy
  PBEquipmentAction::Copy(src, *this);
}

bool SEAnesthesiaMachineExpiratoryValveLeak::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid() && HasSeverity();
}

bool SEAnesthesiaMachineExpiratoryValveLeak::IsActive() const
{
  if (!SEAnesthesiaMachineAction::IsActive())
    return false;
  return HasSeverity() ? !m_Severity->IsZero() : false;
}

void SEAnesthesiaMachineExpiratoryValveLeak::Deactivate()
{
  SEAnesthesiaMachineAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEAnesthesiaMachineExpiratoryValveLeak::GetScalar(const std::string& name)
{
  if (name.compare("Severity") == 0)
    return &GetSeverity();
  return nullptr;
}

bool SEAnesthesiaMachineExpiratoryValveLeak::HasSeverity() const
{
  return m_Severity != nullptr;
}
SEScalar0To1& SEAnesthesiaMachineExpiratoryValveLeak::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
double SEAnesthesiaMachineExpiratoryValveLeak::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SEAnesthesiaMachineExpiratoryValveLeak::ToString(std::ostream &str) const
{
  str << "Anesthesia Machine Action : Expiratory Valve Leak";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tSeverity :"; HasSeverity() ? str << *m_Severity : str << "NaN";
  str << std::flush;
}