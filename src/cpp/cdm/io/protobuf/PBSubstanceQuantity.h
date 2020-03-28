/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL2(GasSubstanceQuantity);
CDM_BIND_DECL2(LiquidSubstanceQuantity);
CDM_BIND_DECL2(SubstanceQuantity);


class CDM_DECL PBSubstanceQuantity
{
public:

  static void Load(const CDM_BIND::LiquidSubstanceQuantityData& src, SELiquidSubstanceQuantity& dst);
  static CDM_BIND::LiquidSubstanceQuantityData* Unload(const SELiquidSubstanceQuantity& src);
  static void Load(const CDM_BIND::GasSubstanceQuantityData& src, SEGasSubstanceQuantity& dst);
  static CDM_BIND::GasSubstanceQuantityData* Unload(const SEGasSubstanceQuantity& src);
  static void Serialize(const CDM_BIND::SubstanceQuantityData& src, SESubstanceQuantity& dst);
  static void Serialize(const SESubstanceQuantity& src, CDM_BIND::SubstanceQuantityData& dst);
  static void Serialize(const CDM_BIND::LiquidSubstanceQuantityData& src, SELiquidSubstanceQuantity& dst);
  static void Serialize(const SELiquidSubstanceQuantity& src, CDM_BIND::LiquidSubstanceQuantityData& dst);
  static void Serialize(const CDM_BIND::GasSubstanceQuantityData& src, SEGasSubstanceQuantity& dst);
  static void Serialize(const SEGasSubstanceQuantity& src, CDM_BIND::GasSubstanceQuantityData& dst);
};