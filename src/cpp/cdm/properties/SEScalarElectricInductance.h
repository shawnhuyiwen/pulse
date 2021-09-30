/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalar.h"

class CDM_DECL ElectricInductanceUnit : public CCompoundUnit
{
public:
  ElectricInductanceUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~ElectricInductanceUnit() {}

  // Please use static units below
  ElectricInductanceUnit(const ElectricInductanceUnit&) = delete;
  ElectricInductanceUnit& operator= (const ElectricInductanceUnit&) = delete;

  static bool IsValidUnit(const std::string& unit);
  static const ElectricInductanceUnit& GetCompoundUnit(const std::string& unit);

  static const ElectricInductanceUnit H;
};

class CDM_DECL SEScalarElectricInductance : public SEScalarQuantity<ElectricInductanceUnit>
{
public:
  SEScalarElectricInductance() {}
  virtual ~SEScalarElectricInductance() {}
};
