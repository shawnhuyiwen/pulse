/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
PUSH_PROTO_WARNINGS()
#include "pulse/cdm/bind/Circuit.pb.h"
POP_PROTO_WARNINGS()
#include "io/protobuf/PBCircuit.h"
#include "io/protobuf/PBProperties.h"
#include "io/protobuf/PBUtils.h"
#include "circuit/electrical/SEElectricalCircuit.h"
#include "circuit/fluid/SEFluidCircuit.h"
#include "circuit/thermal/SEThermalCircuit.h"
#include "circuit/SECircuitManager.h"


template<CIRCUIT_PATH_TEMPLATE>
void PBCircuit::Serialize(const CDM_BIND::CircuitPathData& src, SECircuitPath<CIRCUIT_PATH_TYPES>& dst)
{
  if (!src.name().empty())
    dst.m_Name = src.name();

  dst.SetSwitch((eGate)src.switch_());
  dst.SetNextSwitch((eGate)src.nextswitch());
  dst.SetValve((eGate)src.valve());
  dst.SetNextValve((eGate)src.nextvalve());
  dst.SetPolarizedState((eGate)src.polarizedstate());
  dst.SetNextPolarizedState((eGate)src.nextpolarizedstate());
}

template<CIRCUIT_PATH_TEMPLATE>
void PBCircuit::Serialize(const SECircuitPath<CIRCUIT_PATH_TYPES>& src, CDM_BIND::CircuitPathData& dst)
{
  dst.set_name(src.m_Name);
  dst.set_sourcenode(src.m_SourceNode.GetName());
  dst.set_targetnode(src.m_TargetNode.GetName());
  if (src.HasSwitch())
    dst.set_switch_((CDM_BIND::eGate)src.m_Switch);
  if (src.HasNextSwitch())
    dst.set_nextswitch((CDM_BIND::eGate)src.m_NextSwitch);
  if (src.HasValve())
    dst.set_valve((CDM_BIND::eGate)src.m_Valve);
  if (src.HasNextValve())
    dst.set_nextvalve((CDM_BIND::eGate)src.m_NextValve);
  if (src.HasPolarizedState())
    dst.set_polarizedstate((CDM_BIND::eGate)src.m_PolarizedState);
  if (src.HasNextPolarizedState())
    dst.set_nextpolarizedstate((CDM_BIND::eGate)src.m_NextPolarizedState);
}


template<typename CircuitBindType, CIRCUIT_TEMPLATE>
void PBCircuit::Load(const CircuitBindType& src, SECircuit<CIRCUIT_TYPES>& dst, const std::map<std::string, NodeType*>& nodes, const std::map<std::string, PathType*>& paths)
{
  PBCircuit::Serialize(src, dst, nodes, paths);
  dst.StateChange();
}
template<typename CircuitBindType, CIRCUIT_TEMPLATE>
void PBCircuit::Serialize(const CircuitBindType& src, SECircuit<CIRCUIT_TYPES>& dst, const std::map<std::string, NodeType*>& nodes, const std::map<std::string, PathType*>& paths)
{
  const CDM_BIND::CircuitData& srcC = src.circuit();
  dst.m_Name = srcC.name();
  for (int i = 0; i < srcC.node_size(); i++)
  {
    const std::string name = srcC.node(i);
    auto idx = nodes.find(name);
    if (idx == nodes.end())
    {
      dst.Error(dst.m_Name + " could not find node " + name.c_str());
      return;
    }
    dst.AddNode(*idx->second);
  }
  for (int i = 0; i < srcC.path_size(); i++)
  {
    const std::string name = srcC.path(i);
    auto idx = paths.find(name);
    if (idx == paths.end())
    {
      dst.Error(dst.m_Name + " could not find path " + name.c_str());
      return;
    }
    dst.AddPath(*idx->second);
  }
}
template<typename CircuitBindType, CIRCUIT_TEMPLATE>
CircuitBindType* PBCircuit::Unload(const SECircuit<CIRCUIT_TYPES>& src)
{
  CircuitBindType* dst = new CircuitBindType();
  Serialize(src, *dst);
  return dst;
}
template<typename CircuitBindType, CIRCUIT_TEMPLATE>
void PBCircuit::Serialize(const SECircuit<CIRCUIT_TYPES>& src, CircuitBindType& dst)
{
  CDM_BIND::CircuitData* dstC = dst.mutable_circuit();
  dstC->set_name(src.m_Name);
  for (auto* n : src.m_Nodes)
    dstC->add_node(n->GetName());
  for (auto* p : src.m_Paths)
    dstC->add_path(p->GetName());
}

template<CIRCUIT_NODE_TEMPLATE>
void PBCircuit::Serialize(const CDM_BIND::CircuitNodeData& src, SECircuitNode<CIRCUIT_NODE_TYPES>& dst)
{
  if (!src.name().empty())
    dst.m_Name = src.name();
  dst.m_IsReferenceNode = src.referencenode();
}
template<CIRCUIT_NODE_TEMPLATE>
void PBCircuit::Serialize(const SECircuitNode<CIRCUIT_NODE_TYPES>& src, CDM_BIND::CircuitNodeData& dst)
{
  dst.set_name(src.m_Name);
  dst.set_referencenode(src.m_IsReferenceNode);
}


bool PBCircuit::LoadCircuitManagerFile(SECircuitManager& mgr, const std::string& filename)
{
  CDM_BIND::CircuitManagerData src;
  std::ifstream file_stream(filename, std::ios::in);
  std::string fmsg((std::istreambuf_iterator<char>(file_stream)), std::istreambuf_iterator<char>());
  file_stream.close();
  if (!PBUtils::SerializeFromString(fmsg, src, JSON, mgr.GetLogger()))
    return false;
  PBCircuit::Load(src, mgr);
  return true;

  // If its a binary string in the file...
  //std::ifstream binary_istream(filename, std::ios::in | std::ios::binary);
  //src.ParseFromIstream(&binary_istream);
  // binary_istream.close();
}

void PBCircuit::SaveCircuitManagerFile(const SECircuitManager& mgr, const std::string& filename)
{
  std::string content;
  CDM_BIND::CircuitManagerData* src = PBCircuit::Unload(mgr);
  PBUtils::SerializeToString(*src, content, JSON, mgr.GetLogger());
  std::ofstream ascii_ostream(filename, std::ios::out | std::ios::trunc);
  ascii_ostream << content;
  ascii_ostream.flush();
  ascii_ostream.close();
  delete src;
}

void PBCircuit::Load(const CDM_BIND::CircuitManagerData& src, SECircuitManager& dst)
{
  dst.Clear();
  PBCircuit::Serialize(src, dst);
  dst.StateChange();
}
void PBCircuit::Serialize(const CDM_BIND::CircuitManagerData& src, SECircuitManager& dst)
{
  // Electrical
  for (int i = 0; i < src.electricalnode_size(); i++)
  {
    const CDM_BIND::ElectricalCircuitNodeData& n = src.electricalnode(i);
    PBCircuit::Load(n, dst.CreateNode<ELECTRICAL_LEDGER_TYPES>(n.circuitnode().name(), dst.m_ElectricalLedger));
  }
  for (int i = 0; i < src.electricalpath_size(); i++)
  {
    const CDM_BIND::ElectricalCircuitPathData& p = src.electricalpath(i);
    SEElectricalCircuitNode* srcNode = dst.GetNode(p.circuitpath().sourcenode(), dst.m_ElectricalLedger);
    if (srcNode == nullptr)
    {
      dst.Error("Could not find source node " + p.circuitpath().sourcenode() + " from path " + p.circuitpath().name().c_str());
      continue;
    }
    SEElectricalCircuitNode* tgtNode = dst.GetNode(p.circuitpath().targetnode(), dst.m_ElectricalLedger);
    if (tgtNode == nullptr)
    {
      dst.Error("Could not find target node " + p.circuitpath().targetnode() + " from path " + p.circuitpath().name().c_str());
      continue;
    }
    PBCircuit::Load(p, dst.CreatePath<ELECTRICAL_LEDGER_TYPES>(*srcNode, *tgtNode, p.circuitpath().name(), dst.m_ElectricalLedger));
  }
  for (int i = 0; i < src.electricalcircuit_size(); i++)
  {
    const CDM_BIND::ElectricalCircuitData& c = src.electricalcircuit(i);
    PBCircuit::Load(c, dst.CreateCircuit<ELECTRICAL_LEDGER_TYPES>(c.circuit().name(), dst.m_ElectricalLedger), dst.m_ElectricalLedger.nodes, dst.m_ElectricalLedger.paths);
  }

  // Fluid
  for (int i = 0; i < src.fluidnode_size(); i++)
  {
    const CDM_BIND::FluidCircuitNodeData& n = src.fluidnode(i);
    PBCircuit::Load(n, dst.CreateNode<FLUID_LEDGER_TYPES>(n.circuitnode().name(), dst.m_FluidLedger));
  }
  for (int i = 0; i < src.fluidpath_size(); i++)
  {
    const CDM_BIND::FluidCircuitPathData& p = src.fluidpath(i);
    SEFluidCircuitNode* srcNode = dst.GetNode(p.circuitpath().sourcenode(), dst.m_FluidLedger);
    if (srcNode == nullptr)
    {
      dst.Error("Could not find source node " + p.circuitpath().sourcenode() + " from path " + p.circuitpath().name().c_str());
      continue;
    }
    SEFluidCircuitNode* tgtNode = dst.GetNode(p.circuitpath().targetnode(), dst.m_FluidLedger);
    if (tgtNode == nullptr)
    {
      dst.Error("Could not find target node " + p.circuitpath().targetnode() + " from path " + p.circuitpath().name().c_str());
      continue;
    }
    PBCircuit::Load(p, dst.CreatePath<FLUID_LEDGER_TYPES>(*srcNode, *tgtNode, p.circuitpath().name(), dst.m_FluidLedger));
  }
  for (int i = 0; i < src.fluidcircuit_size(); i++)
  {
    const CDM_BIND::FluidCircuitData& c = src.fluidcircuit(i);
    PBCircuit::Load(c, dst.CreateCircuit<FLUID_LEDGER_TYPES>(c.circuit().name(), dst.m_FluidLedger), dst.m_FluidLedger.nodes, dst.m_FluidLedger.paths);
  }

  // Thermal
  for (int i = 0; i < src.thermalnode_size(); i++)
  {
    const CDM_BIND::ThermalCircuitNodeData& n = src.thermalnode(i);
    PBCircuit::Load(n, dst.CreateNode<THERMAL_LEDGER_TYPES>(n.circuitnode().name(), dst.m_ThermalLedger));
  }
  for (int i = 0; i < src.thermalpath_size(); i++)
  {
    const CDM_BIND::ThermalCircuitPathData& p = src.thermalpath(i);
    SEThermalCircuitNode* srcNode = dst.GetNode(p.circuitpath().sourcenode(), dst.m_ThermalLedger);
    if (srcNode == nullptr)
    {
      dst.Error("Could not find source node " + p.circuitpath().sourcenode() + " from path " + p.circuitpath().name().c_str());
      continue;
    }
    SEThermalCircuitNode* tgtNode = dst.GetNode(p.circuitpath().targetnode(), dst.m_ThermalLedger);
    if (tgtNode == nullptr)
    {
      dst.Error("Could not find target node " + p.circuitpath().targetnode() + " from path " + p.circuitpath().name().c_str());
      continue;
    }
    PBCircuit::Load(p, dst.CreatePath<THERMAL_LEDGER_TYPES>(*srcNode, *tgtNode, p.circuitpath().name(), dst.m_ThermalLedger));
  }
  for (int i = 0; i < src.thermalcircuit_size(); i++)
  {
    const CDM_BIND::ThermalCircuitData& c = src.thermalcircuit(i);
    PBCircuit::Load(c, dst.CreateCircuit<THERMAL_LEDGER_TYPES>(c.circuit().name(), dst.m_ThermalLedger), dst.m_ThermalLedger.nodes, dst.m_ThermalLedger.paths);
  }
}

CDM_BIND::CircuitManagerData* PBCircuit::Unload(const SECircuitManager& src)
{
  CDM_BIND::CircuitManagerData* dst = new CDM_BIND::CircuitManagerData();
  PBCircuit::Serialize(src, *dst);
  return dst;
}
#define ELECTRICAL_CIRCUIT_UNLOAD_TYPES CDM_BIND::ElectricalCircuitData,SEElectricalCircuitNode,SEElectricalCircuitPath
#define FLUID_CIRCUIT_UNLOAD_TYPES CDM_BIND::FluidCircuitData,SEFluidCircuitNode,SEFluidCircuitPath
#define THERMAL_CIRCUIT_UNLOAD_TYPES CDM_BIND::ThermalCircuitData,SEThermalCircuitNode,SEThermalCircuitPath
void PBCircuit::Serialize(const SECircuitManager& src, CDM_BIND::CircuitManagerData& dst)
{
  for (auto itr : src.m_ElectricalLedger.nodes)
    dst.mutable_electricalnode()->AddAllocated(PBCircuit::Unload(*itr.second));
  for (auto itr : src.m_ElectricalLedger.paths)
    dst.mutable_electricalpath()->AddAllocated(PBCircuit::Unload(*itr.second));
  for (auto itr : src.m_ElectricalLedger.circuits)
    dst.mutable_electricalcircuit()->AddAllocated(PBCircuit::Unload<ELECTRICAL_CIRCUIT_UNLOAD_TYPES>(*itr.second));

  for (auto itr : src.m_FluidLedger.nodes)
    dst.mutable_fluidnode()->AddAllocated(PBCircuit::Unload(*itr.second));
  for (auto itr : src.m_FluidLedger.paths)
    dst.mutable_fluidpath()->AddAllocated(PBCircuit::Unload(*itr.second));
  for (auto itr : src.m_FluidLedger.circuits)
    dst.mutable_fluidcircuit()->AddAllocated(PBCircuit::Unload<FLUID_CIRCUIT_UNLOAD_TYPES>(*itr.second));

  for (auto itr : src.m_ThermalLedger.nodes)
    dst.mutable_thermalnode()->AddAllocated(PBCircuit::Unload(*itr.second));
  for (auto itr : src.m_ThermalLedger.paths)
    dst.mutable_thermalpath()->AddAllocated(PBCircuit::Unload(*itr.second));
  for (auto itr : src.m_ThermalLedger.circuits)
    dst.mutable_thermalcircuit()->AddAllocated(PBCircuit::Unload<THERMAL_CIRCUIT_UNLOAD_TYPES>(*itr.second));
}

/////////////////////////
// Electrical Circuits //
/////////////////////////

void PBCircuit::Load(const CDM_BIND::ElectricalCircuitNodeData& src, SEElectricalCircuitNode& dst)
{
  dst.Clear();
  PBCircuit::Serialize(src, dst);
}
void PBCircuit::Serialize(const CDM_BIND::ElectricalCircuitNodeData& src, SEElectricalCircuitNode& dst)
{
  PBCircuit::Serialize(src.circuitnode(), dst);
  if (src.has_voltage())
    PBProperty::Load(src.voltage(), dst.GetVoltage());
  if (src.has_nextvoltage())
    PBProperty::Load(src.nextvoltage(), dst.GetNextVoltage());
  if (src.has_charge())
    PBProperty::Load(src.charge(), dst.GetCharge());
  if (src.has_nextcharge())
    PBProperty::Load(src.nextcharge(), dst.GetNextCharge());
  if (src.has_chargebaseline())
    PBProperty::Load(src.chargebaseline(), dst.GetChargeBaseline());
}

CDM_BIND::ElectricalCircuitNodeData* PBCircuit::Unload(const SEElectricalCircuitNode& src)
{
  CDM_BIND::ElectricalCircuitNodeData* dst = new CDM_BIND::ElectricalCircuitNodeData();
  PBCircuit::Serialize(src, *dst);
  return dst;
}
void PBCircuit::Serialize(const SEElectricalCircuitNode& src, CDM_BIND::ElectricalCircuitNodeData& dst)
{
  PBCircuit::Serialize(src, *dst.mutable_circuitnode());
  if (src.HasVoltage())
    dst.set_allocated_voltage(PBProperty::Unload(*src.m_Potential));
  if (src.HasNextVoltage())
    dst.set_allocated_nextvoltage(PBProperty::Unload(*src.m_NextPotential));
  if (src.HasCharge())
    dst.set_allocated_charge(PBProperty::Unload(*src.m_Quantity));
  if (src.HasNextCharge())
    dst.set_allocated_nextcharge(PBProperty::Unload(*src.m_NextQuantity));
  if (src.HasChargeBaseline())
    dst.set_allocated_chargebaseline(PBProperty::Unload(*src.m_QuantityBaseline));
}

void PBCircuit::Load(const CDM_BIND::ElectricalCircuitPathData& src, SEElectricalCircuitPath& dst)
{
  dst.Clear();
  PBCircuit::Serialize(src, dst);
}
void PBCircuit::Serialize(const CDM_BIND::ElectricalCircuitPathData& src, SEElectricalCircuitPath& dst)
{
  PBCircuit::Serialize(src.circuitpath(), dst);
  if (src.has_resistance())
    PBProperty::Load(src.resistance(), dst.GetResistance());
  if (src.has_nextresistance())
    PBProperty::Load(src.nextresistance(), dst.GetNextResistance());
  if (src.has_resistancebaseline())
    PBProperty::Load(src.resistancebaseline(), dst.GetResistanceBaseline());
  if (src.has_capacitance())
    PBProperty::Load(src.capacitance(), dst.GetCapacitance());
  if (src.has_nextcapacitance())
    PBProperty::Load(src.nextcapacitance(), dst.GetNextCapacitance());
  if (src.has_capacitancebaseline())
    PBProperty::Load(src.capacitancebaseline(), dst.GetCapacitanceBaseline());
  if (src.has_inductance())
    PBProperty::Load(src.inductance(), dst.GetInductance());
  if (src.has_nextinductance())
    PBProperty::Load(src.nextinductance(), dst.GetNextInductance());
  if (src.has_inductancebaseline())
    PBProperty::Load(src.inductancebaseline(), dst.GetInductanceBaseline());
  if (src.has_current())
    PBProperty::Load(src.current(), dst.GetCurrent());
  if (src.has_nextcurrent())
    PBProperty::Load(src.nextcurrent(), dst.GetNextCurrent());
  if (src.has_currentsource())
    PBProperty::Load(src.currentsource(), dst.GetCurrentSource());
  if (src.has_nextcurrentsource())
    PBProperty::Load(src.nextcurrentsource(), dst.GetNextCurrentSource());
  if (src.has_currentsourcebaseline())
    PBProperty::Load(src.currentsourcebaseline(), dst.GetCurrentSourceBaseline());
  if (src.has_voltagesource())
    PBProperty::Load(src.voltagesource(), dst.GetVoltageSource());
  if (src.has_nextvoltagesource())
    PBProperty::Load(src.nextvoltagesource(), dst.GetNextVoltageSource());
  if (src.has_voltagesourcebaseline())
    PBProperty::Load(src.voltagesourcebaseline(), dst.GetVoltageSourceBaseline());
  if (src.has_valvebreakdownvoltage())
    PBProperty::Load(src.valvebreakdownvoltage(), dst.GetValveBreakdownVoltage());

  if (!dst.HasValidElements())
    dst.Warning("Path does not have valid elements");
}

CDM_BIND::ElectricalCircuitPathData* PBCircuit::Unload(const SEElectricalCircuitPath& src)
{
  CDM_BIND::ElectricalCircuitPathData* dst = new CDM_BIND::ElectricalCircuitPathData();
  PBCircuit::Serialize(src, *dst);
  return dst;
}
void PBCircuit::Serialize(const SEElectricalCircuitPath& src, CDM_BIND::ElectricalCircuitPathData& dst)
{
  PBCircuit::Serialize(src, *dst.mutable_circuitpath());
  if (src.HasResistance())
    dst.set_allocated_resistance(PBProperty::Unload(*src.m_Resistance));
  if (src.HasNextResistance())
    dst.set_allocated_nextresistance(PBProperty::Unload(*src.m_NextResistance));
  if (src.HasResistanceBaseline())
    dst.set_allocated_resistancebaseline(PBProperty::Unload(*src.m_ResistanceBaseline));
  if (src.HasCapacitance())
    dst.set_allocated_capacitance(PBProperty::Unload(*src.m_Capacitance));
  if (src.HasNextCapacitance())
    dst.set_allocated_nextcapacitance(PBProperty::Unload(*src.m_NextCapacitance));
  if (src.HasCapacitanceBaseline())
    dst.set_allocated_capacitancebaseline(PBProperty::Unload(*src.m_CapacitanceBaseline));
  if (src.HasInductance())
    dst.set_allocated_inductance(PBProperty::Unload(*src.m_Inductance));
  if (src.HasNextInductance())
    dst.set_allocated_nextinductance(PBProperty::Unload(*src.m_NextInductance));
  if (src.HasInductanceBaseline())
    dst.set_allocated_inductancebaseline(PBProperty::Unload(*src.m_InductanceBaseline));
  if (src.HasCurrent())
    dst.set_allocated_current(PBProperty::Unload(*src.m_Flux));
  if (src.HasNextCurrent())
    dst.set_allocated_nextcurrent(PBProperty::Unload(*src.m_NextFlux));
  if (src.HasCurrentSource())
    dst.set_allocated_currentsource(PBProperty::Unload(*src.m_FluxSource));
  if (src.HasNextCurrentSource())
    dst.set_allocated_nextcurrentsource(PBProperty::Unload(*src.m_NextFluxSource));
  if (src.HasCurrentSourceBaseline())
    dst.set_allocated_currentsourcebaseline(PBProperty::Unload(*src.m_FluxSourceBaseline));
  if (src.HasVoltageSource())
    dst.set_allocated_voltagesource(PBProperty::Unload(*src.m_PotentialSource));
  if (src.HasNextVoltageSource())
    dst.set_allocated_nextvoltagesource(PBProperty::Unload(*src.m_NextPotentialSource));
  if (src.HasVoltageSourceBaseline())
    dst.set_allocated_voltagesourcebaseline(PBProperty::Unload(*src.m_PotentialSourceBaseline));
  if (src.HasValveBreakdownVoltage())
    dst.set_allocated_valvebreakdownvoltage(PBProperty::Unload(*src.m_ValveBreakdownPotential));
}

////////////////////
// Fluid Circuits //
////////////////////

void PBCircuit::Load(const CDM_BIND::FluidCircuitNodeData& src, SEFluidCircuitNode& dst)
{
  dst.Clear();
  PBCircuit::Serialize(src, dst);
}
void PBCircuit::Serialize(const CDM_BIND::FluidCircuitNodeData& src, SEFluidCircuitNode& dst)
{
  PBCircuit::Serialize(src.circuitnode(), dst);
  if (src.has_pressure())
    PBProperty::Load(src.pressure(), dst.GetPressure());
  if (src.has_nextpressure())
    PBProperty::Load(src.nextpressure(), dst.GetNextPressure());
  if (src.has_volume())
    PBProperty::Load(src.volume(), dst.GetVolume());
  if (src.has_nextvolume())
    PBProperty::Load(src.nextvolume(), dst.GetNextVolume());
  if (src.has_volumebaseline())
    PBProperty::Load(src.volumebaseline(), dst.GetVolumeBaseline());
}

CDM_BIND::FluidCircuitNodeData* PBCircuit::Unload(const SEFluidCircuitNode& src)
{
  CDM_BIND::FluidCircuitNodeData* dst = new CDM_BIND::FluidCircuitNodeData();
  PBCircuit::Serialize(src, *dst);
  return dst;
}
void PBCircuit::Serialize(const SEFluidCircuitNode& src, CDM_BIND::FluidCircuitNodeData& dst)
{
  PBCircuit::Serialize(src, *dst.mutable_circuitnode());
  if (src.HasPressure())
    dst.set_allocated_pressure(PBProperty::Unload(*src.m_Potential));
  if (src.HasNextPressure())
    dst.set_allocated_nextpressure(PBProperty::Unload(*src.m_NextPotential));
  if (src.HasVolume())
    dst.set_allocated_volume(PBProperty::Unload(*src.m_Quantity));
  if (src.HasNextVolume())
    dst.set_allocated_nextvolume(PBProperty::Unload(*src.m_NextQuantity));
  if (src.HasVolumeBaseline())
    dst.set_allocated_volumebaseline(PBProperty::Unload(*src.m_QuantityBaseline));
}

void PBCircuit::Load(const CDM_BIND::FluidCircuitPathData& src, SEFluidCircuitPath& dst)
{
  dst.Clear();
  PBCircuit::Serialize(src, dst);
}
void PBCircuit::Serialize(const CDM_BIND::FluidCircuitPathData& src, SEFluidCircuitPath& dst)
{
  PBCircuit::Serialize(src.circuitpath(), dst);
  if (src.has_resistance())
    PBProperty::Load(src.resistance(), dst.GetResistance());
  if (src.has_nextresistance())
    PBProperty::Load(src.nextresistance(), dst.GetNextResistance());
  if (src.has_resistancebaseline())
    PBProperty::Load(src.resistancebaseline(), dst.GetResistanceBaseline());
  if (src.has_compliance())
    PBProperty::Load(src.compliance(), dst.GetCompliance());
  if (src.has_nextcompliance())
    PBProperty::Load(src.nextcompliance(), dst.GetNextCompliance());
  if (src.has_compliancebaseline())
    PBProperty::Load(src.compliancebaseline(), dst.GetComplianceBaseline());
  if (src.has_inertance())
    PBProperty::Load(src.inertance(), dst.GetInertance());
  if (src.has_nextinertance())
    PBProperty::Load(src.nextinertance(), dst.GetNextInertance());
  if (src.has_inertancebaseline())
    PBProperty::Load(src.inertancebaseline(), dst.GetInertanceBaseline());
  if (src.has_flow())
    PBProperty::Load(src.flow(), dst.GetFlow());
  if (src.has_nextflow())
    PBProperty::Load(src.nextflow(), dst.GetNextFlow());
  if (src.has_flowsource())
    PBProperty::Load(src.flowsource(), dst.GetFlowSource());
  if (src.has_nextflowsource())
    PBProperty::Load(src.nextflowsource(), dst.GetNextFlowSource());
  if (src.has_flowsourcebaseline())
    PBProperty::Load(src.flowsourcebaseline(), dst.GetFlowSourceBaseline());
  if (src.has_pressuresource())
    PBProperty::Load(src.pressuresource(), dst.GetPressureSource());
  if (src.has_nextpressuresource())
    PBProperty::Load(src.nextpressuresource(), dst.GetNextPressureSource());
  if (src.has_pressuresourcebaseline())
    PBProperty::Load(src.pressuresourcebaseline(), dst.GetPressureSourceBaseline());
  if (src.has_valvebreakdownpressure())
    PBProperty::Load(src.valvebreakdownpressure(), dst.GetValveBreakdownPressure());

  if (!dst.HasValidElements())
    dst.Warning("Path does not have valid elements");
}

CDM_BIND::FluidCircuitPathData* PBCircuit::Unload(const SEFluidCircuitPath& src)
{
  CDM_BIND::FluidCircuitPathData* dst = new CDM_BIND::FluidCircuitPathData();
  PBCircuit::Serialize(src, *dst);
  return dst;
}
void PBCircuit::Serialize(const SEFluidCircuitPath& src, CDM_BIND::FluidCircuitPathData& dst)
{
  PBCircuit::Serialize(src, *dst.mutable_circuitpath());
  if (src.HasResistance())
    dst.set_allocated_resistance(PBProperty::Unload(*src.m_Resistance));
  if (src.HasNextResistance())
    dst.set_allocated_nextresistance(PBProperty::Unload(*src.m_NextResistance));
  if (src.HasResistanceBaseline())
    dst.set_allocated_resistancebaseline(PBProperty::Unload(*src.m_ResistanceBaseline));
  if (src.HasCompliance())
    dst.set_allocated_compliance(PBProperty::Unload(*src.m_Capacitance));
  if (src.HasNextCompliance())
    dst.set_allocated_nextcompliance(PBProperty::Unload(*src.m_NextCapacitance));
  if (src.HasComplianceBaseline())
    dst.set_allocated_compliancebaseline(PBProperty::Unload(*src.m_CapacitanceBaseline));
  if (src.HasInertance())
    dst.set_allocated_inertance(PBProperty::Unload(*src.m_Inductance));
  if (src.HasNextInertance())
    dst.set_allocated_nextinertance(PBProperty::Unload(*src.m_NextInductance));
  if (src.HasInertanceBaseline())
    dst.set_allocated_inertancebaseline(PBProperty::Unload(*src.m_InductanceBaseline));
  if (src.HasFlow())
    dst.set_allocated_flow(PBProperty::Unload(*src.m_Flux));
  if (src.HasNextFlow())
    dst.set_allocated_nextflow(PBProperty::Unload(*src.m_NextFlux));
  if (src.HasFlowSource())
    dst.set_allocated_flowsource(PBProperty::Unload(*src.m_FluxSource));
  if (src.HasNextFlowSource())
    dst.set_allocated_nextflowsource(PBProperty::Unload(*src.m_NextFluxSource));
  if (src.HasFlowSourceBaseline())
    dst.set_allocated_flowsourcebaseline(PBProperty::Unload(*src.m_FluxSourceBaseline));
  if (src.HasPressureSource())
    dst.set_allocated_pressuresource(PBProperty::Unload(*src.m_PotentialSource));
  if (src.HasNextPressureSource())
    dst.set_allocated_nextpressuresource(PBProperty::Unload(*src.m_NextPotentialSource));
  if (src.HasPressureSourceBaseline())
    dst.set_allocated_pressuresourcebaseline(PBProperty::Unload(*src.m_PotentialSourceBaseline));
  if (src.HasValveBreakdownPressure())
    dst.set_allocated_valvebreakdownpressure(PBProperty::Unload(*src.m_ValveBreakdownPotential));
}

//////////////////////
// Thermal Circuits //
//////////////////////

void PBCircuit::Load(const CDM_BIND::ThermalCircuitNodeData& src, SEThermalCircuitNode& dst)
{
  dst.Clear();
  PBCircuit::Serialize(src, dst);
}
void PBCircuit::Serialize(const CDM_BIND::ThermalCircuitNodeData& src, SEThermalCircuitNode& dst)
{
  PBCircuit::Serialize(src.circuitnode(), dst);
  if (src.has_temperature())
    PBProperty::Load(src.temperature(), dst.GetTemperature());
  if (src.has_nexttemperature())
    PBProperty::Load(src.nexttemperature(), dst.GetNextTemperature());
  if (src.has_heat())
    PBProperty::Load(src.heat(), dst.GetHeat());
  if (src.has_nextheat())
    PBProperty::Load(src.nextheat(), dst.GetNextHeat());
  if (src.has_heatbaseline())
    PBProperty::Load(src.heatbaseline(), dst.GetHeatBaseline());
}

CDM_BIND::ThermalCircuitNodeData* PBCircuit::Unload(const SEThermalCircuitNode& src)
{
  CDM_BIND::ThermalCircuitNodeData* dst = new CDM_BIND::ThermalCircuitNodeData();
  PBCircuit::Serialize(src, *dst);
  return dst;
}
void PBCircuit::Serialize(const SEThermalCircuitNode& src, CDM_BIND::ThermalCircuitNodeData& dst)
{
  PBCircuit::Serialize(src, *dst.mutable_circuitnode());
  if (src.HasTemperature())
    dst.set_allocated_temperature(PBProperty::Unload(*src.m_Potential));
  if (src.HasNextTemperature())
    dst.set_allocated_nexttemperature(PBProperty::Unload(*src.m_NextPotential));
  if (src.HasHeat())
    dst.set_allocated_heat(PBProperty::Unload(*src.m_Quantity));
  if (src.HasNextHeat())
    dst.set_allocated_nextheat(PBProperty::Unload(*src.m_NextQuantity));
  if (src.HasHeatBaseline())
    dst.set_allocated_heatbaseline(PBProperty::Unload(*src.m_QuantityBaseline));
}


void PBCircuit::Load(const CDM_BIND::ThermalCircuitPathData& src, SEThermalCircuitPath& dst)
{
  dst.Clear();
  PBCircuit::Serialize(src, dst);
}
void PBCircuit::Serialize(const CDM_BIND::ThermalCircuitPathData& src, SEThermalCircuitPath& dst)
{
  PBCircuit::Serialize(src.circuitpath(), dst);
  if (src.has_resistance())
    PBProperty::Load(src.resistance(), dst.GetResistance());
  if (src.has_nextresistance())
    PBProperty::Load(src.nextresistance(), dst.GetNextResistance());
  if (src.has_resistancebaseline())
    PBProperty::Load(src.resistancebaseline(), dst.GetResistanceBaseline());
  if (src.has_capacitance())
    PBProperty::Load(src.capacitance(), dst.GetCapacitance());
  if (src.has_nextcapacitance())
    PBProperty::Load(src.nextcapacitance(), dst.GetNextCapacitance());
  if (src.has_capacitancebaseline())
    PBProperty::Load(src.capacitancebaseline(), dst.GetCapacitanceBaseline());
  if (src.has_inductance())
    PBProperty::Load(src.inductance(), dst.GetInductance());
  if (src.has_nextinductance())
    PBProperty::Load(src.nextinductance(), dst.GetNextInductance());
  if (src.has_inductancebaseline())
    PBProperty::Load(src.inductancebaseline(), dst.GetInductanceBaseline());
  if (src.has_heattransferrate())
    PBProperty::Load(src.heattransferrate(), dst.GetHeatTransferRate());
  if (src.has_nextheattransferrate())
    PBProperty::Load(src.nextheattransferrate(), dst.GetNextHeatTransferRate());
  if (src.has_heatsource())
    PBProperty::Load(src.heatsource(), dst.GetHeatSource());
  if (src.has_nextheatsource())
    PBProperty::Load(src.nextheatsource(), dst.GetNextHeatSource());
  if (src.has_heatsourcebaseline())
    PBProperty::Load(src.heatsourcebaseline(), dst.GetHeatSourceBaseline());
  if (src.has_temperaturesource())
    PBProperty::Load(src.temperaturesource(), dst.GetTemperatureSource());
  if (src.has_nexttemperaturesource())
    PBProperty::Load(src.nexttemperaturesource(), dst.GetNextTemperatureSource());
  if (src.has_temperaturesourcebaseline())
    PBProperty::Load(src.temperaturesourcebaseline(), dst.GetTemperatureSourceBaseline());
  if (src.has_valvebreakdowntemperature())
    PBProperty::Load(src.valvebreakdowntemperature(), dst.GetValveBreakdownTemperature());

  if (!dst.HasValidElements())
    dst.Warning("Path does not have valid elements");
}

CDM_BIND::ThermalCircuitPathData* PBCircuit::Unload(const SEThermalCircuitPath& src)
{
  CDM_BIND::ThermalCircuitPathData* dst = new CDM_BIND::ThermalCircuitPathData();
  PBCircuit::Serialize(src, *dst);
  return dst;
}
void PBCircuit::Serialize(const SEThermalCircuitPath& src, CDM_BIND::ThermalCircuitPathData& dst)
{
  PBCircuit::Serialize(src, *dst.mutable_circuitpath());
  if (src.HasResistance())
    dst.set_allocated_resistance(PBProperty::Unload(*src.m_Resistance));
  if (src.HasNextResistance())
    dst.set_allocated_nextresistance(PBProperty::Unload(*src.m_NextResistance));
  if (src.HasResistanceBaseline())
    dst.set_allocated_resistancebaseline(PBProperty::Unload(*src.m_ResistanceBaseline));
  if (src.HasCapacitance())
    dst.set_allocated_capacitance(PBProperty::Unload(*src.m_Capacitance));
  if (src.HasNextCapacitance())
    dst.set_allocated_nextcapacitance(PBProperty::Unload(*src.m_NextCapacitance));
  if (src.HasCapacitanceBaseline())
    dst.set_allocated_capacitancebaseline(PBProperty::Unload(*src.m_CapacitanceBaseline));
  if (src.HasInductance())
    dst.set_allocated_inductance(PBProperty::Unload(*src.m_Inductance));
  if (src.HasNextInductance())
    dst.set_allocated_nextinductance(PBProperty::Unload(*src.m_NextInductance));
  if (src.HasInductanceBaseline())
    dst.set_allocated_inductancebaseline(PBProperty::Unload(*src.m_InductanceBaseline));
  if (src.HasHeatTransferRate())
    dst.set_allocated_heattransferrate(PBProperty::Unload(*src.m_Flux));
  if (src.HasNextHeatTransferRate())
    dst.set_allocated_nextheattransferrate(PBProperty::Unload(*src.m_NextFlux));
  if (src.HasHeatSource())
    dst.set_allocated_heatsource(PBProperty::Unload(*src.m_FluxSource));
  if (src.HasNextHeatSource())
    dst.set_allocated_nextheatsource(PBProperty::Unload(*src.m_NextFluxSource));
  if (src.HasHeatSourceBaseline())
    dst.set_allocated_heatsourcebaseline(PBProperty::Unload(*src.m_FluxSourceBaseline));
  if (src.HasTemperatureSource())
    dst.set_allocated_temperaturesource(PBProperty::Unload(*src.m_PotentialSource));
  if (src.HasNextTemperatureSource())
    dst.set_allocated_nexttemperaturesource(PBProperty::Unload(*src.m_NextPotentialSource));
  if (src.HasTemperatureSourceBaseline())
    dst.set_allocated_temperaturesourcebaseline(PBProperty::Unload(*src.m_PotentialSourceBaseline));
  if (src.HasValveBreakdownTemperature())
    dst.set_allocated_valvebreakdowntemperature(PBProperty::Unload(*src.m_ValveBreakdownPotential));
}