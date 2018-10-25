/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "properties/SEScalar.h"

class CDM_DECL PressureTimePerAreaUnit : public CCompoundUnit
{
public:
  PressureTimePerAreaUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~PressureTimePerAreaUnit() {}

  static bool IsValidUnit(const std::string& unit);
  static const PressureTimePerAreaUnit& GetCompoundUnit(const std::string& unit);

  static const PressureTimePerAreaUnit mmHg_Per_mL_m2;
  static const PressureTimePerAreaUnit cmH2O_Per_mL_m2;
};

class CDM_DECL SEScalarPressureTimePerArea : public SEScalarQuantity<PressureTimePerAreaUnit>
{
public:
  SEScalarPressureTimePerArea() {}
  virtual ~SEScalarPressureTimePerArea() {}
};
