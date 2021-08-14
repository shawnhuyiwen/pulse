/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalar.h"

class CDM_DECL EquivalentWeightPerVolumeUnit : public CCompoundUnit
{
public:
  EquivalentWeightPerVolumeUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~EquivalentWeightPerVolumeUnit() {}

  static bool IsValidUnit(const std::string& unit);
  static const EquivalentWeightPerVolumeUnit& GetCompoundUnit(const std::string& unit);

  static const EquivalentWeightPerVolumeUnit Eq_Per_L;
  static const EquivalentWeightPerVolumeUnit Eq_Per_mL;
  static const EquivalentWeightPerVolumeUnit mEq_Per_L;
  static const EquivalentWeightPerVolumeUnit mEq_Per_mL;
};

class CDM_DECL SEScalarEquivalentWeightPerVolume : public SEScalarQuantity<EquivalentWeightPerVolumeUnit>
{
public:
  SEScalarEquivalentWeightPerVolume() {}
  virtual ~SEScalarEquivalentWeightPerVolume() {}
};
