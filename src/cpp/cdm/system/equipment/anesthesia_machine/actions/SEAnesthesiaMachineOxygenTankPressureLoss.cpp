/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineOxygenTankPressureLoss.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBEquipmentActions.h"

SEAnesthesiaMachineOxygenTankPressureLoss::SEAnesthesiaMachineOxygenTankPressureLoss() : SEAnesthesiaMachineAction()
{
  m_State = eSwitch::Off;
}

SEAnesthesiaMachineOxygenTankPressureLoss::~SEAnesthesiaMachineOxygenTankPressureLoss()
{
  Clear();
}

void SEAnesthesiaMachineOxygenTankPressureLoss::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  m_State = eSwitch::Off;
}

void SEAnesthesiaMachineOxygenTankPressureLoss::Copy(const SEAnesthesiaMachineOxygenTankPressureLoss& src)
{// Using Bindings to make a copy
  PBEquipmentAction::Copy(src, *this);
}

bool SEAnesthesiaMachineOxygenTankPressureLoss::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid();
}
bool SEAnesthesiaMachineOxygenTankPressureLoss::IsActive() const
{
  return m_State == eSwitch::On;
}
void SEAnesthesiaMachineOxygenTankPressureLoss::SetActive(bool b)
{
  m_State = b ? eSwitch::On : eSwitch::Off;
}

void SEAnesthesiaMachineOxygenTankPressureLoss::ToString(std::ostream &str) const
{
  str << "Anesthesia Machine Action : Oxygen Tank Pressure Loss"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str  << "\n\tState: " << eSwitch_Name(m_State);
  str << std::flush;
}