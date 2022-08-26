/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SEScenario;

#include <string>

class CDM_DECL SEScenarioUtils
{
public:
  static bool GenerateScenarioFromLog(const std::string& logFilename, SEScenario& sce);
};
