/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEConsumeNutrients.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarMassPerTime.h"
#include "properties/SEScalarVolume.h"
#include "io/protobuf/PBPatientActions.h"

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

void SEConsumeNutrients::Copy(const SEConsumeNutrients& src, bool preserveState)
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

void SEConsumeNutrients::ToString(std::ostream &str) const
{
  str << "Patient Action : Consume Nutrients"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  if (HasNutritionFile())
  {
    str << "\n\tNutrition File: "; str << m_NutritionFile;
  }
  else if (HasNutrition())
  {
    str << "\n\tCharbohydrates: "; m_Nutrition->HasCarbohydrate() ? str << m_Nutrition->GetCarbohydrate() : str << "None";
    str << "\n\tCharbohydrates Digestion Rate: "; m_Nutrition->HasCarbohydrateDigestionRate() ? str << m_Nutrition->GetCarbohydrateDigestionRate() : str << "None";
    str << "\n\tFat: "; m_Nutrition->HasFat() ? str << m_Nutrition->GetFat() : str << "None";
    str << "\n\tFat Digestion Rate: "; m_Nutrition->HasFatDigestionRate() ? str << m_Nutrition->GetFatDigestionRate() : str << "None";
    str << "\n\tProtein: "; m_Nutrition->HasProtein() ? str << m_Nutrition->GetProtein() : str << "None";
    str << "\n\tProtein Digestion Rate: "; m_Nutrition->HasProteinDigestionRate() ? str << m_Nutrition->GetProteinDigestionRate() : str << "None";
    str << "\n\tCalcium: "; m_Nutrition->HasCalcium() ? str << m_Nutrition->GetCalcium() : str << "None";
    str << "\n\tSodium: "; m_Nutrition->HasSodium() ? str << m_Nutrition->GetSodium() : str << "None";
    str << "\n\tWater: "; m_Nutrition->HasWater() ? str << m_Nutrition->GetWater() : str << "None";
  }
  str << std::flush;
}
