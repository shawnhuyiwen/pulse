/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_DECL_BIND(GasSubstanceQuantity);
CDM_DECL_BIND(LiquidSubstanceQuantity);
CDM_DECL_BIND(SubstanceQuantity);


class CDM_DECL PBSubstanceQuantity
{
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