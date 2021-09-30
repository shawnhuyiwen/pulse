/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalar.h"

class CDM_DECL LengthUnit : public CCompoundUnit
{
public:
  LengthUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~LengthUnit() {}

  // Please use static units below
  LengthUnit(const LengthUnit&) = delete;
  LengthUnit& operator= (const LengthUnit&) = delete;

  static bool IsValidUnit(const std::string& unit);
  static const LengthUnit& GetCompoundUnit(const std::string& unit);

  static const LengthUnit m;
  static const LengthUnit cm;
  static const LengthUnit mm;
  static const LengthUnit um;
  static const LengthUnit in;
  static const LengthUnit ft;
};

class CDM_DECL SEScalarLength : public SEScalarQuantity<LengthUnit>
{
public:
  SEScalarLength() {}
  virtual ~SEScalarLength() {}
};
