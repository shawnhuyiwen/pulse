/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SEScenario;

#include <string>

class CDM_DECL SEScenarioLogLegacy
{
public:
  static bool GenerateScenarioFromLegacyLog(const std::string& logFilename, const std::string& stateFilename, double extraTime_s=0);
};
