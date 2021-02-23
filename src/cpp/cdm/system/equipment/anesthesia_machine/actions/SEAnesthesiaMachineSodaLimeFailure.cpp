/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineSodaLimeFailure.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBEquipmentActions.h"

SEAnesthesiaMachineSodaLimeFailure::SEAnesthesiaMachineSodaLimeFailure(Logger* logger) : SEAnesthesiaMachineAction(logger)
{
  m_Severity = nullptr;
}

SEAnesthesiaMachineSodaLimeFailure::~SEAnesthesiaMachineSodaLimeFailure()
{
  SAFE_DELETE(m_Severity);
}

void SEAnesthesiaMachineSodaLimeFailure::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  INVALIDATE_PROPERTY(m_Severity);
}

void SEAnesthesiaMachineSodaLimeFailure::Copy(const SEAnesthesiaMachineSodaLimeFailure& src, bool preserveState)
{// Using Bindings to make a copy
  PBEquipmentAction::Copy(src, *this);
}

bool SEAnesthesiaMachineSodaLimeFailure::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid() && HasSeverity();
}

bool SEAnesthesiaMachineSodaLimeFailure::IsActive() const
{
  if (!SEAnesthesiaMachineAction::IsActive())
    return false;
  return HasSeverity() ? !m_Severity->IsZero() : false;
}
void SEAnesthesiaMachineSodaLimeFailure::Deactivate()
{
  SEAnesthesiaMachineAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEAnesthesiaMachineSodaLimeFailure::GetScalar(const std::string& name)
{
  if (name.compare("Severity") == 0)
    return &GetSeverity();
  return nullptr;
}

bool SEAnesthesiaMachineSodaLimeFailure::HasSeverity() const
{
  return m_Severity != nullptr;
}
SEScalar0To1& SEAnesthesiaMachineSodaLimeFailure::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
double SEAnesthesiaMachineSodaLimeFailure::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SEAnesthesiaMachineSodaLimeFailure::ToString(std::ostream &str) const
{
  str << "Anesthesia Machine Action : Soda Lime Failure";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tSeverity :"; HasSeverity() ? str << *m_Severity : str << "NaN";
  str << std::flush;
}