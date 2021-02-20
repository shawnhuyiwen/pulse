/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SELobarPneumoniaExacerbation.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBPatientActions.h"

SELobarPneumoniaExacerbation::SELobarPneumoniaExacerbation(Logger* logger) : SEPatientAction(logger)
{
  m_Severity = nullptr;
  m_LeftLungAffected = nullptr;
  m_RightLungAffected = nullptr;
}

SELobarPneumoniaExacerbation::~SELobarPneumoniaExacerbation()
{
  SAFE_DELETE(m_Severity);
  SAFE_DELETE(m_LeftLungAffected);
  SAFE_DELETE(m_RightLungAffected);
}

void SELobarPneumoniaExacerbation::Clear()
{
  SEPatientAction::Clear();
  INVALIDATE_PROPERTY(m_Severity);
  INVALIDATE_PROPERTY(m_LeftLungAffected);
  INVALIDATE_PROPERTY(m_RightLungAffected);
}

void SELobarPneumoniaExacerbation::Copy(const SELobarPneumoniaExacerbation& src, bool preserveState)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}

bool SELobarPneumoniaExacerbation::IsValid() const
{
  return HasSeverity() && HasLeftLungAffected() && HasRightLungAffected();
}

bool SELobarPneumoniaExacerbation::IsActive() const
{
  if (!SEPatientAction::IsActive())
    return false;
  if (GetSeverity() <= 0)
    return false;
  if (GetLeftLungAffected() <= 0 && GetRightLungAffected() <= 0)
    return false;
  return true;
}
void SELobarPneumoniaExacerbation::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SELobarPneumoniaExacerbation::GetScalar(const std::string& name)
{
  if (name.compare("Severity") == 0)
    return &GetSeverity();
  if (name.compare("LeftLungAffected") == 0)
    return &GetLeftLungAffected();
  if (name.compare("RightLungAffected") == 0)
    return &GetRightLungAffected();
  return nullptr;
}

bool SELobarPneumoniaExacerbation::HasSeverity() const
{
  return m_Severity==nullptr?false:m_Severity->IsValid();
}
SEScalar0To1& SELobarPneumoniaExacerbation::GetSeverity()
{
  if(m_Severity==nullptr)
    m_Severity=new SEScalar0To1();
  return *m_Severity;
}
double SELobarPneumoniaExacerbation::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

bool SELobarPneumoniaExacerbation::HasLeftLungAffected() const
{
  return m_LeftLungAffected == nullptr ? false : m_LeftLungAffected->IsValid();
}
SEScalar0To1& SELobarPneumoniaExacerbation::GetLeftLungAffected()
{
  if (m_LeftLungAffected == nullptr)
    m_LeftLungAffected = new SEScalar0To1();
  return *m_LeftLungAffected;
}
double SELobarPneumoniaExacerbation::GetLeftLungAffected() const
{
  if (m_LeftLungAffected == nullptr)
    return SEScalar::dNaN();
  return m_LeftLungAffected->GetValue();
}

bool SELobarPneumoniaExacerbation::HasRightLungAffected() const
{
  return m_RightLungAffected == nullptr ? false : m_RightLungAffected->IsValid();
}
SEScalar0To1& SELobarPneumoniaExacerbation::GetRightLungAffected()
{
  if (m_RightLungAffected == nullptr)
    m_RightLungAffected = new SEScalar0To1();
  return *m_RightLungAffected;
}
double SELobarPneumoniaExacerbation::GetRightLungAffected() const
{
  if (m_RightLungAffected == nullptr)
    return SEScalar::dNaN();
  return m_RightLungAffected->GetValue();
}

void SELobarPneumoniaExacerbation::ToString(std::ostream &str) const
{
  str << "Patient Action : Lobar Pneumonia Exacerbation"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str << "\n\tSeverity: "; HasSeverity() ? str << *m_Severity : str << "NaN";
  str << "\n\tLeftLungAffected: "; HasLeftLungAffected() ? str << *m_LeftLungAffected : str << "NaN";
  str << "\n\tRightLungAffected: "; HasRightLungAffected() ? str << *m_RightLungAffected : str << "NaN";
  str << std::flush;
}
