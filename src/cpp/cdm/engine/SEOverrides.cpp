/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "engine/SEOverrides.h"
#include "io/protobuf/PBActions.h"

SEOverrides::SEOverrides() : SEAction()
{
  
}

SEOverrides::~SEOverrides()
{
  Clear();
}

void SEOverrides::Clear()
{
  SEAction::Clear();
  RemoveProperties();
}

void SEOverrides::Copy(const SEOverrides& src)
{
  PBAction::Copy(src, *this);
}

bool SEOverrides::IsValid() const
{ 
  return HasProperty();
}

void SEOverrides::ToString(std::ostream &str) const
{  
  if(HasComment())
    str<<"\n\tComment : "<<m_Comment;
  for (auto itr : m_ScalarProperties)
  {
    if(itr.unit.empty())
      str << "Override " << itr.name << " with " << itr.value;
    else
      str << "Override " << itr.name << " with " << itr.value << "("<<itr.unit<<")";
  }
}

bool SEOverrides::HasProperty() const
{
  return m_ScalarProperties.size()>1;
}
void SEOverrides::AddScalarProperty(const std::string& name, double value)
{
  m_ScalarProperties.push_back(SEScalarProperty(name, value, ""));
}
void SEOverrides::AddScalarProperty(const std::string& name, double value, std::string unit)
{
  m_ScalarProperties.push_back(SEScalarProperty(name, value, unit));
}
void SEOverrides::AddScalarProperty(const std::string& name, double value, const CCompoundUnit& unit)
{
  m_ScalarProperties.push_back(SEScalarProperty(name,value,unit.GetString()));
}
std::vector<SEScalarProperty>& SEOverrides::GetScalarProperties()
{
  return m_ScalarProperties;
}
const std::vector<SEScalarProperty>& SEOverrides::GetScalarProperties() const
{
  return m_ScalarProperties;
}
void SEOverrides::RemoveProperties()
{
  m_ScalarProperties.clear();
}
