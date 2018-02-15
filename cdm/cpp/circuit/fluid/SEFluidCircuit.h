/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "circuit/SECircuit.h"
#include "circuit/fluid/SEFluidCircuitNode.h"
#include "circuit/fluid/SEFluidCircuitPath.h"
#include "properties/SEScalarFlowCompliance.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalarFlowInertance.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarFlowResistance.h"
CDM_BIND_DECL(FluidCircuitData)

class SECircuitManager;

class CDM_DECL SEFluidCircuit : public SECircuit<cdm::FluidCircuitData, SEFluidCircuitNode, cdm::FluidCircuitNodeData, SEFluidCircuitPath, cdm::FluidCircuitPathData>
{
  friend class SECircuitManager;
protected:
  SEFluidCircuit(const std::string& name, SECircuitManager& mgr);
public:
  virtual ~SEFluidCircuit();

  SEFluidCircuitNode& CreateNode(const std::string& name);
  SEFluidCircuitPath& CreatePath(SEFluidCircuitNode& src, SEFluidCircuitNode& tgt, const std::string& name);

  void AddCircuit(SEFluidCircuit& circuit);

protected:
  SECircuitManager& m_Mgr;
};