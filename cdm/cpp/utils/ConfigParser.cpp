/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "CommonDataModel.h"
#include "ConfigParser.h"

#include <tuple>
#include <fstream>
#include <memory>

void ConfigSet::SetKeyValues(const std::map<std::string, std::string>& keyValues)
{
  m_keyValues = keyValues;
}

void ConfigSet::AddKeyValue(const std::string& key, const std::string& value)
{
  m_keyValues[key] = value;
}

bool ConfigSet::HasKey(const std::string& key) const
{
  return m_keyValues.find(key) != end(m_keyValues);
}

std::string ConfigSet::GetValue(const std::string& key) const
{
  if (!HasKey(key))
  {
    return "";
  }
  else
  {
    return m_keyValues.find(key)->second;
  }
}

ConfigParser::ConfigParser(const std::string& configFilePath)
{
  ParseConfigFile(configFilePath, true);
}
ConfigParser::ConfigParser(const std::string& configFilePath, bool splitGroups)
{
  ParseConfigFile(configFilePath, splitGroups);
}

void ConfigParser::ParseConfigFile(const std::string& verificationConfigFilePath, bool splitGroups)
{
  std::ifstream file(verificationConfigFilePath);

  std::string line;
  while (std::getline(file, line))
  {
    if (line.empty() || line[0] == '#')
    {
      continue;
    }

    if (splitGroups && line[0] == '@')
    {
      if (line.find("group") == 1)
      {
        m_parsingGlobalKeyValues = false;
        m_configSets.push_back(ConfigSet());
        m_configSets.back().SetKeyValues(m_globalKeyValues);
      }
    }
    else if (line.find("=") != std::string::npos)
    {
      std::string key;
      std::string value;

      std::tie(key, value) = ParseKeyValue(line);

      if (m_parsingGlobalKeyValues)
      {
        m_globalKeyValues[key] = value;
      }
      else
      {
        m_configSets.back().AddKeyValue(key, value);
      }
    }
  }
}

ConfigSet* ConfigParser::FileToConfigSet(const std::string& filename)
{
  ConfigParser config(filename, false);
  ConfigSet* cs = new ConfigSet();
  cs->SetKeyValues(config.m_globalKeyValues);
  return cs;
}

std::tuple<std::string, std::string> ConfigParser::ParseKeyValue(const std::string& line)
{
  return std::make_tuple(line.substr(0, line.find("=")), line.substr(line.find("=") + 1));
}