/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "cdm/CommonDefs.h"
#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorMode.h"

SEMechanicalVentilatorMode::SEMechanicalVentilatorMode(Logger* logger) : SEMechanicalVentilatorAction(logger)
{
  m_Connection = eSwitch::Off;
}

SEMechanicalVentilatorMode::~SEMechanicalVentilatorMode()
{
  m_Connection = eSwitch::Off;
}

void SEMechanicalVentilatorMode::Clear()
{
  SEMechanicalVentilatorAction::Clear();
  m_Connection = eSwitch::Off;
}

bool SEMechanicalVentilatorMode::IsValid() const
{
  if (m_Connection == eSwitch::NullSwitch)
    return false;
  return SEMechanicalVentilatorAction::IsValid();
}

bool SEMechanicalVentilatorMode::IsActive() const
{
  return m_Connection == eSwitch::On;
}

bool SEMechanicalVentilatorMode::ToSettings(SEMechanicalVentilatorSettings& s, const SESubstanceManager& /*subMgr*/)
{
  if (!IsValid())
    return false;

  s.Clear();
  s.SetConnection(m_Connection);
  return true;
}

void SEMechanicalVentilatorMode::SetConnection(eSwitch c)
{
  m_Connection = c;
}
eSwitch SEMechanicalVentilatorMode::GetConnection() const
{
  return m_Connection;
}
