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
  RemovePairs();
}

void SEOverrides::Copy(const SEOverrides& src)
{
  PBAction::Copy(src, *this);
}

bool SEOverrides::IsValid() const
{ 
  return HasPairs();
}

void SEOverrides::ToString(std::ostream &str) const
{  
  if(HasComment())
    str<<"\n\tComment : "<<m_Comment;
  for(auto itr : m_Pairs)
    str << "Override " << itr.first << " with " << itr.second;
}

bool SEOverrides::HasPairs() const
{
  return m_Pairs.size()>1;
}
void SEOverrides::AddPair(const std::string& name, double value)
{
  m_Pairs[name] = value;
}
std::map<std::string, double>& SEOverrides::GetPairs()
{
  return m_Pairs;
}
const std::map<std::string, double>& SEOverrides::GetPairs() const
{
  return m_Pairs;
}
void SEOverrides::RemovePairs()
{
  m_Pairs.clear();
}
