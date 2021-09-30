/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalar.h"

class CDM_DECL HeatResistanceUnit : public CCompoundUnit
{
public:
  HeatResistanceUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~HeatResistanceUnit() {}

  // Please use static units below
  HeatResistanceUnit(const HeatResistanceUnit&) = delete;
  HeatResistanceUnit& operator= (const HeatResistanceUnit&) = delete;

  static bool IsValidUnit(const std::string& unit);
  static const HeatResistanceUnit& GetCompoundUnit(const std::string& unit);

  static const HeatResistanceUnit K_Per_W;
  static const HeatResistanceUnit C_Per_W;
  static const HeatResistanceUnit K_s_Per_kcal;
  static const HeatResistanceUnit C_s_Per_kcal;
};

class CDM_DECL SEScalarHeatResistance : public SEScalarQuantity<HeatResistanceUnit>
{
public:
  SEScalarHeatResistance() {}
  virtual ~SEScalarHeatResistance() {}
};