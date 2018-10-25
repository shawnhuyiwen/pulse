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
  static void Load(const cdm::CircuitManagerData& src, SECircuitManager& dst);
  static cdm::CircuitManagerData* Unload(const SECircuitManager& src);

  template<typename CircuitBindType, CIRCUIT_TEMPLATE>
  static void Load(const CircuitBindType& src, SECircuit<CIRCUIT_TYPES>& dst, const std::map<std::string, NodeType*>& nodes, const std::map<std::string, PathType*>& paths);
  template <typename CircuitBindType, CIRCUIT_TEMPLATE>
  static CircuitBindType* Unload(const SECircuit<CIRCUIT_TYPES>& src);

  static void Serialize(const cdm::CircuitManagerData& src, SECircuitManager& dst);
  static void Serialize(const SECircuitManager& src, cdm::CircuitManagerData& dst);

  template<typename CircuitBindType, CIRCUIT_TEMPLATE>
  static void Serialize(const CircuitBindType& src, SECircuit<CIRCUIT_TYPES>& dst, const std::map<std::string, NodeType*>& nodes, const std::map<std::string, PathType*>& paths);
  template<typename CircuitBindType, CIRCUIT_TEMPLATE>
  static void Serialize(const SECircuit<CIRCUIT_TYPES>& src, CircuitBindType& dst);
  template<CIRCUIT_NODE_TEMPLATE>
  static void Serialize(const cdm::CircuitNodeData& src, SECircuitNode<CIRCUIT_NODE_TYPES>& dst);
  template<CIRCUIT_NODE_TEMPLATE>
  static void Serialize(const SECircuitNode<CIRCUIT_NODE_TYPES>& src, cdm::CircuitNodeData& dst);
  template<CIRCUIT_PATH_TEMPLATE>
  static void Serialize(const cdm::CircuitPathData& src, SECircuitPath<CIRCUIT_PATH_TYPES>& dst);
  template<CIRCUIT_PATH_TEMPLATE>
  static void Serialize(const SECircuitPath<CIRCUIT_PATH_TYPES>& src, cdm::CircuitPathData& dst);

  // Electrical
  static void Load(const cdm::ElectricalCircuitPathData& src, SEElectricalCircuitPath& dst);
  static cdm::ElectricalCircuitPathData* Unload(const SEElectricalCircuitPath& src);
  static void Load(const cdm::ElectricalCircuitNodeData& src, SEElectricalCircuitNode& dst);
  static cdm::ElectricalCircuitNodeData* Unload(const SEElectricalCircuitNode& src);
  static void Serialize(const cdm::ElectricalCircuitPathData& src, SEElectricalCircuitPath& dst);
  static void Serialize(const SEElectricalCircuitPath& src, cdm::ElectricalCircuitPathData& dst);
  static void Serialize(const cdm::ElectricalCircuitNodeData& src, SEElectricalCircuitNode& dst);
  static void Serialize(const SEElectricalCircuitNode& src, cdm::ElectricalCircuitNodeData& dst);

  // Fluid
  static void Load(const cdm::FluidCircuitPathData& src, SEFluidCircuitPath& dst);
  static cdm::FluidCircuitPathData* Unload(const SEFluidCircuitPath& src);
  static void Load(const cdm::FluidCircuitNodeData& src, SEFluidCircuitNode& dst);
  static cdm::FluidCircuitNodeData* Unload(const SEFluidCircuitNode& src);
  static void Serialize(const cdm::FluidCircuitPathData& src, SEFluidCircuitPath& dst);
  static void Serialize(const SEFluidCircuitPath& src, cdm::FluidCircuitPathData& dst);
  static void Serialize(const cdm::FluidCircuitNodeData& src, SEFluidCircuitNode& dst);
  static void Serialize(const SEFluidCircuitNode& src, cdm::FluidCircuitNodeData& dst);

  // Thermal
  static void Load(const cdm::ThermalCircuitNodeData& src, SEThermalCircuitNode& dst);
  static cdm::ThermalCircuitNodeData* Unload(const SEThermalCircuitNode& src);
  static void Load(const cdm::ThermalCircuitPathData& src, SEThermalCircuitPath& dst);
  static cdm::ThermalCircuitPathData* Unload(const SEThermalCircuitPath& src);
  static void Serialize(const cdm::ThermalCircuitNodeData& src, SEThermalCircuitNode& dst);
  static void Serialize(const SEThermalCircuitNode& src, cdm::ThermalCircuitNodeData& dst);
  static void Serialize(const cdm::ThermalCircuitPathData& src, SEThermalCircuitPath& dst);
  static void Serialize(const SEThermalCircuitPath& src, cdm::ThermalCircuitPathData& dst);
};