/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/bag_valve_mask/actions/SEBagValveMaskAction.h"
#include "substance/SESubstanceManager.h"

SEBagValveMaskAction::SEBagValveMaskAction(Logger* logger) : SEEquipmentAction(logger)
{

}

SEBagValveMaskAction::~SEBagValveMaskAction()
{

}

void SEBagValveMaskAction::Clear()
{
  SEEquipmentAction::Clear();
}

bool SEBagValveMaskAction::IsValid() const
{
  return SEEquipmentAction::IsValid();
}
