/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h" 
#include "cdm/circuit/electrical/SEElectricalCircuitNode.h"
#include "cdm/properties/SEScalarElectricCharge.h"
#include "cdm/properties/SEScalarElectricPotential.h"

SEElectricalCircuitNode::SEElectricalCircuitNode(const std::string& name, Logger* logger) : SECircuitNode<ELECTRICAL_CIRCUIT_NODE>(name, logger)
{
  
}

SEElectricalCircuitNode::~SEElectricalCircuitNode()
{
  
}

void SEElectricalCircuitNode::Clear()
{
  SECircuitNode::Clear();
}

bool SEElectricalCircuitNode::HasVoltage() const
{
  return HasPotential();
}
SEScalarElectricPotential& SEElectricalCircuitNode::GetVoltage()
{
  return GetPotential();
}
double SEElectricalCircuitNode::GetVoltage(const ElectricPotentialUnit& unit) const
{
  if (m_Potential == nullptr)
    return SEScalar::dNaN();
  return m_Potential->GetValue(unit);
}

bool SEElectricalCircuitNode::HasNextVoltage() const
{
  return HasNextPotential();
}
SEScalarElectricPotential& SEElectricalCircuitNode::GetNextVoltage()
{
  return GetNextPotential();
}
double SEElectricalCircuitNode::GetNextVoltage(const ElectricPotentialUnit& unit) const
{
  if (m_NextPotential == nullptr)
    return SEScalar::dNaN();
  return m_NextPotential->GetValue(unit);
}

bool SEElectricalCircuitNode::HasCharge() const
{
  return HasQuantity();
}
SEScalarElectricCharge& SEElectricalCircuitNode::GetCharge()
{
  return GetQuantity();
}
double SEElectricalCircuitNode::GetCharge(const ElectricChargeUnit& unit) const
{
  if (m_Quantity == nullptr)
    return SEScalar::dNaN();
  return m_Quantity->GetValue(unit);
}

bool SEElectricalCircuitNode::HasNextCharge() const
{
  return HasNextQuantity();
}
SEScalarElectricCharge& SEElectricalCircuitNode::GetNextCharge()
{
  return GetNextQuantity();
}
double SEElectricalCircuitNode::GetNextCharge(const ElectricChargeUnit& unit) const
{
  if (m_NextQuantity == nullptr)
    return SEScalar::dNaN();
  return m_NextQuantity->GetValue(unit);
}

bool SEElectricalCircuitNode::HasChargeBaseline() const
{
  return HasQuantityBaseline();
}
SEScalarElectricCharge& SEElectricalCircuitNode::GetChargeBaseline()
{
  return GetQuantityBaseline();
}
double SEElectricalCircuitNode::GetChargeBaseline(const ElectricChargeUnit& unit) const
{
  if (m_QuantityBaseline == nullptr)
    return SEScalar::dNaN();
  return m_QuantityBaseline->GetValue(unit);
}

SEElectricalCircuitNode* SEElectricalCircuitNode::GetBlackBoxSourceNode() const
{
  return m_BlackBoxSourceNode;
}
SEElectricalCircuitNode* SEElectricalCircuitNode::GetBlackBoxTargetNode() const
{
  return m_BlackBoxTargetNode;
}
void SEElectricalCircuitNode::SetBlackBoxSourceTargetNodes(SEElectricalCircuitNode& src, SEElectricalCircuitNode& tgt)
{
  m_IsBlackBoxMiddle = true;
  m_BlackBoxSourceNode = &src;
  m_BlackBoxTargetNode = &tgt;
}

SEElectricalCircuitPath* SEElectricalCircuitNode::GetBlackBoxSourcePath() const
{
  return m_BlackBoxSourcePath;
}
SEElectricalCircuitPath* SEElectricalCircuitNode::GetBlackBoxTargetPath() const
{
  return m_BlackBoxTargetPath;
}
void SEElectricalCircuitNode::SetBlackBoxSourceTargetPaths(SEElectricalCircuitPath& src, SEElectricalCircuitPath& tgt)
{
  m_IsBlackBoxMiddle = true;
  m_BlackBoxSourcePath = &src;
  m_BlackBoxTargetPath = &tgt;
}
