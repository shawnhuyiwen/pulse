/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorConfiguration.h"
#include "system/equipment/mechanical_ventilator/SEMechanicalVentilator.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"
#include "substance/SESubstanceFraction.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "io/protobuf/PBEquipmentActions.h"

SEMechanicalVentilatorConfiguration::SEMechanicalVentilatorConfiguration(SESubstanceManager& substances) : SEMechanicalVentilatorAction(), m_Substances(substances)
{
  m_Configuration = nullptr;
  InvalidateConfigurationFile();
}

SEMechanicalVentilatorConfiguration::~SEMechanicalVentilatorConfiguration()
{
  Clear();
}

void SEMechanicalVentilatorConfiguration::Clear()
{
  SEMechanicalVentilatorAction::Clear();
  InvalidateConfigurationFile();
  SAFE_DELETE(m_Configuration);
}

void SEMechanicalVentilatorConfiguration::Copy(const SEMechanicalVentilatorConfiguration& src)
{// Using Bindings to make a copy
  PBEquipmentAction::Copy(src, *this);
}

bool SEMechanicalVentilatorConfiguration::IsValid() const
{
  return SEMechanicalVentilatorAction::IsValid() && (HasConfiguration() || HasConfigurationFile());
}

bool SEMechanicalVentilatorConfiguration::HasConfiguration() const
{
  return m_Configuration != nullptr;
}
SEMechanicalVentilator& SEMechanicalVentilatorConfiguration::GetConfiguration()
{
  m_ConfigurationFile = "";
  if (m_Configuration == nullptr)
    m_Configuration = new SEMechanicalVentilator(m_Substances);
  return *m_Configuration;
}
const SEMechanicalVentilator* SEMechanicalVentilatorConfiguration::GetConfiguration() const
{
  return m_Configuration;
}

std::string SEMechanicalVentilatorConfiguration::GetConfigurationFile() const
{
  return m_ConfigurationFile;
}
void SEMechanicalVentilatorConfiguration::SetConfigurationFile(const std::string& fileName)
{
  if (m_Configuration != nullptr)
    SAFE_DELETE(m_Configuration);
  m_ConfigurationFile = fileName;
}
bool SEMechanicalVentilatorConfiguration::HasConfigurationFile() const
{
  return m_ConfigurationFile.empty() ? false : true;
}
void SEMechanicalVentilatorConfiguration::InvalidateConfigurationFile()
{
  m_ConfigurationFile = "";
}

void SEMechanicalVentilatorConfiguration::ToString(std::ostream &str) const
{
  str << "Mechanical Ventilator Configuration";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  if (HasConfigurationFile())
    str << "\n\tConfiguration File: "; str << m_ConfigurationFile;
  if (HasConfiguration())
  {
    str << "\n\tBreath Period: "; m_Configuration->HasBreathPeriod() ? str << m_Configuration->GetBreathPeriod() : str << "NaN";
    str << "\n\tConnection: " << eMechanicalVentilator_Connection_Name(m_Configuration->GetConnection());
    str << "\n\tControl: " << eMechanicalVentilator_Control_Name(m_Configuration->GetControl());
    str << "\n\tDriver Waveform: " << eMechanicalVentilator_DriverWaveform_Name(m_Configuration->GetDriverWaveform());
    str << "\n\tInspiratory Expiratory Ratio: "; m_Configuration->HasInspiratoryExpiratoryRatio() ? str << m_Configuration->GetInspiratoryExpiratoryRatio() : str << "NaN";
    str << "\n\tExpiratory Period: "; m_Configuration->HasExpiratoryPeriod() ? str << m_Configuration->GetExpiratoryPeriod() : str << "NaN";
    str << "\n\tInspiratory Period: "; m_Configuration->HasInspiratoryPeriod() ? str << m_Configuration->GetInspiratoryPeriod() : str << "NaN";
    str << "\n\tPeak Inspiratory Pressure: "; m_Configuration->HasPeakInspiratoryPressure() ? str << m_Configuration->GetPeakInspiratoryPressure() : str << "NaN";
    str << "\n\tPositive End Expired Pressure: "; m_Configuration->HasPositiveEndExpiredPressure() ? str << m_Configuration->GetPositiveEndExpiredPressure() : str << "NaN";
    str << "\n\tRespiratory Rate: "; m_Configuration->HasRespiratoryRate() ? str << m_Configuration->GetRespiratoryRate() : str << "NaN";
    if (m_Configuration->HasFractionInspiredGas())
    {
      for (SESubstanceFraction* sf : m_Configuration->GetFractionInspiredGases())
      {
        str << "\n\tSubstance : " << sf->GetSubstance().GetName() << " Fraction Amount " << sf->GetFractionAmount();
      }
    }

  }
  str << std::flush;
}

