/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalar.h"

class CDM_DECL FrequencyUnit : public CCompoundUnit
{
public:
  FrequencyUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~FrequencyUnit() {}

  // Please use static units below
  FrequencyUnit(const FrequencyUnit&) = delete;
  FrequencyUnit& operator= (const FrequencyUnit&) = delete;

  static bool IsValidUnit(const std::string& unit);
  static const FrequencyUnit& GetCompoundUnit(const std::string& unit);

  static const FrequencyUnit Per_min;
  static const FrequencyUnit Per_s;
  static const FrequencyUnit Hz;
};

class CDM_DECL SEScalarFrequency : public SEScalarQuantity<FrequencyUnit>
{
public:
  SEScalarFrequency() {}
  virtual ~SEScalarFrequency() {}
};
