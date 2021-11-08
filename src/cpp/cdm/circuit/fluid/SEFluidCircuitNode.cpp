/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "circuit/fluid/SEFluidCircuitNode.h"
#include "circuit/fluid/SEFluidCircuitPath.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarVolume.h"

SEFluidCircuitNode::SEFluidCircuitNode(const std::string& name, Logger* logger) : SECircuitNode<FLUID_CIRCUIT_NODE>(name,logger)
{
  
}

SEFluidCircuitNode::~SEFluidCircuitNode()
{
  Clear();
}

void SEFluidCircuitNode::Clear()
{
  SECircuitNode::Clear();
}

bool SEFluidCircuitNode::HasPressure() const
{
  return HasPotential();
}
SEScalarPressure& SEFluidCircuitNode::GetPressure()
{
  return GetPotential();
}
double SEFluidCircuitNode::GetPressure(const PressureUnit& unit) const
{
  if (m_Potential == nullptr)
    return SEScalar::dNaN();
  return m_Potential->GetValue(unit);
}

bool SEFluidCircuitNode::HasNextPressure() const
{
  return HasNextPotential();
}
SEScalarPressure& SEFluidCircuitNode::GetNextPressure()
{
  return GetNextPotential();
}
double SEFluidCircuitNode::GetNextPressure(const PressureUnit& unit) const
{
  if (m_NextPotential == nullptr)
    return SEScalar::dNaN();
  return m_NextPotential->GetValue(unit);
}

bool SEFluidCircuitNode::HasVolume() const
{
  return HasQuantity();
}
SEScalarVolume& SEFluidCircuitNode::GetVolume()
{
  return GetQuantity();
}
double SEFluidCircuitNode::GetVolume(const VolumeUnit& unit) const
{
  if (m_Quantity == nullptr)
    return SEScalar::dNaN();
  return m_Quantity->GetValue(unit);
}

bool SEFluidCircuitNode::HasNextVolume() const
{
  return HasNextQuantity();
}
SEScalarVolume& SEFluidCircuitNode::GetNextVolume()
{
  return GetNextQuantity();
}
double SEFluidCircuitNode::GetNextVolume(const VolumeUnit& unit) const
{
  if (m_NextQuantity == nullptr)
    return SEScalar::dNaN();
  return m_NextQuantity->GetValue(unit);
}

bool SEFluidCircuitNode::HasVolumeBaseline() const
{
  return HasQuantityBaseline();
}
SEScalarVolume& SEFluidCircuitNode::GetVolumeBaseline()
{
  return GetQuantityBaseline();
}
double SEFluidCircuitNode::GetVolumeBaseline(const VolumeUnit& unit) const
{
  if (m_QuantityBaseline == nullptr)
    return SEScalar::dNaN();
  return m_QuantityBaseline->GetValue(unit);
}

SEFluidCircuitNode* SEFluidCircuitNode::GetBlackBoxSourceNode() const
{
  return m_BlackBoxSourceNode;
}
SEFluidCircuitNode* SEFluidCircuitNode::GetBlackBoxTargetNode() const
{
  return m_BlackBoxTargetNode;
}
void SEFluidCircuitNode::SetBlackBoxSourceTargetNodes(SEFluidCircuitNode& src, SEFluidCircuitNode& tgt)
{
  m_IsBlackBoxMiddle = true;
  m_BlackBoxSourceNode = &src;
  m_BlackBoxTargetNode = &tgt;
}

SEFluidCircuitPath* SEFluidCircuitNode::GetBlackBoxSourcePath() const
{
  return m_BlackBoxSourcePath;
}
SEFluidCircuitPath* SEFluidCircuitNode::GetBlackBoxTargetPath() const
{
  return m_BlackBoxTargetPath;
}
void SEFluidCircuitNode::SetBlackBoxSourceTargetPaths(SEFluidCircuitPath& src, SEFluidCircuitPath& tgt)
{
  m_IsBlackBoxMiddle = true;
  m_BlackBoxSourcePath = &src;
  m_BlackBoxTargetPath = &tgt;
}
