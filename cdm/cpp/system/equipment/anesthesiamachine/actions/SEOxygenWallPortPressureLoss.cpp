/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/anesthesiamachine/actions/SEOxygenWallPortPressureLoss.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBAnesthesiaMachineActions.h"

SEOxygenWallPortPressureLoss::SEOxygenWallPortPressureLoss() : SEAnesthesiaMachineAction()
{
  m_State = eSwitch::Off;
}

SEOxygenWallPortPressureLoss::~SEOxygenWallPortPressureLoss()
{
  Clear();
}

void SEOxygenWallPortPressureLoss::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  m_State = eSwitch::Off;
}

void SEOxygenWallPortPressureLoss::Copy(const SEOxygenWallPortPressureLoss& src)
{// Using Bindings to make a copy
  PBAnesthesiaMachineAction::Copy(src, *this);
}

bool SEOxygenWallPortPressureLoss::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid();
}
bool SEOxygenWallPortPressureLoss::IsActive() const
{
  return m_State == eSwitch::On;
}
void SEOxygenWallPortPressureLoss::SetActive(bool b)
{
  m_State = b ? eSwitch::On : eSwitch::Off;
}

void SEOxygenWallPortPressureLoss::ToString(std::ostream &str) const
{
  str << "Anesthesia Machine Action : Oxygen Wall Port Pressure Loss"; 
  if(HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tState: " << eSwitch_Name(m_State);
  str << std::flush;
}