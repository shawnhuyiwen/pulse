/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "properties/SEScalar.h"

class CDM_DECL SEScalar0To1 : public SEScalar
{

public:
  SEScalar0To1();
  virtual ~SEScalar0To1() {}

  double GetValue() const { return SEScalar::GetValue(); }
  double GetValue(const NoUnit& unitless) const { return SEScalar::GetValue(); }
  void SetValue(double d);
  void SetValue(double d, const NoUnit& unitless);
  void ForceValue(double d);
  void ForceValue(double d, const NoUnit& unitless);
};
