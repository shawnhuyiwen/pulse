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
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarTime.h"
#include "io/protobuf/PBMechanicalVentilator.h"

SEMechanicalVentilator::SEMechanicalVentilator(SESubstanceManager& substances) : SEEquipment(substances.GetLogger()), m_Substances(substances)
{
  m_BreathPeriod = nullptr;
  m_Connection = eMechanicalVentilator_Connection::NullConnection;
  m_Control = eMechanicalVentilator_Control::NullControl;
  m_DriverWaveform = eMechanicalVentilator_DriverWaveform::NullDriverWaveform;
  m_InspiratoryExpiratoryRatio = nullptr;
  m_InspiratoryPeriod = nullptr;
  m_ExpiratoryPeriod = nullptr;
  m_PeakInspiratoryPressure = nullptr;
  m_PositiveEndExpiredPressure = nullptr;
  m_RespiratoryRate = nullptr;

}

SEMechanicalVentilator::~SEMechanicalVentilator()
{
  Clear();
}

void SEMechanicalVentilator::Clear()
{
  SEEquipment::Clear();

  SAFE_DELETE(m_BreathPeriod);
  m_Connection = eMechanicalVentilator_Connection::NullConnection;
  m_Control = eMechanicalVentilator_Control::NullControl;
  m_DriverWaveform = eMechanicalVentilator_DriverWaveform::NullDriverWaveform;
  SAFE_DELETE(m_InspiratoryExpiratoryRatio);
  SAFE_DELETE(m_InspiratoryPeriod);
  SAFE_DELETE(m_ExpiratoryPeriod);
  SAFE_DELETE(m_PeakInspiratoryPressure);
  SAFE_DELETE(m_PositiveEndExpiredPressure);
  SAFE_DELETE(m_RespiratoryRate);

  DELETE_VECTOR(m_FractionInspiredGases);
  m_cFractionInspiredGases.clear();
  m_FractionInspiredGases.clear();

  DELETE_VECTOR(m_ConcentrationInspiredAerosols);
  m_cConcentrationInspiredAerosols.clear();
  m_ConcentrationInspiredAerosols.clear();
}

void SEMechanicalVentilator::Merge(const SEMechanicalVentilator& from)
{
  if(from.m_Connection!=eMechanicalVentilator_Connection::NullConnection)
    SetConnection(from.m_Connection);
  if (from.m_Control != eMechanicalVentilator_Control::NullControl)
    SetControl(from.m_Control);
  if (from.m_DriverWaveform != eMechanicalVentilator_DriverWaveform::NullDriverWaveform)
    SetDriverWaveform(from.m_DriverWaveform);

  COPY_PROPERTY(BreathPeriod);
  COPY_PROPERTY(InspiratoryExpiratoryRatio);
  COPY_PROPERTY(InspiratoryPeriod);
  COPY_PROPERTY(ExpiratoryPeriod);
  COPY_PROPERTY(PeakInspiratoryPressure);
  COPY_PROPERTY(PositiveEndExpiredPressure);
  COPY_PROPERTY(RespiratoryRate);

  // Always need to provide a full (fractions sum to 1) substance list that replaces current
  if (from.HasFractionInspiredGas())
  {
    double amt;
    double total = 0;
    RemoveFractionInspiredGases();
    SESubstance* sub;
    SESubstanceFraction* sf;
    for (SESubstanceFraction* osf : from.m_FractionInspiredGases)
    {
      if (&m_Substances != &from.m_Substances)
      {
        sub = m_Substances.GetSubstance(osf->GetSubstance().GetName());
        if (sub == nullptr)
        {
          std::stringstream ss;
          ss << "Do not have substance : " << osf->GetSubstance().GetName();
          Error(ss);
        }
      }
      else
        sub = &osf->GetSubstance();

      sf = new SESubstanceFraction(*sub);
      sf->GetFractionAmount().Set(osf->GetFractionAmount());
      amt = sf->GetFractionAmount().GetValue();
      total += amt;
      m_FractionInspiredGases.push_back(sf);
      m_cFractionInspiredGases.push_back(sf);
      m_Substances.AddActiveSubstance((SESubstance&)sf->m_Substance);
    }
    if (!SEScalar::IsValue(1, total))
      Error("Mechanical Ventilator substance fractions do not sum to 1");
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
bool SEMechanicalVentilator::SerializeToFile(const std::string& filename, SerializationFormat m) const
{
  return PBMechanicalVentilator::SerializeToFile(*this, filename, m);
}
bool SEMechanicalVentilator::SerializeFromString(const std::string& src, SerializationFormat m)
{
  return PBMechanicalVentilator::SerializeFromString(src, *this, m);
}
bool SEMechanicalVentilator::SerializeFromFile(const std::string& filename, SerializationFormat m)
{
  return PBMechanicalVentilator::SerializeFromFile(filename, *this, m);
}

void SEMechanicalVentilator::ProcessConfiguration(SEMechanicalVentilatorConfiguration& config)
{
  if (config.HasConfiguration())
    Merge(config.GetConfiguration());
  else if (config.HasConfigurationFile())
  {
    // Update the action with the file contents
    std::string cfg_file = config.GetConfigurationFile();
    if (!config.GetConfiguration().SerializeFromFile(cfg_file, JSON))
      Error("Unable to load configuration file", "SEMechanicalVentilator::ProcessConfiguration");
    Merge(config.GetConfiguration());
  }
  StateChange();
}

const SEScalar* SEMechanicalVentilator::GetScalar(const std::string& name)
{
  if (name == "BreathPeriod")
    return &GetBreathPeriod();
  if (name == "InspiratoryExpiratoryRatio")
    return &GetInspiratoryExpiratoryRatio();
  if (name == "InspiratoryPeriod")
    return &GetInspiratoryPeriod();
  if (name == "ExpiratoryPeriod")
    return &GetExpiratoryPeriod();
  if (name == "PeakInspiratoryPressure")
    return &GetPeakInspiratoryPressure();
  if (name == "PositiveEndExpiredPressure")
    return &GetPositiveEndExpiredPressure();
  if (name == "RespiratoryRate")
    return &GetRespiratoryRate();
  // I did not support for getting a specific gas/aerosol scalars due to lack of coffee
  return nullptr;
}

bool SEMechanicalVentilator::HasBreathPeriod() const
{
  return m_BreathPeriod == nullptr ? false : m_BreathPeriod->IsValid();
}
SEScalarTime& SEMechanicalVentilator::GetBreathPeriod()
{
  if (m_BreathPeriod == nullptr)
    m_BreathPeriod = new SEScalarTime();
  return *m_BreathPeriod;
}
double SEMechanicalVentilator::GetBreathPeriod(const TimeUnit& unit) const
{
  if (m_BreathPeriod == nullptr)
    return SEScalar::dNaN();
  return m_BreathPeriod->GetValue(unit);
}

void SEMechanicalVentilator::SetConnection(eMechanicalVentilator_Connection c)
{
  m_Connection = c;
}
eMechanicalVentilator_Connection SEMechanicalVentilator::GetConnection() const
{
  return m_Connection;
}

void SEMechanicalVentilator::SetControl(eMechanicalVentilator_Control c)
{
  m_Control = c;
}
eMechanicalVentilator_Control SEMechanicalVentilator::GetControl() const
{
  return m_Control;
}

void SEMechanicalVentilator::SetDriverWaveform(eMechanicalVentilator_DriverWaveform w)
{
  m_DriverWaveform = w;
}
eMechanicalVentilator_DriverWaveform SEMechanicalVentilator::GetDriverWaveform() const
{
  return m_DriverWaveform;
}


bool SEMechanicalVentilator::HasInspiratoryExpiratoryRatio() const
{
  return m_InspiratoryExpiratoryRatio == nullptr ? false : m_InspiratoryExpiratoryRatio->IsValid();
}
SEScalar& SEMechanicalVentilator::GetInspiratoryExpiratoryRatio()
{
  if (m_InspiratoryExpiratoryRatio == nullptr)
    m_InspiratoryExpiratoryRatio = new SEScalar();
  return *m_InspiratoryExpiratoryRatio;
}
double SEMechanicalVentilator::GetInspiratoryExpiratoryRatio() const
{
  if (m_InspiratoryExpiratoryRatio == nullptr)
    return SEScalar::dNaN();
  return m_InspiratoryExpiratoryRatio->GetValue();
}

bool SEMechanicalVentilator::HasInspiratoryPeriod() const
{
  return m_InspiratoryPeriod == nullptr ? false : m_InspiratoryPeriod->IsValid();
}
SEScalarTime& SEMechanicalVentilator::GetInspiratoryPeriod()
{
  if (m_InspiratoryPeriod == nullptr)
    m_InspiratoryPeriod = new SEScalarTime();
  return *m_InspiratoryPeriod;
}
double SEMechanicalVentilator::GetInspiratoryPeriod(const TimeUnit& unit) const
{
  if (m_InspiratoryPeriod == nullptr)
    return SEScalar::dNaN();
  return m_InspiratoryPeriod->GetValue(unit);
}

bool SEMechanicalVentilator::HasExpiratoryPeriod() const
{
  return m_ExpiratoryPeriod == nullptr ? false : m_ExpiratoryPeriod->IsValid();
}
SEScalarTime& SEMechanicalVentilator::GetExpiratoryPeriod()
{
  if (m_ExpiratoryPeriod == nullptr)
    m_ExpiratoryPeriod = new SEScalarTime();
  return *m_ExpiratoryPeriod;
}
double SEMechanicalVentilator::GetExpiratoryPeriod(const TimeUnit& unit) const
{
  if (m_ExpiratoryPeriod == nullptr)
    return SEScalar::dNaN();
  return m_ExpiratoryPeriod->GetValue(unit);
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

bool SEMechanicalVentilator::HasRespiratoryRate() const
{
  return m_RespiratoryRate == nullptr ? false : m_RespiratoryRate->IsValid();
}
SEScalarFrequency& SEMechanicalVentilator::GetRespiratoryRate()
{
  if (m_RespiratoryRate == nullptr)
    m_RespiratoryRate = new SEScalarFrequency();
  return *m_RespiratoryRate;
}
double SEMechanicalVentilator::GetRespiratoryRate(const FrequencyUnit& unit) const
{
  if (m_RespiratoryRate == nullptr)
    return SEScalar::dNaN();
  return m_RespiratoryRate->GetValue(unit);
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
SESubstanceFraction& SEMechanicalVentilator::GetFractionInspiredGas(SESubstance& s)
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
SESubstanceConcentration& SEMechanicalVentilator::GetConcentrationInspiredAerosol(SESubstance& substance)
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
