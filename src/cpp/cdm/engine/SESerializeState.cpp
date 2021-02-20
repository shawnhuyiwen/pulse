/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "engine/SESerializeState.h"

SESerializeState::SESerializeState(Logger* logger) : SEAction(logger)
{
  m_Filename="";
  m_Type = eSerialization_Type::Save;
}

SESerializeState::~SESerializeState()
{

}

void SESerializeState::Clear()
{
  SEAction::Clear();
  m_Filename = "";
  m_Type = eSerialization_Type::Save;
}

bool SESerializeState::IsValid() const
{ 
  return HasFilename();
}

eSerialization_Type SESerializeState::GetType() const
{
  return m_Type;
}
void SESerializeState::SetType(eSerialization_Type Type)
{
  m_Type = Type;
}

bool SESerializeState::HasFilename() const
{
  return !(m_Filename.empty());
}
std::string SESerializeState::GetFilename() const
{
  return m_Filename;
}
void SESerializeState::SetFilename(const std::string& filename)
{
  m_Filename = filename;
}
void SESerializeState::InvalidateFilename()
{
  m_Filename = "";
}

void SESerializeState::ToString(std::ostream &str) const
{  
  if(HasComment())
    str<<"\n\tComment : "<<m_Comment;
  str << "\n\tType : " << eSerialization_Type_Name(m_Type);
  str << "\n\tFilename : " << m_Filename;
}

const SEScalar* SESerializeState::GetScalar(const std::string& name)
{
  return nullptr;
}
