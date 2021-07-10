/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorConfiguration.h"
#include "system/equipment/mechanical_ventilator/SEMechanicalVentilator.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"
#include "substance/SESubstanceConcentration.h"
#include "substance/SESubstanceFraction.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarPressureTimePerVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarTime.h"
#include "io/protobuf/PBEquipmentActions.h"

SEMechanicalVentilatorConfiguration::SEMechanicalVentilatorConfiguration(Logger* logger) : SEMechanicalVentilatorAction(logger)
{
  m_ConfigurationFile = "";
  m_Configuration = nullptr;
  m_MergeType = eMergeType::Append;
}

SEMechanicalVentilatorConfiguration::~SEMechanicalVentilatorConfiguration()
{
  m_ConfigurationFile = "";
  SAFE_DELETE(m_Configuration);
  m_MergeType = eMergeType::Append;
}

void SEMechanicalVentilatorConfiguration::Clear()
{
  SEMechanicalVentilatorAction::Clear();
  m_ConfigurationFile = "";
  if (m_Configuration)
    m_Configuration->Clear();
  m_MergeType = eMergeType::Append;
}

void SEMechanicalVentilatorConfiguration::Copy(const SEMechanicalVentilatorConfiguration& src, const SESubstanceManager& subMgr, bool preserveState)
{// Using Bindings to make a copy
  PBEquipmentAction::Copy(src, *this, subMgr);
}

bool SEMechanicalVentilatorConfiguration::IsValid() const
{
  return SEMechanicalVentilatorAction::IsValid() && (HasConfiguration() || HasConfigurationFile());
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
  return GetConfiguration().GetScalar(name);
}

bool SEMechanicalVentilatorConfiguration::HasConfiguration() const
{
  return m_Configuration != nullptr;
}
SEMechanicalVentilator& SEMechanicalVentilatorConfiguration::GetConfiguration()
{
  if (m_Configuration == nullptr)
    m_Configuration = new SEMechanicalVentilator(GetLogger());
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
  m_ConfigurationFile = fileName;
}
bool SEMechanicalVentilatorConfiguration::HasConfigurationFile() const
{
  return !m_ConfigurationFile.empty();
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
  if (HasConfigurationFile())
  {
    str << "\n\tConfiguration File: "; str << m_ConfigurationFile;
  }
  else if (HasConfiguration())
  {
    str << "\n\tConnection: " << eMechanicalVentilator_Connection_Name(m_Configuration->GetConnection());
    str << "\n\tPositiveEndExpiredPressure: "; m_Configuration->HasPositiveEndExpiredPressure() ? str << m_Configuration->GetPositiveEndExpiredPressure() : str << "NaN";
    str << "\n\tFunctionalResidualCapacity: "; m_Configuration->HasFunctionalResidualCapacity() ? str << m_Configuration->GetFunctionalResidualCapacity() : str << "NaN";
    str << "\n\tExpirationCycleFlow: "; m_Configuration->HasExpirationCycleFlow() ? str << m_Configuration->GetExpirationCycleFlow() : str << "NaN";
    str << "\n\tExpirationCyclePressure: "; m_Configuration->HasExpirationCyclePressure() ? str << m_Configuration->GetExpirationCyclePressure() : str << "NaN";
    str << "\n\tExpirationCycleVolume: "; m_Configuration->HasExpirationCycleVolume() ? str << m_Configuration->GetExpirationCycleVolume() : str << "NaN";
    str << "\n\tExpirationCycleTime: "; m_Configuration->HasExpirationCycleTime() ? str << m_Configuration->GetExpirationCycleTime() : str << "NaN";
    str << "\n\tExpirationTubeResistance: "; m_Configuration->HasExpirationTubeResistance() ? str << m_Configuration->GetExpirationTubeResistance() : str << "NaN";
    str << "\n\tExpirationValveResistance: "; m_Configuration->HasExpirationValveResistance() ? str << m_Configuration->GetExpirationValveResistance() : str << "NaN";
    str << "\n\tExpirationWaveform: " << eMechanicalVentilator_DriverWaveform_Name(m_Configuration->GetExpirationWaveform());
    str << "\n\tExpirationWaveformPeriod: " << m_Configuration->HasExpirationWaveformPeriod() ? str << m_Configuration->GetExpirationWaveformPeriod() : str << "NaN";
    str << "\n\tInspirationLimitFlow: "; m_Configuration->HasInspirationLimitFlow() ? str << m_Configuration->GetInspirationLimitFlow() : str << "NaN";
    str << "\n\tInspirationLimitPressure: "; m_Configuration->HasInspirationLimitPressure() ? str << m_Configuration->GetInspirationLimitPressure() : str << "NaN";
    str << "\n\tInspirationLimitVolume: "; m_Configuration->HasInspirationLimitVolume() ? str << m_Configuration->GetInspirationLimitVolume() : str << "NaN";
    str << "\n\tInspirationPauseTime: "; m_Configuration->HasInspirationPauseTime() ? str << m_Configuration->GetInspirationPauseTime() : str << "NaN";
    str << "\n\tPeakInspiratoryPressure: "; m_Configuration->HasPeakInspiratoryPressure() ? str << m_Configuration->GetPeakInspiratoryPressure() : str << "NaN";
    str << "\n\tInspirationTargetFlow: "; m_Configuration->HasInspirationTargetFlow() ? str << m_Configuration->GetInspirationTargetFlow() : str << "NaN";
    str << "\n\tInspirationMachineTriggerTime: "; m_Configuration->HasInspirationMachineTriggerTime() ? str << m_Configuration->GetInspirationMachineTriggerTime() : str << "NaN";
    str << "\n\tInspirationPatientTriggerFlow: "; m_Configuration->HasInspirationPatientTriggerFlow() ? str << m_Configuration->GetInspirationPatientTriggerFlow() : str << "NaN";
    str << "\n\tInspirationPatientTriggerPressure: "; m_Configuration->HasInspirationPatientTriggerPressure() ? str << m_Configuration->GetInspirationPatientTriggerPressure() : str << "NaN";
    str << "\n\tInspirationTubeResistance: "; m_Configuration->HasInspirationTubeResistance() ? str << m_Configuration->GetInspirationTubeResistance() : str << "NaN";
    str << "\n\tInspirationValveResistance: "; m_Configuration->HasInspirationValveResistance() ? str << m_Configuration->GetInspirationValveResistance() : str << "NaN";
    str << "\n\tInspirationWaveform: " << eMechanicalVentilator_DriverWaveform_Name(m_Configuration->GetInspirationWaveform());
    str << "\n\tInspirationWaveformPeriod: " << m_Configuration->HasInspirationWaveformPeriod() ? str << m_Configuration->GetInspirationWaveformPeriod() : str << "NaN";
    str << "\n\tExpirationLimbVolume: "; m_Configuration->HasExpirationLimbVolume() ? str << m_Configuration->GetExpirationLimbVolume() : str << "NaN";
    str << "\n\tExpirationValveVolume: "; m_Configuration->HasExpirationValveVolume() ? str << m_Configuration->GetExpirationValveVolume() : str << "NaN";
    str << "\n\tInspirationLimbVolume: "; m_Configuration->HasInspirationLimbVolume() ? str << m_Configuration->GetInspirationLimbVolume() : str << "NaN";
    str << "\n\tInspirationValveVolume: "; m_Configuration->HasInspirationValveVolume() ? str << m_Configuration->GetInspirationValveVolume() : str << "NaN";
    str << "\n\tYPieceVolume: "; m_Configuration->HasYPieceVolume() ? str << m_Configuration->GetYPieceVolume() : str << "NaN";
    str << "\n\tConnectionVolume: "; m_Configuration->HasConnectionVolume() ? str << m_Configuration->GetConnectionVolume() : str << "NaN";
    if (m_Configuration->HasFractionInspiredGas())
    {
      for (SESubstanceFraction* sf : m_Configuration->GetFractionInspiredGases())
      {
        str << "\n\tSubstance : " << sf->GetSubstance().GetName() << " Fraction Amount " << sf->GetFractionAmount();
      }
    }
    if (m_Configuration->HasConcentrationInspiredAerosol())
    {
      for (SESubstanceConcentration* sc : m_Configuration->GetConcentrationInspiredAerosols())
      {
        str << "\n\tSubstance : " << sc->GetSubstance().GetName() << " Concentration " << sc->GetConcentration();
      }
    }
  }
  str << "\n\tMergeType: " << eMergeType_Name(m_MergeType);
  str << std::flush;
}
