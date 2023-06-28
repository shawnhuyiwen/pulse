/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/io/protobuf/PBEngine.h"
#include "cdm/utils/FileUtils.h"
#include <cstring>

bool LogMessages::SerializeToString(const LogMessages& msgs, std::string& output, eSerializationFormat m, Logger* logger)
{
  return PBEngine::SerializeToString(msgs, output, m, logger);
}
bool LogMessages::SerializeFromString(const std::string& src, LogMessages& msgs, eSerializationFormat m, Logger* logger)
{
  return PBEngine::SerializeFromString(src, msgs, m, logger);
}

std::string pulse::cdm::to_string(double d)
{
  return to_string(float(d));
}
std::string pulse::cdm::to_string(float f)
{
  char buffer[32];
  memset(buffer, 0, sizeof(buffer));
  snprintf(buffer, sizeof(buffer), "%g", f);
  return std::string(buffer);
}

std::string pulse::cdm::PrettyPrint(const std::string& str, ePrettyPrintType ppt)
{
  std::string typeTag = "";
  switch (ppt)
  {
    case ePrettyPrintType::Action:
      typeTag = "Action";
      break;
    case ePrettyPrintType::Condition:
      typeTag = "Condition";
      break;
    default:
      std::cerr << "ERROR: Unknown pretty print type" << std::endl;
      return "";
  }


  std::stringstream out;
  std::stringstream buffer(str);
  // Replace all '{' '}' with ''
  buffer.str(Replace(buffer.str(), "\"", ""));
  buffer.str(Replace(buffer.str(), "{", ""));
  buffer.str(Replace(buffer.str(), "}", ""));
  buffer.str(Replace(buffer.str(), "[", ""));
  buffer.str(Replace(buffer.str(), "]", ""));
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
      // Remove PrettyPrintType Tags
      idx = line.find(typeTag);
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

      if (buffer.eof())
      {
        out << line << "\n";
        break;
      }
      std::getline(buffer, peek);
      // Check to see if this line is part of a scalar block
      idx = peek.find("Scalar");
      if (idx != std::string::npos)
      {
        std::getline(buffer, peek);// This should be "Value:"
        line += peek.substr(peek.find("Value")+5);
        if (!buffer.eof())
        {
          std::getline(buffer, peek);
          unit = peek.find("Unit:");
          if (unit != std::string::npos)
          {
            line += peek.substr(unit + 5);
            out << line << "\n";
          }
          else
          {
            out << line << "\n";
            line = peek;
          }
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
