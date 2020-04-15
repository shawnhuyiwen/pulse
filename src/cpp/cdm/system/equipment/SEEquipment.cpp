/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "system/equipment/SEEquipment.h"

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
