/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/system/equipment/ecmo/SEECMO.h"
#include "cdm/properties/SEScalarVolumePerTime.h"

SEECMO::SEECMO(Logger* logger) : SEEquipment(logger)
{
  m_Settings = nullptr;
}

SEECMO::~SEECMO()
{
  SAFE_DELETE(m_Settings);
}

void SEECMO::Clear()
{
  SEEquipment::Clear();

  if (m_Settings)
    m_Settings->Clear();
}

void SEECMO::TurnOff()
{
  if (m_Settings)
    m_Settings->Clear();

  GetSettings().GetTransfusionFlow().SetValue(0, VolumePerTimeUnit::mL_Per_s);
}

const SEScalar* SEECMO::GetScalar(const std::string& name)
{
  return GetSettings().GetScalar(name);
}

bool SEECMO::HasSettings() const
{
  return m_Settings != nullptr;
}
SEECMOSettings& SEECMO::GetSettings()
{
  if (m_Settings == nullptr)
    m_Settings = new SEECMOSettings(GetLogger());
  return *m_Settings;
}
const SEECMOSettings* SEECMO::GetSettings() const
{
  return m_Settings;
}
void SEECMO::RemoveSettings()
{
  SAFE_DELETE(m_Settings);
}
