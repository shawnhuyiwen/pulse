/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEAcuteStress.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBPatientActions.h"

SEAcuteStress::SEAcuteStress(Logger* logger) : SEPatientAction(logger)
{
  m_Severity=nullptr;
}

SEAcuteStress::~SEAcuteStress()
{
  SAFE_DELETE(m_Severity);
}

void SEAcuteStress::Clear()
{
  SEPatientAction::Clear();
  INVALIDATE_PROPERTY(m_Severity);
}

void SEAcuteStress::Copy(const SEAcuteStress& src, bool preserveState)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}

bool SEAcuteStress::IsValid() const
{
  return SEPatientAction::IsValid() && HasSeverity();
}

bool SEAcuteStress::IsActive() const
{
  if (!SEPatientAction::IsActive())
    return false;
  return !m_Severity->IsZero();
}
void SEAcuteStress::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEAcuteStress::GetScalar(const std::string& name)
{
  if (name.compare("Severity") == 0)
    return &GetSeverity();
  return nullptr;
}

bool SEAcuteStress::HasSeverity() const
{
  return m_Severity==nullptr?false:m_Severity->IsValid();
}
SEScalar0To1& SEAcuteStress::GetSeverity()
{
  if(m_Severity==nullptr)
    m_Severity=new SEScalar0To1();
  return *m_Severity;
}
double SEAcuteStress::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SEAcuteStress::ToString(std::ostream &str) const
{
  str << "Patient Action : Acute Stress"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str << "\n\tSeverity: "; HasSeverity() ? str << *m_Severity : str << "Not Set";
  str << std::flush;
}