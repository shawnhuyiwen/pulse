/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalar.h"

class CDM_DECL InversePressureUnit : public CCompoundUnit
{
public:
  InversePressureUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~InversePressureUnit() {}

  // Please use static units below
  InversePressureUnit(const InversePressureUnit&) = delete;
  InversePressureUnit& operator= (const InversePressureUnit&) = delete;

  static bool IsValidUnit(const std::string& unit);
  static const InversePressureUnit& GetCompoundUnit(const std::string& unit);

  static const InversePressureUnit Inverse_Pa;
  static const InversePressureUnit Inverse_mmHg;
  static const InversePressureUnit Inverse_cmH2O;
  static const InversePressureUnit Inverse_atm;
};

class CDM_DECL SEScalarInversePressure : public SEScalarQuantity<InversePressureUnit>
{
public:
  SEScalarInversePressure() {}
  virtual ~SEScalarInversePressure() {}
};
