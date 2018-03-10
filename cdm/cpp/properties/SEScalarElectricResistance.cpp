/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "properties/SEScalarElectricResistance.h"
PROTO_PUSH
#include "bind/cdm/Properties.pb.h"
PROTO_POP

const ElectricResistanceUnit ElectricResistanceUnit::Ohm("ohm");

bool ElectricResistanceUnit::IsValidUnit(const std::string& unit)
{
  if (Ohm.GetString().compare(unit) == 0)
    return true;
  return false;
}

const ElectricResistanceUnit& ElectricResistanceUnit::GetCompoundUnit(const std::string& unit)
{
  if (Ohm.GetString().compare(unit) == 0)
    return Ohm;
  std::stringstream err;
  err << unit << " is not a valid ElectricResistance unit";
  throw CommonDataModelException(err.str());
}

void SEScalarElectricResistance::Load(const cdm::ScalarElectricResistanceData& src, SEScalarElectricResistance& dst)
{
  SEScalarElectricResistance::Serialize(src, dst);
}
void SEScalarElectricResistance::Serialize(const cdm::ScalarElectricResistanceData& src, SEScalarElectricResistance& dst)
{
  SEUnitScalar::Serialize(src.scalarelectricresistance(), dst);
}

cdm::ScalarElectricResistanceData* SEScalarElectricResistance::Unload(const SEScalarElectricResistance& src)
{
  if (!src.IsValid())
    return nullptr;
  cdm::ScalarElectricResistanceData* dst = new cdm::ScalarElectricResistanceData();
  Serialize(src, *dst);
  return dst;
}
void SEScalarElectricResistance::Serialize(const SEScalarElectricResistance& src, cdm::ScalarElectricResistanceData& dst)
{
  SEUnitScalar::Serialize(src, *dst.mutable_scalarelectricresistance());
}