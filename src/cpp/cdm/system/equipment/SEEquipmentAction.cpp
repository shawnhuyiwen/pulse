/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/SEEquipmentAction.h"

SEEquipmentAction::SEEquipmentAction() : SEAction()
{

}

SEEquipmentAction::~SEEquipmentAction()
{
  Clear();
}

void SEEquipmentAction::Clear()
{
  SEAction::Clear();
}

bool SEEquipmentAction::IsValid() const
{
  return SEAction::IsValid();
}
