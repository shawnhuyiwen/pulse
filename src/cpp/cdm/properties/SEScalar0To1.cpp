/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "properties/SEScalar0To1.h"

SEScalar0To1::SEScalar0To1() : SEScalar()
{

}

void SEScalar0To1::SetValue(double d)
{ 
  if (d > 1 || d < 0)
    throw CommonDataModelException("SEScalar0To1 must be between [0,1]");
  SEScalar::SetValue(d);
}

void SEScalar0To1::SetValue(double d, const NoUnit& unitless)
{ 
  SEScalar0To1::SetValue(d);
}

void SEScalar0To1::ForceValue(double d)
{
  SEScalar::ForceValue(d);
}

void SEScalar0To1::ForceValue(double d, const NoUnit& unitless)
{
  SEScalar::ForceValue(d);
}
