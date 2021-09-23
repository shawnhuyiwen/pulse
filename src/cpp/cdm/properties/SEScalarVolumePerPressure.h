/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalar.h"

class CDM_DECL VolumePerPressureUnit : public CCompoundUnit
{
public:
  VolumePerPressureUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~VolumePerPressureUnit() {}

  // Please use static units below
  VolumePerPressureUnit(const VolumePerPressureUnit&) = delete;
  VolumePerPressureUnit& operator= (const VolumePerPressureUnit&) = delete;

  static bool IsValidUnit(const std::string& unit);
  static const VolumePerPressureUnit& GetCompoundUnit(const std::string& unit);

  static const VolumePerPressureUnit L_Per_Pa;
  static const VolumePerPressureUnit L_Per_cmH2O;
  static const VolumePerPressureUnit mL_Per_cmH2O;
  static const VolumePerPressureUnit m3_Per_Pa;
  static const VolumePerPressureUnit mL_Per_mmHg;
};

class CDM_DECL SEScalarVolumePerPressure : public SEScalarQuantity<VolumePerPressureUnit>
{
public:
  SEScalarVolumePerPressure() {}
  virtual ~SEScalarVolumePerPressure() {}
};
