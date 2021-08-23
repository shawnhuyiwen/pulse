/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "cdm/CommonDefs.h"
#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorMode.h"

SEMechanicalVentilatorMode::SEMechanicalVentilatorMode(Logger* logger) : SEMechanicalVentilatorAction(logger)
{
  m_Connection = eMechanicalVentilator_Connection::Off;
}

SEMechanicalVentilatorMode::~SEMechanicalVentilatorMode()
{
  m_Connection = eMechanicalVentilator_Connection::Off;
}

void SEMechanicalVentilatorMode::Clear()
{
  SEMechanicalVentilatorAction::Clear();
  m_Connection = eMechanicalVentilator_Connection::Off;
}

bool SEMechanicalVentilatorMode::IsValid() const
{
  if (m_Connection == eMechanicalVentilator_Connection::NullConnection)
    return false;
  return SEMechanicalVentilatorAction::IsValid();
}

bool SEMechanicalVentilatorMode::IsActive() const
{
  if (m_Connection == eMechanicalVentilator_Connection::Mask ||
    m_Connection == eMechanicalVentilator_Connection::Tube)
    return true;
  return false;
}

bool SEMechanicalVentilatorMode::ToConfiguration(SEMechanicalVentilatorConfiguration& cfg)
{
  if (!IsValid())
    return false;

  cfg.GetSettings().SetConnection(m_Connection);
  return true;
}

void SEMechanicalVentilatorMode::SetConnection(eMechanicalVentilator_Connection c)
{
  m_Connection = c;
}
eMechanicalVentilator_Connection SEMechanicalVentilatorMode::GetConnection() const
{
  return m_Connection;
}
