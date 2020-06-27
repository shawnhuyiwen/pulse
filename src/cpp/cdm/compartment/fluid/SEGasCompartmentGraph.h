/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "compartment/fluid/SEGasCompartment.h"
#include "compartment/fluid/SEGasCompartmentLink.h"
#include "compartment/SECompartmentTransportGraph.h"
class SECompartmentManager;

class CDM_DECL SEGasCompartmentGraph : public SECompartmentTransportGraph<SEGasTransportGraph, SEGasTransportVertex, SEGasTransportEdge, SEGasCompartment, SEGasCompartmentLink>
{
  friend class PBCompartment;//friend the serialization class
  friend class SECompartmentManager;
protected:
  SEGasCompartmentGraph(const std::string& name, Logger* logger) : SECompartmentTransportGraph(name, logger) {};
public:  
  virtual ~SEGasCompartmentGraph() {}

  void AddGraph(SEGasCompartmentGraph& graph);

protected:
  void BalanceByIntensive();
};
