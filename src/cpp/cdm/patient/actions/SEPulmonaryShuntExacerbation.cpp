/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEPulmonaryShuntExacerbation.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBPatientActions.h"

SEPulmonaryShuntExacerbation::SEPulmonaryShuntExacerbation(Logger* logger) : SEPatientAction(logger)
{
  m_Severity=nullptr;
}

SEPulmonaryShuntExacerbation::~SEPulmonaryShuntExacerbation()
{
  Clear();
}

void SEPulmonaryShuntExacerbation::Clear()
{
  SEPatientAction::Clear();
  SAFE_DELETE(m_Severity);
}

void SEPulmonaryShuntExacerbation::Copy(const SEPulmonaryShuntExacerbation& src)
{
  PBPatientAction::Copy(src, *this);
}

bool SEPulmonaryShuntExacerbation::IsValid() const
{
  return SEPatientAction::IsValid() && HasSeverity();
}

bool SEPulmonaryShuntExacerbation::IsActive() const
{
  return IsValid() ? !m_Severity->IsZero() : false;
}

bool SEPulmonaryShuntExacerbation::HasSeverity() const
{
  return m_Severity==nullptr?false:m_Severity->IsValid();
}
SEScalar0To1& SEPulmonaryShuntExacerbation::GetSeverity()
{
  if(m_Severity==nullptr)
    m_Severity=new SEScalar0To1();
  return *m_Severity;
}
double SEPulmonaryShuntExacerbation::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SEPulmonaryShuntExacerbation::ToString(std::ostream &str) const
{
  str << "Patient Action : Pulmonary Shunt Exacerbation"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str << "\n\tSeverity: "; HasSeverity() ? str << *m_Severity : str << "Not Set";
  str << std::flush;
}