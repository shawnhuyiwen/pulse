/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "properties/SEScalar.h"

class CDM_DECL AreaUnit : public CCompoundUnit
{
public:
  AreaUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~AreaUnit() {}

  static bool IsValidUnit(const std::string& unit);
  static const AreaUnit& GetCompoundUnit(const std::string& unit);

  static const AreaUnit cm2;
  static const AreaUnit m2;
};

class CDM_DECL SEScalarArea : public SEScalarQuantity<AreaUnit>
{
public:
  SEScalarArea() {}
  virtual ~SEScalarArea() {}
};