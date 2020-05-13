/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineOxygenWallPortPressureLoss.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBEquipmentActions.h"

SEAnesthesiaMachineOxygenWallPortPressureLoss::SEAnesthesiaMachineOxygenWallPortPressureLoss() : SEAnesthesiaMachineAction()
{
  m_State = eSwitch::Off;
}

SEAnesthesiaMachineOxygenWallPortPressureLoss::~SEAnesthesiaMachineOxygenWallPortPressureLoss()
{
  Clear();
}

void SEAnesthesiaMachineOxygenWallPortPressureLoss::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  m_State = eSwitch::Off;
}

void SEAnesthesiaMachineOxygenWallPortPressureLoss::Copy(const SEAnesthesiaMachineOxygenWallPortPressureLoss& src)
{// Using Bindings to make a copy
  PBEquipmentAction::Copy(src, *this);
}

bool SEAnesthesiaMachineOxygenWallPortPressureLoss::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid();
}
bool SEAnesthesiaMachineOxygenWallPortPressureLoss::IsActive() const
{
  return m_State == eSwitch::On;
}
void SEAnesthesiaMachineOxygenWallPortPressureLoss::SetActive(bool b)
{
  m_State = b ? eSwitch::On : eSwitch::Off;
}

void SEAnesthesiaMachineOxygenWallPortPressureLoss::ToString(std::ostream &str) const
{
  str << "Anesthesia Machine Action : Oxygen Wall Port Pressure Loss"; 
  if(HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tState: " << eSwitch_Name(m_State);
  str << std::flush;
}