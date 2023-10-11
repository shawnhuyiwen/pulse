/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalar.h"

class CDM_DECL AmountPerVolumeUnit : public CCompoundUnit
{
public:
  AmountPerVolumeUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~AmountPerVolumeUnit() {}

  // Please use static units below
  AmountPerVolumeUnit(const AmountPerVolumeUnit&) = delete;
  AmountPerVolumeUnit& operator= (const AmountPerVolumeUnit&) = delete;

  static bool IsValidUnit(const std::string& unit);
  static const AmountPerVolumeUnit& GetCompoundUnit(const std::string& unit);

  static AmountPerVolumeUnit mEq_Per_L;
  static AmountPerVolumeUnit mol_Per_L;
  static AmountPerVolumeUnit mol_Per_mL;
  static AmountPerVolumeUnit mmol_Per_L;
  static AmountPerVolumeUnit mmol_Per_mL;
  static AmountPerVolumeUnit pmol_Per_L;
  static AmountPerVolumeUnit ct_Per_L;
  static AmountPerVolumeUnit ct_Per_uL;
};

class CDM_DECL SEScalarAmountPerVolume : public SEScalarQuantity<AmountPerVolumeUnit>
{
public:
  SEScalarAmountPerVolume() {}
  virtual ~SEScalarAmountPerVolume() {}
};
