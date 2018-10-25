/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "SEPatientCondition.h"

SEPatientCondition::SEPatientCondition() : SECondition()
{
  
}

SEPatientCondition::~SEPatientCondition()
{
  Clear();
}

void SEPatientCondition::Clear()
{
  SECondition::Clear();
}
