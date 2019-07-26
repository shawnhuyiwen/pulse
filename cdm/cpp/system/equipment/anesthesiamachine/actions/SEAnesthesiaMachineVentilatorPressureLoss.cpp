/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineVentilatorPressureLoss.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBAnesthesiaMachineActions.h"

SEAnesthesiaMachineVentilatorPressureLoss::SEAnesthesiaMachineVentilatorPressureLoss() : SEAnesthesiaMachineAction()
{
  m_Severity = nullptr;
}

SEAnesthesiaMachineVentilatorPressureLoss::~SEAnesthesiaMachineVentilatorPressureLoss()
{
  Clear();
}

void SEAnesthesiaMachineVentilatorPressureLoss::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  SAFE_DELETE(m_Severity);
}

void SEAnesthesiaMachineVentilatorPressureLoss::Copy(const SEAnesthesiaMachineVentilatorPressureLoss& src)
{// Using Bindings to make a copy
  PBAnesthesiaMachineAction::Copy(src, *this);
}

bool SEAnesthesiaMachineVentilatorPressureLoss::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid() && HasSeverity();
}

bool SEAnesthesiaMachineVentilatorPressureLoss::IsActive() const
{
  return HasSeverity() ? !m_Severity->IsZero() : false;
}

bool SEAnesthesiaMachineVentilatorPressureLoss::HasSeverity() const
{
  return m_Severity != nullptr;
}
SEScalar0To1& SEAnesthesiaMachineVentilatorPressureLoss::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
double SEAnesthesiaMachineVentilatorPressureLoss::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SEAnesthesiaMachineVentilatorPressureLoss::ToString(std::ostream &str) const
{
  str << "Anesthesia Machine Action : Ventilator Pressure Loss";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tSeverity :"; HasSeverity() ? str << *m_Severity : str << "NaN";
  str << std::flush;
}