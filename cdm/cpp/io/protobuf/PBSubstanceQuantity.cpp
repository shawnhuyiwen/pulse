/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/PBSubstanceQuantity.h"
#include "io/protobuf/PBProperties.h"
#include "substance/SESubstance.h"
#include "compartment/substances/SEGasSubstanceQuantity.h"
#include "compartment/substances/SELiquidSubstanceQuantity.h"
#include "compartment/fluid/SEGasCompartment.h"
#include "compartment/fluid/SELiquidCompartment.h"
#include "bind/cdm/SubstanceQuantity.pb.h"

void PBSubstanceQuantity::Serialize(const cdm::SubstanceQuantityData& src, SESubstanceQuantity& dst)
{
  dst.Clear();
  // Name will be used to get substance for ctor
}
void PBSubstanceQuantity::Serialize(const SESubstanceQuantity& src, cdm::SubstanceQuantityData& dst)
{
  dst.set_substance(src.m_Substance.GetName());
}

void PBSubstanceQuantity::Load(const cdm::GasSubstanceQuantityData& src, SEGasSubstanceQuantity& dst)
{
  PBSubstanceQuantity::Serialize(src, dst);
}
void PBSubstanceQuantity::Serialize(const cdm::GasSubstanceQuantityData& src, SEGasSubstanceQuantity& dst)
{
  PBSubstanceQuantity::Serialize(src.substancequantity(), dst);
  if (!dst.m_Compartment.HasChildren())
  {
    if (src.has_partialpressure())
      PBProperty::Load(src.partialpressure(), dst.GetPartialPressure());
    if (src.has_volume())
      PBProperty::Load(src.volume(), dst.GetVolume());
    if (src.has_volumefraction())
      PBProperty::Load(src.volumefraction(), dst.GetVolumeFraction());
  }
}

cdm::GasSubstanceQuantityData* PBSubstanceQuantity::Unload(const SEGasSubstanceQuantity& src)
{
  cdm::GasSubstanceQuantityData* dst = new cdm::GasSubstanceQuantityData();
  PBSubstanceQuantity::Serialize(src, *dst);
  return dst;
}
void PBSubstanceQuantity::Serialize(const SEGasSubstanceQuantity& src, cdm::GasSubstanceQuantityData& dst)
{
  PBSubstanceQuantity::Serialize(src, *dst.mutable_substancequantity());
  // Even if you have children, I am unloading everything, this makes the output actually usefull...
  if (src.HasPartialPressure())
    dst.set_allocated_partialpressure(PBProperty::Unload(*src.m_PartialPressure));
  if (src.HasVolume())
    dst.set_allocated_volume(PBProperty::Unload(*src.m_Volume));
  if (src.HasVolumeFraction())
    dst.set_allocated_volumefraction(PBProperty::Unload(*src.m_VolumeFraction));
}


void PBSubstanceQuantity::Load(const cdm::LiquidSubstanceQuantityData& src, SELiquidSubstanceQuantity& dst)
{
  PBSubstanceQuantity::Serialize(src, dst);
}
void PBSubstanceQuantity::Serialize(const cdm::LiquidSubstanceQuantityData& src, SELiquidSubstanceQuantity& dst)
{
  PBSubstanceQuantity::Serialize(src.substancequantity(), dst);
  if (!dst.m_Compartment.HasChildren())
  {
    if (src.has_concentration())
      PBProperty::Load(src.concentration(), dst.GetConcentration());
    if (src.has_mass())
      PBProperty::Load(src.mass(), dst.GetMass());
    if (src.has_masscleared())
      PBProperty::Load(src.masscleared(), dst.GetMassCleared());
    if (src.has_massdeposited())
      PBProperty::Load(src.massdeposited(), dst.GetMassDeposited());
    if (src.has_massexcreted())
      PBProperty::Load(src.massexcreted(), dst.GetMassExcreted());
    if (src.has_molarity())
      PBProperty::Load(src.molarity(), dst.GetMolarity());
    if (src.has_partialpressure())
      PBProperty::Load(src.partialpressure(), dst.GetPartialPressure());
    if (src.has_saturation())
      PBProperty::Load(src.saturation(), dst.GetSaturation());
  }
}

cdm::LiquidSubstanceQuantityData* PBSubstanceQuantity::Unload(const SELiquidSubstanceQuantity& src)
{
  cdm::LiquidSubstanceQuantityData* dst = new cdm::LiquidSubstanceQuantityData();
  PBSubstanceQuantity::Serialize(src, *dst);
  return dst;
}
void PBSubstanceQuantity::Serialize(const SELiquidSubstanceQuantity& src, cdm::LiquidSubstanceQuantityData& dst)
{
  PBSubstanceQuantity::Serialize(src, *dst.mutable_substancequantity());
  // Even if you have children, I am unloading everything, this makes the output actually usefull...
  if (src.HasConcentration())
    dst.set_allocated_concentration(PBProperty::Unload(*src.m_Concentration));
  if (src.HasMass())
    dst.set_allocated_mass(PBProperty::Unload(*src.m_Mass));
  if (src.HasMassCleared())
    dst.set_allocated_masscleared(PBProperty::Unload(*src.m_MassCleared));
  if (src.HasMassDeposited())
    dst.set_allocated_massdeposited(PBProperty::Unload(*src.m_MassDeposited));
  if (src.HasMassExcreted())
    dst.set_allocated_massexcreted(PBProperty::Unload(*src.m_MassExcreted));
  if (src.HasMolarity())
    dst.set_allocated_molarity(PBProperty::Unload(*src.m_Molarity));
  if (src.HasPartialPressure())
    dst.set_allocated_partialpressure(PBProperty::Unload(*src.m_PartialPressure));
  if (src.HasSaturation())
    dst.set_allocated_saturation(PBProperty::Unload(*src.m_Saturation));
}