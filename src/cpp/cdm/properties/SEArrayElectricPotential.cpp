/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEArrayElectricPotential.h"

void SEArrayElectricPotential::Copy(const SEArrayElectricPotential& src)
{
  SEArray::Copy(src);
  m_ElectricPotentialUnit = src.GetUnit();
}
