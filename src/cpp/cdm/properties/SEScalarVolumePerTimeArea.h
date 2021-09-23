/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalar.h"

class CDM_DECL VolumePerTimeAreaUnit : public CCompoundUnit
{
public:
  VolumePerTimeAreaUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~VolumePerTimeAreaUnit() {}

  // Please use static units below
  VolumePerTimeAreaUnit(const VolumePerTimeAreaUnit&) = delete;
  VolumePerTimeAreaUnit& operator= (const VolumePerTimeAreaUnit&) = delete;

  static bool IsValidUnit(const std::string& unit);
  static const VolumePerTimeAreaUnit& GetCompoundUnit(const std::string& unit);

  static const VolumePerTimeAreaUnit mL_Per_min_m2;
  static const VolumePerTimeAreaUnit mL_Per_s_m2;
  static const VolumePerTimeAreaUnit L_Per_min_m2;
};

class CDM_DECL SEScalarVolumePerTimeArea : public SEScalarQuantity<VolumePerTimeAreaUnit>
{
public:
  SEScalarVolumePerTimeArea() {}
  virtual ~SEScalarVolumePerTimeArea() {}
};
