/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/circuit/SECircuit.h"
#include "cdm/circuit/thermal/SEThermalCircuitNode.h"
#include "cdm/circuit/thermal/SEThermalCircuitPath.h"

class SECircuitManager;

class CDM_DECL SEThermalCircuit : public SECircuit<SEThermalCircuitNode,SEThermalCircuitPath>
{
  friend class PBCircuit;//friend the serialization class
  friend class SECircuitManager;
protected:
  SEThermalCircuit(const std::string& name, SECircuitManager& mgr);
public:
  virtual ~SEThermalCircuit();

  SEThermalCircuitNode& CreateNode(const std::string& name);
  SEThermalCircuitPath& CreatePath(SEThermalCircuitNode& src, SEThermalCircuitNode& tgt, const std::string& name);

  void AddCircuit(SEThermalCircuit& circuit);

protected:
  SECircuitManager& m_Mgr;
};
