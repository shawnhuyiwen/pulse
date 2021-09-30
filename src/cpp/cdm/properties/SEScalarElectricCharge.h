/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalar.h"

class CDM_DECL ElectricChargeUnit : public CCompoundUnit
{
public:
  ElectricChargeUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~ElectricChargeUnit() {}

  // Please use static units below
  ElectricChargeUnit(const ElectricChargeUnit&) = delete;
  ElectricChargeUnit& operator= (const ElectricChargeUnit&) = delete;

  static bool IsValidUnit(const std::string& unit);
  static const ElectricChargeUnit& GetCompoundUnit(const std::string& unit);

  static const ElectricChargeUnit C;
};

class CDM_DECL SEScalarElectricCharge : public SEScalarQuantity<ElectricChargeUnit>
{
public:
  SEScalarElectricCharge() {}
  virtual ~SEScalarElectricCharge() {}
};
