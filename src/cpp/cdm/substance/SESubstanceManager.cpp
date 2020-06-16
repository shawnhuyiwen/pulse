/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "substance/SESubstanceManager.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceCompound.h"
#include "dirent.h"
#include "utils/FileUtils.h"
#include "utils/unitconversion/UnitConversionEngine.h"
#include <cstring>

SESubstanceManager::SESubstanceManager(Logger* logger) : Loggable(logger)
{
  Clear();
}

SESubstanceManager::~SESubstanceManager()
{
  Clear();
  DELETE_VECTOR(m_Substances);
  DELETE_VECTOR(m_Compounds);
}

void SESubstanceManager::Clear()
{
  m_ActiveSubstances.clear();
  m_cActiveSubstances.clear();
  m_ActiveCompounds.clear();
  m_cActiveCompounds.clear();
  m_ActiveGases.clear();
  m_cActiveGases.clear();
  m_ActiveLiquids.clear();
  m_cActiveLiquids.clear();

  std::string name;
  for (SESubstance* sub : m_Substances)
    sub->Clear();
  for (SESubstanceCompound* cmpd : m_Compounds)
    cmpd->Clear();
}

bool SESubstanceManager::HasSubstance(const std::string& name) const
{
  for (SESubstance* s : m_Substances)
  {
    if (name.compare(s->GetName()) == 0)
      true;
  }
  return false;
}

SESubstance* SESubstanceManager::GetSubstance(const std::string& name)
{
  for(SESubstance* s : m_Substances)
  {
    if(s->GetName() == name)
      return s;
  }
  SESubstance* s = new SESubstance(name, GetLogger());
  m_Substances.push_back(s);
  m_cSubstances.push_back(s);
  return s;
}
const SESubstance* SESubstanceManager::GetSubstance(const std::string& name) const
{
  for (SESubstance* s : m_Substances)
  {
    if(name.compare(s->GetName())==0)
      return s;
  }
  return nullptr;
}

const std::vector<SESubstance*>& SESubstanceManager::GetSubstances()
{
  return m_Substances;
}
const std::vector<const SESubstance*>& SESubstanceManager::GetSubstances() const
{
  return m_cSubstances;
}

bool SESubstanceManager::IsActive(const SESubstance& substance) const
{
  for (SESubstance* s : m_ActiveSubstances)
  {
    if(s==&substance)
      return true;
  }
  return false;
}

const std::vector<SESubstance*>& SESubstanceManager::GetActiveSubstances()
{
  return m_ActiveSubstances;
}
const std::vector<const SESubstance*>& SESubstanceManager::GetActiveSubstances() const
{
  return m_cActiveSubstances;
}

void SESubstanceManager::AddActiveSubstance(const SESubstance& substance) 
{
  if (IsActive(substance))
    return;
  if (substance.GetState() == eSubstance_State::Gas)
  {
    m_ActiveGases.push_back(&(SESubstance&)substance);
    m_cActiveGases.push_back(&substance);
  }
  if (substance.GetState() == eSubstance_State::Liquid)
  {
    m_ActiveLiquids.push_back(&(SESubstance&)substance);
    m_cActiveLiquids.push_back(&substance);
  }
  m_ActiveSubstances.push_back(&(SESubstance&)substance);
  m_cActiveSubstances.push_back(&substance);
}

void SESubstanceManager::RemoveActiveSubstance(const SESubstance& substance)
{
  const SESubstance* sub;
  for(unsigned int iSubstance=0; iSubstance < m_ActiveSubstances.size(); iSubstance++)
  {
    sub=m_ActiveSubstances.at(iSubstance);
    if(sub==&substance)
    {
      m_ActiveSubstances.erase(m_ActiveSubstances.begin()+iSubstance);
      m_cActiveSubstances.erase(m_cActiveSubstances.begin()+iSubstance);
      break;
    }
  }
  for(unsigned int iSubstance=0; iSubstance < m_ActiveGases.size(); iSubstance++)
  {
    sub=m_ActiveGases.at(iSubstance); 
    if(sub==&substance)
    {
      m_ActiveGases.erase(m_ActiveGases.begin()+iSubstance);
      m_cActiveGases.erase(m_cActiveGases.begin()+iSubstance);
      break;
    }
  }
  for(unsigned int iSubstance=0; iSubstance < m_ActiveLiquids.size(); iSubstance++)
  {
    sub=m_ActiveLiquids.at(iSubstance); 
    if(sub==&substance)
    {
      m_ActiveLiquids.erase(m_ActiveLiquids.begin()+iSubstance);
      m_cActiveLiquids.erase(m_cActiveLiquids.begin()+iSubstance);
      break;
    }
  }
}

void SESubstanceManager::RemoveActiveSubstances(const std::vector<SESubstance*>& substances)
{
  for(SESubstance* sub : m_ActiveSubstances)
    RemoveActiveSubstance(*sub);
}

void SESubstanceManager::RemoveActiveSubstances()
{
  std::vector<const SESubstance*> copy(m_cActiveSubstances);
  for (const SESubstance* sub : copy)
    RemoveActiveSubstance(*sub);
}

const std::vector<SESubstance*>& SESubstanceManager::GetActiveGases()
{
  return m_ActiveGases;
}
const std::vector<const SESubstance*>& SESubstanceManager::GetActiveGases() const
{
  return m_cActiveGases;
}

const std::vector<SESubstance*>& SESubstanceManager::GetActiveLiquids()
{
  return m_ActiveLiquids;
}
const std::vector<const SESubstance*>& SESubstanceManager::GetActiveLiquids() const
{
  return m_cActiveLiquids;
}

bool SESubstanceManager::HasCompound(const std::string& name) const
{
  for (SESubstanceCompound* c : m_Compounds)
  {
    if (c->GetName() == name)
      return true;
  }
  return false;
}

SESubstanceCompound* SESubstanceManager::GetCompound(const std::string& name)
{
  for(SESubstanceCompound* c : m_Compounds)
  {
    if(c->GetName() == name)
      return c;
  }
  SESubstanceCompound* c = new SESubstanceCompound(name, GetLogger());
  m_Compounds.push_back(c);
  m_cCompounds.push_back(c);
  return c;
}
const SESubstanceCompound* SESubstanceManager::GetCompound(const std::string& name) const
{
  for (SESubstanceCompound* c : m_Compounds)
  {
    if (c->GetName() == name)
      return c;
  }
  return nullptr;
}

const std::vector<SESubstanceCompound*>& SESubstanceManager::GetCompounds()
{
  return m_Compounds;
}
const std::vector<const SESubstanceCompound*>& SESubstanceManager::GetCompounds() const
{
  return m_cCompounds;
}

bool SESubstanceManager::IsActive(const SESubstanceCompound& compound) const
{
  for (SESubstanceCompound* c : m_ActiveCompounds)
  {
    if(c==&compound)
      return true;
  }
  return false;
}

const std::vector<SESubstanceCompound*>& SESubstanceManager::GetActiveCompounds()
{
  return m_ActiveCompounds;
}
const std::vector<const SESubstanceCompound*>& SESubstanceManager::GetActiveCompounds() const
{
  return m_cActiveCompounds;
}

void SESubstanceManager::AddActiveCompound(const SESubstanceCompound& compound) 
{
  for (SESubstanceCompound* c : m_ActiveCompounds)
  {
    if(c==&compound)
      return;
  }
  m_ActiveCompounds.push_back(&(SESubstanceCompound&)compound);
  m_cActiveCompounds.push_back(&compound);
}

void SESubstanceManager::RemoveActiveCompound(const SESubstanceCompound& compound)
{
  const SESubstanceCompound* c;
  for (unsigned int i = 0; i < m_ActiveCompounds.size(); i++)
  {
    c = m_ActiveCompounds.at(i);
    if(c==&compound)
    {
      m_ActiveCompounds.erase(m_ActiveCompounds.begin()+i);
      m_cActiveCompounds.erase(m_cActiveCompounds.begin()+i);
      break;
    }
    i++;
  }
}

void SESubstanceManager::RemoveActiveCompounds(const std::vector<SESubstanceCompound*>& compounds)
{
  for (SESubstanceCompound* c : compounds)
    RemoveActiveCompound(*c);
}

bool SESubstanceManager::LoadSubstanceDirectory(const std::string& data_dir)
{
  bool succeed = true;
  Clear();
  std::stringstream ss;
  DIR *sdir;
  DIR *cdir;
  struct dirent *ent;
  std::string ext = ".json";

#if defined(_WIN32)
  sdir = opendir(std::string(data_dir + "/substances/").c_str());
  cdir = opendir(std::string(data_dir + "/substances/compounds/").c_str());
#else
  // Find the absolute dir?
  sdir = opendir(std::string(data_dir + "/substances/").c_str());
  cdir = opendir(std::string(data_dir + "/substances/compounds/").c_str());
#endif

  if (sdir != nullptr)
  {
    while ((ent = readdir(sdir)) != nullptr)
    {
      ss.str("");
      ss << data_dir << "/substances/" << ent->d_name;
      if (!IsDirectory(ent) && strlen(ent->d_name) > 2 && ss.str().find_last_of(ext) == (ss.str().length()-1))
      {
        try
        {

          // I am assuming the filename is also the substance name
          // If we don't want to make that assumption, we need to have
          // a map of filename to substance ptr and use it rather than GetSubstance by name.
          std::string name(ent->d_name);
          name=name.substr(0, name.size()-ext.size());
          SESubstance* sub = GetSubstance(name);
          if (!sub->SerializeFromFile(ss.str(), JSON))
          {
            Error("Unable to read substance " + ss.str());
            continue;
          }
        }
        catch (...)
        {
          Info("I caught something in " + ss.str());
          continue;
        }
      }
    }
  }
  closedir(sdir);

  if (cdir != nullptr)
  {
    while ((ent = readdir(cdir)) != nullptr)
    {
      ss.str("");
      ss << data_dir << "/substances/compounds/" << ent->d_name;
      if (!IsDirectory(ent) && strlen(ent->d_name) > 2 && ss.str().find_last_of(ext) == (ss.str().length()-1))
      {
        try
        {
          // I am assuming the filename is also the substance name
          // If we don't want to make that assumption, we need to have
          // a map of filename to substance ptr and use it rather than GetSubstance by name.
          SESubstanceCompound* cmpd = GetCompound(ent->d_name);
          if (!cmpd->SerializeFromFile(ss.str(), *this, JSON))
          {
            Error("Unable to read substance compound " + ss.str());
            continue;
          }
        }
        catch (...)
        {
          Info("I caught something in " + ss.str());
          continue;
        }
      }
    }
  }
  closedir(cdir);
  return succeed;
}
