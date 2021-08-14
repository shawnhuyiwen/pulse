/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDataModel.h"
#include "cdm/system/equipment/SEEquipment.h"

SEEquipment::SEEquipment(Logger* logger) : SESystem(logger)
{
  
}

SEEquipment::~SEEquipment()
{
  Clear();
}

void SEEquipment::Clear()
{
  SESystem::Clear();
}
