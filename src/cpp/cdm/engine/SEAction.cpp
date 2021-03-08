/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "engine/SEAction.h"
#include "engine/SEAdvanceTime.h"
#include "engine/SESerializeState.h"
#include "substance/SESubstanceManager.h"
#include "io/protobuf/PBActions.h"


SEAction::SEAction(Logger* logger) : Loggable(logger)
{
  m_Comment="";
}

SEAction::~SEAction()
{
  m_Comment = "";
}

void SEAction::Clear()
{
  m_Comment="";
  m_Active = false;
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

SEAction* SEAction::Copy(const SEAction& action, const SESubstanceManager& subMgr)
{
  return PBAction::Copy(action, subMgr);
}