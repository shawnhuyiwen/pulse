/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalar.h"

class CDM_DECL ElectricCapacitanceUnit : public CCompoundUnit
{
public:
  ElectricCapacitanceUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~ElectricCapacitanceUnit() {}

  // Please use static units below
  ElectricCapacitanceUnit(const ElectricCapacitanceUnit&) = delete;
  ElectricCapacitanceUnit& operator= (const ElectricCapacitanceUnit&) = delete;

  static bool IsValidUnit(const std::string& unit);
  static const ElectricCapacitanceUnit& GetCompoundUnit(const std::string& unit);

  static const ElectricCapacitanceUnit F;
};

class CDM_DECL SEScalarElectricCapacitance : public SEScalarQuantity<ElectricCapacitanceUnit>
{
public:
  SEScalarElectricCapacitance(){}
  virtual ~SEScalarElectricCapacitance() {}
};
