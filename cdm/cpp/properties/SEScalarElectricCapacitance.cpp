/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "properties/SEScalarElectricCapacitance.h"

const ElectricCapacitanceUnit ElectricCapacitanceUnit::F("F");

bool ElectricCapacitanceUnit::IsValidUnit(const std::string& unit)
{
  if (F.GetString().compare(unit) == 0)
    return true;
  return false;
}

const ElectricCapacitanceUnit& ElectricCapacitanceUnit::GetCompoundUnit(const std::string& unit)
{
  if (F.GetString().compare(unit) == 0)
    return F;
  std::stringstream err;
  err << unit << " is not a valid ElectricCapacitance unit";
  throw CommonDataModelException(err.str());
}
