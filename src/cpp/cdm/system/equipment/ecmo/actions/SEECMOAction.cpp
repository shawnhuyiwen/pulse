/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "cdm/CommonDefs.h"
#include "cdm/system/equipment/ecmo/actions/SEECMOAction.h"
#include "cdm/substance/SESubstanceManager.h"

SEECMOAction::SEECMOAction(Logger* logger) : SEEquipmentAction(logger)
{

}

SEECMOAction::~SEECMOAction()
{

}

void SEECMOAction::Clear()
{
  SEEquipmentAction::Clear();
}

bool SEECMOAction::IsValid() const
{
  return SEEquipmentAction::IsValid();
}
