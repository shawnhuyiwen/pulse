/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/anesthesiamachine/actions/SEVaporizerFailure.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBAnesthesiaMachineActions.h"

SEVaporizerFailure::SEVaporizerFailure() : SEAnesthesiaMachineAction()
{
  m_Severity = nullptr;
}

SEVaporizerFailure::~SEVaporizerFailure()
{
  Clear();
}

void SEVaporizerFailure::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  SAFE_DELETE(m_Severity);
}

void SEVaporizerFailure::Copy(const SEVaporizerFailure& src)
{// Using Bindings to make a copy
  PBAnesthesiaMachineAction::Copy(src, *this);
}

bool SEVaporizerFailure::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid() && HasSeverity();
}

bool SEVaporizerFailure::IsActive() const
{
  return HasSeverity() ? !m_Severity->IsZero() : false;
}

bool SEVaporizerFailure::HasSeverity() const
{
  return m_Severity != nullptr;
}
SEScalar0To1& SEVaporizerFailure::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
double SEVaporizerFailure::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SEVaporizerFailure::ToString(std::ostream &str) const
{
  str << "Anesthesia Machine Action : Vaporizer Failure";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tSeverity :"; HasSeverity() ? str << *m_Severity : str << "NaN";
  str << std::flush;
}