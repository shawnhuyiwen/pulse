/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "scenario/SECondition.h"
#include "substance/SESubstanceManager.h"
#include "patient/conditions/SEPatientCondition.h"
#include "system/environment/conditions/SEEnvironmentCondition.h"

SECondition::SECondition() : Loggable()
{
  m_Comment="";
}

SECondition::~SECondition()
{
  Clear();
}

void SECondition::Clear()
{
  m_Comment = "";
}

std::string SECondition::GetComment() const
{
  return m_Comment;
}
void SECondition::SetComment(const std::string& comment)
{
  m_Comment = comment;
}
bool SECondition::HasComment() const
{
  return m_Comment.empty()?false:true;
}
void SECondition::InvalidateComment()
{
  m_Comment = "";
}