/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "properties/SEHistogram.h"

static std::stringstream err;

SEHistogram::SEHistogram() : SEProperty()
{

}

SEHistogram::~SEHistogram()
{
  Clear();
}

void SEHistogram::Clear()
{
  m_Dependent.clear();
  m_Independent.clear();
}

bool SEHistogram::IsValid() const
{  
  if (m_Dependent.size() == 0 || m_Independent.size() == 0)
  {
    std::cerr << "Histogram: No data provided." << std::endl;//todo make properties have a logger
    return false;
  }
  if (m_Dependent.size() + 1 != m_Independent.size())
  {
    std::cerr << "Histogram: Size mismatch between boundaries and bins." << std::endl;//todo make properties have a logger
    return false;
  }
  return true;
}

void SEHistogram::Invalidate()
{
  Clear();
}

size_t SEHistogram::NumberOfBins() const
{
  if(IsValid())
    return m_Dependent.size();
  return 0;
}

size_t SEHistogram::NumberOfBoundaries() const
{
  if (IsValid())
    return m_Independent.size();
  return 0;
}

double SEHistogram::GetDependentValue(size_t index) const
{
  if (index >= m_Dependent.size())
    throw CommonDataModelException("Dependent index out of bounds");
  return m_Dependent[index];
}

std::vector<double>& SEHistogram::GetDependent()                                  
{
  return m_Dependent;
}
const std::vector<double>& SEHistogram::GetDependent() const
{
  return m_Dependent;
}

double SEHistogram::GetIndependentValue(size_t index) const
{
  if (index >= m_Independent.size())
    throw CommonDataModelException("Independent index out of bounds");
  return m_Independent[index];
}

std::vector<double>& SEHistogram::GetIndependent()                                  
{
  return m_Independent;
}
const std::vector<double>& SEHistogram::GetIndependent() const
{
  return m_Independent;
}
