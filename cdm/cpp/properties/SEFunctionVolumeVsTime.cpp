/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "properties/SEFunctionVolumeVsTime.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarVolume.h"


SEFunctionVolumeVsTime::SEFunctionVolumeVsTime() : SEFunction()
{
  
}

SEFunctionVolumeVsTime::~SEFunctionVolumeVsTime()
{
  Clear();
}

void SEFunctionVolumeVsTime::Clear()
{
  SEFunction::Clear();
  m_TimeUnit = nullptr;
  m_VolumeUnit = nullptr;
}

double SEFunctionVolumeVsTime::GetTimeValue(size_t index, const TimeUnit& unit)
{
  if (m_TimeUnit == nullptr)
    throw CommonDataModelException("No time units have been set");
  if (index >= m_Independent.size())
    throw CommonDataModelException("Independent index out of bounds");
  return Convert(m_Independent[index], *m_TimeUnit, unit);
}
std::vector<double>& SEFunctionVolumeVsTime::GetTime()
{
  return m_Independent;
}
const TimeUnit* SEFunctionVolumeVsTime::GetTimeUnit()
{
  return m_TimeUnit;
}
void SEFunctionVolumeVsTime::SetTimeUnit(const TimeUnit& unit)
{
  m_TimeUnit = &unit;
}

double SEFunctionVolumeVsTime::GetVolumeValue(size_t index, const VolumeUnit& unit)
{
  if (m_VolumeUnit == nullptr)
    throw CommonDataModelException("No volume units have been set");
  if (index >= m_Dependent.size())
    throw CommonDataModelException("Dependent index out of bounds");
  return Convert(m_Dependent[index], *m_VolumeUnit, unit);
}
std::vector<double>& SEFunctionVolumeVsTime::GetVolume()
{
  return m_Dependent;
}
const VolumeUnit* SEFunctionVolumeVsTime::GetVolumeUnit()
{
  return m_VolumeUnit;
}
void SEFunctionVolumeVsTime::SetVolumeUnit(const VolumeUnit& unit)
{
  m_VolumeUnit = &unit;
}
