/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalar.h"

class CDM_DECL EnergyUnit : public CCompoundUnit
{
public:
  EnergyUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~EnergyUnit() {}

  // Please use static units below
  EnergyUnit(const EnergyUnit&) = delete;
  EnergyUnit& operator= (const EnergyUnit&) = delete;

  static bool IsValidUnit(const std::string& unit);
  static const EnergyUnit& GetCompoundUnit(const std::string& unit);

  static const EnergyUnit J;
  static const EnergyUnit mJ;
  static const EnergyUnit kJ;
  static const EnergyUnit kcal;
};

class CDM_DECL SEScalarEnergy : public SEScalarQuantity<EnergyUnit>
{
public:
  SEScalarEnergy() {}
  virtual ~SEScalarEnergy() {}
};
