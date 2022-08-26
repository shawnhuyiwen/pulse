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

std::string actionBeginPattern = R"(\[((\d*\.?\d*)\((\w)\))\][ \t]*\[Action\][ \t]*(?:\1,[ \t]*)*([\w \t]*\w)(?:(?:[ \t]*(:)[ \t]*([\w \t]*\w)*)*)[ \t]*(\{)?)";
struct ScenarioAction
{
  std::string Name;
  std::string Type = "";
  double ScenarioTime_s;
  SEScalarProperties Properties;
  std::map<std::string, std::string> Enumerations;
};

bool ParseLegacyLogAction(const std::string& allLines, std::vector<ScenarioAction>& actions, Logger* logger)
{
  ScenarioAction a;

  std::smatch mAction; 
  std::regex rAction(actionBeginPattern);

  // Capture groups:
  //  0: Whole match
  //  1: Time(Time unit) 
  //  2: Time
  //  3: Time unit
  //  4: Action Type/Action Name for type-less legacy actions
  //  5: Colon indicating legacy action
  //  6: Action name for legacy actions with type
  //  7: Bracket indicating non-legacy action
  if (!std::regex_search(allLines, mAction, rAction))
  {
    logger->Error("Unable to parse action");
    return false;
  }
  
  // Get scenario time
  double time = std::stod(mAction[2]);
  if (mAction[3].compare("s") != 0)
  {
    // TODO: Handle other time units
  }
  a.ScenarioTime_s = time;

  // Get action name and type if it exists
  if (mAction[5].compare(":") == 0)
  {
    a.Type = mAction[4];
    a.Name = mAction[6];
  }
  else
  {
    a.Name = mAction[4];
  }

  // Extract properties from remaining lines
  std::string properties = mAction.suffix().str();
  std::string propertyPattern = R"(    (\w+):\s*([a-zA-Z]*)?([\d*\.?\d]*)?(?:\((.*)\))?)";
  std::smatch mProperty; 
  std::regex rProperty(propertyPattern);
  // Capture groups:
  //  0: Whole match
  //  1: Property name
  //  2: NaN or Enumeration Value (note this will currently not get whole enum value if it has non a-zA-Z characters)
  //  3: Numeric value
  //  4: Unit
  while (std::regex_search(properties, mProperty, rProperty))
  {
    std::string propName = mProperty[1];

    double value;
    std::string valueStr;
    if (mProperty[2].compare("NaN") == 0)
    {
      value = std::stod(mProperty[2]);
      a.Properties[propName] = SEScalarPair(value);
    }
    else if (mProperty[3].compare("") != 0) // value(unit) or value
    {
      value = std::stod(mProperty[3]);
      if (mProperty[4].compare("") != 0)
      {
        a.Properties[propName] = SEScalarPair(value, mProperty[4]);
      }
      else
      {
        a.Properties[propName] = SEScalarPair(value);
      }
    }
    else //Enumeration
    {
      a.Enumerations[propName] = mProperty[2];
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

void TestAction(SEAction& a)
{
  std::string s = a.ToString();

  std::cout << "Adding action: " << a << std::endl;
  //SEAction* copy = SEAction::Serialize(s);
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
  std::vector<ScenarioAction> actions;

  // Identify action strings
  std::smatch mActionBegin; 
  std::regex rActionBegin(actionBeginPattern);
  std::string text = content;
  while (std::regex_search(text, mActionBegin, rActionBegin))
  {
    // Capture groups:
    //  0: Whole match
    //  1: Time(Time unit) 
    //  2: Time
    //  3: Time unit
    //  4: Action Type/Action Name for type-less legacy actions
    //  5: Colon indicating legacy action
    //  6: Action name for legacy actions with type
    //  7: Bracket indicating non-legacy action
    std::string remainingText = mActionBegin.suffix().str();
    std::string actionStr = "";
    if (mActionBegin[7].compare("{") == 0)
    {
      // Find end of action
      std::smatch mActionEnd;
      std::regex rActionEnd(R"(\}\n+.*:)");
      if (std::regex_search(remainingText, mActionEnd, rActionEnd))
      {
        actionStr = text.substr(mActionBegin.position(0), mActionBegin.length(0) + mActionEnd.position(0));
      }
      else
      {
        sce.Error("Unable to identify action terminator: " + std::string(mActionBegin[0]));
        return false;
      }

      // TODO: Parse action string

    }
    else // Legacy log
    {
      actionStr += mActionBegin[0];

      // Collect action properties
      std::smatch mActionProperties;
      std::regex rActionProperties(R"(\n    .*)");
      while (std::regex_search(remainingText, mActionProperties, rActionProperties))
      {
        if (mActionProperties.position(0) != 1) // Needs to immediately follow previous match
          break;
        actionStr += mActionProperties[0];
        remainingText = mActionProperties.suffix().str();
      }

      if (!ParseLegacyLogAction(actionStr, actions, sce.GetLogger()))
      {
        sce.Error("Unable to parse action: " + actionStr);
        return false;
      }
    }

    text = mActionBegin.suffix().str();
  }

  //Create SEActions from actions
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
    AddSEAction(sa,sce);
  }

  return true;
}
