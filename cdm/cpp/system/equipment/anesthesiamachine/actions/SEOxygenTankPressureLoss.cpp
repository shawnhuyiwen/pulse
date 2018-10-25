/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/anesthesiamachine/actions/SEOxygenTankPressureLoss.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBAnesthesiaMachineActions.h"

SEOxygenTankPressureLoss::SEOxygenTankPressureLoss() : SEAnesthesiaMachineAction()
{
  m_State = eSwitch::Off;
}

SEOxygenTankPressureLoss::~SEOxygenTankPressureLoss()
{
  Clear();
}

void SEOxygenTankPressureLoss::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  m_State = eSwitch::Off;
}

void SEOxygenTankPressureLoss::Copy(const SEOxygenTankPressureLoss& src)
{// Using Bindings to make a copy
  PBAnesthesiaMachineAction::Copy(src, *this);
}

bool SEOxygenTankPressureLoss::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid();
}
bool SEOxygenTankPressureLoss::IsActive() const
{
  return m_State == eSwitch::On;
}
void SEOxygenTankPressureLoss::SetActive(bool b)
{
  m_State = b ? eSwitch::On : eSwitch::Off;
}

void SEOxygenTankPressureLoss::ToString(std::ostream &str) const
{
  str << "Anesthesia Machine Action : Oxygen Tank Pressure Loss"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str  << "\n\tState: " << eSwitch_Name(m_State);
  str << std::flush;
}