/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/system/physiology/SEHepaticSystem.h"

SEHepaticSystem::SEHepaticSystem(Logger* logger) : SESystem(logger)
{
  
}

SEHepaticSystem::~SEHepaticSystem()
{

}

void SEHepaticSystem::Clear()
{
  SESystem::Clear();

}

DISABLE_UNREFERENCED_FORMAL_PARAMETER
const SEScalar* SEHepaticSystem::GetScalar(const std::string& name)
{
  return nullptr;
}
DISABLE_WARNING_POP
