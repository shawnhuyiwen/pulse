/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SEScenario;

#include <string>

class CDM_DECL SEScenarioLog : public Loggable
{
public:
  SEScenarioLog(Logger* logger);
  virtual ~SEScenarioLog();

  virtual void Clear();

  bool Extract(const std::string& filename);
  
  // Highly experimental and probably won't work, but easy to improve if you really need it
  static bool GenerateScenarioFromLegacyLog(const std::string& logFilename, const std::string& stateFilename, double extraTime_s=0);

protected:
  bool ExtractTagStrings(const std::string& tag, const std::string& content, std::vector<std::string>& tagStrs);
  bool ExtractTagStrings(const std::string& tag, const std::string& content, std::map<double, std::vector<std::string>>& tagStrs);
  bool GetActions(const std::string& content);
  bool GetConditions(const std::string& content);
  bool GetFinalSimTime(const std::string& content);
  bool GetPatient(const std::string& content);
  bool GetSerializeFromFile(const std::string& content);
  bool GetSerializeFromString(const std::string& content);
  bool IdentifyTagStringEnd(const std::string& content, size_t& endIdx);
  bool ParseTime(const std::string& timeStr, double& time_s);

  std::map<double, std::vector<std::string>> m_Actions;
  std::vector<std::string>                   m_Conditions;
  double                                     m_FinalSimTime_s;
  std::string                                m_Patient;
  std::string                                m_StateFilename;
};
