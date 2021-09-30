/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/engine/SEAction.h"
#include "cdm/engine/SEAdvanceTime.h"
#include "cdm/engine/SESerializeState.h"
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/io/protobuf/PBActions.h"


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