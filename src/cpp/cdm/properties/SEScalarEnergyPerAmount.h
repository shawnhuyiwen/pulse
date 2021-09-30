/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalar.h"

class CDM_DECL EnergyPerAmountUnit : public CCompoundUnit
{
public:
  EnergyPerAmountUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~EnergyPerAmountUnit() {}

  // Please use static units below
  EnergyPerAmountUnit(const EnergyPerAmountUnit&) = delete;
  EnergyPerAmountUnit& operator= (const EnergyPerAmountUnit&) = delete;

  static bool IsValidUnit(const std::string& unit);
  static const EnergyPerAmountUnit& GetCompoundUnit(const std::string& unit);

  static const EnergyPerAmountUnit kcal_Per_mol;
  static const EnergyPerAmountUnit kJ_Per_mol;
};

class CDM_DECL SEScalarEnergyPerAmount : public SEScalarQuantity<EnergyPerAmountUnit>
{
public:
  SEScalarEnergyPerAmount() {}
  virtual ~SEScalarEnergyPerAmount() {}
};
