/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "properties/SEScalar.h"

class CDM_DECL PressureTimeSquaredPerVolumeUnit : public CCompoundUnit
{
public:
  PressureTimeSquaredPerVolumeUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~PressureTimeSquaredPerVolumeUnit() {}

  static bool IsValidUnit(const std::string& unit);
  static const PressureTimeSquaredPerVolumeUnit& GetCompoundUnit(const std::string& unit);

  static const PressureTimeSquaredPerVolumeUnit mmHg_s2_Per_mL;
  static const PressureTimeSquaredPerVolumeUnit mmHg_s2_Per_L;
  static const PressureTimeSquaredPerVolumeUnit cmH2O_s2_Per_mL;
  static const PressureTimeSquaredPerVolumeUnit cmH2O_s2_Per_L;
  static const PressureTimeSquaredPerVolumeUnit Pa_s2_Per_m3;
};

class CDM_DECL SEScalarPressureTimeSquaredPerVolume : public SEScalarQuantity<PressureTimeSquaredPerVolumeUnit>
{
public:
  SEScalarPressureTimeSquaredPerVolume() {}
  virtual ~SEScalarPressureTimeSquaredPerVolume() {}
};
