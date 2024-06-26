/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalar.h"

class CDM_DECL TemperatureUnit : public CCompoundUnit
{
public:
  TemperatureUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~TemperatureUnit() {}

  // Please use static units below
  TemperatureUnit(const TemperatureUnit&) = delete;
  TemperatureUnit& operator= (const TemperatureUnit&) = delete;

  static bool IsValidUnit(const std::string& unit);
  static const TemperatureUnit& GetCompoundUnit(const std::string& unit);

  static const TemperatureUnit F;
  static const TemperatureUnit C;
  static const TemperatureUnit K;
  static const TemperatureUnit R;
};

class CDM_DECL SEScalarTemperature : public SEScalarQuantity<TemperatureUnit>
{
public:
  SEScalarTemperature() {}
  virtual ~SEScalarTemperature() {}

  double GetValue(const TemperatureUnit& unit) const override;// Zero is not Zero for all units, gotta remove that logic for this scalar type
};
