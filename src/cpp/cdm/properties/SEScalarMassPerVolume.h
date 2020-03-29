/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "properties/SEScalar.h"

class CDM_DECL MassPerVolumeUnit : public CCompoundUnit
{
public:
  MassPerVolumeUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~MassPerVolumeUnit() {}

  static bool IsValidUnit(const std::string& unit);
  static const MassPerVolumeUnit& GetCompoundUnit(const std::string& unit);

  static const MassPerVolumeUnit ug_Per_mL;
  static const MassPerVolumeUnit mg_Per_m3;
  static const MassPerVolumeUnit kg_Per_m3;
  static const MassPerVolumeUnit ug_Per_L;
  static const MassPerVolumeUnit g_Per_dL;
  static const MassPerVolumeUnit g_Per_L;
  static const MassPerVolumeUnit g_Per_mL;
  static const MassPerVolumeUnit g_Per_cm3;
  static const MassPerVolumeUnit g_Per_m3;  
  static const MassPerVolumeUnit mg_Per_mL;
  static const MassPerVolumeUnit mg_Per_L;
  static const MassPerVolumeUnit mg_Per_dL;
  static const MassPerVolumeUnit kg_Per_mL;
  static const MassPerVolumeUnit kg_Per_L;
};

class CDM_DECL SEScalarMassPerVolume : public SEScalarQuantity<MassPerVolumeUnit>
{
public:
  SEScalarMassPerVolume() {};
  virtual ~SEScalarMassPerVolume() {}
};
