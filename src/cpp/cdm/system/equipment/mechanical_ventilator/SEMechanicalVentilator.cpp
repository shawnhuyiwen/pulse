/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "system/equipment/mechanical_ventilator/SEMechanicalVentilator.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"
#include "substance/SESubstanceConcentration.h"
#include "substance/SESubstanceFraction.h"

// State Actions
#include "system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorConfiguration.h"

#include "properties/SEScalar0To1.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarPressureTimePerVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarTime.h"
#include "io/protobuf/PBMechanicalVentilator.h"

SEMechanicalVentilator::SEMechanicalVentilator(Logger* logger) : SEEquipment(logger)
{
  m_Connection = eMechanicalVentilator_Connection::Off;
  m_EndotrachealTubeResistance = nullptr;

  m_PositiveEndExpiredPressure = nullptr;
  m_FunctionalResidualCapacity = nullptr;

  m_ExpirationCycleFlow = nullptr;
  m_ExpirationCyclePressure = nullptr;
  m_ExpirationCycleVolume = nullptr;
  m_ExpirationCycleTime = nullptr;

  m_ExpirationTubeResistance = nullptr;
  m_ExpirationValveResistance = nullptr;
  m_ExpirationWaveform = eMechanicalVentilator_DriverWaveform::NullDriverWaveform;

  m_InspirationLimitFlow = nullptr;
  m_InspirationLimitPressure = nullptr;
  m_InspirationLimitVolume = nullptr;

  m_InspirationPauseTime = nullptr;

  m_PeakInspiratoryPressure = nullptr;
  m_InspirationTargetFlow = nullptr;

  m_InspirationMachineTriggerTime = nullptr;

  m_InspirationPatientTriggerFlow = nullptr;
  m_InspirationPatientTriggerPressure = nullptr;  

  m_InspirationTubeResistance = nullptr;
  m_InspirationValveResistance = nullptr;
  m_InspirationWaveform = eMechanicalVentilator_DriverWaveform::NullDriverWaveform;
}

SEMechanicalVentilator::~SEMechanicalVentilator()
{
  m_Connection = eMechanicalVentilator_Connection::Off;
  SAFE_DELETE(m_EndotrachealTubeResistance);

  SAFE_DELETE(m_PositiveEndExpiredPressure);
  SAFE_DELETE(m_FunctionalResidualCapacity);

  SAFE_DELETE(m_ExpirationCycleFlow);
  SAFE_DELETE(m_ExpirationCyclePressure);
  SAFE_DELETE(m_ExpirationCycleVolume);
  SAFE_DELETE(m_ExpirationCycleTime);

  SAFE_DELETE(m_ExpirationTubeResistance);
  SAFE_DELETE(m_ExpirationValveResistance);
  m_ExpirationWaveform = eMechanicalVentilator_DriverWaveform::NullDriverWaveform;

  SAFE_DELETE(m_InspirationLimitFlow);
  SAFE_DELETE(m_InspirationLimitPressure);
  SAFE_DELETE(m_InspirationLimitVolume);

  SAFE_DELETE(m_InspirationPauseTime);

  SAFE_DELETE(m_PeakInspiratoryPressure);
  SAFE_DELETE(m_InspirationTargetFlow);

  SAFE_DELETE(m_InspirationMachineTriggerTime);

  SAFE_DELETE(m_InspirationPatientTriggerFlow);
  SAFE_DELETE(m_InspirationPatientTriggerPressure);

  SAFE_DELETE(m_InspirationTubeResistance);
  SAFE_DELETE(m_InspirationValveResistance);
  m_InspirationWaveform = eMechanicalVentilator_DriverWaveform::NullDriverWaveform;

  DELETE_VECTOR(m_FractionInspiredGases);
  m_cFractionInspiredGases.clear();
  m_FractionInspiredGases.clear();

  DELETE_VECTOR(m_ConcentrationInspiredAerosols);
  m_cConcentrationInspiredAerosols.clear();
  m_ConcentrationInspiredAerosols.clear();
}

void SEMechanicalVentilator::Clear()
{
  SEEquipment::Clear();

  m_Connection = eMechanicalVentilator_Connection::Off;
  INVALIDATE_PROPERTY(m_EndotrachealTubeResistance);

  INVALIDATE_PROPERTY(m_PositiveEndExpiredPressure);
  INVALIDATE_PROPERTY(m_FunctionalResidualCapacity);

  INVALIDATE_PROPERTY(m_ExpirationCycleFlow);
  INVALIDATE_PROPERTY(m_ExpirationCyclePressure);
  INVALIDATE_PROPERTY(m_ExpirationCycleVolume);
  INVALIDATE_PROPERTY(m_ExpirationCycleTime);

  INVALIDATE_PROPERTY(m_ExpirationTubeResistance);
  INVALIDATE_PROPERTY(m_ExpirationValveResistance);
  m_ExpirationWaveform = eMechanicalVentilator_DriverWaveform::NullDriverWaveform;

  INVALIDATE_PROPERTY(m_InspirationLimitFlow);
  INVALIDATE_PROPERTY(m_InspirationLimitPressure);
  INVALIDATE_PROPERTY(m_InspirationLimitVolume);

  INVALIDATE_PROPERTY(m_InspirationPauseTime);

  INVALIDATE_PROPERTY(m_PeakInspiratoryPressure);
  INVALIDATE_PROPERTY(m_InspirationTargetFlow);

  INVALIDATE_PROPERTY(m_InspirationMachineTriggerTime);

  INVALIDATE_PROPERTY(m_InspirationPatientTriggerFlow);
  INVALIDATE_PROPERTY(m_InspirationPatientTriggerPressure);

  INVALIDATE_PROPERTY(m_InspirationTubeResistance);
  INVALIDATE_PROPERTY(m_InspirationValveResistance);
  m_InspirationWaveform = eMechanicalVentilator_DriverWaveform::NullDriverWaveform;

  RemoveFractionInspiredGases();
  RemoveConcentrationInspiredAerosols();
}


void SEMechanicalVentilator::ProcessConfiguration(SEMechanicalVentilatorConfiguration& config, SESubstanceManager& subMgr)
{
  if (config.HasConfiguration())
    Merge(config.GetConfiguration(), subMgr);
  else if (config.HasConfigurationFile())
  {
    // Update the action with the file contents
    std::string cfg_file = config.GetConfigurationFile();
    if (!config.GetConfiguration().SerializeFromFile(cfg_file, subMgr))
      Error("Unable to load configuration file", "SEMechanicalVentilator::ProcessConfiguration");
    Merge(config.GetConfiguration(), subMgr);
  }
  StateChange();
}
void SEMechanicalVentilator::Merge(const SEMechanicalVentilator& from, SESubstanceManager& subMgr)
{
  if(from.m_Connection!=eMechanicalVentilator_Connection::NullConnection)
    SetConnection(from.m_Connection);
  COPY_PROPERTY(EndotrachealTubeResistance);

  COPY_PROPERTY(PositiveEndExpiredPressure);
  COPY_PROPERTY(FunctionalResidualCapacity);

  COPY_PROPERTY(ExpirationCycleFlow);
  COPY_PROPERTY(ExpirationCyclePressure);
  COPY_PROPERTY(ExpirationCycleVolume);
  COPY_PROPERTY(ExpirationCycleTime);

  COPY_PROPERTY(ExpirationTubeResistance);
  COPY_PROPERTY(ExpirationValveResistance);
  if (from.m_ExpirationWaveform != eMechanicalVentilator_DriverWaveform::NullDriverWaveform)
    SetExpirationWaveform(from.m_ExpirationWaveform);

  COPY_PROPERTY(InspirationLimitFlow);
  COPY_PROPERTY(InspirationLimitPressure);
  COPY_PROPERTY(InspirationLimitVolume);

  COPY_PROPERTY(InspirationPauseTime);

  COPY_PROPERTY(PeakInspiratoryPressure);
  COPY_PROPERTY(InspirationTargetFlow);

  COPY_PROPERTY(InspirationMachineTriggerTime);

  COPY_PROPERTY(InspirationPatientTriggerFlow);
  COPY_PROPERTY(InspirationPatientTriggerPressure);

  COPY_PROPERTY(InspirationTubeResistance);
  COPY_PROPERTY(InspirationValveResistance);
  if (from.m_InspirationWaveform != eMechanicalVentilator_DriverWaveform::NullDriverWaveform)
    SetInspirationWaveform(from.m_InspirationWaveform);

  // Always need to provide a full (fractions sum to 1) substance list that replaces current
  if (from.HasFractionInspiredGas())
  {
    size_t cnt = 0;
    double amt;
    double total = 0;
    const SESubstance* sub;
    // Since we are allowing only O2 to be specified
    // Remove everything so we know what is intentionally not provided
    // And what is intentially set to 0 (don't just set to 0)
    RemoveFractionInspiredGases();
    for (SESubstanceFraction* osf : from.m_FractionInspiredGases)
    {
      sub = subMgr.GetSubstance(osf->GetSubstance().GetName());
      if (sub == nullptr)
      {
        Error("Do not have substance : " + osf->GetSubstance().GetName());
        continue;
      }
      SESubstanceFraction& sf = GetFractionInspiredGas(*sub);
      amt = osf->GetFractionAmount().GetValue();
      sf.GetFractionAmount().SetValue(amt);
      subMgr.AddActiveSubstance(*sub);
      total += amt;
      if (amt > 0)
        cnt++; // Count of non-zero fractions
    }

    // It's Ok if you ONLY set Oxygen, i.e. FiO2
    // Ventilator models should understand that common setting
    if (!SEScalar::IsValue(1, total))
    {
      bool err = false;
      if (cnt != 1)
        err = true;
      else
      {
        const SESubstance* o2 = subMgr.GetSubstance("Oxygen");
        if (!GetFractionInspiredGas(*o2).GetFractionAmount().IsPositive())
          err = true;
      }
      if(err)
        Error("Mechanical Ventilator substance fractions do not sum to 1");
    }
  }

  if (from.HasConcentrationInspiredAerosol())
  {
    for (SESubstanceConcentration* sc : from.m_ConcentrationInspiredAerosols)
    {
      SESubstanceConcentration& mine = GetConcentrationInspiredAerosol(sc->GetSubstance());
      mine.GetConcentration().Set(sc->GetConcentration());
    }
  }
}

bool SEMechanicalVentilator::SerializeToString(std::string& output, SerializationFormat m) const
{
  return PBMechanicalVentilator::SerializeToString(*this, output, m);
}
bool SEMechanicalVentilator::SerializeToFile(const std::string& filename) const
{
  return PBMechanicalVentilator::SerializeToFile(*this, filename);
}
bool SEMechanicalVentilator::SerializeFromString(const std::string& src, SerializationFormat m, const SESubstanceManager& subMgr)
{
  return PBMechanicalVentilator::SerializeFromString(src, *this, m, subMgr);
}
bool SEMechanicalVentilator::SerializeFromFile(const std::string& filename, const SESubstanceManager& subMgr)
{
  return PBMechanicalVentilator::SerializeFromFile(filename, *this, subMgr);
}

const SEScalar* SEMechanicalVentilator::GetScalar(const std::string& name)
{
  if (name == "EndotrachealTubeResistance")
    return &GetEndotrachealTubeResistance();

  if (name == "PositiveEndExpiredPressure")
    return &GetPositiveEndExpiredPressure();
  if (name == "FunctionalResidualCapacity")
    return &GetFunctionalResidualCapacity();

  if (name == "ExpirationCycleFlow")
    return &GetExpirationCycleFlow();
  if (name == "ExpirationCyclePressure")
    return &GetExpirationCyclePressure();
  if (name == "ExpirationCycleVolume")
    return &GetExpirationCycleVolume();
  if (name == "ExpirationCycleTime")
    return &GetExpirationCycleTime();

  if (name == "ExpirationTubeResistance")
    return &GetExpirationTubeResistance();
  if (name == "ExpirationValveResistance")
    return &GetExpirationValveResistance();

  if (name == "InspirationLimitFlow")
    return &GetInspirationLimitFlow();
  if (name == "InspirationLimitPressure")
    return &GetInspirationLimitPressure();
  if (name == "InspirationLimitVolume")
    return &GetInspirationLimitVolume();

  if (name == "InspirationPauseTime")
    return &GetInspirationPauseTime();

  if (name == "PeakInspiratoryPressure")
    return &GetPeakInspiratoryPressure();
  if (name == "InspirationTargetFlow")
    return &GetInspirationTargetFlow();

  if (name == "InspirationMachineTriggerTime")
    return &GetInspirationMachineTriggerTime();

  if (name == "InspirationPatientTriggerFlow")
    return &GetInspirationPatientTriggerFlow();
  if (name == "InspirationPatientTriggerPressure")
    return &GetInspirationPatientTriggerPressure();

  if (name == "InspirationTubeResistance")
    return &GetInspirationTubeResistance();
  if (name == "InspirationValveResistance")
    return &GetInspirationValveResistance();

  // I did not support for getting a specific gas/aerosol scalars due to lack of coffee
  return nullptr;
}

void SEMechanicalVentilator::SetConnection(eMechanicalVentilator_Connection c)
{
  m_Connection = c;
}
eMechanicalVentilator_Connection SEMechanicalVentilator::GetConnection() const
{
  return m_Connection;
}

bool SEMechanicalVentilator::HasEndotrachealTubeResistance() const
{
  return m_EndotrachealTubeResistance == nullptr ? false : m_EndotrachealTubeResistance->IsValid();
}
SEScalarPressureTimePerVolume& SEMechanicalVentilator::GetEndotrachealTubeResistance()
{
  if (m_EndotrachealTubeResistance == nullptr)
    m_EndotrachealTubeResistance = new SEScalarPressureTimePerVolume();
  return *m_EndotrachealTubeResistance;
}
double SEMechanicalVentilator::GetEndotrachealTubeResistance(const PressureTimePerVolumeUnit& unit) const
{
  if (m_EndotrachealTubeResistance == nullptr)
    return SEScalar::dNaN();
  return m_EndotrachealTubeResistance->GetValue(unit);
}

bool SEMechanicalVentilator::HasPositiveEndExpiredPressure() const
{
  return m_PositiveEndExpiredPressure == nullptr ? false : m_PositiveEndExpiredPressure->IsValid();
}
SEScalarPressure& SEMechanicalVentilator::GetPositiveEndExpiredPressure()
{
  if (m_PositiveEndExpiredPressure == nullptr)
    m_PositiveEndExpiredPressure = new SEScalarPressure();
  return *m_PositiveEndExpiredPressure;
}
double SEMechanicalVentilator::GetPositiveEndExpiredPressure(const PressureUnit& unit) const
{
  if (m_PositiveEndExpiredPressure == nullptr)
    return SEScalar::dNaN();
  return m_PositiveEndExpiredPressure->GetValue(unit);
}

bool SEMechanicalVentilator::HasFunctionalResidualCapacity() const
{
  return m_FunctionalResidualCapacity == nullptr ? false : m_FunctionalResidualCapacity->IsValid();
}
SEScalarPressure& SEMechanicalVentilator::GetFunctionalResidualCapacity()
{
  if (m_FunctionalResidualCapacity == nullptr)
    m_FunctionalResidualCapacity = new SEScalarPressure();
  return *m_FunctionalResidualCapacity;
}
double SEMechanicalVentilator::GetFunctionalResidualCapacity(const PressureUnit& unit) const
{
  if (m_FunctionalResidualCapacity == nullptr)
    return SEScalar::dNaN();
  return m_FunctionalResidualCapacity->GetValue(unit);
}

bool SEMechanicalVentilator::HasExpirationCycleFlow() const
{
  return m_ExpirationCycleFlow == nullptr ? false : m_ExpirationCycleFlow->IsValid();
}
SEScalarVolumePerTime& SEMechanicalVentilator::GetExpirationCycleFlow()
{
  if (m_ExpirationCycleFlow == nullptr)
    m_ExpirationCycleFlow = new SEScalarVolumePerTime();
  return *m_ExpirationCycleFlow;
}
double SEMechanicalVentilator::GetExpirationCycleFlow(const VolumePerTimeUnit& unit) const
{
  if (m_ExpirationCycleFlow == nullptr)
    return SEScalar::dNaN();
  return m_ExpirationCycleFlow->GetValue(unit);
}

bool SEMechanicalVentilator::HasExpirationCyclePressure() const
{
  return m_ExpirationCyclePressure == nullptr ? false : m_ExpirationCyclePressure->IsValid();
}
SEScalarPressure& SEMechanicalVentilator::GetExpirationCyclePressure()
{
  if (m_ExpirationCyclePressure == nullptr)
    m_ExpirationCyclePressure = new SEScalarPressure();
  return *m_ExpirationCyclePressure;
}
double SEMechanicalVentilator::GetExpirationCyclePressure(const PressureUnit& unit) const
{
  if (m_ExpirationCyclePressure == nullptr)
    return SEScalar::dNaN();
  return m_ExpirationCyclePressure->GetValue(unit);
}

bool SEMechanicalVentilator::HasExpirationCycleVolume() const
{
  return m_ExpirationCycleVolume == nullptr ? false : m_ExpirationCycleVolume->IsValid();
}
SEScalarVolume& SEMechanicalVentilator::GetExpirationCycleVolume()
{
  if (m_ExpirationCycleVolume == nullptr)
    m_ExpirationCycleVolume = new SEScalarVolume();
  return *m_ExpirationCycleVolume;
}
double SEMechanicalVentilator::GetExpirationCycleVolume(const VolumeUnit& unit) const
{
  if (m_ExpirationCycleVolume == nullptr)
    return SEScalar::dNaN();
  return m_ExpirationCycleVolume->GetValue(unit);
}

bool SEMechanicalVentilator::HasExpirationCycleTime() const
{
  return m_ExpirationCycleTime == nullptr ? false : m_ExpirationCycleTime->IsValid();
}
SEScalarTime& SEMechanicalVentilator::GetExpirationCycleTime()
{
  if (m_ExpirationCycleTime == nullptr)
    m_ExpirationCycleTime = new SEScalarTime();
  return *m_ExpirationCycleTime;
}
double SEMechanicalVentilator::GetExpirationCycleTime(const TimeUnit& unit) const
{
  if (m_ExpirationCycleTime == nullptr)
    return SEScalar::dNaN();
  return m_ExpirationCycleTime->GetValue(unit);
}

bool SEMechanicalVentilator::HasExpirationTubeResistance() const
{
  return m_ExpirationTubeResistance == nullptr ? false : m_ExpirationTubeResistance->IsValid();
}
SEScalarPressureTimePerVolume& SEMechanicalVentilator::GetExpirationTubeResistance()
{
  if (m_ExpirationTubeResistance == nullptr)
    m_ExpirationTubeResistance = new SEScalarPressureTimePerVolume();
  return *m_ExpirationTubeResistance;
}
double SEMechanicalVentilator::GetExpirationTubeResistance(const PressureTimePerVolumeUnit& unit) const
{
  if (m_ExpirationTubeResistance == nullptr)
    return SEScalar::dNaN();
  return m_ExpirationTubeResistance->GetValue(unit);
}

bool SEMechanicalVentilator::HasExpirationValveResistance() const
{
  return m_ExpirationValveResistance == nullptr ? false : m_ExpirationValveResistance->IsValid();
}
SEScalarPressureTimePerVolume& SEMechanicalVentilator::GetExpirationValveResistance()
{
  if (m_ExpirationValveResistance == nullptr)
    m_ExpirationValveResistance = new SEScalarPressureTimePerVolume();
  return *m_ExpirationValveResistance;
}
double SEMechanicalVentilator::GetExpirationValveResistance(const PressureTimePerVolumeUnit& unit) const
{
  if (m_ExpirationValveResistance == nullptr)
    return SEScalar::dNaN();
  return m_ExpirationValveResistance->GetValue(unit);
}

void SEMechanicalVentilator::SetExpirationWaveform(eMechanicalVentilator_DriverWaveform w)
{
  m_ExpirationWaveform = w;
}
eMechanicalVentilator_DriverWaveform SEMechanicalVentilator::GetExpirationWaveform() const
{
  return m_ExpirationWaveform;
}

bool SEMechanicalVentilator::HasInspirationLimitFlow() const
{
  return m_InspirationLimitFlow == nullptr ? false : m_InspirationLimitFlow->IsValid();
}
SEScalarVolumePerTime& SEMechanicalVentilator::GetInspirationLimitFlow()
{
  if (m_InspirationLimitFlow == nullptr)
    m_InspirationLimitFlow = new SEScalarVolumePerTime();
  return *m_InspirationLimitFlow;
}
double SEMechanicalVentilator::GetInspirationLimitFlow(const VolumePerTimeUnit& unit) const
{
  if (m_InspirationLimitFlow == nullptr)
    return SEScalar::dNaN();
  return m_InspirationLimitFlow->GetValue(unit);
}

bool SEMechanicalVentilator::HasInspirationLimitPressure() const
{
  return m_InspirationLimitPressure == nullptr ? false : m_InspirationLimitPressure->IsValid();
}
SEScalarPressure& SEMechanicalVentilator::GetInspirationLimitPressure()
{
  if (m_InspirationLimitPressure == nullptr)
    m_InspirationLimitPressure = new SEScalarPressure();
  return *m_InspirationLimitPressure;
}
double SEMechanicalVentilator::GetInspirationLimitPressure(const PressureUnit& unit) const
{
  if (m_InspirationLimitPressure == nullptr)
    return SEScalar::dNaN();
  return m_InspirationLimitPressure->GetValue(unit);
}

bool SEMechanicalVentilator::HasInspirationLimitVolume() const
{
  return m_InspirationLimitVolume == nullptr ? false : m_InspirationLimitVolume->IsValid();
}
SEScalarVolume& SEMechanicalVentilator::GetInspirationLimitVolume()
{
  if (m_InspirationLimitVolume == nullptr)
    m_InspirationLimitVolume = new SEScalarVolume();
  return *m_InspirationLimitVolume;
}
double SEMechanicalVentilator::GetInspirationLimitVolume(const VolumeUnit& unit) const
{
  if (m_InspirationLimitVolume == nullptr)
    return SEScalar::dNaN();
  return m_InspirationLimitVolume->GetValue(unit);
}

bool SEMechanicalVentilator::HasInspirationPauseTime() const
{
  return m_InspirationPauseTime == nullptr ? false : m_InspirationPauseTime->IsValid();
}
SEScalarTime& SEMechanicalVentilator::GetInspirationPauseTime()
{
  if (m_InspirationPauseTime == nullptr)
    m_InspirationPauseTime = new SEScalarTime();
  return *m_InspirationPauseTime;
}
double SEMechanicalVentilator::GetInspirationPauseTime(const TimeUnit& unit) const
{
  if (m_InspirationPauseTime == nullptr)
    return SEScalar::dNaN();
  return m_InspirationPauseTime->GetValue(unit);
}

bool SEMechanicalVentilator::HasPeakInspiratoryPressure() const
{
  return m_PeakInspiratoryPressure == nullptr ? false : m_PeakInspiratoryPressure->IsValid();
}
SEScalarPressure& SEMechanicalVentilator::GetPeakInspiratoryPressure()
{
  if (m_PeakInspiratoryPressure == nullptr)
    m_PeakInspiratoryPressure = new SEScalarPressure();
  return *m_PeakInspiratoryPressure;
}
double SEMechanicalVentilator::GetPeakInspiratoryPressure(const PressureUnit& unit) const
{
  if (m_PeakInspiratoryPressure == nullptr)
    return SEScalar::dNaN();
  return m_PeakInspiratoryPressure->GetValue(unit);
}

bool SEMechanicalVentilator::HasInspirationTargetFlow() const
{
  return m_InspirationTargetFlow == nullptr ? false : m_InspirationTargetFlow->IsValid();
}
SEScalarVolumePerTime& SEMechanicalVentilator::GetInspirationTargetFlow()
{
  if (m_InspirationTargetFlow == nullptr)
    m_InspirationTargetFlow = new SEScalarVolumePerTime();
  return *m_InspirationTargetFlow;
}
double SEMechanicalVentilator::GetInspirationTargetFlow(const VolumePerTimeUnit& unit) const
{
  if (m_InspirationTargetFlow == nullptr)
    return SEScalar::dNaN();
  return m_InspirationTargetFlow->GetValue(unit);
}

bool SEMechanicalVentilator::HasInspirationMachineTriggerTime() const
{
  return m_InspirationMachineTriggerTime == nullptr ? false : m_InspirationMachineTriggerTime->IsValid();
}
SEScalarTime& SEMechanicalVentilator::GetInspirationMachineTriggerTime()
{
  if (m_InspirationMachineTriggerTime == nullptr)
    m_InspirationMachineTriggerTime = new SEScalarTime();
  return *m_InspirationMachineTriggerTime;
}
double SEMechanicalVentilator::GetInspirationMachineTriggerTime(const TimeUnit& unit) const
{
  if (m_InspirationMachineTriggerTime == nullptr)
    return SEScalar::dNaN();
  return m_InspirationMachineTriggerTime->GetValue(unit);
}

bool SEMechanicalVentilator::HasInspirationPatientTriggerFlow() const
{
  return m_InspirationPatientTriggerFlow == nullptr ? false : m_InspirationPatientTriggerFlow->IsValid();
}
SEScalarVolumePerTime& SEMechanicalVentilator::GetInspirationPatientTriggerFlow()
{
  if (m_InspirationPatientTriggerFlow == nullptr)
    m_InspirationPatientTriggerFlow = new SEScalarVolumePerTime();
  return *m_InspirationPatientTriggerFlow;
}
double SEMechanicalVentilator::GetInspirationPatientTriggerFlow(const VolumePerTimeUnit& unit) const
{
  if (m_InspirationPatientTriggerFlow == nullptr)
    return SEScalar::dNaN();
  return m_InspirationPatientTriggerFlow->GetValue(unit);
}

bool SEMechanicalVentilator::HasInspirationPatientTriggerPressure() const
{
  return m_InspirationPatientTriggerPressure == nullptr ? false : m_InspirationPatientTriggerPressure->IsValid();
}
SEScalarPressure& SEMechanicalVentilator::GetInspirationPatientTriggerPressure()
{
  if (m_InspirationPatientTriggerPressure == nullptr)
    m_InspirationPatientTriggerPressure = new SEScalarPressure();
  return *m_InspirationPatientTriggerPressure;
}
double SEMechanicalVentilator::GetInspirationPatientTriggerPressure(const PressureUnit& unit) const
{
  if (m_InspirationPatientTriggerPressure == nullptr)
    return SEScalar::dNaN();
  return m_InspirationPatientTriggerPressure->GetValue(unit);
}

bool SEMechanicalVentilator::HasInspirationTubeResistance() const
{
  return m_InspirationTubeResistance == nullptr ? false : m_InspirationTubeResistance->IsValid();
}
SEScalarPressureTimePerVolume& SEMechanicalVentilator::GetInspirationTubeResistance()
{
  if (m_InspirationTubeResistance == nullptr)
    m_InspirationTubeResistance = new SEScalarPressureTimePerVolume();
  return *m_InspirationTubeResistance;
}
double SEMechanicalVentilator::GetInspirationTubeResistance(const PressureTimePerVolumeUnit& unit) const
{
  if (m_InspirationTubeResistance == nullptr)
    return SEScalar::dNaN();
  return m_InspirationTubeResistance->GetValue(unit);
}

bool SEMechanicalVentilator::HasInspirationValveResistance() const
{
  return m_InspirationValveResistance == nullptr ? false : m_InspirationValveResistance->IsValid();
}
SEScalarPressureTimePerVolume& SEMechanicalVentilator::GetInspirationValveResistance()
{
  if (m_InspirationValveResistance == nullptr)
    m_InspirationValveResistance = new SEScalarPressureTimePerVolume();
  return *m_InspirationValveResistance;
}
double SEMechanicalVentilator::GetInspirationValveResistance(const PressureTimePerVolumeUnit& unit) const
{
  if (m_InspirationValveResistance == nullptr)
    return SEScalar::dNaN();
  return m_InspirationValveResistance->GetValue(unit);
}

void SEMechanicalVentilator::SetInspirationWaveform(eMechanicalVentilator_DriverWaveform w)
{
  m_InspirationWaveform = w;
}
eMechanicalVentilator_DriverWaveform SEMechanicalVentilator::GetInspirationWaveform() const
{
  return m_InspirationWaveform;
}

bool SEMechanicalVentilator::HasFractionInspiredGas() const
{
  if (m_FractionInspiredGases.empty())
    return false;
  for (auto a : m_FractionInspiredGases)
    if (a->HasFractionAmount() && a->GetFractionAmount().IsPositive())
      return true;
  return false;
}
bool SEMechanicalVentilator::HasFractionInspiredGas(const SESubstance& s) const
{
  for (const SESubstanceFraction* sf : m_FractionInspiredGases)
  {
    if (&s == &sf->GetSubstance())
      return sf->GetFractionAmount() > 0;
  }
  return false;
}
const std::vector<SESubstanceFraction*>& SEMechanicalVentilator::GetFractionInspiredGases()
{
  return m_FractionInspiredGases;
}
const std::vector<const SESubstanceFraction*>& SEMechanicalVentilator::GetFractionInspiredGases() const
{
  return m_cFractionInspiredGases;
}
SESubstanceFraction& SEMechanicalVentilator::GetFractionInspiredGas(const SESubstance& s)
{
  for (SESubstanceFraction* sf : m_FractionInspiredGases)
  {
    if (&s == &sf->GetSubstance())
      return *sf;
  }
  SESubstanceFraction* sf = new SESubstanceFraction(s);
  sf->GetFractionAmount().SetValue(0);
  m_FractionInspiredGases.push_back(sf);
  m_cFractionInspiredGases.push_back(sf);
  return *sf;
}
const SESubstanceFraction* SEMechanicalVentilator::GetFractionInspiredGas(const SESubstance& s) const
{
  const SESubstanceFraction* sf = nullptr;
  for (unsigned int i = 0; i < m_FractionInspiredGases.size(); i++)
  {
    sf = m_FractionInspiredGases[i];
    if (&s == &sf->GetSubstance())
      return sf;
  }
  return sf;
}
void SEMechanicalVentilator::RemoveFractionInspiredGas(const SESubstance& s)
{
  SESubstanceFraction& sf = GetFractionInspiredGas(s);
  sf.GetFractionAmount().SetValue(0);
}
void SEMechanicalVentilator::RemoveFractionInspiredGases()
{
  for (SESubstanceFraction* sf : m_FractionInspiredGases)
    sf->GetFractionAmount().SetValue(0);
}

bool SEMechanicalVentilator::HasConcentrationInspiredAerosol() const
{
  if (m_ConcentrationInspiredAerosols.empty())
    return false;
  for (auto a : m_ConcentrationInspiredAerosols)
    if (a->HasConcentration() && a->GetConcentration().IsPositive())
      return true;
  return false;
}
bool SEMechanicalVentilator::HasConcentrationInspiredAerosol(const SESubstance& substance) const
{
  for (SESubstanceConcentration* sc : m_ConcentrationInspiredAerosols)
  {
    if (&substance == &sc->GetSubstance())
      return sc->GetConcentration().IsPositive();
  }
  return false;
}
const std::vector<SESubstanceConcentration*>& SEMechanicalVentilator::GetConcentrationInspiredAerosols()
{
  return m_ConcentrationInspiredAerosols;
}
const std::vector<const SESubstanceConcentration*>& SEMechanicalVentilator::GetConcentrationInspiredAerosols() const
{
  return m_cConcentrationInspiredAerosols;
}
SESubstanceConcentration& SEMechanicalVentilator::GetConcentrationInspiredAerosol(const SESubstance& substance)
{
  for (SESubstanceConcentration* sc : m_ConcentrationInspiredAerosols)
  {
    if (&substance == &sc->GetSubstance())
      return *sc;
  }
  SESubstanceConcentration* sc = new SESubstanceConcentration(substance);
  sc->GetConcentration().SetValue(0, MassPerVolumeUnit::ug_Per_L);
  m_ConcentrationInspiredAerosols.push_back(sc);
  m_cConcentrationInspiredAerosols.push_back(sc);
  return *sc;
}
const SESubstanceConcentration* SEMechanicalVentilator::GetConcentrationInspiredAerosol(const SESubstance& substance) const
{
  const SESubstanceConcentration* sc = nullptr;
  for (unsigned int i = 0; i < m_ConcentrationInspiredAerosols.size(); i++)
  {
    sc = m_ConcentrationInspiredAerosols[i];
    if (&substance == &sc->GetSubstance())
      return sc;
  }
  return sc;
}
void SEMechanicalVentilator::RemoveConcentrationInspiredAerosol(const SESubstance& substance)
{
  SESubstanceConcentration& sc = GetConcentrationInspiredAerosol(substance);
  auto& unit = *sc.GetConcentration().GetUnit();
  sc.GetConcentration().SetValue(0, unit);
}
void SEMechanicalVentilator::RemoveConcentrationInspiredAerosols()
{
  for (SESubstanceConcentration* sc : m_ConcentrationInspiredAerosols)
  {
    auto& unit = *sc->GetConcentration().GetUnit();
    sc->GetConcentration().SetValue(0, unit);
  }
}
