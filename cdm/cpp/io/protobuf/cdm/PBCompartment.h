/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "io/protobuf/cdm/PBCircuit.h"
#include "io/protobuf/cdm/PBSubstance.h"
// Forward Declaire SE and Protobuf Compartment Types
CDM_DECL_BIND(Compartment)
CDM_DECL_BIND(CompartmentLink)
CDM_DECL_BIND(CompartmentManager)
CDM_DECL_BIND(GasCompartment);
CDM_DECL_BIND(GasCompartmentGraph);
CDM_DECL_BIND(GasCompartmentLink);
CDM_DECL_BIND(LiquidCompartment);
CDM_DECL_BIND(LiquidCompartmentGraph);
CDM_DECL_BIND(LiquidCompartmentLink);
CDM_DECL_BIND(ThermalCompartment);
CDM_DECL_BIND(ThermalCompartmentLink);
CDM_DECL_BIND(TissueCompartment);
CDM_DECL_BIND(GasSubstanceQuantity);
CDM_DECL_BIND(LiquidSubstanceQuantity);
CDM_DECL_BIND(SubstanceQuantity);
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
public:
  static void Load(const cdm::CompartmentManagerData& src, SECompartmentManager& dst, SECircuitManager* circuits = nullptr);
  static cdm::CompartmentManagerData* Unload(const SECompartmentManager& src);
protected:
  static void Serialize(const cdm::CompartmentManagerData& src, SECompartmentManager& dst, SECircuitManager* circuits = nullptr);
  static void Serialize(const SECompartmentManager& src, cdm::CompartmentManagerData& dst);
  static void Serialize(const cdm::CompartmentData& src, SECompartment& dst);
  static void Serialize(const SECompartment& src, cdm::CompartmentData& dst);
  static void Serialize(const cdm::CompartmentLinkData& src, SECompartmentLink& dst);
  static void Serialize(const SECompartmentLink& src, cdm::CompartmentLinkData& dst);

  // Fluid (Liquid, Gas) Compartments
public:
  static void Load(const cdm::GasCompartmentData& src, SEGasCompartment& dst, SESubstanceManager& subMgr, SECircuitManager* circuits = nullptr);
  static cdm::GasCompartmentData* Unload(const SEGasCompartment& src);
  static void Load(const cdm::GasCompartmentGraphData& src, SEGasCompartmentGraph& dst, SECompartmentManager& cmptMgr);
  static cdm::GasCompartmentGraphData* Unload(const SEGasCompartmentGraph& src);
  static void Load(const cdm::GasCompartmentLinkData& src, SEGasCompartmentLink& dst, SECircuitManager* circuits = nullptr);
  static cdm::GasCompartmentLinkData* Unload(const SEGasCompartmentLink& src);
  static void Load(const cdm::LiquidCompartmentData& src, SELiquidCompartment& dst, SESubstanceManager& subMgr, SECircuitManager* circuits = nullptr);
  static cdm::LiquidCompartmentData* Unload(const SELiquidCompartment& src);
  static void Load(const cdm::LiquidCompartmentGraphData& src, SELiquidCompartmentGraph& dst, SECompartmentManager& cmptMgr);
  static cdm::LiquidCompartmentGraphData* Unload(const SELiquidCompartmentGraph& src);
  static void Load(const cdm::LiquidCompartmentLinkData& src, SELiquidCompartmentLink& dst, SECircuitManager* circuits = nullptr);
  static cdm::LiquidCompartmentLinkData* Unload(const SELiquidCompartmentLink& src);
protected:

  template<FLUID_COMPARTMENT_TEMPLATE>
  static void Serialize(const cdm::FluidCompartmentData& src, SEFluidCompartment<FLUID_COMPARTMENT_TYPES>& dst, SECircuitManager* circuits = nullptr);
  template<FLUID_COMPARTMENT_TEMPLATE>
  static void Serialize(const SEFluidCompartment<FLUID_COMPARTMENT_TYPES>& src, cdm::FluidCompartmentData& dst);
  template<FLUID_COMPARTMENT_LINK_TEMPLATE>
  static void Serialize(const cdm::FluidCompartmentLinkData& src, SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>& dst, SECircuitManager* circuits = nullptr);
  template<FLUID_COMPARTMENT_LINK_TEMPLATE>
  static void Serialize(const SEFluidCompartmentLink<FLUID_COMPARTMENT_LINK_TYPES>& src, cdm::FluidCompartmentLinkData& dst);

  static void Serialize(const cdm::GasCompartmentData& src, SEGasCompartment& dst, SESubstanceManager& subMgr, SECircuitManager* circuits = nullptr);
  static void Serialize(const SEGasCompartment& src, cdm::GasCompartmentData& dst);
  static void Serialize(const cdm::GasCompartmentGraphData& src, SEGasCompartmentGraph& dst, SECompartmentManager& cmptMgr);
  static void Serialize(const SEGasCompartmentGraph& src, cdm::GasCompartmentGraphData& dst);
  static void Serialize(const cdm::GasCompartmentLinkData& src, SEGasCompartmentLink& dst, SECircuitManager* circuits = nullptr);
  static void Serialize(const SEGasCompartmentLink& src, cdm::GasCompartmentLinkData& dst);
  static void Serialize(const cdm::LiquidCompartmentData& src, SELiquidCompartment& dst, SESubstanceManager& subMgr, SECircuitManager* circuits = nullptr);
  static void Serialize(const SELiquidCompartment& src, cdm::LiquidCompartmentData& dst);
  static void Serialize(const cdm::LiquidCompartmentGraphData& src, SELiquidCompartmentGraph& dst, SECompartmentManager& cmptMgr);
  static void Serialize(const SELiquidCompartmentGraph& src, cdm::LiquidCompartmentGraphData& dst);
  static void Serialize(const cdm::LiquidCompartmentLinkData& src, SELiquidCompartmentLink& dst, SECircuitManager* circuits = nullptr);
  static void Serialize(const SELiquidCompartmentLink& src, cdm::LiquidCompartmentLinkData& dst);

  // Thermal
public:
  static void Load(const cdm::ThermalCompartmentData& src, SEThermalCompartment& dst, SECircuitManager* circuits = nullptr);
  static cdm::ThermalCompartmentData* Unload(const SEThermalCompartment& src);
  static void Load(const cdm::ThermalCompartmentLinkData& src, SEThermalCompartmentLink& dst, SECircuitManager* circuits = nullptr);
  static cdm::ThermalCompartmentLinkData* Unload(const SEThermalCompartmentLink& src);
protected:
  static void Serialize(const cdm::ThermalCompartmentData& src, SEThermalCompartment& dst, SECircuitManager* circuits = nullptr);
  static void Serialize(const SEThermalCompartment& src, cdm::ThermalCompartmentData& dst);
  static void Serialize(const cdm::ThermalCompartmentLinkData& src, SEThermalCompartmentLink& dst, SECircuitManager* circuits = nullptr);
  static void Serialize(const SEThermalCompartmentLink& src, cdm::ThermalCompartmentLinkData& dst);

  // Tissue
public:
  static void Load(const cdm::TissueCompartmentData& src, SETissueCompartment& dst);
  static cdm::TissueCompartmentData* Unload(const SETissueCompartment& src);
protected:
  static void Serialize(const cdm::TissueCompartmentData& src, SETissueCompartment& dst);
  static void Serialize(const SETissueCompartment& src, cdm::TissueCompartmentData& dst);
  
  // Substance Quantities
public:
  static void Load(const cdm::LiquidSubstanceQuantityData& src, SELiquidSubstanceQuantity& dst);
  static cdm::LiquidSubstanceQuantityData* Unload(const SELiquidSubstanceQuantity& src);
  static void Load(const cdm::GasSubstanceQuantityData& src, SEGasSubstanceQuantity& dst);
  static cdm::GasSubstanceQuantityData* Unload(const SEGasSubstanceQuantity& src);
protected:
  static void Serialize(const cdm::SubstanceQuantityData& src, SESubstanceQuantity& dst);
  static void Serialize(const SESubstanceQuantity& src, cdm::SubstanceQuantityData& dst);
  static void Serialize(const cdm::LiquidSubstanceQuantityData& src, SELiquidSubstanceQuantity& dst);
  static void Serialize(const SELiquidSubstanceQuantity& src, cdm::LiquidSubstanceQuantityData& dst);
  static void Serialize(const cdm::GasSubstanceQuantityData& src, SEGasSubstanceQuantity& dst);
  static void Serialize(const SEGasSubstanceQuantity& src, cdm::GasSubstanceQuantityData& dst);
};