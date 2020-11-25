/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "blackbox/SEBlackBoxManager.h"

#include "compartment/electrical/SEElectricalCompartment.h"
#include "compartment/electrical/SEElectricalCompartmentLink.h"
#include "compartment/fluid/SEGasCompartment.h"
#include "compartment/fluid/SEGasCompartmentLink.h"
#include "compartment/fluid/SELiquidCompartment.h"
#include "compartment/fluid/SELiquidCompartmentLink.h"
#include "compartment/thermal/SEThermalCompartment.h"
#include "compartment/thermal/SEThermalCompartmentLink.h"

#include "circuit/electrical/SEElectricalCircuitPath.h"
#include "circuit/fluid/SEFluidCircuitPath.h"
#include "circuit/thermal/SEThermalCircuitPath.h"

SEBlackBoxManager::SEBlackBoxManager(Logger* logger) : Loggable(logger)
{

}
SEBlackBoxManager::~SEBlackBoxManager()
{
  DELETE_MAP_SECOND(m_ElectricalBoxes);
  DELETE_MAP_SECOND(m_GasBoxes);
  DELETE_MAP_SECOND(m_LiquidBoxes);
  DELETE_MAP_SECOND(m_ThermalBoxes);
}

std::string SEBlackBoxManager::GetBlackBoxName(const std::string& srcCmptName, const std::string& tgtCmptName) const
{
  std::string name = srcCmptName + "_to_" + tgtCmptName;
  return name;
}

bool SEBlackBoxManager::HasElectricalBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName) const
{
  auto bb = m_ElectricalBoxes.find(GetBlackBoxName(srcCmptName, tgtCmptName));
  return bb != m_ElectricalBoxes.end();
}
SEElectricalBlackBox* SEBlackBoxManager::GetElectricalBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName)
{
  auto bb = m_ElectricalBoxes.find(GetBlackBoxName(srcCmptName, tgtCmptName));
  if (bb != m_ElectricalBoxes.end())
    return bb->second;
  return CreateElectricalBlackBox(srcCmptName, tgtCmptName);
}
SEElectricalBlackBox* SEBlackBoxManager::CreateElectricalBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName)
{
  return CreateBlackBox<SEElectricalBlackBox>(GetBlackBoxName(srcCmptName, tgtCmptName), m_ElectricalBoxes);
}

bool SEBlackBoxManager::HasGasBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName) const
{
  auto bb = m_GasBoxes.find(GetBlackBoxName(srcCmptName, tgtCmptName));
  return bb != m_GasBoxes.end();
}
SEGasBlackBox* SEBlackBoxManager::GetGasBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName)
{
  auto bb = m_GasBoxes.find(GetBlackBoxName(srcCmptName, tgtCmptName));
  if (bb != m_GasBoxes.end())
    return bb->second;
  return CreateGasBlackBox(srcCmptName, tgtCmptName);
}
SEGasBlackBox* SEBlackBoxManager::CreateGasBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName)
{
  return CreateBlackBox<SEGasBlackBox>(GetBlackBoxName(srcCmptName, tgtCmptName), m_GasBoxes);
}

bool SEBlackBoxManager::HasLiquidBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName) const
{
  auto bb = m_LiquidBoxes.find(GetBlackBoxName(srcCmptName, tgtCmptName));
  return bb != m_LiquidBoxes.end();
}
SELiquidBlackBox* SEBlackBoxManager::GetLiquidBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName)
{
  auto bb = m_LiquidBoxes.find(GetBlackBoxName(srcCmptName, tgtCmptName));
  if (bb != m_LiquidBoxes.end())
    return bb->second;
  return CreateLiquidBlackBox(srcCmptName, tgtCmptName);
}
SELiquidBlackBox* SEBlackBoxManager::CreateLiquidBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName)
{
  return CreateBlackBox<SELiquidBlackBox>(GetBlackBoxName(srcCmptName, tgtCmptName), m_LiquidBoxes);
}

bool SEBlackBoxManager::HasThermalBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName) const
{
  auto bb = m_ThermalBoxes.find(GetBlackBoxName(srcCmptName, tgtCmptName));
  return bb != m_ThermalBoxes.end();
}
SEThermalBlackBox* SEBlackBoxManager::GetThermalBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName)
{
  auto bb = m_ThermalBoxes.find(GetBlackBoxName(srcCmptName, tgtCmptName));
  if (bb != m_ThermalBoxes.end())
    return bb->second;
  return CreateThermalBlackBox(srcCmptName, tgtCmptName);
}
SEThermalBlackBox* SEBlackBoxManager::CreateThermalBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName)
{
  return CreateBlackBox<SEThermalBlackBox>(GetBlackBoxName(srcCmptName,tgtCmptName), m_ThermalBoxes);
}

template<typename BlackBoxType>
BlackBoxType* SEBlackBoxManager::CreateBlackBox(const std::string& name, std::map<std::string, BlackBoxType*>& ledger)
{
  BlackBoxType* bb = ledger[name];
  if (bb != nullptr)
  {
    Error("Black box already exists with name " + name);
    return nullptr;
  }
  bb = new BlackBoxType(name, GetLogger());
  ledger[name] = bb;

  return bb;
}
