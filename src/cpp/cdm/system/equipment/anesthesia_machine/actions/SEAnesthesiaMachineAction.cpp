/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "cdm/CommonDefs.h"
#include "cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineAction.h"
#include "cdm/substance/SESubstanceManager.h"

SEAnesthesiaMachineAction::SEAnesthesiaMachineAction(Logger* logger) : SEEquipmentAction(logger)
{

}

SEAnesthesiaMachineAction::~SEAnesthesiaMachineAction()
{

}

void SEAnesthesiaMachineAction::Clear()
{
  SEEquipmentAction::Clear();
}

bool SEAnesthesiaMachineAction::IsValid() const
{
  return SEEquipmentAction::IsValid();
}
