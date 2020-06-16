/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEChronicObstructivePulmonaryDiseaseExacerbation.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBPatientActions.h"

SEChronicObstructivePulmonaryDiseaseExacerbation::SEChronicObstructivePulmonaryDiseaseExacerbation(Logger* logger) : SEPatientAction(logger)
{
  m_BronchitisSeverity = nullptr;
  m_EmphysemaSeverity = nullptr;
}

SEChronicObstructivePulmonaryDiseaseExacerbation::~SEChronicObstructivePulmonaryDiseaseExacerbation()
{
  Clear();
}

void SEChronicObstructivePulmonaryDiseaseExacerbation::Clear()
{
  SEPatientAction::Clear();
  SAFE_DELETE(m_BronchitisSeverity);
  SAFE_DELETE(m_EmphysemaSeverity);
}

void SEChronicObstructivePulmonaryDiseaseExacerbation::Copy(const SEChronicObstructivePulmonaryDiseaseExacerbation& src)
{
  PBPatientAction::Copy(src, *this);
}

bool SEChronicObstructivePulmonaryDiseaseExacerbation::IsValid() const
{
  return HasBronchitisSeverity() || HasEmphysemaSeverity();
}

bool SEChronicObstructivePulmonaryDiseaseExacerbation::IsActive() const
{
  if (!IsValid())
    return false;
  return GetBronchitisSeverity() > 0 || GetEmphysemaSeverity() > 0;
}

bool SEChronicObstructivePulmonaryDiseaseExacerbation::HasBronchitisSeverity() const
{
  return m_BronchitisSeverity == nullptr ? false : m_BronchitisSeverity->IsValid();
}
SEScalar0To1& SEChronicObstructivePulmonaryDiseaseExacerbation::GetBronchitisSeverity()
{
  if (m_BronchitisSeverity == nullptr)
    m_BronchitisSeverity = new SEScalar0To1();
  return *m_BronchitisSeverity;
}
double SEChronicObstructivePulmonaryDiseaseExacerbation::GetBronchitisSeverity() const
{
  if (m_BronchitisSeverity == nullptr)
    return SEScalar::dNaN();
  return m_BronchitisSeverity->GetValue();
}

bool SEChronicObstructivePulmonaryDiseaseExacerbation::HasEmphysemaSeverity() const
{
  return m_EmphysemaSeverity == nullptr ? false : m_EmphysemaSeverity->IsValid();
}
SEScalar0To1& SEChronicObstructivePulmonaryDiseaseExacerbation::GetEmphysemaSeverity()
{
  if (m_EmphysemaSeverity == nullptr)
    m_EmphysemaSeverity = new SEScalar0To1();
  return *m_EmphysemaSeverity;
}
double SEChronicObstructivePulmonaryDiseaseExacerbation::GetEmphysemaSeverity() const
{
  if (m_EmphysemaSeverity == nullptr)
    return SEScalar::dNaN();
  return m_EmphysemaSeverity->GetValue();
}

void SEChronicObstructivePulmonaryDiseaseExacerbation::ToString(std::ostream &str) const
{
  str << "Patient Action : COPD Exacerbation";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tBronchitisSeverity: "; HasBronchitisSeverity() ? str << *m_BronchitisSeverity : str << "NaN";
  str << "\n\tEmphysemaSeverity: "; HasEmphysemaSeverity() ? str << *m_EmphysemaSeverity : str << "NaN";
  str << std::flush;
}


