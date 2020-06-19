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
  m_Connection = eMechanicalVentilator_Connection::NullConnection;
  m_EndotrachealTubeResistance = nullptr;

  m_PositiveEndExpiredPressure = nullptr;
  m_FunctionalResidualCapacity = nullptr;

  m_ExpirationCycleFlow = nullptr;
  m_ExpirationCyclePressure = nullptr;
  m_ExpirationCycleTime = nullptr;

  m_ExpirationTubeResistance = nullptr;
  m_ExpirationValveResistance = nullptr;
  m_ExpirationWaveform = eMechanicalVentilator_DriverWaveform::NullDriverWaveform;

  m_InspirationLimitFlow = nullptr;
  m_InspirationLimitPressure = nullptr;
  m_InspirationLimitVolume = nullptr;

  m_InspirationPauseTime = nullptr;

  m_PeakInspiratoryPressure = nullptr;
  m_EndTidalCarbonDioxidePressure = nullptr;

  m_InspirationTriggerFlow = nullptr;
  m_InspirationTriggerPressure = nullptr;
  m_InspirationTriggerTime = nullptr;

  m_InspirationTubeResistance = nullptr;
  m_InspirationValveResistance = nullptr;
  m_InspirationWaveform = eMechanicalVentilator_DriverWaveform::NullDriverWaveform;
}

SEMechanicalVentilator::~SEMechanicalVentilator()
{
  Clear();
}

void SEMechanicalVentilator::Clear()
{
  SEEquipment::Clear();

  m_Connection = eMechanicalVentilator_Connection::NullConnection;
  SAFE_DELETE(m_EndotrachealTubeResistance);

  SAFE_DELETE(m_PositiveEndExpiredPressure);
  SAFE_DELETE(m_FunctionalResidualCapacity);

  SAFE_DELETE(m_ExpirationCycleFlow);
  SAFE_DELETE(m_ExpirationCyclePressure);
  SAFE_DELETE(m_ExpirationCycleTime);

  SAFE_DELETE(m_ExpirationTubeResistance);
  SAFE_DELETE(m_ExpirationValveResistance);
  m_ExpirationWaveform = eMechanicalVentilator_DriverWaveform::NullDriverWaveform;

  SAFE_DELETE(m_InspirationLimitFlow);
  SAFE_DELETE(m_InspirationLimitPressure);
  SAFE_DELETE(m_InspirationLimitVolume);

  SAFE_DELETE(m_InspirationPauseTime);

  SAFE_DELETE(m_PeakInspiratoryPressure);
  SAFE_DELETE(m_EndTidalCarbonDioxidePressure);

  SAFE_DELETE(m_InspirationTriggerFlow);
  SAFE_DELETE(m_InspirationTriggerPressure);
  SAFE_DELETE(m_InspirationTriggerTime);

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
  COPY_PROPERTY(EndTidalCarbonDioxidePressure);

  COPY_PROPERTY(InspirationTriggerFlow);
  COPY_PROPERTY(InspirationTriggerPressure);
  COPY_PROPERTY(InspirationTriggerTime);

  COPY_PROPERTY(InspirationTubeResistance);
  COPY_PROPERTY(InspirationValveResistance);
  if (from.m_InspirationWaveform != eMechanicalVentilator_DriverWaveform::NullDriverWaveform)
    SetInspirationWaveform(from.m_InspirationWaveform);

  // Always need to provide a full (fractions sum to 1) substance list that replaces current
  if (from.HasFractionInspiredGas())
  {
    double amt;
    double total = 0;
    const SESubstance* sub;
    SESubstanceFraction* sf;
    // Since we are allowing only O2 to be specified
    // Remove everything so we know what is intentionally not provided
    // And what is intentially set to 0 (don't just set to 0)
    RemoveFractionInspiredGases();
    for (SESubstanceFraction* osf : from.m_FractionInspiredGases)
    {
      sub = subMgr.GetSubstance(osf->GetSubstance().GetName());
      if (sub == nullptr)
        Error("Do not have substance : " + osf->GetSubstance().GetName());
      sf = new SESubstanceFraction(*sub);
      sf->GetFractionAmount().Set(osf->GetFractionAmount());
      amt = sf->GetFractionAmount().GetValue();
      total += amt;
      m_FractionInspiredGases.push_back(sf);
      m_cFractionInspiredGases.push_back(sf);
      subMgr.AddActiveSubstance((SESubstance&)sf->m_Substance);
    }
    
    // It's Ok if you ONLY set Oxygen, i.e. FiO2
    // Ventilator models should understand that common setting
    if (!SEScalar::IsValue(1, total))
    {
      bool err = false;
      if (m_FractionInspiredGases.size() != 1)
        err = true;
      else if (m_FractionInspiredGases[0]->GetSubstance().GetName() != "Oxygen")
        err = true;
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
  if (name == "EndTidalCarbonDioxidePressure")
    return &GetEndTidalCarbonDioxidePressure();

  if (name == "InspirationTriggerFlow")
    return &GetInspirationTriggerFlow();
  if (name == "InspirationTriggerPressure")
    return &GetInspirationTriggerPressure();
  if (name == "InspirationTriggerTime")
    return &GetInspirationTriggerTime();

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

bool SEMechanicalVentilator::HasEndTidalCarbonDioxidePressure() const
{
  return m_EndTidalCarbonDioxidePressure == nullptr ? false : m_EndTidalCarbonDioxidePressure->IsValid();
}
SEScalarPressure& SEMechanicalVentilator::GetEndTidalCarbonDioxidePressure()
{
  if (m_EndTidalCarbonDioxidePressure == nullptr)
    m_EndTidalCarbonDioxidePressure = new SEScalarPressure();
  return *m_EndTidalCarbonDioxidePressure;
}
double SEMechanicalVentilator::GetEndTidalCarbonDioxidePressure(const PressureUnit& unit) const
{
  if (m_EndTidalCarbonDioxidePressure == nullptr)
    return SEScalar::dNaN();
  return m_EndTidalCarbonDioxidePressure->GetValue(unit);
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

bool SEMechanicalVentilator::HasInspirationTriggerFlow() const
{
  return m_InspirationTriggerFlow == nullptr ? false : m_InspirationTriggerFlow->IsValid();
}
SEScalarVolumePerTime& SEMechanicalVentilator::GetInspirationTriggerFlow()
{
  if (m_InspirationTriggerFlow == nullptr)
    m_InspirationTriggerFlow = new SEScalarVolumePerTime();
  return *m_InspirationTriggerFlow;
}
double SEMechanicalVentilator::GetInspirationTriggerFlow(const VolumePerTimeUnit& unit) const
{
  if (m_InspirationTriggerFlow == nullptr)
    return SEScalar::dNaN();
  return m_InspirationTriggerFlow->GetValue(unit);
}

bool SEMechanicalVentilator::HasInspirationTriggerPressure() const
{
  return m_InspirationTriggerPressure == nullptr ? false : m_InspirationTriggerPressure->IsValid();
}
SEScalarPressure& SEMechanicalVentilator::GetInspirationTriggerPressure()
{
  if (m_InspirationTriggerPressure == nullptr)
    m_InspirationTriggerPressure = new SEScalarPressure();
  return *m_InspirationTriggerPressure;
}
double SEMechanicalVentilator::GetInspirationTriggerPressure(const PressureUnit& unit) const
{
  if (m_InspirationTriggerPressure == nullptr)
    return SEScalar::dNaN();
  return m_InspirationTriggerPressure->GetValue(unit);
}

bool SEMechanicalVentilator::HasInspirationTriggerTime() const
{
  return m_InspirationTriggerTime == nullptr ? false : m_InspirationTriggerTime->IsValid();
}
SEScalarTime& SEMechanicalVentilator::GetInspirationTriggerTime()
{
  if (m_InspirationTriggerTime == nullptr)
    m_InspirationTriggerTime = new SEScalarTime();
  return *m_InspirationTriggerTime;
}
double SEMechanicalVentilator::GetInspirationTriggerTime(const TimeUnit& unit) const
{
  if (m_InspirationTriggerTime == nullptr)
    return SEScalar::dNaN();
  return m_InspirationTriggerTime->GetValue(unit);
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
  return m_FractionInspiredGases.size() == 0 ? false : true;
}
bool SEMechanicalVentilator::HasFractionInspiredGas(const SESubstance& s) const
{
  for (const SESubstanceFraction* sf : m_FractionInspiredGases)
  {
    if (&s == &sf->GetSubstance())
      return true;
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
  const SESubstanceFraction* sf;
  for (unsigned int i = 0; i < m_FractionInspiredGases.size(); i++)
  {
    sf = m_FractionInspiredGases[i];
    if (&s == &sf->GetSubstance())
    {
      m_FractionInspiredGases.erase(m_FractionInspiredGases.begin() + i);
      m_cFractionInspiredGases.erase(m_cFractionInspiredGases.begin() + i);
      delete sf;
    }
  }
}
void SEMechanicalVentilator::RemoveFractionInspiredGases()
{
  DELETE_VECTOR(m_FractionInspiredGases);
  m_cFractionInspiredGases.clear();
}

bool SEMechanicalVentilator::HasConcentrationInspiredAerosol() const
{
  return m_ConcentrationInspiredAerosols.size() == 0 ? false : true;
}
bool SEMechanicalVentilator::HasConcentrationInspiredAerosol(const SESubstance& substance) const
{
  for (const SESubstanceConcentration* sc : m_ConcentrationInspiredAerosols)
  {
    if (&substance == &sc->GetSubstance())
      return true;
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
  const SESubstanceConcentration* sc;
  for (unsigned int i = 0; i < m_ConcentrationInspiredAerosols.size(); i++)
  {
    sc = m_ConcentrationInspiredAerosols[i];
    if (&substance == &sc->GetSubstance())
    {
      m_ConcentrationInspiredAerosols.erase(m_ConcentrationInspiredAerosols.begin() + i);
      m_cConcentrationInspiredAerosols.erase(m_cConcentrationInspiredAerosols.begin() + i);
      delete sc;
    }
  }
}
void SEMechanicalVentilator::RemoveConcentrationInspiredAerosols()
{
  DELETE_VECTOR(m_ConcentrationInspiredAerosols);
  m_cConcentrationInspiredAerosols.clear();
}
