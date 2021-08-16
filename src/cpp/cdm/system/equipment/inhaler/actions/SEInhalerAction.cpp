/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "cdm/CommonDefs.h"
#include "cdm/system/equipment/inhaler/actions/SEInhalerAction.h"
#include "cdm/substance/SESubstanceManager.h"

SEInhalerAction::SEInhalerAction(Logger* logger) : SEEquipmentAction(logger)
{

}

SEInhalerAction::~SEInhalerAction()
{

}

void SEInhalerAction::Clear()
{
  SEEquipmentAction::Clear();
}

bool SEInhalerAction::IsValid() const
{
  return SEEquipmentAction::IsValid();
}
