/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "properties/SEFunction.h"
#include "properties/SEScalar.h"//Utils

static std::stringstream err;

SEFunction::SEFunction() : SEProperty()
{

}

SEFunction::~SEFunction()
{
  Clear();
}

void SEFunction::Clear()
{
  m_Dependent.clear();
  m_Independent.clear();
}

bool SEFunction::IsValid() const
{  
  if(m_Dependent.size()==0||m_Independent.size()==0)
    return false;
  if(m_Dependent.size()!=m_Independent.size())
    return false;
  return true;
}

void SEFunction::Invalidate()
{
  Clear();
}

size_t SEFunction::Length()
{
  if(IsValid())
    return m_Independent.size();
  return 0;
}

double SEFunction::GetDependentValue(size_t index)
{
  if (index >= m_Dependent.size())
    throw CommonDataModelException("Dependent index out of bounds");
  return m_Dependent[index];
}

std::vector<double>& SEFunction::GetDependent()                                  
{
  return m_Dependent;
}

double SEFunction::GetIndependentValue(size_t index)
{
  if (index >= m_Independent.size())
    throw CommonDataModelException("Independent index out of bounds");
  return m_Independent[index];
}

std::vector<double>& SEFunction::GetIndependent()                                  
{
  return m_Independent;
}
