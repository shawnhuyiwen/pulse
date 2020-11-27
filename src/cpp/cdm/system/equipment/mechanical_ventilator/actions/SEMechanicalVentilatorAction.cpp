/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorAction.h"
#include "substance/SESubstanceManager.h"

SEMechanicalVentilatorAction::SEMechanicalVentilatorAction(Logger* logger) : SEEquipmentAction(logger)
{

}

SEMechanicalVentilatorAction::~SEMechanicalVentilatorAction()
{

}

void SEMechanicalVentilatorAction::Clear()
{
  SEEquipmentAction::Clear();
}

bool SEMechanicalVentilatorAction::IsValid() const
{
  return SEEquipmentAction::IsValid();
}
