/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "SEConsumeMeal.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarMassPerTime.h"
#include "properties/SEScalarVolume.h"
#include "io/protobuf/PBPatientConditions.h"

SEConsumeMeal::SEConsumeMeal(Logger* logger) : SEPatientCondition(logger)
{
  m_Meal = nullptr;
  m_MealFile = "";
}

SEConsumeMeal::~SEConsumeMeal()
{
  SAFE_DELETE(m_Meal);
  m_MealFile = "";
}

void SEConsumeMeal::Clear()
{
  SEPatientCondition::Clear();
  if (m_Meal)
    m_Meal->Clear();
  m_MealFile = "";
}

void SEConsumeMeal::Copy(const SEConsumeMeal& src)
{
  PBPatientCondition::Copy(src, *this);
}

bool SEConsumeMeal::IsValid() const
{
  return (HasMeal() || HasMealFile());
}
bool SEConsumeMeal::IsActive() const
{
  return IsValid();
}

bool SEConsumeMeal::HasMeal() const
{
  return m_Meal != nullptr;
}
SEMeal& SEConsumeMeal::GetMeal()
{
  m_MealFile = "";
  if (m_Meal == nullptr)
    m_Meal = new SEMeal(nullptr);
  return *m_Meal;
}
const SEMeal* SEConsumeMeal::GetMeal() const
{
  return m_Meal;
}

std::string SEConsumeMeal::GetMealFile() const
{
  return m_MealFile;
}
void SEConsumeMeal::SetMealFile(const std::string& fileName)
{
  m_MealFile = fileName;
}
bool SEConsumeMeal::HasMealFile() const
{
  return !m_MealFile.empty();
}

void SEConsumeMeal::ToString(std::ostream &str) const
{
  str << "Patient Condition :  Consume Meal";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  if (HasMealFile())
  {
    str << "\n\tMeal File: "; str << m_MealFile;
  }
  else if (HasMeal())
  {
    str << "\n\tCharbohydrates: "; m_Meal->HasCarbohydrate() ? str << m_Meal->GetCarbohydrate() : str << "None";
    str << "\n\tCharbohydrates Digestion Rate: "; m_Meal->HasCarbohydrateDigestionRate() ? str << m_Meal->GetCarbohydrateDigestionRate() : str << "None";
    str << "\n\tFat: "; m_Meal->HasFat() ? str << m_Meal->GetFat() : str << "None";
    str << "\n\tFat Digestion Rate: "; m_Meal->HasFatDigestionRate() ? str << m_Meal->GetFatDigestionRate() : str << "None";
    str << "\n\tProtein: "; m_Meal->HasProtein() ? str << m_Meal->GetProtein() : str << "None";
    str << "\n\tProtein Digestion Rate: "; m_Meal->HasProteinDigestionRate() ? str << m_Meal->GetProteinDigestionRate() : str << "None";
    str << "\n\tCalcium: "; m_Meal->HasCalcium() ? str << m_Meal->GetCalcium() : str << "None";
    str << "\n\tSodium: "; m_Meal->HasSodium() ? str << m_Meal->GetSodium() : str << "None";
    str << "\n\tWater: "; m_Meal->HasWater() ? str << m_Meal->GetWater() : str << "None";
  }
  str << std::flush;
}
