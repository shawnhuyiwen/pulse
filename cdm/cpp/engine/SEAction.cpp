/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "engine/SEAction.h"
#include "engine/SEAdvanceTime.h"
#include "engine/SESerializeState.h"
#include "substance/SESubstanceManager.h"


SEAction::SEAction() : Loggable()
{
  m_Comment="";
}

SEAction::~SEAction()
{
  Clear();
}

void SEAction::Clear()
{
  m_Comment="";
}

std::string SEAction::GetComment() const
{
  return m_Comment;
}
void SEAction::SetComment(const std::string& comment)
{
  m_Comment = comment;
}
bool SEAction::HasComment() const
{
  return m_Comment.empty()?false:true;
}
void SEAction::InvalidateComment()
{
  m_Comment = "";
}