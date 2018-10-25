/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "properties/SEFunctionElectricPotentialVsTime.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarElectricPotential.h"
#include "utils/GeneralMath.h"
#include "utils/Logger.h"


SEFunctionElectricPotentialVsTime::SEFunctionElectricPotentialVsTime() : SEFunction()
{
  
}

SEFunctionElectricPotentialVsTime::~SEFunctionElectricPotentialVsTime()
{
  Clear();
}

void SEFunctionElectricPotentialVsTime::Clear()
{
  SEFunction::Clear();
  m_TimeUnit = nullptr;
  m_ElectricPotentialUnit = nullptr;
}

double SEFunctionElectricPotentialVsTime::GetTimeValue(size_t index, const TimeUnit& unit)
{
  if (m_TimeUnit==nullptr)
    throw CommonDataModelException("No time units have been set");
  if (index >= m_Independent.size())
    throw CommonDataModelException("Independent index out of bounds");
  return Convert(m_Independent[index], *m_TimeUnit, unit);
}
std::vector<double>& SEFunctionElectricPotentialVsTime::GetTime()                                  
{
  return m_Independent;
}
const TimeUnit* SEFunctionElectricPotentialVsTime::GetTimeUnit()
{
  return m_TimeUnit;
}
void SEFunctionElectricPotentialVsTime::SetTimeUnit(const TimeUnit& unit)
{
  m_TimeUnit = &unit;
}

double SEFunctionElectricPotentialVsTime::GetElectricPotentialValue(size_t index, const ElectricPotentialUnit& unit)
{
  if (m_ElectricPotentialUnit == nullptr)
    throw CommonDataModelException("No electric potential units have been set");
  if (index >= m_Dependent.size())
    throw CommonDataModelException("Dependent index out of bounds");
  return Convert(m_Dependent[index], *m_ElectricPotentialUnit, unit);
}
std::vector<double>& SEFunctionElectricPotentialVsTime::GetElectricPotential()
{
  return m_Dependent;
}
const ElectricPotentialUnit* SEFunctionElectricPotentialVsTime::GetElectricPotentialUnit()
{
  return m_ElectricPotentialUnit;
}
void SEFunctionElectricPotentialVsTime::SetElectricPotentialUnit(const ElectricPotentialUnit& unit)
{
  m_ElectricPotentialUnit = &unit;
}

