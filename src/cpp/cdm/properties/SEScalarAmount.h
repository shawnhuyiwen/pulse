/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "properties/SEScalar.h"

class CDM_DECL AmountUnit : public CCompoundUnit
{
public:
  AmountUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~AmountUnit() {}

  static bool IsValidUnit(const std::string& unit);
  static const AmountUnit& GetCompoundUnit(const std::string& unit);

  static const AmountUnit mol;
  static const AmountUnit pmol;
};

class CDM_DECL SEScalarAmount : public SEScalarQuantity<AmountUnit>
{
public:
  SEScalarAmount() {}
  virtual ~SEScalarAmount() {}
};
