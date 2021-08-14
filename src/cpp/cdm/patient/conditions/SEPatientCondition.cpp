/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDataModel.h"
#include "cdm/patient/conditions/SEPatientCondition.h"

SEPatientCondition::SEPatientCondition(Logger* logger) : SECondition(logger)
{
  
}

SEPatientCondition::~SEPatientCondition()
{

}

void SEPatientCondition::Clear()
{
  SECondition::Clear();
}
