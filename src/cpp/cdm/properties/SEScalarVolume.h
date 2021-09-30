/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalar.h"

class CDM_DECL VolumeUnit : public CCompoundUnit
{
public:
  VolumeUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~VolumeUnit() {}

  // Please use static units below
  VolumeUnit(const VolumeUnit&) = delete;
  VolumeUnit& operator= (const VolumeUnit&) = delete;

  static bool IsValidUnit(const std::string& unit);
  static const VolumeUnit& GetCompoundUnit(const std::string& unit);

  static const VolumeUnit L;
  static const VolumeUnit dL;
  static const VolumeUnit mL;
  static const VolumeUnit m3;
  static const VolumeUnit uL;
};

class CDM_DECL SEScalarVolume : public SEScalarQuantity<VolumeUnit>
{
public:
  SEScalarVolume() {}
  virtual ~SEScalarVolume() {}
};