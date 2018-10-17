/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "compartment/fluid/SELiquidCompartmentGraph.h"
#include "compartment/SECompartmentManager.h"

void SELiquidCompartmentGraph::BalanceByIntensive()
{
  for (auto cmpt : GetCompartments())
    cmpt->Balance(BalanceLiquidBy::Concentration);
}

void SELiquidCompartmentGraph::AddGraph(SELiquidCompartmentGraph& graph)
{
  for (SELiquidCompartment* cmpt : graph.GetCompartments())
    AddCompartment(*cmpt);
  for (SELiquidCompartmentLink* lnk : graph.GetLinks())
    AddLink(*lnk);
}