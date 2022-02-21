/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "cdm/CommonDefs.h"
#include "cdm/system/equipment/ecmo/actions/SEECMOConfiguration.h"
#include "cdm/system/equipment/ecmo/SEECMOSettings.h"
#include "cdm/substance/SESubstance.h"
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/substance/SESubstanceConcentration.h"
#include "cdm/substance/SESubstanceFraction.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarPressureTimePerVolume.h"
#include "cdm/properties/SEScalarVolumePerPressure.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/io/protobuf/PBEquipmentActions.h"

SEECMOConfiguration::SEECMOConfiguration(Logger* logger) : SEECMOAction(logger)
{
  m_SettingsFile = "";
  m_Settings = nullptr;
  m_MergeType = eMergeType::Append;
}

SEECMOConfiguration::~SEECMOConfiguration()
{
  m_SettingsFile = "";
  SAFE_DELETE(m_Settings);
  m_MergeType = eMergeType::Append;
}

void SEECMOConfiguration::Clear()
{
  SEECMOAction::Clear();
  m_SettingsFile = "";
  if (m_Settings)
    m_Settings->Clear();
  m_MergeType = eMergeType::Append;
}

void SEECMOConfiguration::Copy(const SEECMOConfiguration& src, const SESubstanceManager& subMgr, bool /*preserveState*/)
{// Using Bindings to make a copy
  PBEquipmentAction::Copy(src, *this, subMgr);
}

bool SEECMOConfiguration::IsValid() const
{
  return SEECMOAction::IsValid() && (HasSettings() || HasSettingsFile());
}

bool SEECMOConfiguration::IsActive() const
{
  return SEECMOAction::IsActive();
}
void SEECMOConfiguration::Deactivate()
{
  SEECMOAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEECMOConfiguration::GetScalar(const std::string& name)
{
  return GetSettings().GetScalar(name);
}

bool SEECMOConfiguration::HasSettings() const
{
  return m_Settings != nullptr;
}
SEECMOSettings& SEECMOConfiguration::GetSettings()
{
  if (m_Settings == nullptr)
    m_Settings = new SEECMOSettings(GetLogger());
  return *m_Settings;
}
const SEECMOSettings* SEECMOConfiguration::GetSettings() const
{
  return m_Settings;
}

std::string SEECMOConfiguration::GetSettingsFile() const
{
  return m_SettingsFile;
}
void SEECMOConfiguration::SetSettingsFile(const std::string& fileName)
{
  m_SettingsFile = fileName;
}
bool SEECMOConfiguration::HasSettingsFile() const
{
  return !m_SettingsFile.empty();
}

void SEECMOConfiguration::SetMergeType(eMergeType m)
{
  m_MergeType = m;
}
eMergeType SEECMOConfiguration::GetMergeType() const
{
  return m_MergeType;
}

void SEECMOConfiguration::ToString(std::ostream &str) const
{
  str << "ECMO Configuration";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  if (HasSettingsFile())
  {
    str << "\n\tSettings File: "; str << m_SettingsFile;
  }
  else if (HasSettings())
  {
    str << "\n\tConnection: " << eSwitch_Name(m_Settings->GetConnection());
    str << "\n\tPositiveEndExpiredPressure: "; m_Settings->HasPositiveEndExpiredPressure() ? str << m_Settings->GetPositiveEndExpiredPressure() : str << "NaN";
    str << "\n\tFunctionalResidualCapacity: "; m_Settings->HasFunctionalResidualCapacity() ? str << m_Settings->GetFunctionalResidualCapacity() : str << "NaN";
    str << "\n\tExpirationCycleFlow: "; m_Settings->HasExpirationCycleFlow() ? str << m_Settings->GetExpirationCycleFlow() : str << "NaN";
    str << "\n\tExpirationCyclePressure: "; m_Settings->HasExpirationCyclePressure() ? str << m_Settings->GetExpirationCyclePressure() : str << "NaN";
    str << "\n\tExpirationCycleVolume: "; m_Settings->HasExpirationCycleVolume() ? str << m_Settings->GetExpirationCycleVolume() : str << "NaN";
    str << "\n\tExpirationCycleTime: "; m_Settings->HasExpirationCycleTime() ? str << m_Settings->GetExpirationCycleTime() : str << "NaN";
    str << "\n\tExpirationCycleRespiratoryModel: " << eSwitch_Name(m_Settings->GetExpirationCycleRespiratoryModel());
    str << "\n\tExpirationTubeResistance: "; m_Settings->HasExpirationTubeResistance() ? str << m_Settings->GetExpirationTubeResistance() : str << "NaN";
    str << "\n\tExpirationValveResistance: "; m_Settings->HasExpirationValveResistance() ? str << m_Settings->GetExpirationValveResistance() : str << "NaN";
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
    str << "\n\tInspirationPatientTriggerRespiratoryModel: " << eSwitch_Name(m_Settings->GetInspirationPatientTriggerRespiratoryModel());
    str << "\n\tInspirationTubeResistance: "; m_Settings->HasInspirationTubeResistance() ? str << m_Settings->GetInspirationTubeResistance() : str << "NaN";
    str << "\n\tInspirationValveResistance: "; m_Settings->HasInspirationValveResistance() ? str << m_Settings->GetInspirationValveResistance() : str << "NaN";
    str << "\n\tInspirationWaveformPeriod: "; m_Settings->HasInspirationWaveformPeriod() ? str << m_Settings->GetInspirationWaveformPeriod() : str << "NaN";
    str << "\n\tExpirationLimbVolume: "; m_Settings->HasExpirationLimbVolume() ? str << m_Settings->GetExpirationLimbVolume() : str << "NaN";
    str << "\n\tExpirationValveVolume: "; m_Settings->HasExpirationValveVolume() ? str << m_Settings->GetExpirationValveVolume() : str << "NaN";
    str << "\n\tInspirationLimbVolume: "; m_Settings->HasInspirationLimbVolume() ? str << m_Settings->GetInspirationLimbVolume() : str << "NaN";
    str << "\n\tInspirationValveVolume: "; m_Settings->HasInspirationValveVolume() ? str << m_Settings->GetInspirationValveVolume() : str << "NaN";
    str << "\n\tReliefValveThreshold: "; m_Settings->HasReliefValveThreshold() ? str << m_Settings->GetReliefValveThreshold() : str << "NaN";
    str << "\n\tYPieceVolume: "; m_Settings->HasYPieceVolume() ? str << m_Settings->GetYPieceVolume() : str << "NaN";
    str << "\n\tConnectionVolume: "; m_Settings->HasConnectionVolume() ? str << m_Settings->GetConnectionVolume() : str << "NaN";
    str << "\n\tCompliance: "; m_Settings->HasCompliance() ? str << m_Settings->GetCompliance() : str << "NaN";
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
