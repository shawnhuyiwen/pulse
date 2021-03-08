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
  SAFE_DELETE(m_Severity);
}

void SEPulmonaryShuntExacerbation::Clear()
{
  SEPatientAction::Clear();
  INVALIDATE_PROPERTY(m_Severity);
}

void SEPulmonaryShuntExacerbation::Copy(const SEPulmonaryShuntExacerbation& src, bool preserveState)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}

bool SEPulmonaryShuntExacerbation::IsValid() const
{
  return SEPatientAction::IsValid() && HasSeverity();
}

bool SEPulmonaryShuntExacerbation::IsActive() const
{
  if (!SEPatientAction::IsActive())
    return false;
  return !m_Severity->IsZero();
}
void SEPulmonaryShuntExacerbation::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEPulmonaryShuntExacerbation::GetScalar(const std::string& name)
{
  if (name.compare("Severity") == 0)
    return &GetSeverity();
  return nullptr;
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