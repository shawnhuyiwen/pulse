/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/scenario/SEScenarioUtils.h"
#include "cdm/scenario/SEScenario.h"
#include "cdm/utils/FileUtils.h"

#include <vector>
#include <stdexcept>

std::string actionToken = "[Action]";
std::string tab = "    ";
struct ScenarioAction
{
  std::string ActionName;
  double ScenarioTime_s;
  SEScalarProperties Properties;
  std::map<std::string, std::string> Enumerations;
};

bool ParseLogAction(const std::string& allLines, std::vector<ScenarioAction>& actions, Logger* logger)
{
  ScenarioAction a;

  // Get scenario time
  size_t beginIdx = 0;
  size_t endIdx = allLines.find("(", actionToken.length());
  if (endIdx == std::string::npos)
  {
    logger->Error("Unable to parse scenario time");
    return false;
  }
  a.ScenarioTime_s = std::stod(allLines.substr(actionToken.length(), endIdx - beginIdx));

  // Get action name
  beginIdx = allLines.find(",", endIdx);
  if (beginIdx == std::string::npos)
  {
    logger->Error("Unable to parse action name");
    return false;
  }
  beginIdx += 2;
  endIdx = allLines.find("\n", beginIdx+1);
  if (endIdx == std::string::npos)
  {
    logger->Error("Unable to parse action name");
    return false;
  }
  a.ActionName = allLines.substr(beginIdx, endIdx - beginIdx);

  // Extract properties from remaining lines
  while (true)
  {
    beginIdx = endIdx + 1 + tab.length();
    if (beginIdx >= allLines.length())
    {
      break;
    }

    // Get property name from before colon
    endIdx = allLines.find(":", beginIdx+1);
    if (endIdx == std::string::npos)
    {
      logger->Error("Unable to parse property name");
      return false;
    }
    std::string propName = allLines.substr(beginIdx, endIdx - beginIdx);

    // Get property value after colon
    beginIdx = endIdx + 2;
    endIdx = allLines.find("\n", beginIdx);
    if (endIdx == std::string::npos)
    {
      logger->Error("Unable to parse property value");
      return false;
    }
    std::string line = allLines.substr(beginIdx, endIdx - beginIdx);

    // Check for paren for potential number(unit) format
    size_t unitBeginIdx = line.find("(", 0), unitEndIdx;
    size_t valueEndIdx = line.length() - 1;
    if (unitBeginIdx != std::string::npos)
    {
      valueEndIdx = unitBeginIdx - 1;
    }
    try
    {
      double value = std::stod(line.substr(0, valueEndIdx + 1));
      if (unitBeginIdx != std::string::npos && (unitEndIdx = line.find(")", unitBeginIdx+1)) != std::string::npos)
      {
        std::string unit = line.substr(unitBeginIdx+1, unitEndIdx-(unitBeginIdx+1));
        a.Properties[propName] = SEScalarPair(value, unit);
      }
      else if (unitBeginIdx != std::string::npos && unitEndIdx == std::string::npos)
      {
        logger->Error("Unable to parse unit");
        return false;
      }
      else
      {
        a.Properties[propName] = SEScalarPair(value);
      }
    }
    catch (std::invalid_argument& e) // No number found
    {
      a.Enumerations[propName] = line;
    }
  }

  /*std::cout << "Action: " << a.ActionName << std::endl;
  std::cout << "\tTime: " << a.ScenarioTime_s << std::endl;
  for (auto p: a.Properties)
  {
    std::cout << "\t" << p.first << std::endl;
    std::cout << "\t\tvalue:\t" << p.second.value << "\n\t\tunit:\t" << p.second.unit << std::endl;
  }
  for (auto p: a.Enumerations)
  {
    std::cout << "\t" << p.first << std::endl;
    std::cout << "\t\tvalue:\t" << p.second << std::endl;
  }*/

  actions.push_back(a);

  return true;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Generates previously executed scenario based on log file messages.
//--------------------------------------------------------------------------------------------------
bool SEScenarioUtils::GenerateScenarioFromLog(const std::string& filename, SEScenario& sce, Logger* logger)
{
  std::string content;
  if (!ReadFile(filename, content))
  {
    logger->Error("Unable to read file " + filename);
    return false;
  }

  size_t idx = 0;
  std::vector<ScenarioAction> actions;

  // Identify beginning of an action with token
  while ((idx = content.find(actionToken, idx)) != std::string::npos)
  {
    size_t actionBegin = idx;
    size_t actionEnd = idx + 1;

    // Identify end of an action based on indentation
    while ((actionEnd = content.find("\n", actionEnd)) != std::string::npos)
    {
      if (actionEnd + 1 >= content.length() || tab.compare(content.substr(actionEnd + 1, tab.length())) != 0)
      {
        break;
      }
      actionEnd++;
    }

    // Action identified
    if (actionEnd != std::string::npos)
    {
      std::string action = content.substr(actionBegin, actionEnd - actionBegin + 1);

      // TODO: Create actions from action strings
      if (!ParseLogAction(action, actions, logger))
      {
        logger->Error("Unable to parse action: " + action);
        return false;
      }
    }

    idx = actionEnd + 1;
  }

  return true;
}
