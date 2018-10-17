/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "compartment/fluid/SELiquidCompartment.h"
#include "compartment/fluid/SELiquidCompartmentLink.h"
#include "compartment/SECompartmentTransportGraph.h"

class SECompartmentManager;

class CDM_DECL SELiquidCompartmentGraph : public SECompartmentTransportGraph<SELiquidTransportGraph, SELiquidTransportVertex, SELiquidTransportEdge, SELiquidCompartment, SELiquidCompartmentLink>
{
  friend class PBCompartment;//friend the serialization class
  friend class SECompartmentManager;
protected:
  SELiquidCompartmentGraph(const std::string& name, Logger* logger) : SECompartmentTransportGraph(name, logger) {};
public:  
  virtual ~SELiquidCompartmentGraph() {}

  void AddGraph(SELiquidCompartmentGraph& graph);

protected:
  void BalanceByIntensive();
};