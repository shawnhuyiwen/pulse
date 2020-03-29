/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineAction.h"
#include "substance/SESubstanceManager.h"

SEAnesthesiaMachineAction::SEAnesthesiaMachineAction() : SEAction()
{

}

SEAnesthesiaMachineAction::~SEAnesthesiaMachineAction()
{
  Clear();
}

void SEAnesthesiaMachineAction::Clear()
{
  SEAction::Clear();
}

bool SEAnesthesiaMachineAction::IsValid() const
{
  return SEAction::IsValid();
}
