/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalar.h"

class CDM_DECL AmountPerMassUnit : public CCompoundUnit
{
public:
  AmountPerMassUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~AmountPerMassUnit() {}

  // Please use static units below
  AmountPerMassUnit(const AmountPerMassUnit&) = delete;
  AmountPerMassUnit& operator= (const AmountPerMassUnit&) = delete;

  static bool IsValidUnit(const std::string& unit);
  static const AmountPerMassUnit& GetCompoundUnit(const std::string& unit);

  static AmountPerMassUnit ct_Per_g;
  static AmountPerMassUnit ct_Per_ug;
};

class CDM_DECL SEScalarAmountPerMass : public SEScalarQuantity<AmountPerMassUnit>
{

public:
  SEScalarAmountPerMass() {}
  virtual ~SEScalarAmountPerMass() {}
};
