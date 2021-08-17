/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "cdm/CommonDefs.h"
#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorConfiguration.h"
#include "cdm/system/equipment/mechanical_ventilator/SEMechanicalVentilatorSettings.h"
#include "cdm/substance/SESubstance.h"
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/substance/SESubstanceConcentration.h"
#include "cdm/substance/SESubstanceFraction.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarPressureTimePerVolume.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/io/protobuf/PBEquipmentActions.h"

SEMechanicalVentilatorConfiguration::SEMechanicalVentilatorConfiguration(Logger* logger) : SEMechanicalVentilatorAction(logger)
{
  m_SettingsFile = "";
  m_Settings = nullptr;
  m_MergeType = eMergeType::Append;
}

SEMechanicalVentilatorConfiguration::~SEMechanicalVentilatorConfiguration()
{
  m_SettingsFile = "";
  SAFE_DELETE(m_Settings);
  m_MergeType = eMergeType::Append;
}

void SEMechanicalVentilatorConfiguration::Clear()
{
  SEMechanicalVentilatorAction::Clear();
  m_SettingsFile = "";
  if (m_Settings)
    m_Settings->Clear();
  m_MergeType = eMergeType::Append;
}

DISABLE_UNREFERENCED_FORMAL_PARAMETER
void SEMechanicalVentilatorConfiguration::Copy(const SEMechanicalVentilatorConfiguration& src, const SESubstanceManager& subMgr, bool preserveState)
{// Using Bindings to make a copy
  PBEquipmentAction::Copy(src, *this, subMgr);
}
DISABLE_WARNING_POP

bool SEMechanicalVentilatorConfiguration::IsValid() const
{
  return SEMechanicalVentilatorAction::IsValid() && (HasSettings() || HasSettingsFile());
}

bool SEMechanicalVentilatorConfiguration::IsActive() const
{
  return SEMechanicalVentilatorAction::IsActive();
}
void SEMechanicalVentilatorConfiguration::Deactivate()
{
  SEMechanicalVentilatorAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEMechanicalVentilatorConfiguration::GetScalar(const std::string& name)
{
  return GetSettings().GetScalar(name);
}

bool SEMechanicalVentilatorConfiguration::HasSettings() const
{
  return m_Settings != nullptr;
}
SEMechanicalVentilatorSettings& SEMechanicalVentilatorConfiguration::GetSettings()
{
  if (m_Settings == nullptr)
    m_Settings = new SEMechanicalVentilatorSettings(GetLogger());
  return *m_Settings;
}
const SEMechanicalVentilatorSettings* SEMechanicalVentilatorConfiguration::GetSettings() const
{
  return m_Settings;
}

std::string SEMechanicalVentilatorConfiguration::GetSettingsFile() const
{
  return m_SettingsFile;
}
void SEMechanicalVentilatorConfiguration::SetSettingsFile(const std::string& fileName)
{
  m_SettingsFile = fileName;
}
bool SEMechanicalVentilatorConfiguration::HasSettingsFile() const
{
  return !m_SettingsFile.empty();
}

void SEMechanicalVentilatorConfiguration::SetMergeType(eMergeType m)
{
  m_MergeType = m;
}
eMergeType SEMechanicalVentilatorConfiguration::GetMergeType() const
{
  return m_MergeType;
}

void SEMechanicalVentilatorConfiguration::ToString(std::ostream &str) const
{
  str << "Mechanical Ventilator Configuration";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  if (HasSettingsFile())
  {
    str << "\n\tSettings File: "; str << m_SettingsFile;
  }
  else if (HasSettings())
  {
    str << "\n\tConnection: " << eMechanicalVentilator_Connection_Name(m_Settings->GetConnection());
    str << "\n\tPositiveEndExpiredPressure: "; m_Settings->HasPositiveEndExpiredPressure() ? str << m_Settings->GetPositiveEndExpiredPressure() : str << "NaN";
    str << "\n\tFunctionalResidualCapacity: "; m_Settings->HasFunctionalResidualCapacity() ? str << m_Settings->GetFunctionalResidualCapacity() : str << "NaN";
    str << "\n\tExpirationCycleFlow: "; m_Settings->HasExpirationCycleFlow() ? str << m_Settings->GetExpirationCycleFlow() : str << "NaN";
    str << "\n\tExpirationCyclePressure: "; m_Settings->HasExpirationCyclePressure() ? str << m_Settings->GetExpirationCyclePressure() : str << "NaN";
    str << "\n\tExpirationCycleVolume: "; m_Settings->HasExpirationCycleVolume() ? str << m_Settings->GetExpirationCycleVolume() : str << "NaN";
    str << "\n\tExpirationCycleTime: "; m_Settings->HasExpirationCycleTime() ? str << m_Settings->GetExpirationCycleTime() : str << "NaN";
    str << "\n\tExpirationTubeResistance: "; m_Settings->HasExpirationTubeResistance() ? str << m_Settings->GetExpirationTubeResistance() : str << "NaN";
    str << "\n\tExpirationValveResistance: "; m_Settings->HasExpirationValveResistance() ? str << m_Settings->GetExpirationValveResistance() : str << "NaN";
    str << "\n\tExpirationWaveform: " << eMechanicalVentilator_DriverWaveform_Name(m_Settings->GetExpirationWaveform());
    str << "\n\tExpirationWaveformPeriod: "; m_Settings->HasExpirationWaveformPeriod() ? str << m_Settings->GetExpirationWaveformPeriod() : str << "NaN";
    str << "\n\tInspirationLimitFlow: "; m_Settings->HasInspirationLimitFlow() ? str << m_Settings->GetInspirationLimitFlow() : str << "NaN";
    str << "\n\tInspirationLimitPressure: "; m_Settings->HasInspirationLimitPressure() ? str << m_Settings->GetInspirationLimitPressure() : str << "NaN";
    str << "\n\tInspirationLimitVolume: "; m_Settings->HasInspirationLimitVolume() ? str << m_Settings->GetInspirationLimitVolume() : str << "NaN";
    str << "\n\tInspirationPauseTime: "; m_Settings->HasInspirationPauseTime() ? str << m_Settings->GetInspirationPauseTime() : str << "NaN";
    str << "\n\tPeakInspiratoryPressure: "; m_Settings->HasPeakInspiratoryPressure() ? str << m_Settings->GetPeakInspiratoryPressure() : str << "NaN";
    str << "\n\tInspirationTargetFlow: "; m_Settings->HasInspirationTargetFlow() ? str << m_Settings->GetInspirationTargetFlow() : str << "NaN";
    str << "\n\tInspirationMachineTriggerTime: "; m_Settings->HasInspirationMachineTriggerTime() ? str << m_Settings->GetInspirationMachineTriggerTime() : str << "NaN";
    str << "\n\tInspirationPatientTriggerFlow: "; m_Settings->HasInspirationPatientTriggerFlow() ? str << m_Settings->GetInspirationPatientTriggerFlow() : str << "NaN";
    str << "\n\tInspirationPatientTriggerPressure: "; m_Settings->HasInspirationPatientTriggerPressure() ? str << m_Settings->GetInspirationPatientTriggerPressure() : str << "NaN";
    str << "\n\tInspirationTubeResistance: "; m_Settings->HasInspirationTubeResistance() ? str << m_Settings->GetInspirationTubeResistance() : str << "NaN";
    str << "\n\tInspirationValveResistance: "; m_Settings->HasInspirationValveResistance() ? str << m_Settings->GetInspirationValveResistance() : str << "NaN";
    str << "\n\tInspirationWaveform: " << eMechanicalVentilator_DriverWaveform_Name(m_Settings->GetInspirationWaveform());
    str << "\n\tInspirationWaveformPeriod: "; m_Settings->HasInspirationWaveformPeriod() ? str << m_Settings->GetInspirationWaveformPeriod() : str << "NaN";
    str << "\n\tExpirationLimbVolume: "; m_Settings->HasExpirationLimbVolume() ? str << m_Settings->GetExpirationLimbVolume() : str << "NaN";
    str << "\n\tExpirationValveVolume: "; m_Settings->HasExpirationValveVolume() ? str << m_Settings->GetExpirationValveVolume() : str << "NaN";
    str << "\n\tInspirationLimbVolume: "; m_Settings->HasInspirationLimbVolume() ? str << m_Settings->GetInspirationLimbVolume() : str << "NaN";
    str << "\n\tInspirationValveVolume: "; m_Settings->HasInspirationValveVolume() ? str << m_Settings->GetInspirationValveVolume() : str << "NaN";
    str << "\n\tYPieceVolume: "; m_Settings->HasYPieceVolume() ? str << m_Settings->GetYPieceVolume() : str << "NaN";
    str << "\n\tConnectionVolume: "; m_Settings->HasConnectionVolume() ? str << m_Settings->GetConnectionVolume() : str << "NaN";
    if (m_Settings->HasFractionInspiredGas())
    {
      for (SESubstanceFraction* sf : m_Settings->GetFractionInspiredGases())
      {
        str << "\n\tSubstance : " << sf->GetSubstance().GetName() << " Fraction Amount " << sf->GetFractionAmount();
      }
    }
    if (m_Settings->HasConcentrationInspiredAerosol())
    {
      for (SESubstanceConcentration* sc : m_Settings->GetConcentrationInspiredAerosols())
      {
        str << "\n\tSubstance : " << sc->GetSubstance().GetName() << " Concentration " << sc->GetConcentration();
      }
    }
  }
  str << "\n\tMergeType: " << eMergeType_Name(m_MergeType);
  str << std::flush;
}
