/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/engine/SEAction.h"
#include "cdm/engine/SEAdvanceTime.h"
#include "cdm/engine/SESerializeState.h"
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/io/protobuf/PBActions.h"
#include "cdm/utils/FileUtils.h"


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

bool SEAction::SerializeToString(std::string& dst, eSerializationFormat fmt) const
{
  return PBAction::SerializeToString(*this, dst, fmt);
}
SEAction* SEAction::SerializeFromString(const std::string src, eSerializationFormat fmt, const SESubstanceManager& subMgr)
{
  return PBAction::SerializeFromString(src, fmt, subMgr);
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

std::string SEAction::ToJSON() const
{
  std::string s;
  if (!SerializeToString(s, eSerializationFormat::VERBOSE_JSON))
    Error("Unable to serialize action to json");
  return s;
}

std::string SEAction::PrettyPrint(const std::string& str)
{
  std::stringstream out;
  std::stringstream buffer(str);
  // Replace all '{' '}' with ''
  buffer.str(Replace(buffer.str(), "\"", ""));
  buffer.str(Replace(buffer.str(), "{", ""));
  buffer.str(Replace(buffer.str(), "}", ""));
  buffer.str(Replace(buffer.str(), ",", ""));

  size_t idx;
  std::string line;
  std::string peek;
  size_t unit;

  std::getline(buffer, line);
  do
  {
    // Trim the line of white space
    TrimBack(line);
    // Process non empty lines
    if (!line.empty())
    {
      // Remove Action* Tags
      idx = line.find("Action");
      if (idx != std::string::npos)
      {
        // Skip this line and move on
        if (buffer.eof()) break;
        std::getline(buffer, line);
        continue;
      }

      // Remove ReadOnly Tags
      idx = line.find("ReadOnly");
      if (idx != std::string::npos)
      {
        // Skip this line and move on
        if (buffer.eof()) break;
        std::getline(buffer, line);
        continue;
      }

      // Remove Empty Comment Tags
      idx = line.find("Comment:");
      if (idx != std::string::npos && (idx+8) == line.length())
      {
        // Skip this line and move on
        if (buffer.eof()) break;
        std::getline(buffer, line);
        continue;
      }

      if (buffer.eof()) break;
      std::getline(buffer, peek);
      // Check to see if this line is part of a scalar block
      idx = peek.find("Scalar");
      if (idx != std::string::npos)
      {
        std::getline(buffer, peek);// This should be "Value:"
        line += peek.substr(peek.find("Value")+5);
        if (buffer.eof()) break;
        std::getline(buffer, peek);
        unit = peek.find("Unit:");
        if (unit != std::string::npos)
        {
          line += peek.substr(unit+5);
          out << line << "\n";
        }
        else
        {
          out << line << "\n";
          line = peek;
        }
      }
      else // Write what we had
      {
        out << line << "\n";
        line = peek;
        continue;
      }
    }

    if (buffer.eof()) break;
    std::getline(buffer, line);
  } while (true);

  out.str(Replace(out.str(), "::", ":"));
  return out.str();
}
