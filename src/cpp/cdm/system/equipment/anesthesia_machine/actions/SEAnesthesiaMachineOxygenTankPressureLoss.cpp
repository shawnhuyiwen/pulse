/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "cdm/CommonDefs.h"
#include "cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineOxygenTankPressureLoss.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/io/protobuf/PBEquipmentActions.h"

SEAnesthesiaMachineOxygenTankPressureLoss::SEAnesthesiaMachineOxygenTankPressureLoss(Logger* logger) : SEAnesthesiaMachineAction(logger)
{
  m_State = eSwitch::Off;
}

SEAnesthesiaMachineOxygenTankPressureLoss::~SEAnesthesiaMachineOxygenTankPressureLoss()
{
  m_State = eSwitch::Off;
}

void SEAnesthesiaMachineOxygenTankPressureLoss::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  m_State = eSwitch::Off;
}

void SEAnesthesiaMachineOxygenTankPressureLoss::Copy(const SEAnesthesiaMachineOxygenTankPressureLoss& src, bool /*preserveState*/)
{// Using Bindings to make a copy
  PBEquipmentAction::Copy(src, *this);
}

bool SEAnesthesiaMachineOxygenTankPressureLoss::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid();
}
bool SEAnesthesiaMachineOxygenTankPressureLoss::IsActive() const
{
  if (!SEAnesthesiaMachineAction::IsActive())
    return false;
  return m_State.GetEnum() == eSwitch::On;
}
void SEAnesthesiaMachineOxygenTankPressureLoss::Deactivate()
{
  SEAnesthesiaMachineAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEAnesthesiaMachineOxygenTankPressureLoss::GetScalar(const std::string& name)
{
  if (name.compare("State") == 0)
    return &m_State;
  return nullptr;
}

eSwitch SEAnesthesiaMachineOxygenTankPressureLoss::GetState() const
{
  return m_State.GetEnum();
}
void SEAnesthesiaMachineOxygenTankPressureLoss::SetState(eSwitch state)
{
  m_State = state;
}
