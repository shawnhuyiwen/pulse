/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEAcuteRespiratoryDistressSyndromeExacerbation.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBPatientActions.h"

SEAcuteRespiratoryDistressSyndromeExacerbation::SEAcuteRespiratoryDistressSyndromeExacerbation() : SEPatientAction()
{
  m_Severity=nullptr;
}

SEAcuteRespiratoryDistressSyndromeExacerbation::~SEAcuteRespiratoryDistressSyndromeExacerbation()
{
  Clear();
}

void SEAcuteRespiratoryDistressSyndromeExacerbation::Clear()
{
  SEPatientAction::Clear();
  SAFE_DELETE(m_Severity);
}

void SEAcuteRespiratoryDistressSyndromeExacerbation::Copy(const SEAcuteRespiratoryDistressSyndromeExacerbation& src)
{
  PBPatientAction::Copy(src, *this);
}

bool SEAcuteRespiratoryDistressSyndromeExacerbation::IsValid() const
{
  return HasSeverity();
}
bool SEAcuteRespiratoryDistressSyndromeExacerbation::IsActive() const
{
  if (!IsValid())
    return false;
  return GetSeverity() > 0;
}

bool SEAcuteRespiratoryDistressSyndromeExacerbation::HasSeverity() const
{
  return m_Severity==nullptr?false:m_Severity->IsValid();
}

SEScalar0To1& SEAcuteRespiratoryDistressSyndromeExacerbation::GetSeverity()
{
  if(m_Severity==nullptr)
    m_Severity=new SEScalar0To1();
  return *m_Severity;
}
double SEAcuteRespiratoryDistressSyndromeExacerbation::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SEAcuteRespiratoryDistressSyndromeExacerbation::ToString(std::ostream &str) const
{
  str << "Patient Action : ARDS Exacerbation"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str << "\n\tSeverity: "; HasSeverity() ? str << m_Severity : str << "NaN";
  str << std::flush;
}