/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalar.h"

class CDM_DECL TimeUnit : public CCompoundUnit
{
public:
  TimeUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~TimeUnit() {}

  // Please use static units below
  TimeUnit(const TimeUnit&) = delete;
  TimeUnit& operator= (const TimeUnit&) = delete;

  static bool IsValidUnit(const std::string& unit);
  static const TimeUnit& GetCompoundUnit(const std::string& unit);

  static const TimeUnit s;
  static const TimeUnit min;
  static const TimeUnit hr;
  static const TimeUnit day;
  static const TimeUnit yr;
};

class CDM_DECL SEScalarTime : public SEScalarQuantity<TimeUnit>
{
public:
  SEScalarTime() {}
  virtual ~SEScalarTime() {}
};
