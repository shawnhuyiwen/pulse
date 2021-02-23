/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEBronchoconstriction.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBPatientActions.h"

SEBronchoconstriction::SEBronchoconstriction(Logger* logger) : SEPatientAction(logger)
{
  m_Severity=nullptr;
}

SEBronchoconstriction::~SEBronchoconstriction()
{
  SAFE_DELETE(m_Severity);
}

void SEBronchoconstriction::Clear()
{
  SEPatientAction::Clear();
  INVALIDATE_PROPERTY(m_Severity);
}

void SEBronchoconstriction::Copy(const SEBronchoconstriction& src, bool preserveState)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}

bool SEBronchoconstriction::IsValid() const
{
  return SEPatientAction::IsValid() && HasSeverity();
}

bool SEBronchoconstriction::IsActive() const
{
  if (!SEPatientAction::IsActive())
    return false;
  return !m_Severity->IsZero();
}
void SEBronchoconstriction::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEBronchoconstriction::GetScalar(const std::string& name)
{
  if (name.compare("Severity") == 0)
    return &GetSeverity();
  return nullptr;
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