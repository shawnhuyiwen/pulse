/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "properties/SEScalar.h"

class CDM_DECL ElectricCurrentUnit : public CCompoundUnit
{
public:
  ElectricCurrentUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~ElectricCurrentUnit() {}

  static bool IsValidUnit(const std::string& unit);
  static const ElectricCurrentUnit& GetCompoundUnit(const std::string& unit);

  static const ElectricCurrentUnit A;
};

class CDM_DECL SEScalarElectricCurrent : public SEScalarQuantity<ElectricCurrentUnit>
{
public:
  SEScalarElectricCurrent() {}
  virtual ~SEScalarElectricCurrent() {}
};
