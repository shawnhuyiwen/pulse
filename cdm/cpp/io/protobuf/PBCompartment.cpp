/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/PBCompartment.h"
#include "io/protobuf/PBSubstanceQuantity.h"
#include "io/protobuf/PBProperties.h"
#include "io/protobuf/PBUtils.h"
#include "compartment/SECompartment.h"
#include "compartment/SECompartmentGraph.h"
#include "compartment/SECompartmentLink.h"
#include "compartment/SECompartmentNodes.h"
#include "compartment/SECompartmentTransportGraph.h"
#include "compartment/fluid/SEGasCompartment.h"
#include "compartment/fluid/SEGasCompartmentGraph.h"
#include "compartment/fluid/SEGasCompartmentLink.h"
#include "compartment/fluid/SELiquidCompartment.h"
#include "compartment/fluid/SELiquidCompartmentGraph.h"
#include "compartment/thermal/SEThermalCompartment.h"
#include "compartment/thermal/SEThermalCompartmentLink.h"
#include "compartment/tissue/SETissueCompartment.h"
#include "compartment/SECompartmentManager.h"
#include "circuit/thermal/SEThermalCircuitPath.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"
#include "bind/cpp/cdm/Compartment.pb.h"

void PBCompartment::Serialize(const cdm::CompartmentData& src, SECompartment& dst)
{
  dst.Clear();
  // Name is set in ctor
}
void PBCompartment::Serialize(const SECompartment& src, cdm::CompartmentData& dst)
{
  dst.set_name(src.m_Name);
}

void PBCompartment::Serialize(const cdm::CompartmentLinkData& src, SECompartmentLink& dst)
{
  dst.Clear();
  if (!src.name().empty())
    dst.m_Name = src.name();
}
void PBCompartment::Serialize(const SECompartmentLink& src, cdm::CompartmentLinkData& dst)
{
  dst.set_name(src.m_Name);
}

bool PBCompartment::LoadCompartmentManagerFile(SECompartmentManager& mgr, const std::string& filename, SECircuitManager* circuits)
{
  cdm::CompartmentManagerData src;
  std::ifstream file_stream(filename, std::ios::in);
  std::string fmsg((std::istreambuf_iterator<char>(file_stream)), std::istreambuf_iterator<char>());
  if (!PBUtils::SerializeFromString(fmsg, src, JSON))
    return false;
  PBCompartment::Load(src, mgr, circuits);
  return true;

  // If its a binary string in the file...
  //std::ifstream binary_istream(filename, std::ios::in | std::ios::binary);
  //src.ParseFromIstream(&binary_istream);
}

void PBCompartment::SaveCompartmentManagerFile(const SECompartmentManager& mgr, const std::string& filename)
{
  std::string content;
  cdm::CompartmentManagerData* src = PBCompartment::Unload(mgr);
  PBUtils::SerializeToString(*src, content,JSON);
  std::ofstream ascii_ostream(filename, std::ios::out | std::ios::trunc);
  ascii_ostream << content;
  ascii_ostream.flush();
  ascii_ostream.close();
  delete src;
}

void PBCompartment::Load(const cdm::CompartmentManagerData& src, SECompartmentManager& dst, SECircuitManager* circuits)
{
  PBCompartment::Serialize(src, dst, circuits);
  dst.StateChange();
}
void PBCompartment::Serialize(const cdm::CompartmentManagerData& src, SECompartmentManager& dst, SECircuitManager* circuits)
{
  dst.Clear();

  for (int i = 0; i < src.gascompartment_size(); i++)
  {
    auto& cData = src.gascompartment(i);
    PBCompartment::Load(cData, dst.CreateGasCompartment(cData.fluidcompartment().compartment().name()), dst.m_subMgr, circuits);
  }
  for (int i = 0; i < src.gaslink_size(); i++)
  {
    auto& cData = src.gaslink(i);
    auto* srcCmpt = dst.GetGasCompartment(cData.fluidlink().link().sourcecompartment());
    if (srcCmpt == nullptr)
    {
      dst.Error("Unable to find source compartment " + cData.fluidlink().link().sourcecompartment() + " for link " + cData.fluidlink().link().name());
      continue;
    }
    auto* tgtCmpt = dst.GetGasCompartment(cData.fluidlink().link().targetcompartment());
    if (tgtCmpt == nullptr)
    {
      dst.Error("Unable to find target compartment " + cData.fluidlink().link().targetcompartment() + " for link " + cData.fluidlink().link().name());
      continue;
    }
    PBCompartment::Load(cData, dst.CreateGasLink(*srcCmpt, *tgtCmpt, cData.fluidlink().link().name()), circuits);
  }
  for (int i = 0; i < src.gascompartment_size(); i++)
  {
    auto& cData = src.gascompartment(i);
    auto* cmpt = dst.GetGasCompartment(cData.fluidcompartment().compartment().name());
    for (int j = 0; j < cData.fluidcompartment().compartment().child_size(); j++)
    {
      std::string name = cData.fluidcompartment().compartment().child(j);
      auto* child = dst.GetGasCompartment(name);
      if (child == nullptr)
      {
        dst.Error("Could not find child " + name + " for node " + cmpt->GetName());
        continue;
      }
      cmpt->AddChild(*child);
    }
  }
  for (int i = 0; i < src.gassubstance_size(); i++)
  {
    std::string subName = src.gassubstance(i);
    SESubstance* sub = dst.m_subMgr.GetSubstance(subName);
    if (sub == nullptr)
    {
      dst.Error("Could not find substance " + subName);
      continue;
    }
    dst.AddGasCompartmentSubstance(*sub);
  }
  for (int i = 0; i < src.gasgraph_size(); i++)
  {
    auto& cData = src.gasgraph(i);
    PBCompartment::Load(cData, dst.CreateGasGraph(cData.fluidgraph().graph().name()), dst);
  }

  for (int i = 0; i < src.liquidcompartment_size(); i++)
  {
    auto& cData = src.liquidcompartment(i);
    PBCompartment::Load(cData, dst.CreateLiquidCompartment(cData.fluidcompartment().compartment().name()), dst.m_subMgr, circuits);
  }
  for (int i = 0; i < src.liquidlink_size(); i++)
  {
    auto& cData = src.liquidlink(i);
    auto* srcCmpt = dst.GetLiquidCompartment(cData.fluidlink().link().sourcecompartment());
    if (srcCmpt == nullptr)
    {
      dst.Error("Unable to find source compartment " + cData.fluidlink().link().sourcecompartment() + " for link " + cData.fluidlink().link().name());
      continue;
    }
    auto* tgtCmpt = dst.GetLiquidCompartment(cData.fluidlink().link().targetcompartment());
    if (tgtCmpt == nullptr)
    {
      dst.Error("Unable to find target compartment " + cData.fluidlink().link().targetcompartment() + " for link " + cData.fluidlink().link().name());
      continue;
    }
    PBCompartment::Load(cData, dst.CreateLiquidLink(*srcCmpt, *tgtCmpt, cData.fluidlink().link().name()), circuits);
  }
  for (int i = 0; i < src.liquidcompartment_size(); i++)
  {
    auto& cData = src.liquidcompartment(i);
    auto* cmpt = dst.GetLiquidCompartment(cData.fluidcompartment().compartment().name());
    for (int j = 0; j < cData.fluidcompartment().compartment().child_size(); j++)
    {
      std::string name = cData.fluidcompartment().compartment().child(j);
      auto* child = dst.GetLiquidCompartment(name);
      if (child == nullptr)
      {
        dst.Error("Could not find child " + name + " for node " + cmpt->GetName());
        continue;
      }
      cmpt->AddChild(*child);
    }
  }
  for (int i = 0; i < src.liquidsubstance_size(); i++)
  {
    std::string subName = src.liquidsubstance(i);
    SESubstance* sub = dst.m_subMgr.GetSubstance(subName);
    if (sub == nullptr)
    {
      dst.Error("Could not find substance " + subName);
      continue;
    }
    dst.AddLiquidCompartmentSubstance(*sub);
  }
  for (int i = 0; i < src.liquidgraph_size(); i++)
  {
    auto& cData = src.liquidgraph(i);
    PBCompartment::Load(cData, dst.CreateLiquidGraph(cData.fluidgraph().graph().name()), dst);
  }

  for (int i = 0; i < src.thermalcompartment_size(); i++)
  {
    auto& cData = src.thermalcompartment(i);
    PBCompartment::Load(cData, dst.CreateThermalCompartment(cData.compartment().name()), circuits);
  }
  for (int i = 0; i < src.thermallink_size(); i++)
  {
    auto& cData = src.thermallink(i);
    auto* srcCmpt = dst.GetThermalCompartment(cData.link().sourcecompartment());
    if (srcCmpt == nullptr)
    {
      dst.Error("Unable to find source compartment " + cData.link().sourcecompartment() + " for link " + cData.link().name());
      continue;
    }
    auto* tgtCmpt = dst.GetThermalCompartment(cData.link().targetcompartment());
    if (tgtCmpt == nullptr)
    {
      dst.Error("Unable to find target compartment " + cData.link().targetcompartment() + " for link " + cData.link().name());
      continue;
    }
    PBCompartment::Load(cData, dst.CreateThermalLink(*srcCmpt, *tgtCmpt, cData.link().name()), circuits);
  }
  for (int i = 0; i < src.thermalcompartment_size(); i++)
  {
    auto& cData = src.thermalcompartment(i);
    auto* cmpt = dst.GetThermalCompartment(cData.compartment().name());
    for (int j = 0; j < cData.compartment().child_size(); j++)
    {
      std::string name = cData.compartment().child(j);
      auto* child = dst.GetThermalCompartment(name);
      if (child == nullptr)
      {
        dst.Error("Could not find child " + name + " for node " + cmpt->GetName());
        continue;
      }
      cmpt->AddChild(*child);
    }
  }

  for (int i = 0; i < src.tissuecompartment_size(); i++)
  {
    auto& cData = src.tissuecompartment(i);
    PBCompartment::Load(cData, dst.CreateTissueCompartment(cData.compartment().name()));
  }
}

cdm::CompartmentManagerData* PBCompartment::Unload(const SECompartmentManager& src)
{
  cdm::CompartmentManagerData* dst = new cdm::CompartmentManagerData();
  PBCompartment::Serialize(src, *dst);
  return dst;
}
void PBCompartment::Serialize(const SECompartmentManager& src, cdm::CompartmentManagerData& dst)
{
  for (SELiquidCompartment* cmpt : src.m_LiquidCompartments)
    dst.mutable_liquidcompartment()->AddAllocated(PBCompartment::Unload(*cmpt));
  for (SELiquidCompartmentLink* link : src.m_LiquidLinks)
    dst.mutable_liquidlink()->AddAllocated(PBCompartment::Unload(*link));
  for (SELiquidCompartmentGraph* graph : src.m_LiquidGraphs)
    dst.mutable_liquidgraph()->AddAllocated(PBCompartment::Unload(*graph));
  for (SESubstance* sub : src.m_LiquidSubstances)
    dst.mutable_liquidsubstance()->Add(sub->GetName());

  for (SEGasCompartment* cmpt : src.m_GasCompartments)
    dst.mutable_gascompartment()->AddAllocated(PBCompartment::Unload(*cmpt));
  for (SEGasCompartmentLink* link : src.m_GasLinks)
    dst.mutable_gaslink()->AddAllocated(PBCompartment::Unload(*link));
  for (SEGasCompartmentGraph* graph : src.m_GasGraphs)
    dst.mutable_gasgraph()->AddAllocated(PBCompartment::Unload(*graph));
  for (SESubstance* sub : src.m_GasSubstances)
    dst.mutable_gassubstance()->Add(sub->GetName());

  for (SEThermalCompartment* cmpt : src.m_ThermalCompartments)
    dst.mutable_thermalcompartment()->AddAllocated(PBCompartment::Unload(*cmpt));
  for (SEThermalCompartmentLink* link : src.m_ThermalLinks)
    dst.mutable_thermallink()->AddAllocated(PBCompartment::Unload(*link));

  for (SETissueCompartment* cmpt : src.m_TissueCompartments)
    dst.mutable_tissuecompartment()->AddAllocated(PBCompartment::Unload(*cmpt));
}

///////////////////////
// Fluid Compartment //
///////////////////////

template<FLUID_COMPARTMENT_TEMPLATE>
void PBCompartment::Serialize(const cdm::FluidCompartmentData& src, SEFluidCompartment<FLUID_COMPARTMENT_TYPES>& dst, SECircuitManager* circuits)
{
  PBCompartment::Serialize(src.compartment(), dst);
  // This compartment has children
  // We will not load any data as those are calculated on demand based on children
  if (src.compartment().child_size() > 0)
    return;
  // Now let's see if we are mapped to a circuit node
  // If we are, the circuit node(s) hold the data, not us
  else if (src.compartment().node_size() > 0)
  {
    if (circuits == nullptr)
    {
      dst.Error("Compartment is mapped to circuit nodes, but no circuit manager was provided, cannot load");
      return;
    }
    for (int i = 0; i < src.compartment().node_size(); i++)
    {
      const std::string name = src.compartment().node(i);
      SEFluidCircuitNode* node = circuits->GetFluidNode(name);
      if (node == nullptr)
        dst.Error("Compartment is mapped to circuit node, " + name + ", but provided circuit manager did not have that node");
      else
        dst.MapNode(*node);
    }
  }
  else
  {// Only load these if you don't have children or nodes
    if (src.has_pressure())
      PBProperty::Load(src.pressure(), dst.GetPressure());
    if (src.has_volume())
      PBProperty::Load(src.volume(), dst.GetVolume());
  }
}
template<FLUID_COMPARTMENT_TEMPLATE>
void PBCompartment::Serialize(const SEFluidCompartment<FLUID_COMPARTMENT_TYPES>& src, cdm::FluidCompartmentData& dst)
{
  PBCompartment::Serialize(src, *dst.mutable_compartment());
  for (SEFluidCompartment<FLUID_COMPARTMENT_TYPES>* child : src.m_FluidChildren)
    dst.mutable_compartment()->add_child(child->GetName());
  for (SEFluidCircuitNode* nodes : src.m_Nodes.GetNodes())
    dst.mutable_compartment()->add_node(nodes->GetName());
  // Even if you have children or nodes, I am unloading everything, this makes the json actually usefull...
  if (src.HasInFlow())
    dst.set_allocated_inflow(PBProperty::Unload(src.GetInFlow()));
  if (src.HasOutFlow())
    dst.set_allocated_outflow(PBProperty::Unload(src.GetOutFlow()));

  // Yeah, I know
  // But, these will only modify member variables if they are being used as temporary variables
  SEFluidCompartment<FLUID_COMPARTMENT_TYPES>& mutable_src = const_cast<SEFluidCompartment<FLUID_COMPARTMENT_TYPES>&>(src);
  if (src.HasPressure())
    dst.set_allocated_pressure(PBProperty::Unload(mutable_src.GetPressure()));
  if (src.HasVolume())
    dst.set_allocated_volume(PBProperty::Unload(mutable_src.GetVolume()));
}


template<FLUID_COMPARTMENT_LINK_TEMPLATE>
void PBCompartment::Serialize(const cdm::FluidCompartmentLinkData& src, SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>& dst, SECircuitManager* circuits)
{
  PBCompartment::Serialize(src.link(), dst);

  if (!src.link().path().empty())
  {
    if (circuits == nullptr)
    {
      dst.Error("Link is mapped to circuit path, " + src.link().path() + ", but no circuit manager was provided, cannot load");
      return;
    }
    SEFluidCircuitPath* path = circuits->GetFluidPath(src.link().path());
    if (path == nullptr)
    {
      dst.Error("Link is mapped to circuit path, " + src.link().path() + ", but provided circuit manager did not have that path");
      return;
    }
    dst.MapPath(*path);
  }
  else
  {
    if (src.has_flow())
      PBProperty::Load(src.flow(), dst.GetFlow());
  }
}
template<FLUID_COMPARTMENT_LINK_TEMPLATE>
void PBCompartment::Serialize(const SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>& src, cdm::FluidCompartmentLinkData& dst)
{
  PBCompartment::Serialize(src, *dst.mutable_link());
  dst.mutable_link()->set_sourcecompartment(src.m_SourceCmpt.GetName());
  dst.mutable_link()->set_targetcompartment(src.m_TargetCmpt.GetName());
  if (src.m_Path != nullptr)
    dst.mutable_link()->set_path(src.m_Path->GetName());
  // Yeah, I know
  // But, these will only modify member variables if they are being used as temporary variables
  // Even if you have a path, I am unloading everything, this makes the json actually usefull...
  SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>& mutable_src = const_cast<SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>&>(src);
  if (src.HasFlow())
    dst.set_allocated_flow(PBProperty::Unload(mutable_src.GetFlow()));
}


void PBCompartment::Load(const cdm::GasCompartmentData& src, SEGasCompartment& dst, SESubstanceManager& subMgr, SECircuitManager* circuits)
{
  PBCompartment::Serialize(src, dst, subMgr, circuits);
}
void PBCompartment::Serialize(const cdm::GasCompartmentData& src, SEGasCompartment& dst, SESubstanceManager& subMgr, SECircuitManager* circuits)
{
  PBCompartment::Serialize(src.fluidcompartment(), dst, circuits);

  if (src.substancequantity_size() > 0)
  {
    for (int i = 0; i < src.substancequantity_size(); i++)
    {
      const cdm::GasSubstanceQuantityData& d = src.substancequantity(i);
      SESubstance* sub = subMgr.GetSubstance(d.substancequantity().substance());
      if (sub == nullptr)
      {
        dst.Error("Could not find a substance for " + d.substancequantity().substance());
        continue;
      }
      PBSubstanceQuantity::Load(d, dst.CreateSubstanceQuantity(*sub));
    }
  }
}

cdm::GasCompartmentData* PBCompartment::Unload(const SEGasCompartment& src)
{
  cdm::GasCompartmentData* dst = new cdm::GasCompartmentData();
  PBCompartment::Serialize(src, *dst);
  return dst;
}
void PBCompartment::Serialize(const SEGasCompartment& src, cdm::GasCompartmentData& dst)
{
  PBCompartment::Serialize(src, *dst.mutable_fluidcompartment());
  for (SEGasSubstanceQuantity* subQ : src.m_SubstanceQuantities)
    dst.mutable_substancequantity()->AddAllocated(PBSubstanceQuantity::Unload(*subQ));
}


void PBCompartment::Load(const cdm::GasCompartmentGraphData& src, SEGasCompartmentGraph& dst, SECompartmentManager& cmptMgr)
{
  PBCompartment::Serialize(src, dst, cmptMgr);
}
void PBCompartment::Serialize(const cdm::GasCompartmentGraphData& src, SEGasCompartmentGraph& dst, SECompartmentManager& cmptMgr)
{
  dst.m_Name = src.fluidgraph().graph().name();
  for (int i = 0; i < src.fluidgraph().graph().compartment_size(); i++)
  {
    std::string name = src.fluidgraph().graph().compartment(i);
    SEGasCompartment* cmpt = cmptMgr.GetGasCompartment(name);
    if (cmpt == nullptr)
    {
      dst.Error("Could not find compartment " + name + " for graph " + dst.m_Name);
      continue;
    }
    dst.AddCompartment(*cmpt);
  }
  for (int i = 0; i < src.fluidgraph().graph().link_size(); i++)
  {
    std::string name = src.fluidgraph().graph().link(i);
    SEGasCompartmentLink* link = cmptMgr.GetGasLink(name);
    if (link == nullptr)
    {
      dst.Error("Could not find link " + name + " for graph " + dst.m_Name);
      continue;
    }
    dst.AddLink(*link);
  }
}

cdm::GasCompartmentGraphData* PBCompartment::Unload(const SEGasCompartmentGraph& src)
{
  cdm::GasCompartmentGraphData* dst = new cdm::GasCompartmentGraphData();
  PBCompartment::Serialize(src, *dst);
  return dst;
}
void PBCompartment::Serialize(const SEGasCompartmentGraph& src, cdm::GasCompartmentGraphData& dst)
{
  dst.mutable_fluidgraph()->mutable_graph()->set_name(src.m_Name);
  for (SEGasCompartment* cmpt : src.m_Compartments)
    dst.mutable_fluidgraph()->mutable_graph()->add_compartment(cmpt->GetName());
  for (SEGasCompartmentLink* link : src.m_CompartmentLinks)
    dst.mutable_fluidgraph()->mutable_graph()->add_link(link->GetName());
}

void PBCompartment::Load(const cdm::GasCompartmentLinkData& src, SEGasCompartmentLink& dst, SECircuitManager* circuits)
{
  PBCompartment::Serialize(src, dst, circuits);
}
void PBCompartment::Serialize(const cdm::GasCompartmentLinkData& src, SEGasCompartmentLink& dst, SECircuitManager* circuits)
{
  PBCompartment::Serialize(src.fluidlink(), dst, circuits);
}

cdm::GasCompartmentLinkData* PBCompartment::Unload(const SEGasCompartmentLink& src)
{
  cdm::GasCompartmentLinkData* dst = new cdm::GasCompartmentLinkData();
  PBCompartment::Serialize(src, *dst);
  return dst;
}
void PBCompartment::Serialize(const SEGasCompartmentLink& src, cdm::GasCompartmentLinkData& dst)
{
  PBCompartment::Serialize(src, *dst.mutable_fluidlink());
}


void PBCompartment::Load(const cdm::LiquidCompartmentData& src, SELiquidCompartment& dst, SESubstanceManager& subMgr, SECircuitManager* circuits)
{
  PBCompartment::Serialize(src, dst, subMgr, circuits);
}
void PBCompartment::Serialize(const cdm::LiquidCompartmentData& src, SELiquidCompartment& dst, SESubstanceManager& subMgr, SECircuitManager* circuits)
{
  PBCompartment::Serialize(src.fluidcompartment(), dst, circuits);

  if (src.substancequantity_size() > 0)
  {
    for (int i = 0; i < src.substancequantity_size(); i++)
    {
      const cdm::LiquidSubstanceQuantityData& d = src.substancequantity(i);
      SESubstance* sub = subMgr.GetSubstance(d.substancequantity().substance());
      if (sub == nullptr)
      {
        dst.Error("Could not find a substance for " + d.substancequantity().substance());
        continue;
      }
      PBSubstanceQuantity::Load(d, dst.CreateSubstanceQuantity(*sub));
    }
  }
  if (src.has_ph())
    PBProperty::Load(src.ph(), dst.GetPH());
  if (src.has_watervolumefraction())
    PBProperty::Load(src.watervolumefraction(), dst.GetWaterVolumeFraction());
}

cdm::LiquidCompartmentData* PBCompartment::Unload(const SELiquidCompartment& src)
{
  cdm::LiquidCompartmentData* dst = new cdm::LiquidCompartmentData();
  PBCompartment::Serialize(src, *dst);
  return dst;
}
void PBCompartment::Serialize(const SELiquidCompartment& src, cdm::LiquidCompartmentData& dst)
{
  PBCompartment::Serialize(src, *dst.mutable_fluidcompartment());
  for (SELiquidSubstanceQuantity* subQ : src.m_SubstanceQuantities)
    dst.mutable_substancequantity()->AddAllocated(PBSubstanceQuantity::Unload(*subQ));

  if (src.HasPH())
    dst.set_allocated_ph(PBProperty::Unload(*src.m_pH));
  if (src.HasWaterVolumeFraction())
    dst.set_allocated_watervolumefraction(PBProperty::Unload(*src.m_WaterVolumeFraction));
}


void PBCompartment::Load(const cdm::LiquidCompartmentGraphData& src, SELiquidCompartmentGraph& dst, SECompartmentManager& cmptMgr)
{
  PBCompartment::Serialize(src, dst, cmptMgr);
}
void PBCompartment::Serialize(const cdm::LiquidCompartmentGraphData& src, SELiquidCompartmentGraph& dst, SECompartmentManager& cmptMgr)
{
  dst.m_Name = src.fluidgraph().graph().name();
  for (int i = 0; i < src.fluidgraph().graph().compartment_size(); i++)
  {
    std::string name = src.fluidgraph().graph().compartment(i);
    SELiquidCompartment* cmpt = cmptMgr.GetLiquidCompartment(name);
    if (cmpt == nullptr)
    {
      dst.Error("Could not find compartment " + name + " for graph " + dst.m_Name);
      continue;
    }
    dst.AddCompartment(*cmpt);
  }
  for (int i = 0; i < src.fluidgraph().graph().link_size(); i++)
  {
    std::string name = src.fluidgraph().graph().link(i);
    SELiquidCompartmentLink* link = cmptMgr.GetLiquidLink(name);
    if (link == nullptr)
    {
      dst.Error("Could not find link " + name + " for graph " + dst.m_Name);
      continue;
    }
    dst.AddLink(*link);
  }
}

cdm::LiquidCompartmentGraphData* PBCompartment::Unload(const SELiquidCompartmentGraph& src)
{
  cdm::LiquidCompartmentGraphData* dst = new cdm::LiquidCompartmentGraphData();
  PBCompartment::Serialize(src, *dst);
  return dst;
}
void PBCompartment::Serialize(const SELiquidCompartmentGraph& src, cdm::LiquidCompartmentGraphData& dst)
{
  dst.mutable_fluidgraph()->mutable_graph()->set_name(src.m_Name);
  for (SELiquidCompartment* cmpt : src.m_Compartments)
    dst.mutable_fluidgraph()->mutable_graph()->add_compartment(cmpt->GetName());
  for (SELiquidCompartmentLink* link : src.m_CompartmentLinks)
    dst.mutable_fluidgraph()->mutable_graph()->add_link(link->GetName());
}

void PBCompartment::Load(const cdm::LiquidCompartmentLinkData& src, SELiquidCompartmentLink& dst, SECircuitManager* circuits)
{
  PBCompartment::Serialize(src, dst, circuits);
}
void PBCompartment::Serialize(const cdm::LiquidCompartmentLinkData& src, SELiquidCompartmentLink& dst, SECircuitManager* circuits)
{
  PBCompartment::Serialize(src.fluidlink(), dst, circuits);
}

cdm::LiquidCompartmentLinkData* PBCompartment::Unload(const SELiquidCompartmentLink& src)
{
  cdm::LiquidCompartmentLinkData* dst = new cdm::LiquidCompartmentLinkData();
  PBCompartment::Serialize(src, *dst);
  return dst;
}
void PBCompartment::Serialize(const SELiquidCompartmentLink& src, cdm::LiquidCompartmentLinkData& dst)
{
  PBCompartment::Serialize(src, *dst.mutable_fluidlink());
}

//////////////////////////
// Thermal Compartments //
//////////////////////////


void PBCompartment::Load(const cdm::ThermalCompartmentData& src, SEThermalCompartment& dst, SECircuitManager* circuits)
{
  PBCompartment::Serialize(src, dst, circuits);
}
void PBCompartment::Serialize(const cdm::ThermalCompartmentData& src, SEThermalCompartment& dst, SECircuitManager* circuits)
{
  PBCompartment::Serialize(src.compartment(), dst);
  // This compartment has children
  // We will not load any data as those are calculated on demand based on children
  if (src.compartment().child_size() > 0)
    return;
  // Now let's see if we are mapped to a circuit node
  // If we are, the circuit node(s) hold the data, not us
  else if (src.compartment().node_size() > 0)
  {
    if (circuits == nullptr)
    {
      dst.Error("Compartment is mapped to circuit nodes, but no circuit manager was provided, cannot load");
      return;
    }
    for (int i = 0; i < src.compartment().node_size(); i++)
    {
      const std::string name = src.compartment().node(i);
      SEThermalCircuitNode* node = circuits->GetThermalNode(name);
      if (node == nullptr)
        dst.Error("Compartment is mapped to circuit node, " + name + ", but provided circuit manager did not have that node");
      else
        dst.MapNode(*node);
    }
  }
  else
  {// Only load these if you don't have children or nodes
    if (src.has_heat())
      PBProperty::Load(src.heat(), dst.GetHeat());
    if (src.has_temperature())
      PBProperty::Load(src.temperature(), dst.GetTemperature());
  }
}

cdm::ThermalCompartmentData* PBCompartment::Unload(const SEThermalCompartment& src)
{
  cdm::ThermalCompartmentData* dst = new cdm::ThermalCompartmentData();
  PBCompartment::Serialize(src, *dst);
  return dst;
}
void PBCompartment::Serialize(const SEThermalCompartment& src, cdm::ThermalCompartmentData& dst)
{
  PBCompartment::Serialize(src, *dst.mutable_compartment());
  for (SEThermalCompartment* child : src.m_Children)
    dst.mutable_compartment()->add_child(child->GetName());
  for (SEThermalCircuitNode* nodes : src.m_Nodes.GetNodes())
    dst.mutable_compartment()->add_node(nodes->GetName());
  // Even if you have children or nodes, I am unloading everything, this makes the json actually usefull...
  if (src.HasHeatTransferRateIn())
    dst.set_allocated_heattransferratein(PBProperty::Unload(src.GetHeatTransferRateIn()));
  if (src.HasHeatTransferRateOut())
    dst.set_allocated_heattransferrateout(PBProperty::Unload(src.GetHeatTransferRateOut()));

  // Yeah, I know
  // But, these will only modify member variables if they are being used as temporary variables
  SEThermalCompartment& mutable_src = const_cast<SEThermalCompartment&>(src);
  if (src.HasHeat())
    dst.set_allocated_heat(PBProperty::Unload(mutable_src.GetHeat()));
  if (src.HasTemperature())
    dst.set_allocated_temperature(PBProperty::Unload(mutable_src.GetTemperature()));
}


void PBCompartment::Load(const cdm::ThermalCompartmentLinkData& src, SEThermalCompartmentLink& dst, SECircuitManager* circuits)
{
  PBCompartment::Serialize(src, dst, circuits);
}
void PBCompartment::Serialize(const cdm::ThermalCompartmentLinkData& src, SEThermalCompartmentLink& dst, SECircuitManager* circuits)
{
  PBCompartment::Serialize(src.link(), dst);

  if (!src.link().path().empty())
  {
    if (circuits == nullptr)
    {
      dst.Error("Link is mapped to circuit path, " + src.link().path() + ", but no circuit manager was provided, cannot load");
      return;
    }
    SEThermalCircuitPath* path = circuits->GetThermalPath(src.link().path());
    if (path == nullptr)
    {
      dst.Error("Link is mapped to circuit path, " + src.link().path() + ", but provided circuit manager did not have that path");
      return;
    }
    dst.MapPath(*path);
  }
  else
  {
    if (src.has_heattransferrate())
      PBProperty::Load(src.heattransferrate(), dst.GetHeatTransferRate());
  }
}

cdm::ThermalCompartmentLinkData* PBCompartment::Unload(const SEThermalCompartmentLink& src)
{
  cdm::ThermalCompartmentLinkData* dst = new cdm::ThermalCompartmentLinkData();
  PBCompartment::Serialize(src, *dst);
  return dst;
}
void PBCompartment::Serialize(const SEThermalCompartmentLink& src, cdm::ThermalCompartmentLinkData& dst)
{
  PBCompartment::Serialize(src, *dst.mutable_link());
  dst.mutable_link()->set_sourcecompartment(src.m_SourceCmpt.GetName());
  dst.mutable_link()->set_targetcompartment(src.m_TargetCmpt.GetName());
  if (src.m_Path != nullptr)
    dst.mutable_link()->set_path(src.m_Path->GetName());
  // Yeah, I know
  // But, these will only modify member variables if they are being used as temporary variables
  // Even if you have a path, I am unloading everything, this makes the json actually usefull...
  SEThermalCompartmentLink& mutable_src = const_cast<SEThermalCompartmentLink&>(src);
  if (src.HasHeatTransferRate())
    dst.set_allocated_heattransferrate(PBProperty::Unload(mutable_src.GetHeatTransferRate()));
}

////////////////////////
// Tissue Compartment //
////////////////////////


void PBCompartment::Load(const cdm::TissueCompartmentData& src, SETissueCompartment& dst)
{
  PBCompartment::Serialize(src, dst);
}
void PBCompartment::Serialize(const cdm::TissueCompartmentData& src, SETissueCompartment& dst)
{
  PBCompartment::Serialize(src.compartment(), dst);
  if (src.has_acidicphospohlipidconcentration())
    PBProperty::Load(src.acidicphospohlipidconcentration(), dst.GetAcidicPhospohlipidConcentration());
  if (src.has_matrixvolume())
    PBProperty::Load(src.matrixvolume(), dst.GetMatrixVolume());
  if (src.has_neutrallipidsvolumefraction())
    PBProperty::Load(src.neutrallipidsvolumefraction(), dst.GetNeutralLipidsVolumeFraction());
  if (src.has_neutralphospholipidsvolumefraction())
    PBProperty::Load(src.neutralphospholipidsvolumefraction(), dst.GetNeutralPhospholipidsVolumeFraction());
  if (src.has_tissuetoplasmaalbuminratio())
    PBProperty::Load(src.tissuetoplasmaalbuminratio(), dst.GetTissueToPlasmaAlbuminRatio());
  if (src.has_tissuetoplasmaalphaacidglycoproteinratio())
    PBProperty::Load(src.tissuetoplasmaalphaacidglycoproteinratio(), dst.GetTissueToPlasmaAlphaAcidGlycoproteinRatio());
  if (src.has_tissuetoplasmalipoproteinratio())
    PBProperty::Load(src.tissuetoplasmalipoproteinratio(), dst.GetTissueToPlasmaLipoproteinRatio());
  if (src.has_totalmass())
    PBProperty::Load(src.totalmass(), dst.GetTotalMass());
}

cdm::TissueCompartmentData* PBCompartment::Unload(const SETissueCompartment& src)
{
  cdm::TissueCompartmentData* dst = new cdm::TissueCompartmentData();
  PBCompartment::Serialize(src, *dst);
  return dst;
}
void PBCompartment::Serialize(const SETissueCompartment& src, cdm::TissueCompartmentData& dst)
{
  PBCompartment::Serialize(src, *dst.mutable_compartment());
  if (src.HasAcidicPhospohlipidConcentration())
    dst.set_allocated_acidicphospohlipidconcentration(PBProperty::Unload(*src.m_AcidicPhospohlipidConcentration));
  if (src.HasMatrixVolume())
    dst.set_allocated_matrixvolume(PBProperty::Unload(*src.m_MatrixVolume));
  if (src.HasNeutralLipidsVolumeFraction())
    dst.set_allocated_neutrallipidsvolumefraction(PBProperty::Unload(*src.m_NeutralLipidsVolumeFraction));
  if (src.HasNeutralPhospholipidsVolumeFraction())
    dst.set_allocated_neutralphospholipidsvolumefraction(PBProperty::Unload(*src.m_NeutralPhospholipidsVolumeFraction));
  if (src.HasTissueToPlasmaAlbuminRatio())
    dst.set_allocated_tissuetoplasmaalbuminratio(PBProperty::Unload(*src.m_TissueToPlasmaAlbuminRatio));
  if (src.HasTissueToPlasmaAlphaAcidGlycoproteinRatio())
    dst.set_allocated_tissuetoplasmaalphaacidglycoproteinratio(PBProperty::Unload(*src.m_TissueToPlasmaAlphaAcidGlycoproteinRatio));
  if (src.HasTissueToPlasmaLipoproteinRatio())
    dst.set_allocated_tissuetoplasmalipoproteinratio(PBProperty::Unload(*src.m_TissueToPlasmaLipoproteinRatio));
  if (src.HasTotalMass())
    dst.set_allocated_totalmass(PBProperty::Unload(*src.m_TotalMass));
}