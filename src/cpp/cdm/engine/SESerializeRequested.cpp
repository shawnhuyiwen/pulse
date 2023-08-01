/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/engine/SESerializeRequested.h"

SESerializeRequested::SESerializeRequested(Logger* logger) : SEAction(logger)
{
  m_Filename="";
  m_ClearCache = true;
  m_ID = 0;
}

SESerializeRequested::~SESerializeRequested()
{

}

void SESerializeRequested::Clear()
{
  SEAction::Clear();
  m_Filename = "";
  m_ClearCache = true;
  m_ID = 0;
}

bool SESerializeRequested::IsValid() const
{ 
  return HasFilename();
}

bool SESerializeRequested::HasFilename() const
{
  return !(m_Filename.empty());
}
std::string SESerializeRequested::GetFilename() const
{
  return m_Filename;
}
void SESerializeRequested::SetFilename(const std::string& filename)
{
  m_Filename = filename;
  std::replace(m_Filename.begin(), m_Filename.end(), '\\', '/');
}
void SESerializeRequested::InvalidateFilename()
{
  m_Filename = "";
}

const SEScalar* SESerializeRequested::GetScalar(const std::string& /*name*/)
{
  return nullptr;
}
