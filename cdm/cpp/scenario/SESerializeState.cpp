/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "scenario/SESerializeState.h"
#include "bind/cdm/Actions.pb.h"
#include "bind/cdm/ActionEnums.pb.h"

const std::string& eSerialization_Type_Name(eSerialization_Type m)
{
  return cdm::eSerialization_Type_Name((cdm::eSerialization_Type)m);
}

SESerializeState::SESerializeState() : SEAction()
{
  m_Filename="";
  m_Type = eSerialization_Type::Save;
}

SESerializeState::~SESerializeState()
{
  Clear();
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

void SESerializeState::Load(const cdm::SerializeStateData& src, SESerializeState& dst)
{
  SESerializeState::Serialize(src, dst);
}
void SESerializeState::Serialize(const cdm::SerializeStateData& src, SESerializeState& dst)
{
  dst.Clear();
  dst.SetType((eSerialization_Type)src.type());
  dst.SetFilename(src.filename());
}

cdm::SerializeStateData* SESerializeState::Unload(const SESerializeState& src)
{
  cdm::SerializeStateData* dst = new cdm::SerializeStateData();
  SESerializeState::Serialize(src, *dst);
  return dst;
}
void SESerializeState::Serialize(const SESerializeState& src, cdm::SerializeStateData& dst)
{
  dst.set_type((cdm::eSerialization_Type)src.m_Type);
  if (src.HasFilename())
    dst.set_filename(src.m_Filename);
}


void SESerializeState::ToString(std::ostream &str) const
{  
  if(HasComment())
    str<<"\n\tComment : "<<m_Comment;
  str << "\n\tType : " << eSerialization_Type_Name(m_Type);
  str << "\n\tFilename : " << m_Filename;
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