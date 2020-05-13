/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

// Forward Declaire SE and Protobuf Circuit Types
CDM_BIND_DECL2(CircuitManager)
CDM_BIND_DECL2(ElectricalCircuit)
CDM_BIND_DECL2(ElectricalCircuitNode)
CDM_BIND_DECL2(ElectricalCircuitPath)
CDM_BIND_DECL2(FluidCircuit)
CDM_BIND_DECL2(FluidCircuitNode)
CDM_BIND_DECL2(FluidCircuitPath)
CDM_BIND_DECL2(ThermalCircuit)
CDM_BIND_DECL2(ThermalCircuitNode)
CDM_BIND_DECL2(ThermalCircuitPath)
// Include Templates
#include "circuit/SECircuit.h"
#include "circuit/SECircuitNode.h"
#include "circuit/SECircuitPath.h"
CDM_BIND_DECL(CircuitData)
CDM_BIND_DECL(CircuitNodeData)
CDM_BIND_DECL(CircuitPathData)

class CDM_DECL PBCircuit
{
public:

  static bool LoadCircuitManagerFile(SECircuitManager& mgr, const std::string& filename);
  static void SaveCircuitManagerFile(const SECircuitManager& mgr, const std::string& filename);

  // General Circuit
  static void Load(const CDM_BIND::CircuitManagerData& src, SECircuitManager& dst);
  static CDM_BIND::CircuitManagerData* Unload(const SECircuitManager& src);

  template<typename CircuitBindType, CIRCUIT_TEMPLATE>
  static void Load(const CircuitBindType& src, SECircuit<CIRCUIT_TYPES>& dst, const std::map<std::string, NodeType*>& nodes, const std::map<std::string, PathType*>& paths);
  template <typename CircuitBindType, CIRCUIT_TEMPLATE>
  static CircuitBindType* Unload(const SECircuit<CIRCUIT_TYPES>& src);

  static void Serialize(const CDM_BIND::CircuitManagerData& src, SECircuitManager& dst);
  static void Serialize(const SECircuitManager& src, CDM_BIND::CircuitManagerData& dst);

  template<typename CircuitBindType, CIRCUIT_TEMPLATE>
  static void Serialize(const CircuitBindType& src, SECircuit<CIRCUIT_TYPES>& dst, const std::map<std::string, NodeType*>& nodes, const std::map<std::string, PathType*>& paths);
  template<typename CircuitBindType, CIRCUIT_TEMPLATE>
  static void Serialize(const SECircuit<CIRCUIT_TYPES>& src, CircuitBindType& dst);
  template<CIRCUIT_NODE_TEMPLATE>
  static void Serialize(const CDM_BIND::CircuitNodeData& src, SECircuitNode<CIRCUIT_NODE_TYPES>& dst);
  template<CIRCUIT_NODE_TEMPLATE>
  static void Serialize(const SECircuitNode<CIRCUIT_NODE_TYPES>& src, CDM_BIND::CircuitNodeData& dst);
  template<CIRCUIT_PATH_TEMPLATE>
  static void Serialize(const CDM_BIND::CircuitPathData& src, SECircuitPath<CIRCUIT_PATH_TYPES>& dst);
  template<CIRCUIT_PATH_TEMPLATE>
  static void Serialize(const SECircuitPath<CIRCUIT_PATH_TYPES>& src, CDM_BIND::CircuitPathData& dst);

  // Electrical
  static void Load(const CDM_BIND::ElectricalCircuitPathData& src, SEElectricalCircuitPath& dst);
  static CDM_BIND::ElectricalCircuitPathData* Unload(const SEElectricalCircuitPath& src);
  static void Load(const CDM_BIND::ElectricalCircuitNodeData& src, SEElectricalCircuitNode& dst);
  static CDM_BIND::ElectricalCircuitNodeData* Unload(const SEElectricalCircuitNode& src);
  static void Serialize(const CDM_BIND::ElectricalCircuitPathData& src, SEElectricalCircuitPath& dst);
  static void Serialize(const SEElectricalCircuitPath& src, CDM_BIND::ElectricalCircuitPathData& dst);
  static void Serialize(const CDM_BIND::ElectricalCircuitNodeData& src, SEElectricalCircuitNode& dst);
  static void Serialize(const SEElectricalCircuitNode& src, CDM_BIND::ElectricalCircuitNodeData& dst);

  // Fluid
  static void Load(const CDM_BIND::FluidCircuitPathData& src, SEFluidCircuitPath& dst);
  static CDM_BIND::FluidCircuitPathData* Unload(const SEFluidCircuitPath& src);
  static void Load(const CDM_BIND::FluidCircuitNodeData& src, SEFluidCircuitNode& dst);
  static CDM_BIND::FluidCircuitNodeData* Unload(const SEFluidCircuitNode& src);
  static void Serialize(const CDM_BIND::FluidCircuitPathData& src, SEFluidCircuitPath& dst);
  static void Serialize(const SEFluidCircuitPath& src, CDM_BIND::FluidCircuitPathData& dst);
  static void Serialize(const CDM_BIND::FluidCircuitNodeData& src, SEFluidCircuitNode& dst);
  static void Serialize(const SEFluidCircuitNode& src, CDM_BIND::FluidCircuitNodeData& dst);

  // Thermal
  static void Load(const CDM_BIND::ThermalCircuitNodeData& src, SEThermalCircuitNode& dst);
  static CDM_BIND::ThermalCircuitNodeData* Unload(const SEThermalCircuitNode& src);
  static void Load(const CDM_BIND::ThermalCircuitPathData& src, SEThermalCircuitPath& dst);
  static CDM_BIND::ThermalCircuitPathData* Unload(const SEThermalCircuitPath& src);
  static void Serialize(const CDM_BIND::ThermalCircuitNodeData& src, SEThermalCircuitNode& dst);
  static void Serialize(const SEThermalCircuitNode& src, CDM_BIND::ThermalCircuitNodeData& dst);
  static void Serialize(const CDM_BIND::ThermalCircuitPathData& src, SEThermalCircuitPath& dst);
  static void Serialize(const SEThermalCircuitPath& src, CDM_BIND::ThermalCircuitPathData& dst);
};