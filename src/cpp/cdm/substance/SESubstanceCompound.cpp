/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "substance/SESubstanceCompound.h"
#include "substance/SESubstanceConcentration.h"
#include "substance/SESubstanceManager.h"
#include "properties/SEScalarMassPerVolume.h"
#include "io/protobuf/PBSubstance.h"

SESubstanceCompound::SESubstanceCompound(const std::string& name, Logger* logger) : Loggable(logger)
{
  m_Name = name;
}

SESubstanceCompound::~SESubstanceCompound()
{
  DELETE_VECTOR(m_Components);
  m_cComponents.clear();
}

void SESubstanceCompound::Clear()
{
  for (SESubstanceConcentration* sq : m_Components)
    sq->Clear();
}

bool SESubstanceCompound::SerializeToString(std::string& output, SerializationFormat m) const
{
  return PBSubstance::SerializeToString(*this, output, m);
}
bool SESubstanceCompound::SerializeToFile(const std::string& filename) const
{
  return PBSubstance::SerializeToFile(*this, filename);
}
bool SESubstanceCompound::SerializeFromString(const std::string& src, const SESubstanceManager& subMgr, SerializationFormat m)
{
  return PBSubstance::SerializeFromString(src, *this, subMgr, m);
}
bool SESubstanceCompound::SerializeFromFile(const std::string& filename, const SESubstanceManager& subMgr)
{
  return PBSubstance::SerializeFromFile(filename, *this, subMgr);
}

std::string SESubstanceCompound::GetName() const
{
  return m_Name;
}

bool SESubstanceCompound::HasComponent() const
{
  return m_Components.size()==0?false:true;
}
bool SESubstanceCompound::HasComponent(const SESubstance& substance) const
{
  for (SESubstanceConcentration* q : m_Components)
  {
    if(&substance==&q->GetSubstance())
      return true;
  }
  return false;
}
const std::vector<SESubstanceConcentration*>& SESubstanceCompound::GetComponents()
{
  return m_Components;
}
const std::vector<const SESubstanceConcentration*>& SESubstanceCompound::GetComponents() const
{
  return m_cComponents;
}
SESubstanceConcentration& SESubstanceCompound::GetComponent(const SESubstance& substance)
{
  for (SESubstanceConcentration* sq : m_Components)
  {
    if(&substance==&sq->GetSubstance())
      return *sq;
  }
  SESubstanceConcentration* sq = new SESubstanceConcentration(substance);
  sq->GetConcentration().SetValue(0,MassPerVolumeUnit::ug_Per_mL);
  m_Components.push_back(sq);
  m_cComponents.push_back(sq);
  return *sq;
}
const SESubstanceConcentration* SESubstanceCompound::GetComponent(const SESubstance& substance) const
{
  for (SESubstanceConcentration* sq : m_Components)
  {
    if (&substance == &sq->GetSubstance())
      return sq;
  }
  return nullptr;
}
void SESubstanceCompound::RemoveComponent(const SESubstance& substance)
{
  unsigned int i = 0;
  for (SESubstanceConcentration* sq : m_Components)
  {
    if(&substance==&sq->GetSubstance())
    {
      m_Components.erase(m_Components.begin() + i);
      m_cComponents.erase(m_cComponents.begin() + i);
      delete sq;
      break;
    }
    i++;
  }
}

