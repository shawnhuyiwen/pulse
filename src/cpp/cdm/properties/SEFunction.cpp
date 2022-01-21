/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEFunction.h"
#include "cdm/properties/SEScalar.h"
#include "cdm/utils/FileUtils.h"

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

size_t SEFunction::Length() const
{
  if(IsValid())
    return m_Independent.size();
  return 0;
}

double SEFunction::GetDependentValue(size_t index) const
{
  if (index >= m_Dependent.size())
    throw CommonDataModelException("Dependent index out of bounds");
  return m_Dependent[index];
}

std::vector<double>& SEFunction::GetDependent()
{
  return m_Dependent;
}

double SEFunction::GetIndependentValue(size_t index) const
{
  if (index >= m_Independent.size())
    throw CommonDataModelException("Independent index out of bounds");
  return m_Independent[index];
}

std::vector<double>& SEFunction::GetIndependent()
{
  return m_Independent;
}

bool SEFunction::WriteCSVFile(const std::string& filename) const
{
  std::stringstream content;
  content <<
    (GetIndependentUnit()==nullptr ? "unitless":GetIndependentUnit()->GetString())
    << "," <<
    (GetDependentUnit()==nullptr ? "unitless":GetDependentUnit()->GetString())
    << "\n";

  for (size_t i = 0; i < Length(); i++)
    content << GetIndependentValue(i) << "," << GetDependentValue(i) << "\n";

  return WriteFile(content.str(), filename);
}
