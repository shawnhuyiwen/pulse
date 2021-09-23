/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalar.h"

class CDM_DECL MassPerMassUnit : public CCompoundUnit
{
public:
  MassPerMassUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~MassPerMassUnit() {}

  // Please use static units below
  MassPerMassUnit(const MassPerMassUnit&) = delete;
  MassPerMassUnit& operator= (const MassPerMassUnit&) = delete;

  static bool IsValidUnit(const std::string& unit);
  static const MassPerMassUnit& GetCompoundUnit(const std::string& unit);

  static const MassPerMassUnit ug_Per_kg;
  static const MassPerMassUnit mg_Per_g;
};

class CDM_DECL SEScalarMassPerMass : public SEScalarQuantity<MassPerMassUnit>
{
public:
  SEScalarMassPerMass() {}
  virtual ~SEScalarMassPerMass() {}
};
