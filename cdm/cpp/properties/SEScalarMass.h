/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "properties/SEScalar.h"

class CDM_DECL MassUnit : public CCompoundUnit
{
public:
  MassUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~MassUnit() {}

  static bool IsValidUnit(const std::string& unit);
  static const MassUnit& GetCompoundUnit(const std::string& unit);

  static const MassUnit ug;
  static const MassUnit mg;
  static const MassUnit g;
  static const MassUnit kg;
  static const MassUnit lb;
};

class CDM_DECL SEScalarMass : public SEScalarQuantity<MassUnit>
{
public:
  SEScalarMass() {}
  virtual ~SEScalarMass() {}
};
