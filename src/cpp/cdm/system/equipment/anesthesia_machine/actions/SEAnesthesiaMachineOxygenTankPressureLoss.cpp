/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineOxygenTankPressureLoss.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBEquipmentActions.h"

SEAnesthesiaMachineOxygenTankPressureLoss::SEAnesthesiaMachineOxygenTankPressureLoss(Logger* logger) : SEAnesthesiaMachineAction(logger)
{
  m_State.SetEnum(eSwitch::Off);
}

SEAnesthesiaMachineOxygenTankPressureLoss::~SEAnesthesiaMachineOxygenTankPressureLoss()
{
  m_State.SetEnum(eSwitch::Off);
}

void SEAnesthesiaMachineOxygenTankPressureLoss::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  m_State.SetEnum(eSwitch::Off);
}

void SEAnesthesiaMachineOxygenTankPressureLoss::Copy(const SEAnesthesiaMachineOxygenTankPressureLoss& src, bool preserveState)
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
  m_State.SetEnum(state);
}

void SEAnesthesiaMachineOxygenTankPressureLoss::ToString(std::ostream &str) const
{
  str << "Anesthesia Machine Action : Oxygen Tank Pressure Loss"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str  << "\n\tState: " << eSwitch_Name(m_State.GetEnum());
  str << std::flush;
}