/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEExercise.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBPatientActions.h"

SEExercise::SEExercise() : SEPatientAction()
{
  m_Intensity = nullptr;
}

SEExercise::~SEExercise()
{
  Clear();
}

void SEExercise::Clear()
{
  SEPatientAction::Clear();
  SAFE_DELETE(m_Intensity);
}

void SEExercise::Copy(const SEExercise& src)
{
  PBPatientAction::Copy(src, *this);
}

bool SEExercise::IsValid() const
{
  return SEPatientAction::IsValid() && HasIntensity();
}

bool SEExercise::IsActive() const
{
  if (HasIntensity())
    return m_Intensity->IsPositive();
  return false;  
}

bool SEExercise::HasIntensity() const
{
  return m_Intensity == nullptr ? false : m_Intensity->IsValid();
}
SEScalar0To1& SEExercise::GetIntensity()
{
  if (m_Intensity == nullptr)
    m_Intensity = new SEScalar0To1();
  return *m_Intensity;
}
double SEExercise::GetIntensity() const
{
  if (m_Intensity == nullptr)
    return SEScalar::dNaN();
  return m_Intensity->GetValue();
}

void SEExercise::ToString(std::ostream &str) const
{
  str << "Patient Action : Exercise";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tIntensity: "; HasIntensity() ? str << *m_Intensity : str << "NaN";
  str << std::flush;
}
