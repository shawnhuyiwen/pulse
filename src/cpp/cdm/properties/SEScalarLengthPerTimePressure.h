/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "properties/SEScalar.h"

class CDM_DECL LengthPerTimePressureUnit : public CCompoundUnit
{
public:
  LengthPerTimePressureUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~LengthPerTimePressureUnit() {}

  static bool IsValidUnit(const std::string& unit);
  static const LengthPerTimePressureUnit& GetCompoundUnit(const std::string& unit);

  static const LengthPerTimePressureUnit m_Per_s_mmHg;
  static const LengthPerTimePressureUnit cm_Per_s_mmHg;
  static const LengthPerTimePressureUnit m_Per_min_mmHg;
  static const LengthPerTimePressureUnit cm_Per_min_mmHg;
};

class CDM_DECL SEScalarLengthPerTimePressure : public SEScalarQuantity<LengthPerTimePressureUnit>
{
public:
  SEScalarLengthPerTimePressure() {}
  virtual ~SEScalarLengthPerTimePressure() {}
};
