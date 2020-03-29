/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "SEEnvironmentCondition.h"
#include "substance/SESubstanceManager.h"

SEEnvironmentCondition::SEEnvironmentCondition() : SECondition()
{

}

SEEnvironmentCondition::~SEEnvironmentCondition()
{
  Clear();
}

void SEEnvironmentCondition::Clear()
{
  SECondition::Clear();
}
