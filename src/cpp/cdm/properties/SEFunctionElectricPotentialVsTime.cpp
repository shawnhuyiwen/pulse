/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEFunctionElectricPotentialVsTime.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/properties/SEScalarElectricPotential.h"
#include "cdm/utils/GeneralMath.h"
#include "cdm/utils/Logger.h"


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

double SEFunctionElectricPotentialVsTime::GetTimeValue(size_t index, const TimeUnit& unit) const
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
const TimeUnit* SEFunctionElectricPotentialVsTime::GetTimeUnit() const
{
  return m_TimeUnit;
}
void SEFunctionElectricPotentialVsTime::SetTimeUnit(const TimeUnit& unit)
{
  m_TimeUnit = &unit;
}

double SEFunctionElectricPotentialVsTime::GetElectricPotentialValue(size_t index, const ElectricPotentialUnit& unit) const
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
const ElectricPotentialUnit* SEFunctionElectricPotentialVsTime::GetElectricPotentialUnit() const
{
  return m_ElectricPotentialUnit;
}
void SEFunctionElectricPotentialVsTime::SetElectricPotentialUnit(const ElectricPotentialUnit& unit)
{
  m_ElectricPotentialUnit = &unit;
}

