/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "substance/SESubstanceManager.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceCompound.h"
#include "dirent.h"
#include "utils/FileUtils.h"
#include "utils/unitconversion/UnitConversionEngine.h"

SESubstanceManager::SESubstanceManager(Logger* logger) : Loggable(logger)
{
  Clear();
}

SESubstanceManager::~SESubstanceManager()
{
  Clear();
}

void SESubstanceManager::Clear()
{
  DELETE_VECTOR(m_Substances);
  DELETE_VECTOR(m_Compounds);
  m_ActiveSubstances.clear();
  m_ActiveCompounds.clear();
  m_ActiveGases.clear();
  m_ActiveLiquids.clear();
  m_OriginalCompoundData.clear();
  m_OriginalSubstanceData.clear();
}

void SESubstanceManager::Reset()
{
  m_ActiveCompounds.clear();
  m_ActiveSubstances.clear();
  m_ActiveGases.clear();
  m_ActiveLiquids.clear();
  for (auto itr : m_OriginalSubstanceData)
    itr.first->SerializeFromString(itr.second, BINARY);
  for (auto itr : m_OriginalCompoundData)
    itr.first->SerializeFromString(itr.second, *this, BINARY);
}

/**
 * TRANSFERS OWNERSHIP TO THE MANAGER
 */
void SESubstanceManager::AddSubstance(SESubstance& substance) 
{
  SESubstance* s;
  for(unsigned int i=0; i<m_Substances.size(); i++)
  {
    s=m_Substances.at(i);
    if(s==&substance)
      return;
  }
  m_Substances.push_back(&substance);
}

SESubstance* SESubstanceManager::GetSubstance(const std::string& name) const
{
  SESubstance* s;
  for(unsigned int i=0; i<m_Substances.size(); i++)
  {
    s=m_Substances.at(i);
    if(name.compare(s->GetName())==0)
      return s;
  }
  return nullptr;
}

const std::vector<SESubstance*>& SESubstanceManager::GetSubstances() const
{
  return m_Substances;
}

bool SESubstanceManager::IsActive(const SESubstance& substance) const
{
  SESubstance* sub;
  for(unsigned int iSubstance=0; iSubstance < m_ActiveSubstances.size(); iSubstance++)
  {
    sub=m_ActiveSubstances.at(iSubstance); 
    if(sub==&substance)
      return true;
  }
  return false;
}

const std::vector<SESubstance*>& SESubstanceManager::GetActiveSubstances() const
{
  return m_ActiveSubstances;
}

void SESubstanceManager::AddActiveSubstance(SESubstance& substance) 
{
  if (IsActive(substance))
    return;
  if(substance.GetState()==eSubstance_State::Gas)
    m_ActiveGases.push_back(&substance);
  if(substance.GetState()== eSubstance_State::Liquid)
    m_ActiveLiquids.push_back(&substance);
    m_ActiveSubstances.push_back(&substance);
}

void SESubstanceManager::RemoveActiveSubstance(const SESubstance& substance)
{
  SESubstance* sub;
  for(unsigned int iSubstance=0; iSubstance < m_ActiveSubstances.size(); iSubstance++)
  {
    sub=m_ActiveSubstances.at(iSubstance); 
    if(sub==&substance)
    {
      m_ActiveSubstances.erase(m_ActiveSubstances.begin()+iSubstance);
      break;
    }
  }
  for(unsigned int iSubstance=0; iSubstance < m_ActiveGases.size(); iSubstance++)
  {
    sub=m_ActiveGases.at(iSubstance); 
    if(sub==&substance)
    {
      m_ActiveGases.erase(m_ActiveGases.begin()+iSubstance);
      break;
    }
  }
  for(unsigned int iSubstance=0; iSubstance < m_ActiveLiquids.size(); iSubstance++)
  {
    sub=m_ActiveLiquids.at(iSubstance); 
    if(sub==&substance)
    {
      m_ActiveLiquids.erase(m_ActiveLiquids.begin()+iSubstance);
      break;
    }
  }
}

void SESubstanceManager::RemoveActiveSubstances(const std::vector<SESubstance*>& substances)
{
  for(SESubstance* sub : substances)
    RemoveActiveSubstance(*sub);
}

void SESubstanceManager::RemoveActiveSubstances()
{
  std::vector<SESubstance*> copy(m_ActiveSubstances);
  for (SESubstance* sub : copy)
    RemoveActiveSubstance(*sub);
}


const std::vector<SESubstance*>& SESubstanceManager::GetActiveGases() const
{
  return m_ActiveGases;
}

const std::vector<SESubstance*>& SESubstanceManager::GetActiveLiquids() const
{
  return m_ActiveLiquids;
}

void SESubstanceManager::AddCompound(SESubstanceCompound& compound) 
{
  SESubstanceCompound* c;
  for(unsigned int i=0; i<m_Compounds.size(); i++)
  {
    c=m_Compounds.at(i);
    if(c==&compound)
      return;
  }
  m_Compounds.push_back(&compound);
}

SESubstanceCompound* SESubstanceManager::GetCompound(const std::string& name) const
{
  SESubstanceCompound* c;
  for(unsigned int i=0; i<m_Compounds.size(); i++)
  {
    c=m_Compounds.at(i);
    if(name.compare(c->GetName())==0)
      return c;
  }
  return nullptr;
}

const std::vector<SESubstanceCompound*>& SESubstanceManager::GetCompounds() const
{
  return m_Compounds;
}

bool SESubstanceManager::IsActive(const SESubstanceCompound& compound) const
{
  SESubstanceCompound* c;
  for(unsigned int i=0; i < m_ActiveCompounds.size(); i++)
  {
    c=m_ActiveCompounds.at(i); 
    if(c==&compound)
      return true;
  }
  return false;
}

const std::vector<SESubstanceCompound*>& SESubstanceManager::GetActiveCompounds() const
{
  return m_ActiveCompounds;
}

void SESubstanceManager::AddActiveCompound(SESubstanceCompound& compound) 
{
  SESubstanceCompound* c;
  for(unsigned int i=0; i < m_ActiveCompounds.size(); i++)
  {
    c=m_ActiveCompounds.at(i); 
    if(c==&compound)
      return;
  }
    m_ActiveCompounds.push_back(&compound);
}

void SESubstanceManager::RemoveActiveCompound(SESubstanceCompound& compound)
{
  unsigned int i = 0;
  for (SESubstanceCompound* c : m_ActiveCompounds)
  {
    if(c==&compound)
    {
      m_ActiveCompounds.erase(m_ActiveCompounds.begin()+i);
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

bool SESubstanceManager::LoadSubstanceDirectory()
{
  bool succeed = true;
  Clear();
  std::stringstream ss;
  DIR *sdir;
  DIR *cdir;
  struct dirent *ent;

  std::string workingDirectory = GetCurrentWorkingDirectory();

#if defined(_WIN32)
  sdir = opendir("./substances/");
  cdir = opendir("./substances/compounds");
#else
  sdir = opendir(std::string(workingDirectory + std::string("/substances/")).c_str());
  cdir = opendir(std::string(workingDirectory + std::string("/substances/compounds/")).c_str());
#endif

  if (sdir != nullptr)
  {
    while ((ent = readdir(sdir)) != nullptr)
    {
      ss.str("");
      ss << workingDirectory << "/substances/" << ent->d_name;
      if (!IsDirectory(ent) && strlen(ent->d_name) > 2)
      {
        try
        {
          SESubstance* sub = new SESubstance(GetLogger());
          if (!sub->SerializeFromFile(ss.str(), ASCII))
          {
            delete sub;
            Error("Unable to read substance " + ss.str());
            continue;
          }
          else
          {
            std::string binary;
            if (!sub->SerializeToString(binary, BINARY))
            {
              delete sub;
              Error("Unable to serialize substance " + ss.str());
              continue;
            }
            m_OriginalSubstanceData[sub] = binary;
            AddSubstance(*sub);
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
      ss << workingDirectory << "/substances/compounds/" << ent->d_name;
      if (!IsDirectory(ent) && strlen(ent->d_name) > 2)
      {
        try
        {
          SESubstanceCompound* cmpd = new SESubstanceCompound(GetLogger());
          if (!cmpd->SerializeFromFile(ss.str(), *this, ASCII))
          {
            delete cmpd;
            Error("Unable to read substance compound " + ss.str());
            continue;
          }
          else
          {
            std::string binary;
            if (!cmpd->SerializeToString(binary, BINARY))
            {
              delete cmpd;
              Error("Unable to serialize substance compound " + ss.str());
              continue;
            }
            m_OriginalCompoundData[cmpd] = binary;
            AddCompound(*cmpd);
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
