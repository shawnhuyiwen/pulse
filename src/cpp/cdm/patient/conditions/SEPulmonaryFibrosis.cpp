/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/conditions/SEPulmonaryFibrosis.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/io/protobuf/PBPatientConditions.h"

SEPulmonaryFibrosis::SEPulmonaryFibrosis(Logger* logger) : SEPatientCondition(logger)
{
  m_Severity = nullptr;
}

SEPulmonaryFibrosis::~SEPulmonaryFibrosis()
{
  SAFE_DELETE(m_Severity);
}

void SEPulmonaryFibrosis::Clear()
{
  SEPatientCondition::Clear();
  INVALIDATE_PROPERTY(m_Severity);
}

void SEPulmonaryFibrosis::Copy(const SEPulmonaryFibrosis& src)
{
  PBPatientCondition::Copy(src, *this);
}

bool SEPulmonaryFibrosis::IsValid() const
{
  return HasSeverity();
}

bool SEPulmonaryFibrosis::IsActive() const
{
  if (!IsValid())
    return false;
  return GetSeverity()>0;
}

bool SEPulmonaryFibrosis::HasSeverity() const
{
  return m_Severity == nullptr ? false : m_Severity->IsValid();
}
SEScalar0To1& SEPulmonaryFibrosis::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
double SEPulmonaryFibrosis::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SEPulmonaryFibrosis::ToString(std::ostream &str) const
{
  SEPatientCondition::ToString(str);
  str << "\n\tSeverity: "; HasSeverity() ? str << *m_Severity : str << "NaN";
  str << std::flush;
}
