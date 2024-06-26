/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalar.h"

class CDM_DECL PowerPerAreaTemperatureToTheFourthUnit : public CCompoundUnit
{
public:
  PowerPerAreaTemperatureToTheFourthUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~PowerPerAreaTemperatureToTheFourthUnit() {}

  // Please use static units below
  PowerPerAreaTemperatureToTheFourthUnit(const PowerPerAreaTemperatureToTheFourthUnit&) = delete;
  PowerPerAreaTemperatureToTheFourthUnit& operator= (const PowerPerAreaTemperatureToTheFourthUnit&) = delete;

  static bool IsValidUnit(const std::string& unit);
  static const PowerPerAreaTemperatureToTheFourthUnit& GetCompoundUnit(const std::string& unit);

  static const PowerPerAreaTemperatureToTheFourthUnit W_Per_m2_K4;
};

class CDM_DECL SEScalarPowerPerAreaTemperatureToTheFourth : public SEScalarQuantity<PowerPerAreaTemperatureToTheFourthUnit>
{
public:
  SEScalarPowerPerAreaTemperatureToTheFourth() {}
  virtual ~SEScalarPowerPerAreaTemperatureToTheFourth() {}
};
