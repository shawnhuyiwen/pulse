/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/scenario/SEScenarioUtils.h"
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

struct ScenarioAction
{
  std::string Name;
  std::string Type = "";
  double ScenarioTime_s;
  SEScalarProperties Properties;
  std::map<std::string, std::string> Enumerations;
};

const std::string WHITESPACE = " \n\r\t\f\v";
 
std::string ltrim(const std::string &s)
{
  size_t start = s.find_first_not_of(WHITESPACE);
  return (start == std::string::npos) ? "" : s.substr(start);
}
 
std::string rtrim(const std::string &s)
{
  size_t end = s.find_last_not_of(WHITESPACE);
  return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
 
std::string trim(const std::string &s) {
  return rtrim(ltrim(s));
}

bool ParseTime(const std::string& timeStr, double& time_s)
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

bool ParseLegacyLogAction(const std::string& allLines, std::vector<ScenarioAction>& actions, Logger* logger)
{
  ScenarioAction a;

  std::smatch mAction;
  std::string actionBeginPattern = R"(\[(\d*\.?\d*\(.*\))\][ \t]*\[Action\][ \t]*(?:\1,[ \t]*)(\S.*\S))";
  std::regex rAction(actionBeginPattern);

  // Capture groups:
  //  0: Whole match
  //  1: Time(Time unit) 
  //  2: Action Type/Action Name
  if (!std::regex_search(allLines, mAction, rAction))
  {
    logger->Error("Unable to parse action");
    return false;
  }
  
  // Get scenario time
  if (!ParseTime(mAction[1], a.ScenarioTime_s))
  {
    logger->Error("Unable to parse action time");
  }

  // Get action name and type if it exists
  size_t colonIdx;
  std::string actionIdentifier(mAction[2]);
  if ((colonIdx = actionIdentifier.find(":")) != std::string::npos)
  {
    a.Type = trim(actionIdentifier.substr(0, colonIdx));
    a.Name = trim(actionIdentifier.substr(colonIdx + 1));
  }
  else
  {
    a.Name = mAction[2];
  }

  // Extract properties from remaining lines
  std::string properties = mAction.suffix().str();
  std::string propertyPattern = R"(    (\w+):\s*([\d*\.?\d]*)?(?:\((.*)\))?(\S*)?)";
  std::smatch mProperty; 
  std::regex rProperty(propertyPattern);
  while (std::regex_search(properties, mProperty, rProperty))
  {
    // Capture groups:
    //  0: Whole match
    //  1: Property name
    //  2: Numeric value
    //  3: Unit
    //  4: NaN or Enumeration Value

    std::string propName = mProperty[1];

    double value;
    std::string valueStr;
    if (mProperty[4].compare("NaN") == 0)
    {
      value = std::stod(mProperty[4]);
      a.Properties[propName] = SEScalarPair(value);
    }
    else if (mProperty[2].compare("") != 0) // value(unit) or value
    {
      value = std::stod(mProperty[2]);
      if (mProperty[3].compare("") != 0) // unit
      {
        a.Properties[propName] = SEScalarPair(value, mProperty[3]);
      }
      else
      {
        a.Properties[propName] = SEScalarPair(value);
      }
    }
    else //Enumeration
    {
      a.Enumerations[propName] = mProperty[4];
    }
    // TODO: Account for properties that result in further indentation

    properties = mProperty.suffix().str();
  }

  /*std::cout << "Action: " << a.Name << std::endl;
  std::cout << "\tType: " << a.Type << std::endl;
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

bool ParseLogAction(const std::string& allLines, std::vector<ScenarioAction>& actions, Logger* logger)
{
  std::smatch mAction;
  std::string actionBeginPattern = R"(\[(\d*\.?\d*\(.*\))\][ \t]*\[Action\]\s*([^\n\{]*)\s+(\{?))";
  std::regex rAction(actionBeginPattern);

  // Capture groups:
  //  0: Whole match
  //  1: Time(Time unit) 
  //  2: Action Type (non-legacy)
  //  3: Bracket indicating non-legacy action
  if (!std::regex_search(allLines, mAction, rAction))
  {
    logger->Error("Unable to parse action");
    return false;
  }

  if (mAction[3].compare("{") != 0)
  {
    return ParseLegacyLogAction(allLines, actions, logger);
  }

  // TODO: Parse new log actions

  return true;
}

bool IdentifyTagStrings(const std::string& tag, const std::string& content, std::vector<std::string>& tagStrs, Logger* logger)
{
  tagStrs.clear();

  std::string tagPattern = R"(\[(\d*\.?\d*\(.*\))\][ \t]*\[)" + tag + R"(\][^\{\n]*(\{?))";

  std::string text = content;

  std::smatch mTagBegin; 
  std::regex rTagBegin(tagPattern);
  while (std::regex_search(text, mTagBegin, rTagBegin))
  {
    // Capture groups:
    //  0: Whole match
    //  1: Time(Time unit) 
    //  2: Open bracket indicating non-legacy action
    std::string remainingText = mTagBegin.suffix().str();
    std::string tagStr = "";
    if (mTagBegin[2].compare("{") == 0)
    {
      // Find end of action
      std::smatch mTagEnd;
      std::regex rTagEnd(R"(\}[\r\n]{3,})");
      if (std::regex_search(remainingText, mTagEnd, rTagEnd))
      {
        tagStr = text.substr(mTagBegin.position(0), mTagBegin.length(0) + mTagEnd.position(0) + 1);
      }
      else
      {
        // Failed to find terminating bracket
        logger->Error("Unable to identify tag string terminator: " + std::string(mTagBegin[0]));
        return false;
      }
    }
    else // Legacy log
    {
      tagStr += mTagBegin[0];

      // Collect action properties
      std::smatch mTagProperties;
      std::regex rTagProperties(R"(\n    [^\n]*)");
      while (std::regex_search(remainingText, mTagProperties, rTagProperties))
      {
        if (mTagProperties.position(0) != 0) // Needs to immediately follow previous match
          break;
        tagStr += mTagProperties[0];
        remainingText = mTagProperties.suffix().str();
      }
    }

    tagStrs.push_back(tagStr);
    text = mTagBegin.suffix().str().substr(tagStr.length() - mTagBegin[0].length());
  }

  return true;
}

void TestAction(SEAction& a)
{
  std::cout << "Adding action: " << a << std::endl;
}

bool AddSEAction(ScenarioAction& sa, SEScenario& sce)
{
  if (sa.Type == SEPatientAction::ActionType)
  {
    if (sa.Name == SEArrhythmia::Name)
    {
      SEArrhythmia a;
      if (sa.Enumerations.find("Rhythm") != sa.Enumerations.end())
        a.SetRhythm(eHeartRhythm::NormalSinus);

      sce.AddAction(a);
      TestAction(a);
      return true;
    }
    if (sa.Name == SEChestCompressionInstantaneous::Name)
    {
      SEChestCompressionInstantaneous a;
      if (sa.Properties.find("Force") != sa.Properties.end())
        a.GetForce().SetValue(sa.Properties["Force"].value, ForceUnit::N);

      sce.AddAction(a);
      TestAction(a);
      return true;
    }
  }

  if (sa.Type == SEBagValveMaskAction::ActionType)
  {
    if (sa.Name == SEBagValveMaskConfiguration::Name)
    {
      SEBagValveMaskConfiguration a;
      if (sa.Properties.find("ValvePositiveEndExpiredPressure") != sa.Properties.end())
        a.GetConfiguration().GetValvePositiveEndExpiredPressure().SetValue(sa.Properties["ValvePositiveEndExpiredPressure"].value, PressureUnit::cmH2O);
      if (sa.Enumerations.find("Connection") != sa.Enumerations.end())
      {
        std::string c = sa.Enumerations["Connection"];
        if (c == "On")
          a.GetConfiguration().SetConnection(eSwitch::On);
        else if (c == "Off")
          a.GetConfiguration().SetConnection(eSwitch::Off);
      }

      sce.AddAction(a);
      TestAction(a);
      return true;
    }
    if (sa.Name == SEBagValveMaskSqueeze::Name)
    {
      SEBagValveMaskSqueeze a;
      if (sa.Properties.find("ExpiratoryPeriod") != sa.Properties.end())
        a.GetExpiratoryPeriod().SetValue(sa.Properties["ExpiratoryPeriod"].value, TimeUnit::s);
      if (sa.Properties.find("InspiratoryPeriod") != sa.Properties.end())
        a.GetInspiratoryPeriod().SetValue(sa.Properties["InspiratoryPeriod"].value, TimeUnit::s);

      sce.AddAction(a);
      TestAction(a);
      return true;
    }
  }

  std::cerr << "Unknown action : " << sa.Type << " " << sa.Name << std::endl;
  return false;
}

bool GetActions(const std::string& content, SEScenario& sce)
{
  // Identify action strings
  std::vector<std::string> actionStrs;
  if (!IdentifyTagStrings("Action", content, actionStrs, sce.GetLogger()))
  {
    return false;
  }

  // Parse actions from strings
  std::vector<ScenarioAction> actions;
  for( auto& a: actionStrs)
  {
    if (!ParseLogAction(a, actions, sce.GetLogger()))
    {
      return false;
    }
  }
  
  // Create SEActions from actions
  double time_s = 0;
  for (ScenarioAction& sa : actions)
  {
    if (sa.ScenarioTime_s > time_s)
    {
      SEAdvanceTime adv;
      adv.GetTime().SetValue(sa.ScenarioTime_s - time_s, TimeUnit::s);
      time_s += sa.ScenarioTime_s;
      std::cout << adv;
    }
    if (!AddSEAction(sa,sce))
    {
      return false;
    }
  }

  return true;
}

bool GetConditions(const std::string& content, SEScenario& sce)
{
  // Identify action strings
  std::vector<std::string> conditionStrs;
  if (!IdentifyTagStrings("Condition", content, conditionStrs, sce.GetLogger()))
  {
    sce.Error("Unable to identify condition strings");
    return false;
  }

  // TODO: Parse conditions from strings

  return true;
}

bool GetFinalSimTime(const std::string& content, SEScenario& sce)
{
  // Capture groups:
  //  0: Whole match
  //  1: Time(Time unit) 
  std::smatch mFinalSimTime; 
  std::string finalSimTimePattern = R"(\[Final SimTime\]\s*(\d*\.?\d*\(.*\)))";
  std::regex rFinalSimTime(finalSimTimePattern);
  if (!std::regex_search(content, mFinalSimTime, rFinalSimTime))
  {
    sce.Warning("Unable to locate Final SimTime");
    return false;
  }

  double time_s;
  if (!ParseTime(mFinalSimTime[1], time_s))
  {
    sce.Error("Unable to parse Final SimTime");
    return false;
  }

  // TODO: Do something with final sim time
  std::cout << "Final sim time (s): " << time_s << std::endl;

  return true;
}

bool GetPatient(const std::string& content, SEScenario& sce)
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
    sce.Warning("Unable to locate patient information");
    return false;
  }

  // Locate end of patient info
  std::string remainingText = mPatient.suffix().str();
  std::smatch mPatientEnd;
  std::regex rPatientEnd(R"(\}[\r\n]{3,})");
  std::string patientStr = "";
  if (std::regex_search(remainingText, mPatientEnd, rPatientEnd))
  {
    patientStr = text.substr(mPatient.position(1), mPatient.length(1) + mPatientEnd.position(0) + 1);
  }
  else
  {
    // Failed to find terminating bracket
    sce.Error("Unable to identify tag string terminator: " + std::string(mPatient[0]));
    return false;
  }

  // TODO: Do something with patient info
  std::cout << patientStr << std::endl;

  return true;
}

void GetSerializeFromFile(const std::string& content, SEScenario& sce)
{
  std::vector<std::string> filenames;

  // Capture groups:
  //  0: Whole match
  //  1: Time(unit)
  //  2: Remainder of line, left trimmed (file info) 
  std::string serializeFromFilePattern = R"(\[(\d*\.?\d*\(.*\))\][ \t]*\[SerializingFromFile\][ \t]*([^\n]*))";
  std::smatch mSerializeFromFile; 
  std::regex rSerializeFromFile(serializeFromFilePattern);
  std::string text = content;
  while (std::regex_search(text, mSerializeFromFile, rSerializeFromFile))
  {
    filenames.push_back(trim(std::string(mSerializeFromFile[2])));

    // TODO: Do something with serialize from file
    std::cout << "Serialize from file: " << filenames[filenames.size()-1] << std::endl;

    text = mSerializeFromFile.suffix();
  }
}

void GetSerializeFromString(const std::string& content, SEScenario& sce)
{
  // Capture groups:
  //  0: Whole match
  //  1: Time(unit)
  //  2: Remainder of line, left trimmed (currently empty)
  std::string serializeFromStringPattern = R"(\[(\d*\.?\d*\(.*\))\][ \t]*\[SerializingFromString\][ \t]*([^\n]*))";
  std::smatch mSerializeFromString; 
  std::regex rSerializeFromString(serializeFromStringPattern);
  std::string text = content;
  while (std::regex_search(text, mSerializeFromString, rSerializeFromString))
  {
    // TODO: Do something with serialize from file
    std::cout << "Serialize from string" << std::endl;

    text = mSerializeFromString.suffix();
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Generates previously executed scenario based on log file messages.
//--------------------------------------------------------------------------------------------------
bool SEScenarioUtils::GenerateScenarioFromLog(const std::string& filename, SEScenario& sce)
{
  std::string content;
  if (!ReadFile(filename, content))
  {
    sce.Error("Unable to read file " + filename);
    return false;
  }

  sce.Clear();

  if (!GetActions(content, sce))
  {
    sce.Error("Failed to retrieve scenario actions");
    return false;
  }

  if (!GetConditions(content, sce))
  {
    sce.Error("Failed to retrieve scenario conditions");
    return false;
  }

  if (!GetFinalSimTime(content, sce))
  {
    sce.Warning("Failed to retrieve final sim time");
  }

  if (!GetPatient(content, sce))
  {
    sce.Warning("Failed to retrieve patient information");
  }

  GetSerializeFromFile(content, sce);
  GetSerializeFromString(content, sce);

  return true;
}
