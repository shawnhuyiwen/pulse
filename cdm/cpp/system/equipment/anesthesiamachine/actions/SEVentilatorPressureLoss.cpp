/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/anesthesiamachine/actions/SEVentilatorPressureLoss.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBAnesthesiaMachineActions.h"

SEVentilatorPressureLoss::SEVentilatorPressureLoss() : SEAnesthesiaMachineAction()
{
  m_Severity = nullptr;
}

SEVentilatorPressureLoss::~SEVentilatorPressureLoss()
{
  Clear();
}

void SEVentilatorPressureLoss::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  SAFE_DELETE(m_Severity);
}

void SEVentilatorPressureLoss::Copy(const SEVentilatorPressureLoss& src)
{// Using Bindings to make a copy
  PBAnesthesiaMachineAction::Copy(src, *this);
}

bool SEVentilatorPressureLoss::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid() && HasSeverity();
}

bool SEVentilatorPressureLoss::IsActive() const
{
  return HasSeverity() ? !m_Severity->IsZero() : false;
}

bool SEVentilatorPressureLoss::HasSeverity() const
{
  return m_Severity != nullptr;
}
SEScalar0To1& SEVentilatorPressureLoss::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
double SEVentilatorPressureLoss::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SEVentilatorPressureLoss::ToString(std::ostream &str) const
{
  str << "Anesthesia Machine Action : Ventilator Pressure Loss";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tSeverity :"; HasSeverity() ? str << *m_Severity : str << "NaN";
  str << std::flush;
}