/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "controller/BlackBoxes.h"
#include "controller/Circuits.h"
#include "circuit/fluid/SEFluidCircuit.h"
#include "circuit/thermal/SEThermalCircuit.h"
#include "controller/Compartments.h"
#include "compartment/fluid/SELiquidCompartmentGraph.h"
#include "compartment/fluid/SEGasCompartmentLink.h"
#include "compartment/fluid/SELiquidCompartmentLink.h"
#include "compartment/thermal/SEThermalCompartmentLink.h"
#include "substance/SESubstance.h"
#include "properties/SEScalarMassPerVolume.h"
#include "physiology/Saturation.h"

PulseBlackBoxes::PulseBlackBoxes(PulseController& data) : SEBlackBoxManager(data.GetLogger()), m_data(data)
{
  
}

template<COMPATIBLE_BLACK_BOX_TEMPLATE>
bool PulseBlackBoxes::IsValidBlackBoxRequest(CompartmentType* srcCmpt, CompartmentType* tgtCmpt)
{
  if (srcCmpt == nullptr)
  {
    Error("Source Compartment does not exist", "PulseBlackBoxes::IsValidBlackBoxRequest");
    return false;
  }
  if (srcCmpt == nullptr)
  {
    Error("Target Compartment, does not exist", "PulseBlackBoxes::IsValidBlackBoxRequest");
    return false;
  }

  return true;
}

template<typename CompartmentType, typename LinkType>
LinkType* GetLinkBetween(CompartmentType& src, CompartmentType& tgt)
{
  // Are these compartments next to each other?
  for (LinkType* link : src.GetLinks())
  {
    if ((&link->GetSourceCompartment() == &src &&
         &link->GetTargetCompartment() == &tgt) ||
        (&link->GetSourceCompartment() == &tgt &&
         &link->GetTargetCompartment() == &src))
    {
      return link;
    }
  }
  return nullptr;
}

SEGasBlackBox* PulseBlackBoxes::CreateGasBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName, const std::string& name)
{
  // TODO Finish
  return nullptr;
}


#define LIQUID_COMPONENTS SELiquidBlackBox, \
                          SEFluidCircuit, SEFluidCircuitNode, SEFluidCircuitPath, \
                          SELiquidCompartmentGraph, SELiquidCompartment, SELiquidCompartmentLink, \
                          PressureUnit, VolumeUnit, VolumePerTimeUnit
SELiquidBlackBox* PulseBlackBoxes::CreateLiquidBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName, const std::string& name)
{
  if (HasLiquidBlackBox(name))
  {
    Error("There is already a black box " + name);
    return nullptr;
  }
  if (HasLiquidBlackBox(srcCmptName, tgtCmptName))
  {
    Error("There is already a black box between " + srcCmptName + " and " + tgtCmptName);
    return nullptr;
  }
  SELiquidCompartment* srcCmpt = m_data.GetCompartments().GetLiquidCompartment(srcCmptName);
  SELiquidCompartment* tgtCmpt = m_data.GetCompartments().GetLiquidCompartment(tgtCmptName);
  if (!IsValidBlackBoxRequest(srcCmpt, tgtCmpt))
  {
    Error("Unable to create black box between " + srcCmptName + " and " + tgtCmptName);
    return nullptr;
  }
  // \TODO Figure out how to handle all cases: 1) Replace a single path (what's here now)
  //                                           2) Create a path where none existed before
  //                                           3) Replace a circuit chunk, like an entire three element windkessel
  //                                           Maybe assign a compartment to replace that can can any number of nodes and paths mapped?

  // Are these compartments next to each other?
  SELiquidCompartmentLink* replaceLink = GetLinkBetween<SELiquidCompartment, SELiquidCompartmentLink>(*srcCmpt, *tgtCmpt);
  if (replaceLink != nullptr)
  {
    SEFluidCircuitNode& srcNode = replaceLink->GetPath()->GetSourceNode();
    SEFluidCircuitNode& tgtNode = replaceLink->GetPath()->GetTargetNode();

    // \TODO Protect CreateLiquidBlackBox and make new public methods for CreateCardiovascularBlackBox that pass in the circuit and graph to this method
    SEFluidCircuit& circuit = m_data.GetCircuits().GetActiveCardiovascularCircuit();
    SELiquidCompartmentGraph& graph = m_data.GetCompartments().GetActiveCardiovascularGraph();

    // ---------------------------------------------------------
    // Create the black box
    SELiquidBlackBox* bb = SEBlackBoxManager::CreateLiquidBlackBox(srcCmptName, tgtCmptName, name);
    if (!CreateComponents<LIQUID_COMPONENTS>(*bb, srcNode, tgtNode, *srcCmpt, *tgtCmpt, *replaceLink, circuit, graph, PressureUnit::mmHg, VolumeUnit::mL, VolumePerTimeUnit::mL_Per_s))
    {
      // Remove bb
    }
    return bb;
  }
  Error("Currently, Compartments must be linked");
  return nullptr;
}

SEThermalBlackBox* PulseBlackBoxes::CreateThermalBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName, const std::string& name)
{
  // TODO Finish
  return nullptr;
}

template<CREATE_BLACK_BOX_COMPONENTS_TEMPLATE> bool PulseBlackBoxes::CreateComponents(BlackBoxType& bb,
                                                                                      NodeType& srcNode, NodeType& tgtNode,
                                                                                      CompartmentType& srcCmpt, CompartmentType& tgtCmpt, LinkType& replaceLink,
                                                                                      CircuitType& circuit, GraphType& graph,
                                                                                      const PotentialUnit& pUnit, const QuantityUnit& qUnit, const FluxUnit& fUnit)
{
  // ---------------------------------------------------------
  // Make our black box circuit components
  NodeType& bbNode     = circuit.CreateNode(bb.GetName());
  PathType& src2bbPath = circuit.CreatePath(srcNode, bbNode, bb.GetName()+"Source");
  PathType& bb2tgtPath = circuit.CreatePath(bbNode, tgtNode, bb.GetName()+"Target");

  // ---------------------------------------------------------
  // Initialize quantity
  // We need to do this before creating the compartment, so it will pass the has quantity check
  // TODO Figure out how to intialize the black box volume. Maybe as an optional input? Allow for no volume too...
  //   And for some use cases, we'll probably want to subtract the same volume from the source and/or target compartment/node as well
  // For now, we'll assume a small volume for the black box
  double volume_mL = 1.0;
  bbNode.GetNextVolume().SetValue(volume_mL, VolumeUnit::mL);
  bbNode.GetVolume().SetValue(volume_mL, VolumeUnit::mL);
  bbNode.GetVolumeBaseline().SetValue(volume_mL, VolumeUnit::mL);

  // ---------------------------------------------------------
  // Add our black box compartment components
  CompartmentType& bbCmpt = m_data.GetCompartments().CreateLiquidCompartment(bb.GetName());
  LinkType& src2bbLink = m_data.GetCompartments().CreateLiquidLink(srcCmpt, bbCmpt,  bb.GetName()+"Source");
  LinkType& bb2tgtLink = m_data.GetCompartments().CreateLiquidLink(bbCmpt,  tgtCmpt, bb.GetName()+"Target");
  // Add our new cmpt to our correct cmpt list TODO AMB Support new cmpts/links to our state
  if (pulse::VascularCompartment::HasValue(srcCmpt.GetName()) && pulse::VascularCompartment::HasValue(tgtCmpt.GetName()))
    pulse::VascularCompartment::AddValue(bb.GetName());

  // Add new components to the graph
  graph.AddCompartment(bbCmpt);
  graph.AddLink(src2bbLink);
  graph.AddLink(bb2tgtLink);
  // Remove replaced path and link
  circuit.RemovePath(*replaceLink.GetPath());
  graph.RemoveLink(replaceLink);
  // Apply mappings
  bbCmpt.GetNodeMapping().MapNode(bbNode);
  src2bbLink.MapPath(src2bbPath);
  bb2tgtLink.MapPath(bb2tgtPath);
  MapBlackBox<BlackBoxType, CompartmentType, LinkType, NodeType, PathType>(bb, src2bbLink, bb2tgtLink);

  // ---------------------------------------------------------
  // Initialize potentials and fluxes
  // Nodes already existed and probably already have potential values
  // Paths are new, so map the exiting path fluxes to them
  src2bbPath.GetNextFlux().Set(replaceLink.GetPath()->GetFlux());
  bb2tgtPath.GetNextFlux().Set(replaceLink.GetPath()->GetFlux());
  // Get rid of the old path/link now
  m_data.GetCircuits().DeleteFluidPath(replaceLink.GetPath()->GetName());
  m_data.GetCompartments().DeleteLiquidLink(replaceLink.GetName());

  // Set any missing potentials and fluxes to 0
  if (!srcNode.HasNextPotential())
    srcNode.GetNextPotential().SetValue(0.0, pUnit);
  if (!tgtNode.HasNextPotential())
    tgtNode.GetNextPotential().SetValue(0.0, pUnit);
  if (!src2bbPath.HasNextFlux())
    src2bbPath.GetNextFlux().SetValue(0.0, fUnit);
  if (!bb2tgtPath.HasNextFlux())
    bb2tgtPath.GetNextFlux().SetValue(0.0, fUnit);

  // Set black box potential to the average of source and target
  double bbNodePotential = (srcNode.GetNextPotential().GetValue(pUnit) + tgtNode.GetNextPotential().GetValue(pUnit)) * 0.5;
  bbNode.GetNextPotential().SetValue(bbNodePotential, pUnit);

  // ---------------------------------------------------------
  // Initialize substances to the source compartment concentration values
  graph.StateChange();
  if (bbCmpt.HasVolume())
  {
    for (SELiquidSubstanceQuantity* subQ : srcCmpt.GetSubstanceQuantities())
    {
      bbCmpt.GetSubstanceQuantity(subQ->GetSubstance())->GetConcentration().Set(subQ->GetConcentration());
    }
    bbCmpt.Balance(BalanceLiquidBy::Concentration);
    // TODO Only do this for cardiovascular compartments
    bbCmpt.GetPH().Set(srcCmpt.GetPH());
    m_data.GetSaturationCalculator().CalculateBloodGasDistribution(bbCmpt);
  }

  // ---------------------------------------------------------
  // Set the changes
  m_data.GetCompartments().StateChange();
  circuit.StateChange();
  graph.StateChange();
  return true;
}
