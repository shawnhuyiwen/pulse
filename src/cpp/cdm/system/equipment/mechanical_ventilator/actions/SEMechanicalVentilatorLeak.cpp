/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "cdm/CommonDefs.h"
#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorLeak.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/io/protobuf/PBEquipmentActions.h"

SEMechanicalVentilatorLeak::SEMechanicalVentilatorLeak(Logger* logger) : SEMechanicalVentilatorAction(logger)
{
  m_Severity = nullptr;
}

SEMechanicalVentilatorLeak::~SEMechanicalVentilatorLeak()
{
  SAFE_DELETE(m_Severity);
}

void SEMechanicalVentilatorLeak::Clear()
{
  SEMechanicalVentilatorAction::Clear();
  INVALIDATE_PROPERTY(m_Severity);
}

void SEMechanicalVentilatorLeak::Copy(const SEMechanicalVentilatorLeak& src, bool /*preserveState*/)
{// Using Bindings to make a copy
  PBEquipmentAction::Copy(src, *this);
}

bool SEMechanicalVentilatorLeak::IsValid() const
{
  return SEMechanicalVentilatorAction::IsValid() && HasSeverity();
}

bool SEMechanicalVentilatorLeak::IsActive() const
{
  if (!SEMechanicalVentilatorAction::IsActive())
    return false;
  return HasSeverity() ? !m_Severity->IsZero() : false;
}
void SEMechanicalVentilatorLeak::Deactivate()
{
  SEMechanicalVentilatorAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEMechanicalVentilatorLeak::GetScalar(const std::string& name)
{
  if (name.compare("Severity") == 0)
    return &GetSeverity();
  return nullptr;
}

bool SEMechanicalVentilatorLeak::HasSeverity() const
{
  return m_Severity != nullptr;
}
SEScalar0To1& SEMechanicalVentilatorLeak::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
double SEMechanicalVentilatorLeak::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SEMechanicalVentilatorLeak::ToString(std::ostream& str) const
{
  str << "Mechanical Ventilator Action : Leak";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tSeverity :"; HasSeverity() ? str << *m_Severity : str << "NaN";
  str << std::flush;
}