/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/system/equipment/mechanical_ventilator/SEMechanicalVentilator.h"

#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarPressureTimePerVolume.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/io/protobuf/PBMechanicalVentilator.h"

SEMechanicalVentilator::SEMechanicalVentilator(Logger* logger) : SEEquipment(logger)
{

  m_Settings = nullptr;
}

SEMechanicalVentilator::~SEMechanicalVentilator()
{

  SAFE_DELETE(m_Settings);
}

void SEMechanicalVentilator::Clear()
{
  SEEquipment::Clear();


  if (m_Settings)
    m_Settings->Clear();
}

const SEScalar* SEMechanicalVentilator::GetScalar(const std::string& name)
{
  
  return GetSettings().GetScalar(name);
}



bool SEMechanicalVentilator::HasSettings() const
{
  return m_Settings != nullptr;
}
SEMechanicalVentilatorSettings& SEMechanicalVentilator::GetSettings()
{
  if (m_Settings == nullptr)
    m_Settings = new SEMechanicalVentilatorSettings(GetLogger());
  return *m_Settings;
}
const SEMechanicalVentilatorSettings* SEMechanicalVentilator::GetSettings() const
{
  return m_Settings;
}
void SEMechanicalVentilator::RemoveSettings()
{
  SAFE_DELETE(m_Settings);
}
