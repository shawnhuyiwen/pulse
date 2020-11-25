/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/SEMeal.h"
#include "properties/SEScalarTime.h"
#include "io/protobuf/PBPatientNutrition.h"

SEMeal::SEMeal(Logger* logger) : SENutrition(logger)
{
  m_ElapsedTime = nullptr;
}

SEMeal::~SEMeal()
{
  SAFE_DELETE(m_ElapsedTime);
}

void SEMeal::Clear()
{
  SENutrition::Clear();
  INVALIDATE_PROPERTY(m_ElapsedTime);
}

bool SEMeal::SerializeToString(std::string& output, SerializationFormat m) const
{
  return PBPatientNutrition::SerializeToString(*this, output, m);
}
bool SEMeal::SerializeToFile(const std::string& filename) const
{
  return PBPatientNutrition::SerializeToFile(*this, filename);
}
bool SEMeal::SerializeFromString(const std::string& src, SerializationFormat m)
{
  return PBPatientNutrition::SerializeFromString(src, *this, m);
}
bool SEMeal::SerializeFromFile(const std::string& filename)
{
  return PBPatientNutrition::SerializeFromFile(filename, *this);
}

bool SEMeal::HasElapsedTime() const
{
  return m_ElapsedTime == nullptr ? false : m_ElapsedTime->IsValid();
}
SEScalarTime& SEMeal::GetElapsedTime()
{
  if (m_ElapsedTime == nullptr)
    m_ElapsedTime = new SEScalarTime();
  return *m_ElapsedTime;
}
double SEMeal::GetElapsedTime(const TimeUnit& unit) const
{
  if (m_ElapsedTime == nullptr)
    return SEScalar::dNaN();
  return m_ElapsedTime->GetValue(unit);
}