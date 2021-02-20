/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEAsthmaAttack.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBPatientActions.h"

SEAsthmaAttack::SEAsthmaAttack(Logger* logger) : SEPatientAction(logger)
{
  m_Severity=nullptr;
}

SEAsthmaAttack::~SEAsthmaAttack()
{
  SAFE_DELETE(m_Severity);
}

void SEAsthmaAttack::Clear()
{
  SEPatientAction::Clear();
  INVALIDATE_PROPERTY(m_Severity);
}

void SEAsthmaAttack::Copy(const SEAsthmaAttack& src, bool preserveState)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}

bool SEAsthmaAttack::IsValid() const
{
  return SEPatientAction::IsValid() && HasSeverity();
}

bool SEAsthmaAttack::IsActive() const
{
  if (!SEPatientAction::IsActive())
    return false;
  return !m_Severity->IsZero();
}
void SEAsthmaAttack::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEAsthmaAttack::GetScalar(const std::string& name)
{
  if (name.compare("Severity") == 0)
    return &GetSeverity();
  return nullptr;
}

bool SEAsthmaAttack::HasSeverity() const
{
  return m_Severity==nullptr?false:m_Severity->IsValid();
}
SEScalar0To1& SEAsthmaAttack::GetSeverity()
{
  if(m_Severity==nullptr)
    m_Severity=new SEScalar0To1();
  return *m_Severity;
}
double SEAsthmaAttack::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SEAsthmaAttack::ToString(std::ostream &str) const
{
  str << "Patient Action : AsthmaAttack"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str << "\n\tSeverity: "; HasSeverity() ? str << *m_Severity : str << "NaN";
  str <<std::flush;
}
