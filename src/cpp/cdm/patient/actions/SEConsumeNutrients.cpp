/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/actions/SEConsumeNutrients.h"
#include "cdm/properties/SEScalarMass.h"
#include "cdm/properties/SEScalarMassPerTime.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/io/protobuf/PBPatientActions.h"

SEConsumeNutrients::SEConsumeNutrients(Logger* logger) : SEPatientAction(logger)
{
  m_Nutrition = nullptr;
  m_NutritionFile = "";
}

SEConsumeNutrients::~SEConsumeNutrients()
{
  SAFE_DELETE(m_Nutrition);
  m_NutritionFile = "";
}

void SEConsumeNutrients::Clear()
{
  SEPatientAction::Clear();
  if (m_Nutrition)
    m_Nutrition->Clear();
  m_NutritionFile = "";
}

void SEConsumeNutrients::Copy(const SEConsumeNutrients& src, bool /*preserveState*/)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}

bool SEConsumeNutrients::IsValid() const
{
  return SEPatientAction::IsValid() && (HasNutrition() || HasNutritionFile());
}

bool SEConsumeNutrients::IsActive() const
{
  return SEPatientAction::IsActive();
}
void SEConsumeNutrients::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEConsumeNutrients::GetScalar(const std::string& name)
{
  return GetNutrition().GetScalar(name);
}

bool SEConsumeNutrients::HasNutrition() const
{
  return m_Nutrition != nullptr;
}
SENutrition& SEConsumeNutrients::GetNutrition()
{
  if (m_Nutrition == nullptr)
    m_Nutrition = new SENutrition(nullptr);
  return *m_Nutrition;
}
const SENutrition* SEConsumeNutrients::GetNutrition() const
{
  return m_Nutrition;
}

std::string SEConsumeNutrients::GetNutritionFile() const
{
  return m_NutritionFile;
}
void SEConsumeNutrients::SetNutritionFile(const std::string& fileName)
{
  m_NutritionFile = fileName;
}
bool SEConsumeNutrients::HasNutritionFile() const
{
  return !m_NutritionFile.empty();
}
