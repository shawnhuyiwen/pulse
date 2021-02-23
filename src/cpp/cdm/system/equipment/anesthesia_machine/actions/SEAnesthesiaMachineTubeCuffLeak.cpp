/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineTubeCuffLeak.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBEquipmentActions.h"

SEAnesthesiaMachineTubeCuffLeak::SEAnesthesiaMachineTubeCuffLeak(Logger* logger) : SEAnesthesiaMachineAction(logger)
{
  m_Severity = nullptr;
}

SEAnesthesiaMachineTubeCuffLeak::~SEAnesthesiaMachineTubeCuffLeak()
{
  SAFE_DELETE(m_Severity);
}

void SEAnesthesiaMachineTubeCuffLeak::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  INVALIDATE_PROPERTY(m_Severity);
}

void SEAnesthesiaMachineTubeCuffLeak::Copy(const SEAnesthesiaMachineTubeCuffLeak& src, bool preserveState)
{// Using Bindings to make a copy
  PBEquipmentAction::Copy(src, *this);
}

bool SEAnesthesiaMachineTubeCuffLeak::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid() && HasSeverity();
}

bool SEAnesthesiaMachineTubeCuffLeak::IsActive() const
{
  if (!SEAnesthesiaMachineAction::IsActive())
    return false;
  return HasSeverity() ? !m_Severity->IsZero() : false;
}
void SEAnesthesiaMachineTubeCuffLeak::Deactivate()
{
  SEAnesthesiaMachineAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEAnesthesiaMachineTubeCuffLeak::GetScalar(const std::string& name)
{
  if (name.compare("Severity") == 0)
    return &GetSeverity();
  return nullptr;
}

bool SEAnesthesiaMachineTubeCuffLeak::HasSeverity() const
{
  return m_Severity != nullptr;
}
SEScalar0To1& SEAnesthesiaMachineTubeCuffLeak::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
double SEAnesthesiaMachineTubeCuffLeak::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SEAnesthesiaMachineTubeCuffLeak::ToString(std::ostream &str) const
{
  str << "Anesthesia Machine Action : Tube Cuff Leak";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tSeverity :"; HasSeverity() ? str << *m_Severity : str << "NaN";
  str << std::flush;
}