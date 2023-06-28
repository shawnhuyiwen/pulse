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
SECondition* SECondition::SerializeFromString(const std::string src, eSerializationFormat fmt, const SESubstanceManager& subMgr)
{
  return PBCondition::SerializeFromString(src, fmt, subMgr);
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

std::string SECondition::ToJSON() const
{
  std::string s;
  if (!SerializeToString(s, eSerializationFormat::VERBOSE_JSON))
    Error("Unable to serialize condition to json");
  return s;
}
std::string SECondition::ToString() const
{
  return PrettyPrint(this->ToJSON());
}

std::string SECondition::PrettyPrint(const std::string& str)
{
  return pulse::cdm::PrettyPrint(str, ePrettyPrintType::Condition);
}
