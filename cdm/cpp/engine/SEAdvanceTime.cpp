/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "engine/SEAdvanceTime.h"
#include "properties/SEScalarTime.h"

SEAdvanceTime::SEAdvanceTime() : SEAction()
{
  m_Time=nullptr;
}

SEAdvanceTime::~SEAdvanceTime()
{
  Clear();
}

void SEAdvanceTime::Clear()
{
  SEAction::Clear();
  SAFE_DELETE(m_Time);
}

bool SEAdvanceTime::IsValid() const
{ 
  return HasTime() && m_Time->IsValid();
}

void SEAdvanceTime::ToString(std::ostream &str) const
{  
  if(HasComment())
    str<<"\n\tComment : "<<m_Comment;
  str << "Advance Time : " << m_Time;
}

bool SEAdvanceTime::HasTime() const
{
  return m_Time==nullptr?false:m_Time->IsValid();
}
SEScalarTime& SEAdvanceTime::GetTime()
{
  if(m_Time==nullptr)
    m_Time=new SEScalarTime();
  return *m_Time;
}
double SEAdvanceTime::GetTime(const TimeUnit& unit) const
{
  if (m_Time == nullptr)
    return SEScalar::dNaN();
  return m_Time->GetValue(unit);
}