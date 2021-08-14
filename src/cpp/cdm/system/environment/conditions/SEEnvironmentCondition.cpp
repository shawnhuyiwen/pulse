/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "cdm/CommonDataModel.h"
#include "cdm/system/environment/conditions/SEEnvironmentCondition.h"
#include "cdm/substance/SESubstanceManager.h"

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
