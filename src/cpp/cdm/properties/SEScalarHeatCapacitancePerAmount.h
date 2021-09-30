/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalar.h"

class CDM_DECL HeatCapacitancePerAmountUnit : public CCompoundUnit
{
public:
  HeatCapacitancePerAmountUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~HeatCapacitancePerAmountUnit() {}

  // Please use static units below
  HeatCapacitancePerAmountUnit(const HeatCapacitancePerAmountUnit&) = delete;
  HeatCapacitancePerAmountUnit& operator= (const HeatCapacitancePerAmountUnit&) = delete;

  static bool IsValidUnit(const std::string& unit);
  static const HeatCapacitancePerAmountUnit& GetCompoundUnit(const std::string& unit);
  
  static const HeatCapacitancePerAmountUnit J_Per_K_mol;
};

class CDM_DECL SEScalarHeatCapacitancePerAmount : public SEScalarQuantity<HeatCapacitancePerAmountUnit>
{
public:
  SEScalarHeatCapacitancePerAmount() {}
  virtual ~SEScalarHeatCapacitancePerAmount() {}
};