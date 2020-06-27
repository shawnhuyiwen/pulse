/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "properties/SEScalarNegative1To1.h"

SEScalarNegative1To1::SEScalarNegative1To1() : SEScalar()
{

}

void SEScalarNegative1To1::SetValue(double d)
{ 
  if (d > 1 || d < -1)
    throw CommonDataModelException("SEScalarNegative1To1 must be between [-1,1]");
  SEScalar::SetValue(d);
}

void SEScalarNegative1To1::SetValue(double d, const NoUnit& unitless)
{ 
  SEScalarNegative1To1::SetValue(d);
}

void SEScalarNegative1To1::ForceValue(double d)
{
  SEScalar::ForceValue(d);
}

void SEScalarNegative1To1::ForceValue(double d, const NoUnit& unitless)
{
  SEScalar::ForceValue(d);
}
