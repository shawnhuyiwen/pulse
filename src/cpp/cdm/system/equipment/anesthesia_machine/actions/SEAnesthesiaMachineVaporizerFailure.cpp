/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineVaporizerFailure.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBEquipmentActions.h"

SEAnesthesiaMachineVaporizerFailure::SEAnesthesiaMachineVaporizerFailure(Logger* logger) : SEAnesthesiaMachineAction(logger)
{
  m_Severity = nullptr;
}

SEAnesthesiaMachineVaporizerFailure::~SEAnesthesiaMachineVaporizerFailure()
{
  SAFE_DELETE(m_Severity);
}

void SEAnesthesiaMachineVaporizerFailure::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  INVALIDATE_PROPERTY(m_Severity);
}

void SEAnesthesiaMachineVaporizerFailure::Copy(const SEAnesthesiaMachineVaporizerFailure& src, bool preserveState)
{// Using Bindings to make a copy
  PBEquipmentAction::Copy(src, *this);
}

bool SEAnesthesiaMachineVaporizerFailure::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid() && HasSeverity();
}

bool SEAnesthesiaMachineVaporizerFailure::IsActive() const
{
  if (!SEAnesthesiaMachineAction::IsActive())
    return false;
  return HasSeverity() ? !m_Severity->IsZero() : false;
}
void SEAnesthesiaMachineVaporizerFailure::Deactivate()
{
  SEAnesthesiaMachineAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEAnesthesiaMachineVaporizerFailure::GetScalar(const std::string& name)
{
  if (name.compare("Severity") == 0)
    return &GetSeverity();
  return nullptr;
}

bool SEAnesthesiaMachineVaporizerFailure::HasSeverity() const
{
  return m_Severity != nullptr;
}
SEScalar0To1& SEAnesthesiaMachineVaporizerFailure::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
double SEAnesthesiaMachineVaporizerFailure::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SEAnesthesiaMachineVaporizerFailure::ToString(std::ostream &str) const
{
  str << "Anesthesia Machine Action : Vaporizer Failure";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tSeverity :"; HasSeverity() ? str << *m_Severity : str << "NaN";
  str << std::flush;
}