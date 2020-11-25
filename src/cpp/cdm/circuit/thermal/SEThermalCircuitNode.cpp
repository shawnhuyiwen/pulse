/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h" 
#include "circuit/thermal/SEThermalCircuitNode.h"
#include "properties/SEScalarEnergy.h"
#include "properties/SEScalarTemperature.h"

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
