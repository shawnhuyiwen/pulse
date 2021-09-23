/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalar.h"

class CDM_DECL PressureTimePerVolumeAreaUnit : public CCompoundUnit
{
public:
  PressureTimePerVolumeAreaUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~PressureTimePerVolumeAreaUnit() {}

  // Please use static units below
  PressureTimePerVolumeAreaUnit(const PressureTimePerVolumeAreaUnit&) = delete;
  PressureTimePerVolumeAreaUnit& operator= (const PressureTimePerVolumeAreaUnit&) = delete;

  static bool IsValidUnit(const std::string& unit);
  static const PressureTimePerVolumeAreaUnit& GetCompoundUnit(const std::string& unit);

  static const PressureTimePerVolumeAreaUnit mmHg_min_Per_mL_m2;
  static const PressureTimePerVolumeAreaUnit mmHg_s_Per_mL_m2;
  static const PressureTimePerVolumeAreaUnit dyn_s_Per_cm5_m2;
};

class CDM_DECL SEScalarPressureTimePerVolumeArea : public SEScalarQuantity<PressureTimePerVolumeAreaUnit>
{
public:
  SEScalarPressureTimePerVolumeArea() {}
  virtual ~SEScalarPressureTimePerVolumeArea() {}
};
