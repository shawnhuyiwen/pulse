/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "properties/SEScalar.h"

class CDM_DECL PressureTimePerVolumeUnit : public CCompoundUnit
{
public:
  PressureTimePerVolumeUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~PressureTimePerVolumeUnit() {}

  static bool IsValidUnit(const std::string& unit);
  static const PressureTimePerVolumeUnit& GetCompoundUnit(const std::string& unit);

  static const PressureTimePerVolumeUnit cmH2O_s_Per_L;
  static const PressureTimePerVolumeUnit cmH2O_s_Per_mL;
  static const PressureTimePerVolumeUnit mmHg_s_Per_mL;
  static const PressureTimePerVolumeUnit mmHg_min_Per_mL;
  static const PressureTimePerVolumeUnit mmHg_min_Per_L;
  static const PressureTimePerVolumeUnit Pa_s_Per_m3;
};

class CDM_DECL SEScalarPressureTimePerVolume : public SEScalarQuantity<PressureTimePerVolumeUnit>
{
public:
  SEScalarPressureTimePerVolume() {}
  virtual ~SEScalarPressureTimePerVolume() {}
};
