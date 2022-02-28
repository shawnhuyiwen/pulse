/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h" 
#include "cdm/circuit/thermal/SEThermalCircuitNode.h"
#include "cdm/properties/SEScalarEnergy.h"
#include "cdm/properties/SEScalarTemperature.h"

SEThermalCircuitNode::SEThermalCircuitNode(const std::string& name, Logger* logger) : SECircuitNode<THERMAL_CIRCUIT_NODE>(name,logger)
{
  
}

SEThermalCircuitNode::~SEThermalCircuitNode()
{
  Clear();
}

void SEThermalCircuitNode::Clear()
{
  SECircuitNode::Clear();
}


bool SEThermalCircuitNode::HasTemperature() const
{
  return HasPotential();
}
SEScalarTemperature& SEThermalCircuitNode::GetTemperature()
{
  return GetPotential();
}
double SEThermalCircuitNode::GetTemperature(const TemperatureUnit& unit) const
{
  if (m_Potential == nullptr)
    return SEScalar::dNaN();
  return m_Potential->GetValue(unit);
}

bool SEThermalCircuitNode::HasNextTemperature() const
{
  return HasNextPotential();
}
SEScalarTemperature& SEThermalCircuitNode::GetNextTemperature()
{
  return GetNextPotential();
}
double SEThermalCircuitNode::GetNextTemperature(const TemperatureUnit& unit) const
{
  if (m_NextPotential == nullptr)
    return SEScalar::dNaN();
  return m_NextPotential->GetValue(unit);
}

bool SEThermalCircuitNode::HasHeat() const
{
  return HasQuantity();
}
SEScalarEnergy& SEThermalCircuitNode::GetHeat()
{
  return GetQuantity();
}
double SEThermalCircuitNode::GetHeat(const EnergyUnit& unit) const
{
  if (m_Quantity == nullptr)
    return SEScalar::dNaN();
  return m_Quantity->GetValue(unit);
}

bool SEThermalCircuitNode::HasNextHeat() const
{
  return HasNextQuantity();
}
SEScalarEnergy& SEThermalCircuitNode::GetNextHeat()
{
  return GetNextQuantity();
}
double SEThermalCircuitNode::GetNextHeat(const EnergyUnit& unit) const
{
  if (m_NextQuantity == nullptr)
    return SEScalar::dNaN();
  return m_NextQuantity->GetValue(unit);
}

bool SEThermalCircuitNode::HasHeatBaseline() const
{
  return HasQuantityBaseline();
}
SEScalarEnergy& SEThermalCircuitNode::GetHeatBaseline()
{
  return GetQuantityBaseline();
}
double SEThermalCircuitNode::GetHeatBaseline(const EnergyUnit& unit) const
{
  if (m_QuantityBaseline == nullptr)
    return SEScalar::dNaN();
  return m_QuantityBaseline->GetValue(unit);
}

SEThermalCircuitNode* SEThermalCircuitNode::GetBlackBoxSourceNode() const
{
  return m_BlackBoxSourceNode;
}
SEThermalCircuitNode* SEThermalCircuitNode::GetBlackBoxTargetNode() const
{
  return m_BlackBoxTargetNode;
}
void SEThermalCircuitNode::SetBlackBoxSourceTargetNodes(SEThermalCircuitNode& src, SEThermalCircuitNode& tgt)
{
  m_IsBlackBoxMiddle = true;
  m_BlackBoxSourceNode = &src;
  m_BlackBoxTargetNode = &tgt;
}

SEThermalCircuitPath* SEThermalCircuitNode::GetBlackBoxSourcePath() const
{
  return m_BlackBoxSourcePath;
}
SEThermalCircuitPath* SEThermalCircuitNode::GetBlackBoxTargetPath() const
{
  return m_BlackBoxTargetPath;
}
void SEThermalCircuitNode::SetBlackBoxSourceTargetPaths(SEThermalCircuitPath& src, SEThermalCircuitPath& tgt)
{
  m_IsBlackBoxMiddle = true;
  m_BlackBoxSourcePath = &src;
  m_BlackBoxTargetPath = &tgt;
}
