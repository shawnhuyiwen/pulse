/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/scenario/SEScenarioUtils.h"
#include "cdm/scenario/SEScenario.h"
#include "cdm/utils/FileUtils.h"

#include <iostream>

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

  size_t idx = 0, count=0;
  std::string token = "[Action]";
  std::string tab = "    ";
  // Identify beginning of an action with token
  while ((idx = content.find(token, idx)) != std::string::npos)
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

      // TODO: Create actions from action strings instead of just printing them
      std::cout << "found action: " << ++count << "\n" << action;
    }

    idx = actionEnd + 1;
  }

  return true;
}
