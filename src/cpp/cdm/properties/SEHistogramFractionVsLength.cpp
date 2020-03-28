/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "properties/SEHistogramFractionVsLength.h"
#include "properties/SEScalarLength.h"
#include "properties/SEScalar0To1.h"
#include <numeric>


SEHistogramFractionVsLength::SEHistogramFractionVsLength() : SEHistogram()
{
  
}

SEHistogramFractionVsLength::~SEHistogramFractionVsLength()
{
  Clear();
}

void SEHistogramFractionVsLength::Clear()
{
  SEHistogram::Clear();
  m_LengthUnit = nullptr;
}

bool SEHistogramFractionVsLength::IsVaild() const
{
  if (!SEHistogram::IsValid())
    return false;

  double binned = std::accumulate(m_Dependent.begin(), m_Dependent.end(), 0.);
  if (!SEScalar::IsValue(1, binned))
  {
   std::cerr << "Particle distribution histogram: Failed to correct a distribution error. Check distribution histogram." << std::endl; // todo properties need a logger
   return false;
  }
  return true;
}

double SEHistogramFractionVsLength::GetLengthValue(size_t index, const LengthUnit& unit) const
{
  if (m_LengthUnit == nullptr)
    throw CommonDataModelException("No length units have been set");
  if (index >= m_Independent.size())
    throw CommonDataModelException("Independent index out of bounds");
  return Convert(m_Independent[index], *m_LengthUnit, unit);
}
std::vector<double>& SEHistogramFractionVsLength::GetLength()
{
  return m_Independent;
}
const std::vector<double>& SEHistogramFractionVsLength::GetLength() const
{
  return m_Independent;
}
const LengthUnit* SEHistogramFractionVsLength::GetLengthUnit() const
{
  return m_LengthUnit;
}
void SEHistogramFractionVsLength::SetLengthUnit(const LengthUnit& unit)
{
  m_LengthUnit = &unit;
}

double SEHistogramFractionVsLength::GetFractionValue(size_t index) const
{
  if (index >= m_Dependent.size())
    throw CommonDataModelException("Dependent index out of bounds");
  return m_Dependent[index];
}
std::vector<double>& SEHistogramFractionVsLength::GetFraction()
{
  return m_Dependent;
}
const std::vector<double>& SEHistogramFractionVsLength::GetFraction() const
{
  return m_Dependent;
}

