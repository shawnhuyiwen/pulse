/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "engine/SECondition.h"
#include "substance/SESubstanceManager.h"
#include "patient/conditions/SEPatientCondition.h"
#include "system/environment/conditions/SEEnvironmentCondition.h"

SECondition::SECondition(Logger* logger) : Loggable(logger)
{
  m_Comment="";
}

SECondition::~SECondition()
{
  m_Comment = "";
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