/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/actions/SEExercise.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/io/protobuf/PBPatientActions.h"

SEExercise::SEExercise(Logger* logger) : SEPatientAction(logger)
{
  m_Intensity = nullptr;
}

SEExercise::~SEExercise()
{
  SAFE_DELETE(m_Intensity);
}

void SEExercise::Clear()
{
  SEPatientAction::Clear();
  INVALIDATE_PROPERTY(m_Intensity);
}

void SEExercise::Copy(const SEExercise& src, bool /*preserveState*/)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}

bool SEExercise::IsValid() const
{
  return SEPatientAction::IsValid() && HasIntensity();
}

bool SEExercise::IsActive() const
{
  if (!SEPatientAction::IsActive())
    return false;
  if (HasIntensity())
    return m_Intensity->IsPositive();
  return false;
}
void SEExercise::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEExercise::GetScalar(const std::string& name)
{
  if (name.compare("Intensity") == 0)
    return &GetIntensity();
  return nullptr;
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
  SEPatientAction::ToString(str);
  str << "\n\tIntensity: "; HasIntensity() ? str << *m_Intensity : str << "NaN";
  str << std::flush;
}
