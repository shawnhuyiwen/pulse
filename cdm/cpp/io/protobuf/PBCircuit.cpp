/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/PBCircuit.h"
#include "circuit/electrical/SEElectricalCircuit.h"
#include "circuit/fluid/SEFluidCircuit.h"
#include "circuit/thermal/SEThermalCircuit.h"
#include "circuit/SECircuitManager.h"
#include "bind/cdm/Circuit.pb.h"
#include <google/protobuf/text_format.h>


template<CIRCUIT_PATH_TEMPLATE>
void PBCircuit::Serialize(const cdm::CircuitPathData& src, SECircuitPath<CIRCUIT_PATH_TYPES>& dst)
{
  dst.Clear();
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
void PBCircuit::Serialize(const SECircuitPath<CIRCUIT_PATH_TYPES>& src, cdm::CircuitPathData& dst)
{
  dst.set_name(src.m_Name);
  dst.set_sourcenode(src.m_SourceNode.GetName());
  dst.set_targetnode(src.m_TargetNode.GetName());
  if (src.HasSwitch())
    dst.set_switch_((cdm::eGate)src.m_Switch);
  if (src.HasNextSwitch())
    dst.set_nextswitch((cdm::eGate)src.m_NextSwitch);
  if (src.HasValve())
    dst.set_valve((cdm::eGate)src.m_Valve);
  if (src.HasNextValve())
    dst.set_nextvalve((cdm::eGate)src.m_NextValve);
  if (src.HasPolarizedState())
    dst.set_polarizedstate((cdm::eGate)src.m_PolarizedState);
  if (src.HasNextPolarizedState())
    dst.set_nextpolarizedstate((cdm::eGate)src.m_NextPolarizedState);
}


template<typename CircuitBindType, CIRCUIT_TEMPLATE>
void PBCircuit::Load(const CircuitBindType& src, SECircuit<CIRCUIT_TYPES>& dst, const std::map<std::string, NodeType*>& nodes, const std::map<std::string, PathType*>& paths)
{
  PBCircuit::Serialize(src, dst, nodes, paths);
  dst.StateChange();
}
template<typename CircuitBindType, CIRCUIT_TEMPLATE>
void PBCircuit::Serialize(const CircuitBindType& src, SECircuit<CIRCUIT_TYPES>& dst, const std::map<std::string, NodeType*>& nodes, const std::map<std::string, PathType*>& paths)
{// note: not clearing here as the derived class needs to clear and call this super class Load last to get the ref node hooked up
  dst.Clear();
  const cdm::CircuitData& srcC = src.circuit();
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
  cdm::CircuitData* dstC = dst.mutable_circuit();
  dstC->set_name(src.m_Name);
  for (auto* n : src.m_Nodes)
    dstC->add_node(n->GetName());
  for (auto* p : src.m_Paths)
    dstC->add_path(p->GetName());
}

template<CIRCUIT_NODE_TEMPLATE>
void PBCircuit::Serialize(const cdm::CircuitNodeData& src, SECircuitNode<CIRCUIT_NODE_TYPES>& dst)
{
  dst.Clear();
  if (!src.name().empty())
    dst.m_Name = src.name();
  dst.m_IsReferenceNode = src.referencenode();
}
template<CIRCUIT_NODE_TEMPLATE>
void PBCircuit::Serialize(const SECircuitNode<CIRCUIT_NODE_TYPES>& src, cdm::CircuitNodeData& dst)
{
  dst.set_name(src.m_Name);
  dst.set_referencenode(src.m_IsReferenceNode);
}


bool PBCircuit::LoadCircuitManagerFile(SECircuitManager& mgr, const std::string& filename)
{
  cdm::CircuitManagerData src;
  std::ifstream file_stream(filename, std::ios::in);
  std::string fmsg((std::istreambuf_iterator<char>(file_stream)), std::istreambuf_iterator<char>());
  if (!google::protobuf::TextFormat::ParseFromString(fmsg, &src))
    return false;
  PBCircuit::Load(src, mgr);
  return true;

  // If its a binary string in the file...
  //std::ifstream binary_istream(filename, std::ios::in | std::ios::binary);
  //src.ParseFromIstream(&binary_istream);
}

void PBCircuit::SaveCircuitManagerFile(const SECircuitManager& mgr, const std::string& filename)
{
  std::string content;
  cdm::CircuitManagerData* src = PBCircuit::Unload(mgr);
  google::protobuf::TextFormat::PrintToString(*src, &content);
  std::ofstream ascii_ostream(filename, std::ios::out | std::ios::trunc);
  ascii_ostream << content;
  ascii_ostream.flush();
  ascii_ostream.close();
  delete src;
}

void PBCircuit::Load(const cdm::CircuitManagerData& src, SECircuitManager& dst)
{
  PBCircuit::Serialize(src, dst);
  dst.StateChange();
}
void PBCircuit::Serialize(const cdm::CircuitManagerData& src, SECircuitManager& dst)
{
  dst.Clear();
  // Electrical
  for (int i = 0; i < src.electricalnode_size(); i++)
  {
    const cdm::ElectricalCircuitNodeData& n = src.electricalnode(i);
    PBCircuit::Load(n, dst.CreateNode<ELECTRICAL_LEDGER_TYPES>(n.circuitnode().name(), dst.m_ElectricalLedger));
  }
  for (int i = 0; i < src.electricalpath_size(); i++)
  {
    const cdm::ElectricalCircuitPathData& p = src.electricalpath(i);
    SEElectricalCircuitNode* src = dst.GetNode(p.circuitpath().sourcenode(), dst.m_ElectricalLedger);
    if (src == nullptr)
    {
      dst.Error("Could not find source node " + p.circuitpath().sourcenode() + " from path " + p.circuitpath().name().c_str());
      continue;
    }
    SEElectricalCircuitNode* tgt = dst.GetNode(p.circuitpath().targetnode(), dst.m_ElectricalLedger);
    if (tgt == nullptr)
    {
      dst.Error("Could not find target node " + p.circuitpath().targetnode() + " from path " + p.circuitpath().name().c_str());
      continue;
    }
    PBCircuit::Load(p, dst.CreatePath<ELECTRICAL_LEDGER_TYPES>(*src, *tgt, p.circuitpath().name(), dst.m_ElectricalLedger));
  }
  for (int i = 0; i < src.electricalcircuit_size(); i++)
  {
    const cdm::ElectricalCircuitData& c = src.electricalcircuit(i);
    PBCircuit::Load(c, dst.CreateCircuit<ELECTRICAL_LEDGER_TYPES>(c.circuit().name(), dst.m_ElectricalLedger), dst.m_ElectricalLedger.nodes, dst.m_ElectricalLedger.paths);
  }

  // Fluid
  for (int i = 0; i < src.fluidnode_size(); i++)
  {
    const cdm::FluidCircuitNodeData& n = src.fluidnode(i);
    PBCircuit::Load(n, dst.CreateNode<FLUID_LEDGER_TYPES>(n.circuitnode().name(), dst.m_FluidLedger));
  }
  for (int i = 0; i < src.fluidpath_size(); i++)
  {
    const cdm::FluidCircuitPathData& p = src.fluidpath(i);
    SEFluidCircuitNode* src = dst.GetNode(p.circuitpath().sourcenode(), dst.m_FluidLedger);
    if (src == nullptr)
    {
      dst.Error("Could not find source node " + p.circuitpath().sourcenode() + " from path " + p.circuitpath().name().c_str());
      continue;
    }
    SEFluidCircuitNode* tgt = dst.GetNode(p.circuitpath().targetnode(), dst.m_FluidLedger);
    if (tgt == nullptr)
    {
      dst.Error("Could not find target node " + p.circuitpath().targetnode() + " from path " + p.circuitpath().name().c_str());
      continue;
    }
    PBCircuit::Load(p, dst.CreatePath<FLUID_LEDGER_TYPES>(*src, *tgt, p.circuitpath().name(), dst.m_FluidLedger));
  }
  for (int i = 0; i < src.fluidcircuit_size(); i++)
  {
    const cdm::FluidCircuitData& c = src.fluidcircuit(i);
    PBCircuit::Load(c, dst.CreateCircuit<FLUID_LEDGER_TYPES>(c.circuit().name(), dst.m_FluidLedger), dst.m_FluidLedger.nodes, dst.m_FluidLedger.paths);
  }

  // Thermal
  for (int i = 0; i < src.thermalnode_size(); i++)
  {
    const cdm::ThermalCircuitNodeData& n = src.thermalnode(i);
    PBCircuit::Load(n, dst.CreateNode<THERMAL_LEDGER_TYPES>(n.circuitnode().name(), dst.m_ThermalLedger));
  }
  for (int i = 0; i < src.thermalpath_size(); i++)
  {
    const cdm::ThermalCircuitPathData& p = src.thermalpath(i);
    SEThermalCircuitNode* src = dst.GetNode(p.circuitpath().sourcenode(), dst.m_ThermalLedger);
    if (src == nullptr)
    {
      dst.Error("Could not find source node " + p.circuitpath().sourcenode() + " from path " + p.circuitpath().name().c_str());
      continue;
    }
    SEThermalCircuitNode* tgt = dst.GetNode(p.circuitpath().targetnode(), dst.m_ThermalLedger);
    if (tgt == nullptr)
    {
      dst.Error("Could not find target node " + p.circuitpath().targetnode() + " from path " + p.circuitpath().name().c_str());
      continue;
    }
    PBCircuit::Load(p, dst.CreatePath<THERMAL_LEDGER_TYPES>(*src, *tgt, p.circuitpath().name(), dst.m_ThermalLedger));
  }
  for (int i = 0; i < src.thermalcircuit_size(); i++)
  {
    const cdm::ThermalCircuitData& c = src.thermalcircuit(i);
    PBCircuit::Load(c, dst.CreateCircuit<THERMAL_LEDGER_TYPES>(c.circuit().name(), dst.m_ThermalLedger), dst.m_ThermalLedger.nodes, dst.m_ThermalLedger.paths);
  }
}

cdm::CircuitManagerData* PBCircuit::Unload(const SECircuitManager& src)
{
  cdm::CircuitManagerData* dst = new cdm::CircuitManagerData();
  PBCircuit::Serialize(src, *dst);
  return dst;
}
#define ELECTRICAL_CIRCUIT_UNLOAD_TYPES cdm::ElectricalCircuitData,SEElectricalCircuitNode,SEElectricalCircuitPath
#define FLUID_CIRCUIT_UNLOAD_TYPES cdm::FluidCircuitData,SEFluidCircuitNode,SEFluidCircuitPath
#define THERMAL_CIRCUIT_UNLOAD_TYPES cdm::ThermalCircuitData,SEThermalCircuitNode,SEThermalCircuitPath
void PBCircuit::Serialize(const SECircuitManager& src, cdm::CircuitManagerData& dst)
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

void PBCircuit::Load(const cdm::ElectricalCircuitNodeData& src, SEElectricalCircuitNode& dst)
{
  PBCircuit::Serialize(src, dst);
}
void PBCircuit::Serialize(const cdm::ElectricalCircuitNodeData& src, SEElectricalCircuitNode& dst)
{
  PBCircuit::Serialize(src.circuitnode(), dst);
  if (src.has_voltage())
    SEScalarElectricPotential::Load(src.voltage(), dst.GetVoltage());
  if (src.has_nextvoltage())
    SEScalarElectricPotential::Load(src.nextvoltage(), dst.GetNextVoltage());
  if (src.has_charge())
    SEScalarElectricCharge::Load(src.charge(), dst.GetCharge());
  if (src.has_nextcharge())
    SEScalarElectricCharge::Load(src.nextcharge(), dst.GetNextCharge());
  if (src.has_chargebaseline())
    SEScalarElectricCharge::Load(src.chargebaseline(), dst.GetChargeBaseline());
}

cdm::ElectricalCircuitNodeData* PBCircuit::Unload(const SEElectricalCircuitNode& src)
{
  cdm::ElectricalCircuitNodeData* dst = new cdm::ElectricalCircuitNodeData();
  PBCircuit::Serialize(src, *dst);
  return dst;
}
void PBCircuit::Serialize(const SEElectricalCircuitNode& src, cdm::ElectricalCircuitNodeData& dst)
{
  PBCircuit::Serialize(src, *dst.mutable_circuitnode());
  if (src.HasVoltage())
    dst.set_allocated_voltage(SEScalarElectricPotential::Unload(*src.m_Potential));
  if (src.HasNextVoltage())
    dst.set_allocated_nextvoltage(SEScalarElectricPotential::Unload(*src.m_NextPotential));
  if (src.HasCharge())
    dst.set_allocated_charge(SEScalarElectricCharge::Unload(*src.m_Quantity));
  if (src.HasNextCharge())
    dst.set_allocated_nextcharge(SEScalarElectricCharge::Unload(*src.m_NextQuantity));
  if (src.HasChargeBaseline())
    dst.set_allocated_chargebaseline(SEScalarElectricCharge::Unload(*src.m_QuantityBaseline));
}

void PBCircuit::Load(const cdm::ElectricalCircuitPathData& src, SEElectricalCircuitPath& dst)
{
  PBCircuit::Serialize(src, dst);
}
void PBCircuit::Serialize(const cdm::ElectricalCircuitPathData& src, SEElectricalCircuitPath& dst)
{
  PBCircuit::Serialize(src.circuitpath(), dst);
  if (src.has_resistance())
    SEScalarElectricResistance::Load(src.resistance(), dst.GetResistance());
  if (src.has_nextresistance())
    SEScalarElectricResistance::Load(src.nextresistance(), dst.GetNextResistance());
  if (src.has_resistancebaseline())
    SEScalarElectricResistance::Load(src.resistancebaseline(), dst.GetResistanceBaseline());
  if (src.has_capacitance())
    SEScalarElectricCapacitance::Load(src.capacitance(), dst.GetCapacitance());
  if (src.has_nextcapacitance())
    SEScalarElectricCapacitance::Load(src.nextcapacitance(), dst.GetNextCapacitance());
  if (src.has_capacitancebaseline())
    SEScalarElectricCapacitance::Load(src.capacitancebaseline(), dst.GetCapacitanceBaseline());
  if (src.has_inductance())
    SEScalarElectricInductance::Load(src.inductance(), dst.GetInductance());
  if (src.has_nextinductance())
    SEScalarElectricInductance::Load(src.nextinductance(), dst.GetNextInductance());
  if (src.has_inductancebaseline())
    SEScalarElectricInductance::Load(src.inductancebaseline(), dst.GetInductanceBaseline());
  if (src.has_current())
    SEScalarElectricCurrent::Load(src.current(), dst.GetCurrent());
  if (src.has_nextcurrent())
    SEScalarElectricCurrent::Load(src.nextcurrent(), dst.GetNextCurrent());
  if (src.has_currentsource())
    SEScalarElectricCurrent::Load(src.currentsource(), dst.GetCurrentSource());
  if (src.has_nextcurrentsource())
    SEScalarElectricCurrent::Load(src.nextcurrentsource(), dst.GetNextCurrentSource());
  if (src.has_currentsourcebaseline())
    SEScalarElectricCurrent::Load(src.currentsourcebaseline(), dst.GetCurrentSourceBaseline());
  if (src.has_voltagesource())
    SEScalarElectricPotential::Load(src.voltagesource(), dst.GetVoltageSource());
  if (src.has_nextvoltagesource())
    SEScalarElectricPotential::Load(src.nextvoltagesource(), dst.GetNextVoltageSource());
  if (src.has_voltagesourcebaseline())
    SEScalarElectricPotential::Load(src.voltagesourcebaseline(), dst.GetVoltageSourceBaseline());
  if (src.has_valvebreakdownvoltage())
    SEScalarElectricPotential::Load(src.valvebreakdownvoltage(), dst.GetValveBreakdownVoltage());

  if (!dst.HasValidElements())
    dst.Warning("Path does not have valid elements");
}

cdm::ElectricalCircuitPathData* PBCircuit::Unload(const SEElectricalCircuitPath& src)
{
  cdm::ElectricalCircuitPathData* dst = new cdm::ElectricalCircuitPathData();
  PBCircuit::Serialize(src, *dst);
  return dst;
}
void PBCircuit::Serialize(const SEElectricalCircuitPath& src, cdm::ElectricalCircuitPathData& dst)
{
  PBCircuit::Serialize(src, *dst.mutable_circuitpath());
  if (src.HasResistance())
    dst.set_allocated_resistance(SEScalarElectricResistance::Unload(*src.m_Resistance));
  if (src.HasNextResistance())
    dst.set_allocated_nextresistance(SEScalarElectricResistance::Unload(*src.m_NextResistance));
  if (src.HasResistanceBaseline())
    dst.set_allocated_resistancebaseline(SEScalarElectricResistance::Unload(*src.m_ResistanceBaseline));
  if (src.HasCapacitance())
    dst.set_allocated_capacitance(SEScalarElectricCapacitance::Unload(*src.m_Capacitance));
  if (src.HasNextCapacitance())
    dst.set_allocated_nextcapacitance(SEScalarElectricCapacitance::Unload(*src.m_NextCapacitance));
  if (src.HasCapacitanceBaseline())
    dst.set_allocated_capacitancebaseline(SEScalarElectricCapacitance::Unload(*src.m_CapacitanceBaseline));
  if (src.HasInductance())
    dst.set_allocated_inductance(SEScalarElectricInductance::Unload(*src.m_Inductance));
  if (src.HasNextInductance())
    dst.set_allocated_nextinductance(SEScalarElectricInductance::Unload(*src.m_NextInductance));
  if (src.HasInductanceBaseline())
    dst.set_allocated_inductancebaseline(SEScalarElectricInductance::Unload(*src.m_InductanceBaseline));
  if (src.HasCurrent())
    dst.set_allocated_current(SEScalarElectricCurrent::Unload(*src.m_Flux));
  if (src.HasNextCurrent())
    dst.set_allocated_nextcurrent(SEScalarElectricCurrent::Unload(*src.m_NextFlux));
  if (src.HasCurrentSource())
    dst.set_allocated_currentsource(SEScalarElectricCurrent::Unload(*src.m_FluxSource));
  if (src.HasNextCurrentSource())
    dst.set_allocated_nextcurrentsource(SEScalarElectricCurrent::Unload(*src.m_NextFluxSource));
  if (src.HasCurrentSourceBaseline())
    dst.set_allocated_currentsourcebaseline(SEScalarElectricCurrent::Unload(*src.m_FluxSourceBaseline));
  if (src.HasVoltageSource())
    dst.set_allocated_voltagesource(SEScalarElectricPotential::Unload(*src.m_PotentialSource));
  if (src.HasNextVoltageSource())
    dst.set_allocated_nextvoltagesource(SEScalarElectricPotential::Unload(*src.m_NextPotentialSource));
  if (src.HasVoltageSourceBaseline())
    dst.set_allocated_voltagesourcebaseline(SEScalarElectricPotential::Unload(*src.m_PotentialSourceBaseline));
  if (src.HasValveBreakdownVoltage())
    dst.set_allocated_valvebreakdownvoltage(SEScalarElectricPotential::Unload(*src.m_ValveBreakdownPotential));
}

////////////////////
// Fluid Circuits //
////////////////////

void PBCircuit::Load(const cdm::FluidCircuitNodeData& src, SEFluidCircuitNode& dst)
{
  PBCircuit::Serialize(src, dst);
}
void PBCircuit::Serialize(const cdm::FluidCircuitNodeData& src, SEFluidCircuitNode& dst)
{
  PBCircuit::Serialize(src.circuitnode(), dst);
  if (src.has_pressure())
    SEScalarPressure::Load(src.pressure(), dst.GetPressure());
  if (src.has_nextpressure())
    SEScalarPressure::Load(src.nextpressure(), dst.GetNextPressure());
  if (src.has_volume())
    SEScalarVolume::Load(src.volume(), dst.GetVolume());
  if (src.has_nextvolume())
    SEScalarVolume::Load(src.nextvolume(), dst.GetNextVolume());
  if (src.has_volumebaseline())
    SEScalarVolume::Load(src.volumebaseline(), dst.GetVolumeBaseline());
}

cdm::FluidCircuitNodeData* PBCircuit::Unload(const SEFluidCircuitNode& src)
{
  cdm::FluidCircuitNodeData* dst = new cdm::FluidCircuitNodeData();
  PBCircuit::Serialize(src, *dst);
  return dst;
}
void PBCircuit::Serialize(const SEFluidCircuitNode& src, cdm::FluidCircuitNodeData& dst)
{
  PBCircuit::Serialize(src, *dst.mutable_circuitnode());
  if (src.HasPressure())
    dst.set_allocated_pressure(SEScalarPressure::Unload(*src.m_Potential));
  if (src.HasNextPressure())
    dst.set_allocated_nextpressure(SEScalarPressure::Unload(*src.m_NextPotential));
  if (src.HasVolume())
    dst.set_allocated_volume(SEScalarVolume::Unload(*src.m_Quantity));
  if (src.HasNextVolume())
    dst.set_allocated_nextvolume(SEScalarVolume::Unload(*src.m_NextQuantity));
  if (src.HasVolumeBaseline())
    dst.set_allocated_volumebaseline(SEScalarVolume::Unload(*src.m_QuantityBaseline));
}

void PBCircuit::Load(const cdm::FluidCircuitPathData& src, SEFluidCircuitPath& dst)
{
  PBCircuit::Serialize(src, dst);
}
void PBCircuit::Serialize(const cdm::FluidCircuitPathData& src, SEFluidCircuitPath& dst)
{
  PBCircuit::Serialize(src.circuitpath(), dst);
  if (src.has_resistance())
    SEScalarFlowResistance::Load(src.resistance(), dst.GetResistance());
  if (src.has_nextresistance())
    SEScalarFlowResistance::Load(src.nextresistance(), dst.GetNextResistance());
  if (src.has_resistancebaseline())
    SEScalarFlowResistance::Load(src.resistancebaseline(), dst.GetResistanceBaseline());
  if (src.has_compliance())
    SEScalarFlowCompliance::Load(src.compliance(), dst.GetCompliance());
  if (src.has_nextcompliance())
    SEScalarFlowCompliance::Load(src.nextcompliance(), dst.GetNextCompliance());
  if (src.has_compliancebaseline())
    SEScalarFlowCompliance::Load(src.compliancebaseline(), dst.GetComplianceBaseline());
  if (src.has_inertance())
    SEScalarFlowInertance::Load(src.inertance(), dst.GetInertance());
  if (src.has_nextinertance())
    SEScalarFlowInertance::Load(src.nextinertance(), dst.GetNextInertance());
  if (src.has_inertancebaseline())
    SEScalarFlowInertance::Load(src.inertancebaseline(), dst.GetInertanceBaseline());
  if (src.has_flow())
    SEScalarVolumePerTime::Load(src.flow(), dst.GetFlow());
  if (src.has_nextflow())
    SEScalarVolumePerTime::Load(src.nextflow(), dst.GetNextFlow());
  if (src.has_flowsource())
    SEScalarVolumePerTime::Load(src.flowsource(), dst.GetFlowSource());
  if (src.has_nextflowsource())
    SEScalarVolumePerTime::Load(src.nextflowsource(), dst.GetNextFlowSource());
  if (src.has_flowsourcebaseline())
    SEScalarVolumePerTime::Load(src.flowsourcebaseline(), dst.GetFlowSourceBaseline());
  if (src.has_pressuresource())
    SEScalarPressure::Load(src.pressuresource(), dst.GetPressureSource());
  if (src.has_nextpressuresource())
    SEScalarPressure::Load(src.nextpressuresource(), dst.GetNextPressureSource());
  if (src.has_pressuresourcebaseline())
    SEScalarPressure::Load(src.pressuresourcebaseline(), dst.GetPressureSourceBaseline());
  if (src.has_valvebreakdownpressure())
    SEScalarPressure::Load(src.valvebreakdownpressure(), dst.GetValveBreakdownPressure());

  if (!dst.HasValidElements())
    dst.Warning("Path does not have valid elements");
}

cdm::FluidCircuitPathData* PBCircuit::Unload(const SEFluidCircuitPath& src)
{
  cdm::FluidCircuitPathData* dst = new cdm::FluidCircuitPathData();
  PBCircuit::Serialize(src, *dst);
  return dst;
}
void PBCircuit::Serialize(const SEFluidCircuitPath& src, cdm::FluidCircuitPathData& dst)
{
  PBCircuit::Serialize(src, *dst.mutable_circuitpath());
  if (src.HasResistance())
    dst.set_allocated_resistance(SEScalarFlowResistance::Unload(*src.m_Resistance));
  if (src.HasNextResistance())
    dst.set_allocated_nextresistance(SEScalarFlowResistance::Unload(*src.m_NextResistance));
  if (src.HasResistanceBaseline())
    dst.set_allocated_resistancebaseline(SEScalarFlowResistance::Unload(*src.m_ResistanceBaseline));
  if (src.HasCompliance())
    dst.set_allocated_compliance(SEScalarFlowCompliance::Unload(*src.m_Capacitance));
  if (src.HasNextCompliance())
    dst.set_allocated_nextcompliance(SEScalarFlowCompliance::Unload(*src.m_NextCapacitance));
  if (src.HasComplianceBaseline())
    dst.set_allocated_compliancebaseline(SEScalarFlowCompliance::Unload(*src.m_CapacitanceBaseline));
  if (src.HasInertance())
    dst.set_allocated_inertance(SEScalarFlowInertance::Unload(*src.m_Inductance));
  if (src.HasNextInertance())
    dst.set_allocated_nextinertance(SEScalarFlowInertance::Unload(*src.m_NextInductance));
  if (src.HasInertanceBaseline())
    dst.set_allocated_inertancebaseline(SEScalarFlowInertance::Unload(*src.m_InductanceBaseline));
  if (src.HasFlow())
    dst.set_allocated_flow(SEScalarVolumePerTime::Unload(*src.m_Flux));
  if (src.HasNextFlow())
    dst.set_allocated_nextflow(SEScalarVolumePerTime::Unload(*src.m_NextFlux));
  if (src.HasFlowSource())
    dst.set_allocated_flowsource(SEScalarVolumePerTime::Unload(*src.m_FluxSource));
  if (src.HasNextFlowSource())
    dst.set_allocated_nextflowsource(SEScalarVolumePerTime::Unload(*src.m_NextFluxSource));
  if (src.HasFlowSourceBaseline())
    dst.set_allocated_flowsourcebaseline(SEScalarVolumePerTime::Unload(*src.m_FluxSourceBaseline));
  if (src.HasPressureSource())
    dst.set_allocated_pressuresource(SEScalarPressure::Unload(*src.m_PotentialSource));
  if (src.HasNextPressureSource())
    dst.set_allocated_nextpressuresource(SEScalarPressure::Unload(*src.m_NextPotentialSource));
  if (src.HasPressureSourceBaseline())
    dst.set_allocated_pressuresourcebaseline(SEScalarPressure::Unload(*src.m_PotentialSourceBaseline));
  if (src.HasValveBreakdownPressure())
    dst.set_allocated_valvebreakdownpressure(SEScalarPressure::Unload(*src.m_ValveBreakdownPotential));
}

//////////////////////
// Thermal Circuits //
//////////////////////

void PBCircuit::Load(const cdm::ThermalCircuitNodeData& src, SEThermalCircuitNode& dst)
{
  PBCircuit::Serialize(src, dst);
}
void PBCircuit::Serialize(const cdm::ThermalCircuitNodeData& src, SEThermalCircuitNode& dst)
{
  PBCircuit::Serialize(src.circuitnode(), dst);
  if (src.has_temperature())
    SEScalarTemperature::Load(src.temperature(), dst.GetTemperature());
  if (src.has_nexttemperature())
    SEScalarTemperature::Load(src.nexttemperature(), dst.GetNextTemperature());
  if (src.has_heat())
    SEScalarEnergy::Load(src.heat(), dst.GetHeat());
  if (src.has_nextheat())
    SEScalarEnergy::Load(src.nextheat(), dst.GetNextHeat());
  if (src.has_heatbaseline())
    SEScalarEnergy::Load(src.heatbaseline(), dst.GetHeatBaseline());
}

cdm::ThermalCircuitNodeData* PBCircuit::Unload(const SEThermalCircuitNode& src)
{
  cdm::ThermalCircuitNodeData* dst = new cdm::ThermalCircuitNodeData();
  PBCircuit::Serialize(src, *dst);
  return dst;
}
void PBCircuit::Serialize(const SEThermalCircuitNode& src, cdm::ThermalCircuitNodeData& dst)
{
  PBCircuit::Serialize(src, *dst.mutable_circuitnode());
  if (src.HasTemperature())
    dst.set_allocated_temperature(SEScalarTemperature::Unload(*src.m_Potential));
  if (src.HasNextTemperature())
    dst.set_allocated_nexttemperature(SEScalarTemperature::Unload(*src.m_NextPotential));
  if (src.HasHeat())
    dst.set_allocated_heat(SEScalarEnergy::Unload(*src.m_Quantity));
  if (src.HasNextHeat())
    dst.set_allocated_nextheat(SEScalarEnergy::Unload(*src.m_NextQuantity));
  if (src.HasHeatBaseline())
    dst.set_allocated_heatbaseline(SEScalarEnergy::Unload(*src.m_QuantityBaseline));
}


void PBCircuit::Load(const cdm::ThermalCircuitPathData& src, SEThermalCircuitPath& dst)
{
  PBCircuit::Serialize(src, dst);
}
void PBCircuit::Serialize(const cdm::ThermalCircuitPathData& src, SEThermalCircuitPath& dst)
{
  PBCircuit::Serialize(src.circuitpath(), dst);
  if (src.has_resistance())
    SEScalarHeatResistance::Load(src.resistance(), dst.GetResistance());
  if (src.has_nextresistance())
    SEScalarHeatResistance::Load(src.nextresistance(), dst.GetNextResistance());
  if (src.has_resistancebaseline())
    SEScalarHeatResistance::Load(src.resistancebaseline(), dst.GetResistanceBaseline());
  if (src.has_capacitance())
    SEScalarHeatCapacitance::Load(src.capacitance(), dst.GetCapacitance());
  if (src.has_nextcapacitance())
    SEScalarHeatCapacitance::Load(src.nextcapacitance(), dst.GetNextCapacitance());
  if (src.has_capacitancebaseline())
    SEScalarHeatCapacitance::Load(src.capacitancebaseline(), dst.GetCapacitanceBaseline());
  if (src.has_inductance())
    SEScalarHeatInductance::Load(src.inductance(), dst.GetInductance());
  if (src.has_nextinductance())
    SEScalarHeatInductance::Load(src.nextinductance(), dst.GetNextInductance());
  if (src.has_inductancebaseline())
    SEScalarHeatInductance::Load(src.inductancebaseline(), dst.GetInductanceBaseline());
  if (src.has_heattransferrate())
    SEScalarPower::Load(src.heattransferrate(), dst.GetHeatTransferRate());
  if (src.has_nextheattransferrate())
    SEScalarPower::Load(src.nextheattransferrate(), dst.GetNextHeatTransferRate());
  if (src.has_heatsource())
    SEScalarPower::Load(src.heatsource(), dst.GetHeatSource());
  if (src.has_nextheatsource())
    SEScalarPower::Load(src.nextheatsource(), dst.GetNextHeatSource());
  if (src.has_heatsourcebaseline())
    SEScalarPower::Load(src.heatsourcebaseline(), dst.GetHeatSourceBaseline());
  if (src.has_temperaturesource())
    SEScalarTemperature::Load(src.temperaturesource(), dst.GetTemperatureSource());
  if (src.has_nexttemperaturesource())
    SEScalarTemperature::Load(src.nexttemperaturesource(), dst.GetNextTemperatureSource());
  if (src.has_temperaturesourcebaseline())
    SEScalarTemperature::Load(src.temperaturesourcebaseline(), dst.GetTemperatureSourceBaseline());
  if (src.has_valvebreakdowntemperature())
    SEScalarTemperature::Load(src.valvebreakdowntemperature(), dst.GetValveBreakdownTemperature());

  if (!dst.HasValidElements())
    dst.Warning("Path does not have valid elements");
}

cdm::ThermalCircuitPathData* PBCircuit::Unload(const SEThermalCircuitPath& src)
{
  cdm::ThermalCircuitPathData* dst = new cdm::ThermalCircuitPathData();
  PBCircuit::Serialize(src, *dst);
  return dst;
}
void PBCircuit::Serialize(const SEThermalCircuitPath& src, cdm::ThermalCircuitPathData& dst)
{
  PBCircuit::Serialize(src, *dst.mutable_circuitpath());
  if (src.HasResistance())
    dst.set_allocated_resistance(SEScalarHeatResistance::Unload(*src.m_Resistance));
  if (src.HasNextResistance())
    dst.set_allocated_nextresistance(SEScalarHeatResistance::Unload(*src.m_NextResistance));
  if (src.HasResistanceBaseline())
    dst.set_allocated_resistancebaseline(SEScalarHeatResistance::Unload(*src.m_ResistanceBaseline));
  if (src.HasCapacitance())
    dst.set_allocated_capacitance(SEScalarHeatCapacitance::Unload(*src.m_Capacitance));
  if (src.HasNextCapacitance())
    dst.set_allocated_nextcapacitance(SEScalarHeatCapacitance::Unload(*src.m_NextCapacitance));
  if (src.HasCapacitanceBaseline())
    dst.set_allocated_capacitancebaseline(SEScalarHeatCapacitance::Unload(*src.m_CapacitanceBaseline));
  if (src.HasInductance())
    dst.set_allocated_inductance(SEScalarHeatInductance::Unload(*src.m_Inductance));
  if (src.HasNextInductance())
    dst.set_allocated_nextinductance(SEScalarHeatInductance::Unload(*src.m_NextInductance));
  if (src.HasInductanceBaseline())
    dst.set_allocated_inductancebaseline(SEScalarHeatInductance::Unload(*src.m_InductanceBaseline));
  if (src.HasHeatTransferRate())
    dst.set_allocated_heattransferrate(SEScalarPower::Unload(*src.m_Flux));
  if (src.HasNextHeatTransferRate())
    dst.set_allocated_nextheattransferrate(SEScalarPower::Unload(*src.m_NextFlux));
  if (src.HasHeatSource())
    dst.set_allocated_heatsource(SEScalarPower::Unload(*src.m_FluxSource));
  if (src.HasNextHeatSource())
    dst.set_allocated_nextheatsource(SEScalarPower::Unload(*src.m_NextFluxSource));
  if (src.HasHeatSourceBaseline())
    dst.set_allocated_heatsourcebaseline(SEScalarPower::Unload(*src.m_FluxSourceBaseline));
  if (src.HasTemperatureSource())
    dst.set_allocated_temperaturesource(SEScalarTemperature::Unload(*src.m_PotentialSource));
  if (src.HasNextTemperatureSource())
    dst.set_allocated_nexttemperaturesource(SEScalarTemperature::Unload(*src.m_NextPotentialSource));
  if (src.HasTemperatureSourceBaseline())
    dst.set_allocated_temperaturesourcebaseline(SEScalarTemperature::Unload(*src.m_PotentialSourceBaseline));
  if (src.HasValveBreakdownTemperature())
    dst.set_allocated_valvebreakdowntemperature(SEScalarTemperature::Unload(*src.m_ValveBreakdownPotential));
}