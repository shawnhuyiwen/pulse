/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/system/equipment/ecmo/SEECMOSettings.h"
#include "cdm/system/equipment/ecmo/actions/SEECMOConfiguration.h"

#include "cdm/substance/SESubstance.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarPressureTimePerVolume.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarVolumePerPressure.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/io/protobuf/PBECMO.h"

SEECMOSettings::SEECMOSettings(Logger* logger) : Loggable(logger)
{
  m_Connection = eSwitch::NullSwitch;
  m_ConnectionVolume = nullptr;
  m_Compliance = nullptr;
  m_DriverDampingParameter = nullptr;

  m_PositiveEndExpiredPressure = nullptr;
  m_FunctionalResidualCapacity = nullptr;

  m_ExpirationCycleFlow = nullptr;
  m_ExpirationCyclePressure = nullptr;
  m_ExpirationCycleVolume = nullptr;
  m_ExpirationCycleTime = nullptr;
  m_ExpirationCycleRespiratoryModel = eSwitch::NullSwitch;

  m_ExpirationTubeResistance = nullptr;
  m_ExpirationValveResistance = nullptr;
  m_ExpirationWaveform = eECMO_DriverWaveform::NullDriverWaveform;
  m_ExpirationWaveformPeriod = nullptr;

  m_InspirationLimitFlow = nullptr;
  m_InspirationLimitPressure = nullptr;
  m_InspirationLimitVolume = nullptr;

  m_InspirationPauseTime = nullptr;

  m_PeakInspiratoryPressure = nullptr;
  m_InspirationTargetFlow = nullptr;

  m_InspirationMachineTriggerTime = nullptr;

  m_InspirationPatientTriggerFlow = nullptr;
  m_InspirationPatientTriggerPressure = nullptr;
  m_InspirationPatientTriggerRespiratoryModel = eSwitch::NullSwitch;

  m_InspirationTubeResistance = nullptr;
  m_InspirationValveResistance = nullptr;
  m_InspirationWaveform = eECMO_DriverWaveform::NullDriverWaveform;
  m_InspirationWaveformPeriod = nullptr;

  m_ExpirationLimbVolume = nullptr;
  m_ExpirationValveVolume = nullptr;
  m_InspirationLimbVolume = nullptr;
  m_InspirationValveVolume = nullptr;

  m_ReliefValveThreshold = nullptr;
  m_YPieceVolume = nullptr;
}

SEECMOSettings::~SEECMOSettings()
{
  m_Connection = eSwitch::NullSwitch;
  SAFE_DELETE(m_ConnectionVolume);
  SAFE_DELETE(m_Compliance);
  SAFE_DELETE(m_DriverDampingParameter);

  SAFE_DELETE(m_PositiveEndExpiredPressure);
  SAFE_DELETE(m_FunctionalResidualCapacity);

  SAFE_DELETE(m_ExpirationCycleFlow);
  SAFE_DELETE(m_ExpirationCyclePressure);
  SAFE_DELETE(m_ExpirationCycleVolume);
  SAFE_DELETE(m_ExpirationCycleTime);
  m_ExpirationCycleRespiratoryModel = eSwitch::NullSwitch;

  SAFE_DELETE(m_ExpirationTubeResistance);
  SAFE_DELETE(m_ExpirationValveResistance);
  m_ExpirationWaveform = eECMO_DriverWaveform::NullDriverWaveform;
  SAFE_DELETE(m_ExpirationWaveformPeriod);

  SAFE_DELETE(m_InspirationLimitFlow);
  SAFE_DELETE(m_InspirationLimitPressure);
  SAFE_DELETE(m_InspirationLimitVolume);

  SAFE_DELETE(m_InspirationPauseTime);

  SAFE_DELETE(m_PeakInspiratoryPressure);
  SAFE_DELETE(m_InspirationTargetFlow);

  SAFE_DELETE(m_InspirationMachineTriggerTime);

  SAFE_DELETE(m_InspirationPatientTriggerFlow);
  SAFE_DELETE(m_InspirationPatientTriggerPressure);
  m_InspirationPatientTriggerRespiratoryModel = eSwitch::NullSwitch;

  SAFE_DELETE(m_InspirationTubeResistance);
  SAFE_DELETE(m_InspirationValveResistance);
  m_InspirationWaveform = eECMO_DriverWaveform::NullDriverWaveform;
  SAFE_DELETE(m_InspirationWaveformPeriod);

  SAFE_DELETE(m_ExpirationLimbVolume);
  SAFE_DELETE(m_ExpirationValveVolume);
  SAFE_DELETE(m_InspirationLimbVolume);
  SAFE_DELETE(m_InspirationValveVolume);

  SAFE_DELETE(m_ReliefValveThreshold);
  SAFE_DELETE(m_YPieceVolume);

  DELETE_VECTOR(m_FractionInspiredGases);
  m_cFractionInspiredGases.clear();
  m_FractionInspiredGases.clear();

  DELETE_VECTOR(m_ConcentrationInspiredAerosols);
  m_cConcentrationInspiredAerosols.clear();
  m_ConcentrationInspiredAerosols.clear();
}

void SEECMOSettings::Clear()
{
  m_Connection = eSwitch::NullSwitch;
  INVALIDATE_PROPERTY(m_ConnectionVolume);
  INVALIDATE_PROPERTY(m_Compliance);
  INVALIDATE_PROPERTY(m_DriverDampingParameter);

  INVALIDATE_PROPERTY(m_PositiveEndExpiredPressure);
  INVALIDATE_PROPERTY(m_FunctionalResidualCapacity);

  INVALIDATE_PROPERTY(m_ExpirationCycleFlow);
  INVALIDATE_PROPERTY(m_ExpirationCyclePressure);
  INVALIDATE_PROPERTY(m_ExpirationCycleVolume);
  INVALIDATE_PROPERTY(m_ExpirationCycleTime);
  m_ExpirationCycleRespiratoryModel = eSwitch::NullSwitch;

  INVALIDATE_PROPERTY(m_ExpirationTubeResistance);
  INVALIDATE_PROPERTY(m_ExpirationValveResistance);
  m_ExpirationWaveform = eECMO_DriverWaveform::NullDriverWaveform;
  INVALIDATE_PROPERTY(m_ExpirationWaveformPeriod);

  INVALIDATE_PROPERTY(m_InspirationLimitFlow);
  INVALIDATE_PROPERTY(m_InspirationLimitPressure);
  INVALIDATE_PROPERTY(m_InspirationLimitVolume);

  INVALIDATE_PROPERTY(m_InspirationPauseTime);

  INVALIDATE_PROPERTY(m_PeakInspiratoryPressure);
  INVALIDATE_PROPERTY(m_InspirationTargetFlow);

  INVALIDATE_PROPERTY(m_InspirationMachineTriggerTime);

  INVALIDATE_PROPERTY(m_InspirationPatientTriggerFlow);
  INVALIDATE_PROPERTY(m_InspirationPatientTriggerPressure);
  m_InspirationPatientTriggerRespiratoryModel = eSwitch::NullSwitch;

  INVALIDATE_PROPERTY(m_InspirationTubeResistance);
  INVALIDATE_PROPERTY(m_InspirationValveResistance);
  m_InspirationWaveform = eECMO_DriverWaveform::NullDriverWaveform;
  INVALIDATE_PROPERTY(m_InspirationWaveformPeriod);

  INVALIDATE_PROPERTY(m_ExpirationLimbVolume);
  INVALIDATE_PROPERTY(m_ExpirationValveVolume);
  INVALIDATE_PROPERTY(m_InspirationLimbVolume);
  INVALIDATE_PROPERTY(m_InspirationValveVolume);

  INVALIDATE_PROPERTY(m_ReliefValveThreshold);
  INVALIDATE_PROPERTY(m_YPieceVolume);

  RemoveFractionInspiredGases();
  RemoveConcentrationInspiredAerosols();
}

void SEECMOSettings::ProcessConfiguration(SEECMOConfiguration& config, SESubstanceManager& subMgr)
{
  if (config.GetMergeType() == eMergeType::Replace)
    Clear();
  if (config.HasSettings())
    Merge(config.GetSettings(), subMgr);
  else if (config.HasSettingsFile())
  {
    // Update the action with the file contents
    std::string cfg_file = config.GetSettingsFile();
    if (!config.GetSettings().SerializeFromFile(cfg_file, subMgr))
      Error("Unable to load settings file", "SEECMOSettings::ProcessConfiguration");
    Merge(config.GetSettings(), subMgr);
  }
}
void SEECMOSettings::Merge(const SEECMOSettings& from, SESubstanceManager& subMgr)
{
  if(from.m_Connection!=eSwitch::NullSwitch)
    SetConnection(from.m_Connection);
  COPY_PROPERTY(ConnectionVolume);
  COPY_PROPERTY(Compliance);
  COPY_PROPERTY(DriverDampingParameter);

  COPY_PROPERTY(PositiveEndExpiredPressure);
  COPY_PROPERTY(FunctionalResidualCapacity);

  COPY_PROPERTY(ExpirationCycleFlow);
  COPY_PROPERTY(ExpirationCyclePressure);
  COPY_PROPERTY(ExpirationCycleVolume);
  COPY_PROPERTY(ExpirationCycleTime);
  if (from.m_ExpirationCycleRespiratoryModel != eSwitch::NullSwitch)
    SetExpirationCycleRespiratoryModel(from.m_ExpirationCycleRespiratoryModel);

  COPY_PROPERTY(ExpirationTubeResistance);
  COPY_PROPERTY(ExpirationValveResistance);
  if (from.m_ExpirationWaveform != eECMO_DriverWaveform::NullDriverWaveform)
    SetExpirationWaveform(from.m_ExpirationWaveform);
  COPY_PROPERTY(ExpirationWaveformPeriod);

  COPY_PROPERTY(InspirationLimitFlow);
  COPY_PROPERTY(InspirationLimitPressure);
  COPY_PROPERTY(InspirationLimitVolume);

  COPY_PROPERTY(InspirationPauseTime);

  COPY_PROPERTY(PeakInspiratoryPressure);
  COPY_PROPERTY(InspirationTargetFlow);

  COPY_PROPERTY(InspirationMachineTriggerTime);

  COPY_PROPERTY(InspirationPatientTriggerFlow);
  COPY_PROPERTY(InspirationPatientTriggerPressure);
  if (from.m_InspirationPatientTriggerRespiratoryModel != eSwitch::NullSwitch)
    SetInspirationPatientTriggerRespiratoryModel(from.m_InspirationPatientTriggerRespiratoryModel);

  COPY_PROPERTY(InspirationTubeResistance);
  COPY_PROPERTY(InspirationValveResistance);
  if (from.m_InspirationWaveform != eECMO_DriverWaveform::NullDriverWaveform)
    SetInspirationWaveform(from.m_InspirationWaveform);
  COPY_PROPERTY(InspirationWaveformPeriod);

  COPY_PROPERTY(ExpirationLimbVolume);
  COPY_PROPERTY(ExpirationValveVolume);
  COPY_PROPERTY(InspirationLimbVolume);
  COPY_PROPERTY(InspirationValveVolume);
  COPY_PROPERTY(ReliefValveThreshold);
  COPY_PROPERTY(YPieceVolume);

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

  //std::string out;
  //this->SerializeToString(out, eSerializationFormat::JSON);
  //std::cout << out << std::endl;
}

bool SEECMOSettings::SerializeToString(std::string& output, eSerializationFormat m) const
{
  return PBECMO::SerializeToString(*this, output, m);
}
bool SEECMOSettings::SerializeToFile(const std::string& filename) const
{
  return PBECMO::SerializeToFile(*this, filename);
}
bool SEECMOSettings::SerializeFromString(const std::string& src, eSerializationFormat m, const SESubstanceManager& subMgr)
{
  return PBECMO::SerializeFromString(src, *this, m, subMgr);
}
bool SEECMOSettings::SerializeFromFile(const std::string& filename, const SESubstanceManager& subMgr)
{
  return PBECMO::SerializeFromFile(filename, *this, subMgr);
}

const SEScalar* SEECMOSettings::GetScalar(const std::string& name)
{

  if (name == "ConnectionVolume")
    return &GetConnectionVolume();
  if (name == "Compliance")
    return &GetCompliance();
  if (name == "DriverDampingParameter")
    return &GetDriverDampingParameter();

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
  if (name == "ExpirationWaveformPeriod")
    return &GetExpirationWaveformPeriod();

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

  if (name == "InspirationWaveformPeriod")
    return &GetInspirationWaveformPeriod();

  if (name == "InspirationTubeResistance")
    return &GetInspirationTubeResistance();
  if (name == "InspirationValveResistance")
    return &GetInspirationValveResistance();

  if (name == "ExpirationLimbVolume")
    return &GetExpirationLimbVolume();
  if (name == "ExpirationValveVolume")
    return &GetExpirationValveVolume();
  if (name == "InspirationLimbVolume")
    return &GetInspirationLimbVolume();
  if (name == "InspirationValveVolume")
    return &GetInspirationValveVolume();

  if (name == "ReliefValveThreshold")
    return &GetReliefValveThreshold();
  if (name == "YPieceVolume")
    return &GetYPieceVolume();

  // I did not support for getting a specific gas/aerosol scalars due to lack of coffee
  return nullptr;
}

void SEECMOSettings::SetConnection(eSwitch c)
{
  m_Connection = c;
}
eSwitch SEECMOSettings::GetConnection() const
{
  return m_Connection;
}

bool SEECMOSettings::HasPositiveEndExpiredPressure() const
{
  return m_PositiveEndExpiredPressure == nullptr ? false : m_PositiveEndExpiredPressure->IsValid();
}
SEScalarPressure& SEECMOSettings::GetPositiveEndExpiredPressure()
{
  if (m_PositiveEndExpiredPressure == nullptr)
    m_PositiveEndExpiredPressure = new SEScalarPressure();
  return *m_PositiveEndExpiredPressure;
}
double SEECMOSettings::GetPositiveEndExpiredPressure(const PressureUnit& unit) const
{
  if (m_PositiveEndExpiredPressure == nullptr)
    return SEScalar::dNaN();
  return m_PositiveEndExpiredPressure->GetValue(unit);
}

bool SEECMOSettings::HasFunctionalResidualCapacity() const
{
  return m_FunctionalResidualCapacity == nullptr ? false : m_FunctionalResidualCapacity->IsValid();
}
SEScalarPressure& SEECMOSettings::GetFunctionalResidualCapacity()
{
  if (m_FunctionalResidualCapacity == nullptr)
    m_FunctionalResidualCapacity = new SEScalarPressure();
  return *m_FunctionalResidualCapacity;
}
double SEECMOSettings::GetFunctionalResidualCapacity(const PressureUnit& unit) const
{
  if (m_FunctionalResidualCapacity == nullptr)
    return SEScalar::dNaN();
  return m_FunctionalResidualCapacity->GetValue(unit);
}

bool SEECMOSettings::HasExpirationCycleFlow() const
{
  return m_ExpirationCycleFlow == nullptr ? false : m_ExpirationCycleFlow->IsValid();
}
SEScalarVolumePerTime& SEECMOSettings::GetExpirationCycleFlow()
{
  if (m_ExpirationCycleFlow == nullptr)
    m_ExpirationCycleFlow = new SEScalarVolumePerTime();
  return *m_ExpirationCycleFlow;
}
double SEECMOSettings::GetExpirationCycleFlow(const VolumePerTimeUnit& unit) const
{
  if (m_ExpirationCycleFlow == nullptr)
    return SEScalar::dNaN();
  return m_ExpirationCycleFlow->GetValue(unit);
}

bool SEECMOSettings::HasExpirationCyclePressure() const
{
  return m_ExpirationCyclePressure == nullptr ? false : m_ExpirationCyclePressure->IsValid();
}
SEScalarPressure& SEECMOSettings::GetExpirationCyclePressure()
{
  if (m_ExpirationCyclePressure == nullptr)
    m_ExpirationCyclePressure = new SEScalarPressure();
  return *m_ExpirationCyclePressure;
}
double SEECMOSettings::GetExpirationCyclePressure(const PressureUnit& unit) const
{
  if (m_ExpirationCyclePressure == nullptr)
    return SEScalar::dNaN();
  return m_ExpirationCyclePressure->GetValue(unit);
}

bool SEECMOSettings::HasExpirationCycleVolume() const
{
  return m_ExpirationCycleVolume == nullptr ? false : m_ExpirationCycleVolume->IsValid();
}
SEScalarVolume& SEECMOSettings::GetExpirationCycleVolume()
{
  if (m_ExpirationCycleVolume == nullptr)
    m_ExpirationCycleVolume = new SEScalarVolume();
  return *m_ExpirationCycleVolume;
}
double SEECMOSettings::GetExpirationCycleVolume(const VolumeUnit& unit) const
{
  if (m_ExpirationCycleVolume == nullptr)
    return SEScalar::dNaN();
  return m_ExpirationCycleVolume->GetValue(unit);
}

bool SEECMOSettings::HasExpirationCycleTime() const
{
  return m_ExpirationCycleTime == nullptr ? false : m_ExpirationCycleTime->IsValid();
}
SEScalarTime& SEECMOSettings::GetExpirationCycleTime()
{
  if (m_ExpirationCycleTime == nullptr)
    m_ExpirationCycleTime = new SEScalarTime();
  return *m_ExpirationCycleTime;
}
double SEECMOSettings::GetExpirationCycleTime(const TimeUnit& unit) const
{
  if (m_ExpirationCycleTime == nullptr)
    return SEScalar::dNaN();
  return m_ExpirationCycleTime->GetValue(unit);
}

void SEECMOSettings::SetExpirationCycleRespiratoryModel(eSwitch c)
{
  m_ExpirationCycleRespiratoryModel = c;
}
eSwitch SEECMOSettings::GetExpirationCycleRespiratoryModel() const
{
  return m_ExpirationCycleRespiratoryModel;
}

bool SEECMOSettings::HasExpirationTubeResistance() const
{
  return m_ExpirationTubeResistance == nullptr ? false : m_ExpirationTubeResistance->IsValid();
}
SEScalarPressureTimePerVolume& SEECMOSettings::GetExpirationTubeResistance()
{
  if (m_ExpirationTubeResistance == nullptr)
    m_ExpirationTubeResistance = new SEScalarPressureTimePerVolume();
  return *m_ExpirationTubeResistance;
}
double SEECMOSettings::GetExpirationTubeResistance(const PressureTimePerVolumeUnit& unit) const
{
  if (m_ExpirationTubeResistance == nullptr)
    return SEScalar::dNaN();
  return m_ExpirationTubeResistance->GetValue(unit);
}

bool SEECMOSettings::HasExpirationValveResistance() const
{
  return m_ExpirationValveResistance == nullptr ? false : m_ExpirationValveResistance->IsValid();
}
SEScalarPressureTimePerVolume& SEECMOSettings::GetExpirationValveResistance()
{
  if (m_ExpirationValveResistance == nullptr)
    m_ExpirationValveResistance = new SEScalarPressureTimePerVolume();
  return *m_ExpirationValveResistance;
}
double SEECMOSettings::GetExpirationValveResistance(const PressureTimePerVolumeUnit& unit) const
{
  if (m_ExpirationValveResistance == nullptr)
    return SEScalar::dNaN();
  return m_ExpirationValveResistance->GetValue(unit);
}

void SEECMOSettings::SetExpirationWaveform(eECMO_DriverWaveform w)
{
  m_ExpirationWaveform = w;
}
eECMO_DriverWaveform SEECMOSettings::GetExpirationWaveform() const
{
  return m_ExpirationWaveform;
}

bool SEECMOSettings::HasExpirationWaveformPeriod() const
{
  return m_ExpirationWaveformPeriod == nullptr ? false : m_ExpirationWaveformPeriod->IsValid();
}
SEScalarTime& SEECMOSettings::GetExpirationWaveformPeriod()
{
  if (m_ExpirationWaveformPeriod == nullptr)
    m_ExpirationWaveformPeriod = new SEScalarTime();
  return *m_ExpirationWaveformPeriod;
}
double SEECMOSettings::GetExpirationWaveformPeriod(const TimeUnit& unit) const
{
  if (m_ExpirationWaveformPeriod == nullptr)
    return SEScalar::dNaN();
  return m_ExpirationWaveformPeriod->GetValue(unit);
}

bool SEECMOSettings::HasInspirationLimitFlow() const
{
  return m_InspirationLimitFlow == nullptr ? false : m_InspirationLimitFlow->IsValid();
}
SEScalarVolumePerTime& SEECMOSettings::GetInspirationLimitFlow()
{
  if (m_InspirationLimitFlow == nullptr)
    m_InspirationLimitFlow = new SEScalarVolumePerTime();
  return *m_InspirationLimitFlow;
}
double SEECMOSettings::GetInspirationLimitFlow(const VolumePerTimeUnit& unit) const
{
  if (m_InspirationLimitFlow == nullptr)
    return SEScalar::dNaN();
  return m_InspirationLimitFlow->GetValue(unit);
}

bool SEECMOSettings::HasInspirationLimitPressure() const
{
  return m_InspirationLimitPressure == nullptr ? false : m_InspirationLimitPressure->IsValid();
}
SEScalarPressure& SEECMOSettings::GetInspirationLimitPressure()
{
  if (m_InspirationLimitPressure == nullptr)
    m_InspirationLimitPressure = new SEScalarPressure();
  return *m_InspirationLimitPressure;
}
double SEECMOSettings::GetInspirationLimitPressure(const PressureUnit& unit) const
{
  if (m_InspirationLimitPressure == nullptr)
    return SEScalar::dNaN();
  return m_InspirationLimitPressure->GetValue(unit);
}

bool SEECMOSettings::HasInspirationLimitVolume() const
{
  return m_InspirationLimitVolume == nullptr ? false : m_InspirationLimitVolume->IsValid();
}
SEScalarVolume& SEECMOSettings::GetInspirationLimitVolume()
{
  if (m_InspirationLimitVolume == nullptr)
    m_InspirationLimitVolume = new SEScalarVolume();
  return *m_InspirationLimitVolume;
}
double SEECMOSettings::GetInspirationLimitVolume(const VolumeUnit& unit) const
{
  if (m_InspirationLimitVolume == nullptr)
    return SEScalar::dNaN();
  return m_InspirationLimitVolume->GetValue(unit);
}

bool SEECMOSettings::HasInspirationPauseTime() const
{
  return m_InspirationPauseTime == nullptr ? false : m_InspirationPauseTime->IsValid();
}
SEScalarTime& SEECMOSettings::GetInspirationPauseTime()
{
  if (m_InspirationPauseTime == nullptr)
    m_InspirationPauseTime = new SEScalarTime();
  return *m_InspirationPauseTime;
}
double SEECMOSettings::GetInspirationPauseTime(const TimeUnit& unit) const
{
  if (m_InspirationPauseTime == nullptr)
    return SEScalar::dNaN();
  return m_InspirationPauseTime->GetValue(unit);
}

bool SEECMOSettings::HasPeakInspiratoryPressure() const
{
  return m_PeakInspiratoryPressure == nullptr ? false : m_PeakInspiratoryPressure->IsValid();
}
SEScalarPressure& SEECMOSettings::GetPeakInspiratoryPressure()
{
  if (m_PeakInspiratoryPressure == nullptr)
    m_PeakInspiratoryPressure = new SEScalarPressure();
  return *m_PeakInspiratoryPressure;
}
double SEECMOSettings::GetPeakInspiratoryPressure(const PressureUnit& unit) const
{
  if (m_PeakInspiratoryPressure == nullptr)
    return SEScalar::dNaN();
  return m_PeakInspiratoryPressure->GetValue(unit);
}

bool SEECMOSettings::HasInspirationTargetFlow() const
{
  return m_InspirationTargetFlow == nullptr ? false : m_InspirationTargetFlow->IsValid();
}
SEScalarVolumePerTime& SEECMOSettings::GetInspirationTargetFlow()
{
  if (m_InspirationTargetFlow == nullptr)
    m_InspirationTargetFlow = new SEScalarVolumePerTime();
  return *m_InspirationTargetFlow;
}
double SEECMOSettings::GetInspirationTargetFlow(const VolumePerTimeUnit& unit) const
{
  if (m_InspirationTargetFlow == nullptr)
    return SEScalar::dNaN();
  return m_InspirationTargetFlow->GetValue(unit);
}

bool SEECMOSettings::HasInspirationMachineTriggerTime() const
{
  return m_InspirationMachineTriggerTime == nullptr ? false : m_InspirationMachineTriggerTime->IsValid();
}
SEScalarTime& SEECMOSettings::GetInspirationMachineTriggerTime()
{
  if (m_InspirationMachineTriggerTime == nullptr)
    m_InspirationMachineTriggerTime = new SEScalarTime();
  return *m_InspirationMachineTriggerTime;
}
double SEECMOSettings::GetInspirationMachineTriggerTime(const TimeUnit& unit) const
{
  if (m_InspirationMachineTriggerTime == nullptr)
    return SEScalar::dNaN();
  return m_InspirationMachineTriggerTime->GetValue(unit);
}

bool SEECMOSettings::HasInspirationPatientTriggerFlow() const
{
  return m_InspirationPatientTriggerFlow == nullptr ? false : m_InspirationPatientTriggerFlow->IsValid();
}
SEScalarVolumePerTime& SEECMOSettings::GetInspirationPatientTriggerFlow()
{
  if (m_InspirationPatientTriggerFlow == nullptr)
    m_InspirationPatientTriggerFlow = new SEScalarVolumePerTime();
  return *m_InspirationPatientTriggerFlow;
}
double SEECMOSettings::GetInspirationPatientTriggerFlow(const VolumePerTimeUnit& unit) const
{
  if (m_InspirationPatientTriggerFlow == nullptr)
    return SEScalar::dNaN();
  return m_InspirationPatientTriggerFlow->GetValue(unit);
}

bool SEECMOSettings::HasInspirationPatientTriggerPressure() const
{
  return m_InspirationPatientTriggerPressure == nullptr ? false : m_InspirationPatientTriggerPressure->IsValid();
}
SEScalarPressure& SEECMOSettings::GetInspirationPatientTriggerPressure()
{
  if (m_InspirationPatientTriggerPressure == nullptr)
    m_InspirationPatientTriggerPressure = new SEScalarPressure();
  return *m_InspirationPatientTriggerPressure;
}
double SEECMOSettings::GetInspirationPatientTriggerPressure(const PressureUnit& unit) const
{
  if (m_InspirationPatientTriggerPressure == nullptr)
    return SEScalar::dNaN();
  return m_InspirationPatientTriggerPressure->GetValue(unit);
}

void SEECMOSettings::SetInspirationPatientTriggerRespiratoryModel(eSwitch c)
{
  m_InspirationPatientTriggerRespiratoryModel = c;
}
eSwitch SEECMOSettings::GetInspirationPatientTriggerRespiratoryModel() const
{
  return m_InspirationPatientTriggerRespiratoryModel;
}

bool SEECMOSettings::HasInspirationTubeResistance() const
{
  return m_InspirationTubeResistance == nullptr ? false : m_InspirationTubeResistance->IsValid();
}
SEScalarPressureTimePerVolume& SEECMOSettings::GetInspirationTubeResistance()
{
  if (m_InspirationTubeResistance == nullptr)
    m_InspirationTubeResistance = new SEScalarPressureTimePerVolume();
  return *m_InspirationTubeResistance;
}
double SEECMOSettings::GetInspirationTubeResistance(const PressureTimePerVolumeUnit& unit) const
{
  if (m_InspirationTubeResistance == nullptr)
    return SEScalar::dNaN();
  return m_InspirationTubeResistance->GetValue(unit);
}

bool SEECMOSettings::HasInspirationValveResistance() const
{
  return m_InspirationValveResistance == nullptr ? false : m_InspirationValveResistance->IsValid();
}
SEScalarPressureTimePerVolume& SEECMOSettings::GetInspirationValveResistance()
{
  if (m_InspirationValveResistance == nullptr)
    m_InspirationValveResistance = new SEScalarPressureTimePerVolume();
  return *m_InspirationValveResistance;
}
double SEECMOSettings::GetInspirationValveResistance(const PressureTimePerVolumeUnit& unit) const
{
  if (m_InspirationValveResistance == nullptr)
    return SEScalar::dNaN();
  return m_InspirationValveResistance->GetValue(unit);
}

bool SEECMOSettings::HasExpirationLimbVolume() const
{
  return m_ExpirationLimbVolume == nullptr ? false : m_ExpirationLimbVolume->IsValid();
}
SEScalarVolume& SEECMOSettings::GetExpirationLimbVolume()
{
  if (m_ExpirationLimbVolume == nullptr)
    m_ExpirationLimbVolume = new SEScalarVolume();
  return *m_ExpirationLimbVolume;
}
double SEECMOSettings::GetExpirationLimbVolume(const VolumeUnit& unit) const
{
  if (m_ExpirationLimbVolume == nullptr)
    return SEScalar::dNaN();
  return m_ExpirationLimbVolume->GetValue(unit);
}

bool SEECMOSettings::HasExpirationValveVolume() const
{
  return m_ExpirationValveVolume == nullptr ? false : m_ExpirationValveVolume->IsValid();
}
SEScalarVolume& SEECMOSettings::GetExpirationValveVolume()
{
  if (m_ExpirationValveVolume == nullptr)
    m_ExpirationValveVolume = new SEScalarVolume();
  return *m_ExpirationValveVolume;
}
double SEECMOSettings::GetExpirationValveVolume(const VolumeUnit& unit) const
{
  if (m_ExpirationValveVolume == nullptr)
    return SEScalar::dNaN();
  return m_ExpirationValveVolume->GetValue(unit);
}

bool SEECMOSettings::HasInspirationLimbVolume() const
{
  return m_InspirationLimbVolume == nullptr ? false : m_InspirationLimbVolume->IsValid();
}
SEScalarVolume& SEECMOSettings::GetInspirationLimbVolume()
{
  if (m_InspirationLimbVolume == nullptr)
    m_InspirationLimbVolume = new SEScalarVolume();
  return *m_InspirationLimbVolume;
}
double SEECMOSettings::GetInspirationLimbVolume(const VolumeUnit& unit) const
{
  if (m_InspirationLimbVolume == nullptr)
    return SEScalar::dNaN();
  return m_InspirationLimbVolume->GetValue(unit);
}

bool SEECMOSettings::HasInspirationValveVolume() const
{
  return m_InspirationValveVolume == nullptr ? false : m_InspirationValveVolume->IsValid();
}
SEScalarVolume& SEECMOSettings::GetInspirationValveVolume()
{
  if (m_InspirationValveVolume == nullptr)
    m_InspirationValveVolume = new SEScalarVolume();
  return *m_InspirationValveVolume;
}
double SEECMOSettings::GetInspirationValveVolume(const VolumeUnit& unit) const
{
  if (m_InspirationValveVolume == nullptr)
    return SEScalar::dNaN();
  return m_InspirationValveVolume->GetValue(unit);
}

bool SEECMOSettings::HasReliefValveThreshold() const
{
  return m_ReliefValveThreshold == nullptr ? false : m_ReliefValveThreshold->IsValid();
}
SEScalarPressure& SEECMOSettings::GetReliefValveThreshold()
{
  if (m_ReliefValveThreshold == nullptr)
    m_ReliefValveThreshold = new SEScalarPressure();
  return *m_ReliefValveThreshold;
}
double SEECMOSettings::GetReliefValveThreshold(const PressureUnit& unit) const
{
  if (m_ReliefValveThreshold == nullptr)
    return SEScalar::dNaN();
  return m_ReliefValveThreshold->GetValue(unit);
}

bool SEECMOSettings::HasYPieceVolume() const
{
  return m_YPieceVolume == nullptr ? false : m_YPieceVolume->IsValid();
}
SEScalarVolume& SEECMOSettings::GetYPieceVolume()
{
  if (m_YPieceVolume == nullptr)
    m_YPieceVolume = new SEScalarVolume();
  return *m_YPieceVolume;
}
double SEECMOSettings::GetYPieceVolume(const VolumeUnit& unit) const
{
  if (m_YPieceVolume == nullptr)
    return SEScalar::dNaN();
  return m_YPieceVolume->GetValue(unit);
}

bool SEECMOSettings::HasConnectionVolume() const
{
  return m_ConnectionVolume == nullptr ? false : m_ConnectionVolume->IsValid();
}
SEScalarVolume& SEECMOSettings::GetConnectionVolume()
{
  if (m_ConnectionVolume == nullptr)
    m_ConnectionVolume = new SEScalarVolume();
  return *m_ConnectionVolume;
}
double SEECMOSettings::GetConnectionVolume(const VolumeUnit& unit) const
{
  if (m_ConnectionVolume == nullptr)
    return SEScalar::dNaN();
  return m_ConnectionVolume->GetValue(unit);
}

bool SEECMOSettings::HasCompliance() const
{
  return m_Compliance == nullptr ? false : m_Compliance->IsValid();
}
SEScalarVolumePerPressure& SEECMOSettings::GetCompliance()
{
  if (m_Compliance == nullptr)
    m_Compliance = new SEScalarVolumePerPressure();
  return *m_Compliance;
}
double SEECMOSettings::GetCompliance(const VolumePerPressureUnit& unit) const
{
  if (m_Compliance == nullptr)
    return SEScalar::dNaN();
  return m_Compliance->GetValue(unit);
}

bool SEECMOSettings::HasDriverDampingParameter() const
{
  return m_DriverDampingParameter == nullptr ? false : m_DriverDampingParameter->IsValid();
}
SEScalarFrequency& SEECMOSettings::GetDriverDampingParameter()
{
  if (m_DriverDampingParameter == nullptr)
    m_DriverDampingParameter = new SEScalarFrequency();
  return *m_DriverDampingParameter;
}
double SEECMOSettings::GetDriverDampingParameter(const FrequencyUnit& unit) const
{
  if (m_DriverDampingParameter == nullptr)
    return SEScalar::dNaN();
  return m_DriverDampingParameter->GetValue(unit);
}

void SEECMOSettings::SetInspirationWaveform(eECMO_DriverWaveform w)
{
  m_InspirationWaveform = w;
}
eECMO_DriverWaveform SEECMOSettings::GetInspirationWaveform() const
{
  return m_InspirationWaveform;
}

bool SEECMOSettings::HasInspirationWaveformPeriod() const
{
  return m_InspirationWaveformPeriod == nullptr ? false : m_InspirationWaveformPeriod->IsValid();
}
SEScalarTime& SEECMOSettings::GetInspirationWaveformPeriod()
{
  if (m_InspirationWaveformPeriod == nullptr)
    m_InspirationWaveformPeriod = new SEScalarTime();
  return *m_InspirationWaveformPeriod;
}
double SEECMOSettings::GetInspirationWaveformPeriod(const TimeUnit& unit) const
{
  if (m_InspirationWaveformPeriod == nullptr)
    return SEScalar::dNaN();
  return m_InspirationWaveformPeriod->GetValue(unit);
}

bool SEECMOSettings::HasFractionInspiredGas() const
{
  if (m_FractionInspiredGases.empty())
    return false;
  for (auto a : m_FractionInspiredGases)
    if (a->HasFractionAmount() && a->GetFractionAmount().IsPositive())
      return true;
  return false;
}
bool SEECMOSettings::HasFractionInspiredGas(const SESubstance& s) const
{
  for (const SESubstanceFraction* sf : m_FractionInspiredGases)
  {
    if (&s == &sf->GetSubstance())
      return sf->GetFractionAmount() > 0;
  }
  return false;
}
const std::vector<SESubstanceFraction*>& SEECMOSettings::GetFractionInspiredGases()
{
  return m_FractionInspiredGases;
}
const std::vector<const SESubstanceFraction*>& SEECMOSettings::GetFractionInspiredGases() const
{
  return m_cFractionInspiredGases;
}
SESubstanceFraction& SEECMOSettings::GetFractionInspiredGas(const SESubstance& s)
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
const SESubstanceFraction* SEECMOSettings::GetFractionInspiredGas(const SESubstance& s) const
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
void SEECMOSettings::RemoveFractionInspiredGas(const SESubstance& s)
{
  SESubstanceFraction& sf = GetFractionInspiredGas(s);
  sf.GetFractionAmount().SetValue(0);
}
void SEECMOSettings::RemoveFractionInspiredGases()
{
  for (SESubstanceFraction* sf : m_FractionInspiredGases)
    sf->GetFractionAmount().SetValue(0);
}

bool SEECMOSettings::HasConcentrationInspiredAerosol() const
{
  if (m_ConcentrationInspiredAerosols.empty())
    return false;
  for (auto a : m_ConcentrationInspiredAerosols)
    if (a->HasConcentration() && a->GetConcentration().IsPositive())
      return true;
  return false;
}
bool SEECMOSettings::HasConcentrationInspiredAerosol(const SESubstance& substance) const
{
  for (SESubstanceConcentration* sc : m_ConcentrationInspiredAerosols)
  {
    if (&substance == &sc->GetSubstance())
      return sc->GetConcentration().IsPositive();
  }
  return false;
}
const std::vector<SESubstanceConcentration*>& SEECMOSettings::GetConcentrationInspiredAerosols()
{
  return m_ConcentrationInspiredAerosols;
}
const std::vector<const SESubstanceConcentration*>& SEECMOSettings::GetConcentrationInspiredAerosols() const
{
  return m_cConcentrationInspiredAerosols;
}
SESubstanceConcentration& SEECMOSettings::GetConcentrationInspiredAerosol(const SESubstance& substance)
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
const SESubstanceConcentration* SEECMOSettings::GetConcentrationInspiredAerosol(const SESubstance& substance) const
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
void SEECMOSettings::RemoveConcentrationInspiredAerosol(const SESubstance& substance)
{
  SESubstanceConcentration& sc = GetConcentrationInspiredAerosol(substance);
  auto& unit = *sc.GetConcentration().GetUnit();
  sc.GetConcentration().SetValue(0, unit);
}
void SEECMOSettings::RemoveConcentrationInspiredAerosols()
{
  for (SESubstanceConcentration* sc : m_ConcentrationInspiredAerosols)
  {
    auto& unit = *sc->GetConcentration().GetUnit();
    sc->GetConcentration().SetValue(0, unit);
  }
}
