/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineOxygenWallPortPressureLoss.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBEquipmentActions.h"

SEAnesthesiaMachineOxygenWallPortPressureLoss::SEAnesthesiaMachineOxygenWallPortPressureLoss(Logger* logger) : SEAnesthesiaMachineAction(logger)
{
  m_State.SetEnum(eSwitch::Off);
}

SEAnesthesiaMachineOxygenWallPortPressureLoss::~SEAnesthesiaMachineOxygenWallPortPressureLoss()
{
  m_State.SetEnum(eSwitch::Off);
}

void SEAnesthesiaMachineOxygenWallPortPressureLoss::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  m_State.SetEnum(eSwitch::Off);
}

void SEAnesthesiaMachineOxygenWallPortPressureLoss::Copy(const SEAnesthesiaMachineOxygenWallPortPressureLoss& src, bool preserveState)
{// Using Bindings to make a copy
  PBEquipmentAction::Copy(src, *this);
}

bool SEAnesthesiaMachineOxygenWallPortPressureLoss::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid();
}
bool SEAnesthesiaMachineOxygenWallPortPressureLoss::IsActive() const
{
  if (!SEAnesthesiaMachineAction::IsActive())
    return false;
  return m_State.GetEnum() == eSwitch::On;
}
void SEAnesthesiaMachineOxygenWallPortPressureLoss::Deactivate()
{
  SEAnesthesiaMachineAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEAnesthesiaMachineOxygenWallPortPressureLoss::GetScalar(const std::string& name)
{
  if (name.compare("State") == 0)
    return &m_State;
  return nullptr;
}


eSwitch SEAnesthesiaMachineOxygenWallPortPressureLoss::GetState() const
{
  return m_State.GetEnum();
}
void SEAnesthesiaMachineOxygenWallPortPressureLoss::SetState(eSwitch state)
{
  m_State.SetEnum(state);
}

void SEAnesthesiaMachineOxygenWallPortPressureLoss::ToString(std::ostream &str) const
{
  str << "Anesthesia Machine Action : Oxygen Wall Port Pressure Loss"; 
  if(HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tState: " << eSwitch_Name(m_State.GetEnum());
  str << std::flush;
}