/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEBronchoconstriction.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBPatientActions.h"

SEBronchoconstriction::SEBronchoconstriction() : SEPatientAction()
{
  m_Severity=nullptr;
}

SEBronchoconstriction::~SEBronchoconstriction()
{
  Clear();
}

void SEBronchoconstriction::Clear()
{
  SEPatientAction::Clear();
  SAFE_DELETE(m_Severity);
}

void SEBronchoconstriction::Copy(const SEBronchoconstriction& src)
{
  PBPatientAction::Copy(src, *this);
}

bool SEBronchoconstriction::IsValid() const
{
  return SEPatientAction::IsValid() && HasSeverity();
}

bool SEBronchoconstriction::IsActive() const
{
  return IsValid() ? !m_Severity->IsZero() : false;
}

bool SEBronchoconstriction::HasSeverity() const
{
  return m_Severity==nullptr?false:m_Severity->IsValid();
}
SEScalar0To1& SEBronchoconstriction::GetSeverity()
{
  if(m_Severity==nullptr)
    m_Severity=new SEScalar0To1();
  return *m_Severity;
}
double SEBronchoconstriction::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SEBronchoconstriction::ToString(std::ostream &str) const
{
  str << "Patient Action : Bronchoconstriction"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str << "\n\tSeverity: "; HasSeverity() ? str << *m_Severity : str << "NaN";
  str <<std::flush;
}