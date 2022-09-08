/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/conditions/SEConsumeMeal.h"
#include "cdm/properties/SEScalarMass.h"
#include "cdm/properties/SEScalarMassPerTime.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/io/protobuf/PBPatientConditions.h"

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
