/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "SEEnvironmentCondition.h"
#include "substance/SESubstanceManager.h"

SEEnvironmentCondition::SEEnvironmentCondition(Logger* logger) : SECondition(logger)
{

}

SEEnvironmentCondition::~SEEnvironmentCondition()
{

}

void SEEnvironmentCondition::Clear()
{
  SECondition::Clear();
}
