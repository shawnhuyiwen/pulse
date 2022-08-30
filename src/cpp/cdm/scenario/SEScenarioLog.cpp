/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/scenario/SEScenarioLog.h"
#include "cdm/scenario/SEScenario.h"
#include "cdm/utils/FileUtils.h"
// Supported Actions
#include "cdm/engine/SEAdvanceTime.h"
#include "cdm/patient/actions/SEArrhythmia.h"
#include "cdm/patient/actions/SEChestCompressionInstantaneous.h"
#include "cdm/system/equipment/bag_valve_mask/SEBagValveMask.h"
#include "cdm/system/equipment/bag_valve_mask/actions/SEBagValveMaskConfiguration.h"
#include "cdm/system/equipment/bag_valve_mask/actions/SEBagValveMaskSqueeze.h"

#include "cdm/properties/SEScalarForce.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarTime.h"

#include <regex>

SEScenarioLog::SEScenarioLog(Logger* logger) : Loggable(logger)
{
  Clear();
}

SEScenarioLog::~SEScenarioLog()
{
  Clear();
}

void SEScenarioLog::Clear()
{
  m_Actions.clear();
  m_Conditions.clear();
  m_FinalSimTime_s = 0;
  m_Patient = "";
  m_StateFilename = "";
}

bool SEScenarioLog::Extract(const std::string& filename)
{
  std::string content;
  if (!ReadFile(filename, content))
    return false;

  Clear();

  if (!GetActions(content))
  {
    Error("Failed to retrieve scenario actions");
    return false;
  }

  if (!GetConditions(content))
  {
    Error("Failed to retrieve scenario conditions");
    return false;
  }

  if (!GetFinalSimTime(content))
  {
    Error("Failed to retrieve final sim time");
    return false;
  }

  if (!GetPatient(content))
  {
    Error("Failed to retrieve patient information");
    return false;
  }

  if (!GetSerializeFromFile(content))
  {
    Error("Failed to retrieve serializing from file information");
    return false;
  }

  if (!GetSerializeFromString(content))
  {
    Error("Failed to retrieve serializing from string information");
    return false;
  }

  /*std::cout << "Actions: " << std::endl;
  for (auto& [time, actions]: m_Actions)
  {
    std::cout << "Time: " << time << std::endl;
    for (auto& a: actions)
      std::cout << a << std::endl;
  }

  std::cout << "\nConditions: " << std::endl;
  for (auto& c: m_Conditions)
      std::cout << c << std::endl;

  std::cout << "\nFinal time: " << m_FinalSimTime_s << std::endl;
  std::cout << "\nPatient: " << m_Patient << std::endl;
  std::cout << "\nState filename: " << m_StateFilename << std::endl;*/

  return true;
}

bool SEScenarioLog::ExtractTagStrings(const std::string& tag, const std::string& content, std::vector<std::string>& tagStrs)
{
  std::string tagPattern = R"(\[)" + tag + R"(\]([^\{\n]*(\{?)))";
  std::string text = content;

  // Capture groups:
  //  0: Whole match
  //  1: Remainder of line after tag
  //  2: Open bracket if it exists
  std::smatch mTagBegin; 
  std::regex rTagBegin(tagPattern);
  while (std::regex_search(text, mTagBegin, rTagBegin))
  {
    std::string tagStr = "";

    // Locate end of tag string
    size_t endIdx = 0;
    if(mTagBegin[2].compare("{") == 0 && !IdentifyTagStringEnd(mTagBegin.suffix().str(), endIdx))
    {
      Error("Unable to identify tag string terminator: " + std::string(mTagBegin[0]));
      return false;
    }
    tagStr = text.substr(mTagBegin.position(1), mTagBegin.length(1) + endIdx);

    tagStrs.push_back(tagStr);
    text = mTagBegin.suffix().str();
  }

  return true;
}

bool SEScenarioLog::ExtractTagStrings(const std::string& tag, const std::string& content, std::map<double, std::vector<std::string>>& tagStrs)
{
  std::string tagPattern = R"((\[\d*\.?\d*\(.*\)\])\s*\[)" + tag + R"(\]([^\{\n]*(\{?)))";
  std::string text = content;

  // Capture groups:
  //  0: Whole match
  //  1: [Time(unit)]
  //  2: Remainder of line after tag
  //  3: Open bracket if it exists
  std::smatch mTagBegin; 
  std::regex rTagBegin(tagPattern);
  while (std::regex_search(text, mTagBegin, rTagBegin))
  {
    std::string tagStr = "";

    // Parse time
    double time_s;
    ParseTime(mTagBegin[1], time_s);

    // Locate end of tag string
    size_t endIdx = 0;
    if(mTagBegin[3].compare("{") == 0 && !IdentifyTagStringEnd(mTagBegin.suffix().str(), endIdx))
    {
      Error("Unable to identify tag string terminator: " + std::string(mTagBegin[0]));
      return false;
    }
    tagStr = text.substr(mTagBegin.position(2), mTagBegin.length(2) + endIdx);

    if (tagStrs.find(time_s) == tagStrs.end())
    {
      tagStrs[time_s] = std::vector<std::string>();
    }
    tagStrs[time_s].push_back(tagStr);
    text = mTagBegin.suffix().str();
  }

  return true;
}

bool SEScenarioLog::IdentifyTagStringEnd(const std::string& content, size_t& endIdx)
{
  std::smatch mTagEnd;
  std::regex rTagEnd(R"(\}[\r\n]{3,})");
  if (std::regex_search(content, mTagEnd, rTagEnd))
  {
    endIdx = mTagEnd.position(0) + 1;
    return true;
  }
  else // Failed to find terminating bracket
  {
    return false;
  }

  return true;
}

bool SEScenarioLog::ParseTime(const std::string& timeStr, double& time_s)
{
  std::smatch mTime;
  std::string timePattern = R"((\d*\.?\d*)\((.*)\))";
  std::regex rTime(timePattern);

  if (!std::regex_search(timeStr, mTime, rTime))
    return false;

  // Capture groups:
  //  0: Whole match
  //  1: Time
  //  2: Time unit
  double rawTime = std::stod(mTime[1]);
  std::string rawUnit = mTime[2];

  // Convert to seconds
  SEScalarTime time;
  time.SetValue(rawTime, TimeUnit(rawUnit));
  time_s = time.GetValue(TimeUnit::s);

  return true;
}

bool SEScenarioLog::GetActions(const std::string& content)
{
  if (!ExtractTagStrings("Action", content, m_Actions))
  {
    Error("Unable to identify action strings");
    return false;
  }

  return true;
}

bool SEScenarioLog::GetConditions(const std::string& content)
{
  if (!ExtractTagStrings("Condition", content, m_Conditions))
  {
    Error("Unable to identify condition strings");
    return false;
  }

  return true;
}

bool SEScenarioLog::GetFinalSimTime(const std::string& content)
{
  std::vector<std::string> finalSimTimeStrs;
  if (!ExtractTagStrings("Final SimTime", content, finalSimTimeStrs))
  {
    Error("Unable to identify Final SimTime string");
    return false;
  }

  if (finalSimTimeStrs.size() == 0 || !ParseTime(finalSimTimeStrs[0], m_FinalSimTime_s))
  {
    Error("Unable to parse Final SimTime string");
    return false;
  }

  return true;
}

bool SEScenarioLog::GetPatient(const std::string& content)
{
  // Capture groups:
  //  0: Whole match
  //  1: Remainder of line, left trimmed (name info)
  std::string patientPattern = R"(\[Patient\]\s*([^\n]*))";
  std::smatch mPatient; 
  std::regex rPatient(patientPattern);
  std::string text = content;
  if (!std::regex_search(text, mPatient, rPatient))
  {
    Error("Unable to locate patient information");
    return true; // Legacy logs do not have this information
  }

  // Locate end of patient info
  std::string remainingText = mPatient.suffix().str();
  std::smatch mPatientEnd;
  std::regex rPatientEnd(R"(\}[\r\n]{3,})");
  if (std::regex_search(remainingText, mPatientEnd, rPatientEnd))
  {
    m_Patient = text.substr(mPatient.position(1), mPatient.length(1) + mPatientEnd.position(0) + 1);
  }
  else
  {
    // Failed to find terminating bracket
    Error("Unable to identify tag string terminator: " + std::string(mPatient[0]));
    return false;
  }

  return true;
}

bool SEScenarioLog::GetSerializeFromFile(const std::string& content)
{
  std::vector<std::string> serializeFromFileStrs;
  if (!ExtractTagStrings("SerializingFromFile", content, serializeFromFileStrs))
  {
    Error("Unable to identify SerializingFromFile strings");
    return false;
  }

  if (serializeFromFileStrs.size() > 0)
  {
    m_StateFilename = serializeFromFileStrs[serializeFromFileStrs.size() - 1];
  }

  return true;
}

bool SEScenarioLog::GetSerializeFromString(const std::string& content)
{
  std::vector<std::string> serializeFromStringStrs;
  if (!ExtractTagStrings("SerializingFromString", content, serializeFromStringStrs))
  {
    Error("Unable to identify SerializingFromString strings");
    return false;
  }

  if (serializeFromStringStrs.size() > 0)
  {
    m_StateFilename = serializeFromStringStrs[serializeFromStringStrs.size() - 1];
  }

  return true;
}


