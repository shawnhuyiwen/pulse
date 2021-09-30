/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalar.h"

class CDM_DECL ElectricResistanceUnit : public CCompoundUnit
{
public:
  ElectricResistanceUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~ElectricResistanceUnit() {}

  // Please use static units below
  ElectricResistanceUnit(const ElectricResistanceUnit&) = delete;
  ElectricResistanceUnit& operator= (const ElectricResistanceUnit&) = delete;

  static bool IsValidUnit(const std::string& unit);
  static const ElectricResistanceUnit& GetCompoundUnit(const std::string& unit);

  static const ElectricResistanceUnit Ohm;
};

class CDM_DECL SEScalarElectricResistance : public SEScalarQuantity<ElectricResistanceUnit>
{
public:
  SEScalarElectricResistance() {}
  virtual ~SEScalarElectricResistance() {}
};
