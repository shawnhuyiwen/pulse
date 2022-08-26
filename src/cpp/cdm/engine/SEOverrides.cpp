/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/engine/SEOverrides.h"
#include "cdm/io/protobuf/PBActions.h"
#include "cdm/properties/SEScalar.h"

SEOverrides::SEOverrides(Logger* logger) : SEAction(logger)
{

}

SEOverrides::~SEOverrides()
{

}

void SEOverrides::Clear()
{
  SEAction::Clear();
  m_ScalarProperties.clear();
}

void SEOverrides::Copy(const SEOverrides& src)
{
  PBAction::Copy(src, *this);
}

bool SEOverrides::IsValid() const
{ 
  return HasScalarProperties();
}

bool SEOverrides::HasScalarProperties() const
{
  return !m_ScalarProperties.empty();
}
SEScalarProperties& SEOverrides::GetScalarProperties()
{
  return m_ScalarProperties;
}
const SEScalarProperties& SEOverrides::GetScalarProperties() const
{
  return m_ScalarProperties;
}

const SEScalar* SEOverrides::GetScalar(const std::string& /*name*/)
{
  // Probably want to iterate over the properties and look for the name?
  return nullptr;
}
