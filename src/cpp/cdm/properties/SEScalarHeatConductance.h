/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalar.h"

class CDM_DECL HeatConductanceUnit : public CCompoundUnit
{
public:
  HeatConductanceUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~HeatConductanceUnit() {}

  // Please use static units below
  HeatConductanceUnit(const HeatConductanceUnit&) = delete;
  HeatConductanceUnit& operator= (const HeatConductanceUnit&) = delete;

  static bool IsValidUnit(const std::string& unit);
  static const HeatConductanceUnit& GetCompoundUnit(const std::string& unit);

  static const HeatConductanceUnit W_Per_K;
  static const HeatConductanceUnit W_Per_C;
  static const HeatConductanceUnit kcal_Per_K_s;
  static const HeatConductanceUnit kcal_Per_C_s;
};

class CDM_DECL SEScalarHeatConductance : public SEScalarQuantity<HeatConductanceUnit>
{
public:
  SEScalarHeatConductance() {}
  virtual ~SEScalarHeatConductance() {}
};