/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "properties/SEScalar.h"

class CDM_DECL LengthPerTimeUnit : public CCompoundUnit
{
public:
  LengthPerTimeUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~LengthPerTimeUnit() {}

  static bool IsValidUnit(const std::string& unit);
  static const LengthPerTimeUnit& GetCompoundUnit(const std::string& unit);

  static const LengthPerTimeUnit m_Per_s;
  static const LengthPerTimeUnit cm_Per_s;
  static const LengthPerTimeUnit m_Per_min;
  static const LengthPerTimeUnit cm_Per_min;
  static const LengthPerTimeUnit ft_Per_s;
  static const LengthPerTimeUnit ft_Per_min;
  static const LengthPerTimeUnit km_Per_hr;
};

class CDM_DECL SEScalarLengthPerTime : public SEScalarQuantity<LengthPerTimeUnit>
{
public:
  SEScalarLengthPerTime() {}
  virtual ~SEScalarLengthPerTime() {}
};
