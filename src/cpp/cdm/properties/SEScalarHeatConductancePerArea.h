/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalar.h"

class CDM_DECL HeatConductancePerAreaUnit : public CCompoundUnit
{
public:
  HeatConductancePerAreaUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~HeatConductancePerAreaUnit() {}

  // Please use static units below
  HeatConductancePerAreaUnit(const HeatConductancePerAreaUnit&) = delete;
  HeatConductancePerAreaUnit& operator= (const HeatConductancePerAreaUnit&) = delete;

  static bool IsValidUnit(const std::string& unit);
  static const HeatConductancePerAreaUnit& GetCompoundUnit(const std::string& unit);

  static const HeatConductancePerAreaUnit W_Per_m2_K;
  static const HeatConductancePerAreaUnit W_Per_m2_C;
  static const HeatConductancePerAreaUnit BTU_Per_hr_ft2_C;
};

class CDM_DECL SEScalarHeatConductancePerArea : public SEScalarQuantity<HeatConductancePerAreaUnit>
{
public:
  SEScalarHeatConductancePerArea() {}
  virtual ~SEScalarHeatConductancePerArea() {}
};