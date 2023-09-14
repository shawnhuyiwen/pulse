/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/utils/unitconversion/UCCommon.h"

class CDM_DECL SEProperty
{
protected:

public:

  SEProperty();
  virtual ~SEProperty();

  virtual void Invalidate() = 0;
  virtual bool IsValid() const = 0;
};

CDM_DECL bool   CompatibleUnits(const CCompoundUnit& u1, const CCompoundUnit& u2);
CDM_DECL double Convert(double d, const CCompoundUnit& from, const CCompoundUnit& to);
