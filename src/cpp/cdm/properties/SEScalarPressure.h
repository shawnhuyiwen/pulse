/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalar.h"

class CDM_DECL PressureUnit : public CCompoundUnit
{
public:
  PressureUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~PressureUnit() {}

  // Please use static units below
  PressureUnit(const PressureUnit&) = delete;
  PressureUnit& operator= (const PressureUnit&) = delete;

  static bool IsValidUnit(const std::string& unit);
  static const PressureUnit& GetCompoundUnit(const std::string& unit);

  static const PressureUnit Pa;
  static const PressureUnit mmHg;
  static const PressureUnit cmH2O;
  static const PressureUnit psi;
  static const PressureUnit atm;
  static const PressureUnit mbar;
};

class CDM_DECL SEScalarPressure : public SEScalarQuantity<PressureUnit>
{
public:
  SEScalarPressure() {};
  virtual ~SEScalarPressure() {}
};
