/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "io/protobuf/PBCircuit.h"
#include "io/protobuf/PBSubstance.h"
// Forward Declaire SE and Protobuf Compartment Types
CDM_BIND_DECL2(Compartment)
CDM_BIND_DECL2(CompartmentLink)
CDM_BIND_DECL2(CompartmentManager)
CDM_BIND_DECL2(GasCompartment);
CDM_BIND_DECL2(GasCompartmentGraph);
CDM_BIND_DECL2(GasCompartmentLink);
CDM_BIND_DECL2(LiquidCompartment);
CDM_BIND_DECL2(LiquidCompartmentGraph);
CDM_BIND_DECL2(LiquidCompartmentLink);
CDM_BIND_DECL2(ThermalCompartment);
CDM_BIND_DECL2(ThermalCompartmentLink);
CDM_BIND_DECL2(TissueCompartment);
// Include Templates
#include "compartment/fluid/SEFluidCompartment.h"
#include "compartment/fluid/SEFluidCompartmentLink.h"
CDM_BIND_DECL(FluidCompartmentData);
CDM_BIND_DECL(FluidCompartmentLinkData);

class CDM_DECL PBCompartment
{
public:

  static bool LoadCompartmentManagerFile(SECompartmentManager& mgr, const std::string& filename, SECircuitManager* circuits);
  static void SaveCompartmentManagerFile(const SECompartmentManager& mgr, const std::string& filename);

  // Generic Compartments
  static void Load(const CDM_BIND::CompartmentManagerData& src, SECompartmentManager& dst, SECircuitManager* circuits = nullptr);
  static CDM_BIND::CompartmentManagerData* Unload(const SECompartmentManager& src);
  static void Serialize(const CDM_BIND::CompartmentManagerData& src, SECompartmentManager& dst, SECircuitManager* circuits = nullptr);
  static void Serialize(const SECompartmentManager& src, CDM_BIND::CompartmentManagerData& dst);
  static void Serialize(const CDM_BIND::CompartmentData& src, SECompartment& dst);
  static void Serialize(const SECompartment& src, CDM_BIND::CompartmentData& dst);
  static void Serialize(const CDM_BIND::CompartmentLinkData& src, SECompartmentLink& dst);
  static void Serialize(const SECompartmentLink& src, CDM_BIND::CompartmentLinkData& dst);

  // Fluid (Liquid, Gas) Compartments
  static void Load(const CDM_BIND::GasCompartmentData& src, SEGasCompartment& dst, SESubstanceManager& subMgr, SECircuitManager* circuits = nullptr);
  static CDM_BIND::GasCompartmentData* Unload(const SEGasCompartment& src);
  static void Load(const CDM_BIND::GasCompartmentGraphData& src, SEGasCompartmentGraph& dst, SECompartmentManager& cmptMgr);
  static CDM_BIND::GasCompartmentGraphData* Unload(const SEGasCompartmentGraph& src);
  static void Load(const CDM_BIND::GasCompartmentLinkData& src, SEGasCompartmentLink& dst, SECircuitManager* circuits = nullptr);
  static CDM_BIND::GasCompartmentLinkData* Unload(const SEGasCompartmentLink& src);
  static void Load(const CDM_BIND::LiquidCompartmentData& src, SELiquidCompartment& dst, SESubstanceManager& subMgr, SECircuitManager* circuits = nullptr);
  static CDM_BIND::LiquidCompartmentData* Unload(const SELiquidCompartment& src);
  static void Load(const CDM_BIND::LiquidCompartmentGraphData& src, SELiquidCompartmentGraph& dst, SECompartmentManager& cmptMgr);
  static CDM_BIND::LiquidCompartmentGraphData* Unload(const SELiquidCompartmentGraph& src);
  static void Load(const CDM_BIND::LiquidCompartmentLinkData& src, SELiquidCompartmentLink& dst, SECircuitManager* circuits = nullptr);
  static CDM_BIND::LiquidCompartmentLinkData* Unload(const SELiquidCompartmentLink& src);

  template<FLUID_COMPARTMENT_TEMPLATE>
  static void Serialize(const CDM_BIND::FluidCompartmentData& src, SEFluidCompartment<FLUID_COMPARTMENT_TYPES>& dst, SECircuitManager* circuits = nullptr);
  template<FLUID_COMPARTMENT_TEMPLATE>
  static void Serialize(const SEFluidCompartment<FLUID_COMPARTMENT_TYPES>& src, CDM_BIND::FluidCompartmentData& dst);
  template<FLUID_COMPARTMENT_LINK_TEMPLATE>
  static void Serialize(const CDM_BIND::FluidCompartmentLinkData& src, SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>& dst, SECircuitManager* circuits = nullptr);
  template<FLUID_COMPARTMENT_LINK_TEMPLATE>
  static void Serialize(const SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>& src, CDM_BIND::FluidCompartmentLinkData& dst);

  static void Serialize(const CDM_BIND::GasCompartmentData& src, SEGasCompartment& dst, SESubstanceManager& subMgr, SECircuitManager* circuits = nullptr);
  static void Serialize(const SEGasCompartment& src, CDM_BIND::GasCompartmentData& dst);
  static void Serialize(const CDM_BIND::GasCompartmentGraphData& src, SEGasCompartmentGraph& dst, SECompartmentManager& cmptMgr);
  static void Serialize(const SEGasCompartmentGraph& src, CDM_BIND::GasCompartmentGraphData& dst);
  static void Serialize(const CDM_BIND::GasCompartmentLinkData& src, SEGasCompartmentLink& dst, SECircuitManager* circuits = nullptr);
  static void Serialize(const SEGasCompartmentLink& src, CDM_BIND::GasCompartmentLinkData& dst);
  static void Serialize(const CDM_BIND::LiquidCompartmentData& src, SELiquidCompartment& dst, SESubstanceManager& subMgr, SECircuitManager* circuits = nullptr);
  static void Serialize(const SELiquidCompartment& src, CDM_BIND::LiquidCompartmentData& dst);
  static void Serialize(const CDM_BIND::LiquidCompartmentGraphData& src, SELiquidCompartmentGraph& dst, SECompartmentManager& cmptMgr);
  static void Serialize(const SELiquidCompartmentGraph& src, CDM_BIND::LiquidCompartmentGraphData& dst);
  static void Serialize(const CDM_BIND::LiquidCompartmentLinkData& src, SELiquidCompartmentLink& dst, SECircuitManager* circuits = nullptr);
  static void Serialize(const SELiquidCompartmentLink& src, CDM_BIND::LiquidCompartmentLinkData& dst);

  // Thermal
  static void Load(const CDM_BIND::ThermalCompartmentData& src, SEThermalCompartment& dst, SECircuitManager* circuits = nullptr);
  static CDM_BIND::ThermalCompartmentData* Unload(const SEThermalCompartment& src);
  static void Load(const CDM_BIND::ThermalCompartmentLinkData& src, SEThermalCompartmentLink& dst, SECircuitManager* circuits = nullptr);
  static CDM_BIND::ThermalCompartmentLinkData* Unload(const SEThermalCompartmentLink& src);
  static void Serialize(const CDM_BIND::ThermalCompartmentData& src, SEThermalCompartment& dst, SECircuitManager* circuits = nullptr);
  static void Serialize(const SEThermalCompartment& src, CDM_BIND::ThermalCompartmentData& dst);
  static void Serialize(const CDM_BIND::ThermalCompartmentLinkData& src, SEThermalCompartmentLink& dst, SECircuitManager* circuits = nullptr);
  static void Serialize(const SEThermalCompartmentLink& src, CDM_BIND::ThermalCompartmentLinkData& dst);

  // Tissue
  static void Load(const CDM_BIND::TissueCompartmentData& src, SETissueCompartment& dst);
  static CDM_BIND::TissueCompartmentData* Unload(const SETissueCompartment& src);
  static void Serialize(const CDM_BIND::TissueCompartmentData& src, SETissueCompartment& dst);
  static void Serialize(const SETissueCompartment& src, CDM_BIND::TissueCompartmentData& dst);
};