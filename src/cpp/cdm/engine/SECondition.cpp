/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/engine/SECondition.h"
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/patient/conditions/SEPatientCondition.h"
#include "cdm/system/environment/conditions/SEEnvironmentCondition.h"
#include "cdm/io/protobuf/PBConditions.h"

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

bool SECondition::SerializeToString(std::string& dst, eSerializationFormat fmt) const
{
  return PBCondition::SerializeToString(*this, dst, fmt);
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

