/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "system/equipment/bag_valve_mask/SEBagValveMask.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"
#include "substance/SESubstanceConcentration.h"
#include "substance/SESubstanceFraction.h"

// State Actions
#include "system/equipment/bag_valve_mask/actions/SEBagValveMaskConfiguration.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarPressureTimePerVolume.h"
#include "properties/SEScalarVolume.h"
#include "io/protobuf/PBBagValveMask.h"

SEBagValveMask::SEBagValveMask(Logger* logger) : SEEquipment(logger)
{
  m_Connection = eBagValveMask_Connection::Off;

  m_BreathFrequency = nullptr;
  m_InspiratoryExpiratoryRatio = nullptr;
  m_SqueezePressure = nullptr;
  m_SqueezeVolume = nullptr;
  m_ValvePositiveEndExpiredPressure = nullptr;

  m_BagResistance = nullptr;
  m_FilterResistance = nullptr;
  m_FilterVolume = nullptr;
  m_ValveResistance = nullptr;
}

SEBagValveMask::~SEBagValveMask()
{
  m_Connection = eBagValveMask_Connection::Off;

  SAFE_DELETE(m_BreathFrequency);
  SAFE_DELETE(m_InspiratoryExpiratoryRatio);
  SAFE_DELETE(m_SqueezePressure);
  SAFE_DELETE(m_SqueezeVolume);
  SAFE_DELETE(m_ValvePositiveEndExpiredPressure);

  SAFE_DELETE(m_BagResistance);
  SAFE_DELETE(m_FilterResistance);
  SAFE_DELETE(m_FilterVolume);
  SAFE_DELETE(m_ValveResistance);

  DELETE_VECTOR(m_FractionInspiredGases);
  m_cFractionInspiredGases.clear();
  m_FractionInspiredGases.clear();

  DELETE_VECTOR(m_ConcentrationInspiredAerosols);
  m_cConcentrationInspiredAerosols.clear();
  m_ConcentrationInspiredAerosols.clear();
}

void SEBagValveMask::Clear()
{
  SEEquipment::Clear();

  m_Connection = eBagValveMask_Connection::Off;

  INVALIDATE_PROPERTY(m_BreathFrequency);
  INVALIDATE_PROPERTY(m_InspiratoryExpiratoryRatio);
  INVALIDATE_PROPERTY(m_SqueezePressure);
  INVALIDATE_PROPERTY(m_SqueezeVolume);
  INVALIDATE_PROPERTY(m_ValvePositiveEndExpiredPressure);

  INVALIDATE_PROPERTY(m_BagResistance);
  INVALIDATE_PROPERTY(m_FilterResistance);
  INVALIDATE_PROPERTY(m_FilterVolume);
  INVALIDATE_PROPERTY(m_ValveResistance);

  RemoveFractionInspiredGases();
  RemoveConcentrationInspiredAerosols();
}


void SEBagValveMask::ProcessConfiguration(SEBagValveMaskConfiguration& config, SESubstanceManager& subMgr)
{
  if (config.HasConfiguration())
    Merge(config.GetConfiguration(), subMgr);
  else if (config.HasConfigurationFile())
  {
    // Update the action with the file contents
    std::string cfg_file = config.GetConfigurationFile();
    if (!config.GetConfiguration().SerializeFromFile(cfg_file, subMgr))
      Error("Unable to load configuration file", "SEBagValveMask::ProcessConfiguration");
    Merge(config.GetConfiguration(), subMgr);
  }
  StateChange();
}
void SEBagValveMask::Merge(const SEBagValveMask& from, SESubstanceManager& subMgr)
{
  if(from.m_Connection!=eBagValveMask_Connection::NullConnection)
    SetConnection(from.m_Connection);
  COPY_PROPERTY(BagResistance);
  COPY_PROPERTY(FilterResistance);
  COPY_PROPERTY(FilterVolume);
  COPY_PROPERTY(ValveResistance);

  COPY_PROPERTY(BreathFrequency);
  COPY_PROPERTY(InspiratoryExpiratoryRatio);
  COPY_PROPERTY(SqueezePressure);
  COPY_PROPERTY(SqueezeVolume);
  COPY_PROPERTY(ValvePositiveEndExpiredPressure);

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

bool SEBagValveMask::SerializeToString(std::string& output, SerializationFormat m) const
{
  return PBBagValveMask::SerializeToString(*this, output, m);
}
bool SEBagValveMask::SerializeToFile(const std::string& filename) const
{
  return PBBagValveMask::SerializeToFile(*this, filename);
}
bool SEBagValveMask::SerializeFromString(const std::string& src, SerializationFormat m, const SESubstanceManager& subMgr)
{
  return PBBagValveMask::SerializeFromString(src, *this, m, subMgr);
}
bool SEBagValveMask::SerializeFromFile(const std::string& filename, const SESubstanceManager& subMgr)
{
  return PBBagValveMask::SerializeFromFile(filename, *this, subMgr);
}

const SEScalar* SEBagValveMask::GetScalar(const std::string& name)
{
  if (name == "BreathFrequency")
    return &GetBreathFrequency();
  if (name == "InspiratoryExpiratoryRatio")
    return &GetInspiratoryExpiratoryRatio();
  if (name == "SqueezePressure")
    return &GetSqueezePressure();
  if (name == "SqueezeVolume")
    return &GetSqueezeVolume();
  if (name == "ValvePositiveEndExpiredPressure")
    return &GetValvePositiveEndExpiredPressure();

  if (name == "BagResistance")
    return &GetBagResistance();
  if (name == "FilterResistance")
    return &GetFilterResistance();
  if (name == "FilterVolume")
    return &GetFilterVolume();
  if (name == "ValveResistance")
    return &GetValveResistance();

  // I did not support for getting a specific gas/aerosol scalars due to lack of coffee
  return nullptr;
}

void SEBagValveMask::SetConnection(eBagValveMask_Connection c)
{
  m_Connection = c;
}
eBagValveMask_Connection SEBagValveMask::GetConnection() const
{
  return m_Connection;
}

bool SEBagValveMask::HasBreathFrequency() const
{
  return m_BreathFrequency == nullptr ? false : m_BreathFrequency->IsValid();
}
SEScalarFrequency& SEBagValveMask::GetBreathFrequency()
{
  if (m_BreathFrequency == nullptr)
    m_BreathFrequency = new SEScalarFrequency();
  return *m_BreathFrequency;
}
double SEBagValveMask::GetBreathFrequency(const FrequencyUnit& unit) const
{
  if (m_BreathFrequency == nullptr)
    return SEScalar::dNaN();
  return m_BreathFrequency->GetValue(unit);
}

bool SEBagValveMask::HasInspiratoryExpiratoryRatio() const
{
  return m_InspiratoryExpiratoryRatio == nullptr ? false : m_InspiratoryExpiratoryRatio->IsValid();
}
SEScalar& SEBagValveMask::GetInspiratoryExpiratoryRatio()
{
  if (m_InspiratoryExpiratoryRatio == nullptr)
    m_InspiratoryExpiratoryRatio = new SEScalar();
  return *m_InspiratoryExpiratoryRatio;
}
double SEBagValveMask::GetInspiratoryExpiratoryRatio() const
{
  if (m_InspiratoryExpiratoryRatio == nullptr)
    return SEScalar::dNaN();
  return m_InspiratoryExpiratoryRatio->GetValue();
}

bool SEBagValveMask::HasSqueezePressure() const
{
  return m_SqueezePressure == nullptr ? false : m_SqueezePressure->IsValid();
}
SEScalarPressure& SEBagValveMask::GetSqueezePressure()
{
  if (m_SqueezePressure == nullptr)
    m_SqueezePressure = new SEScalarPressure();
  return *m_SqueezePressure;
}
double SEBagValveMask::GetSqueezePressure(const PressureUnit& unit) const
{
  if (m_SqueezePressure == nullptr)
    return SEScalar::dNaN();
  return m_SqueezePressure->GetValue(unit);
}

bool SEBagValveMask::HasSqueezeVolume() const
{
  return m_SqueezeVolume == nullptr ? false : m_SqueezeVolume->IsValid();
}
SEScalarVolume& SEBagValveMask::GetSqueezeVolume()
{
  if (m_SqueezeVolume == nullptr)
    m_SqueezeVolume = new SEScalarVolume();
  return *m_SqueezeVolume;
}
double SEBagValveMask::GetSqueezeVolume(const VolumeUnit& unit) const
{
  if (m_SqueezeVolume == nullptr)
    return SEScalar::dNaN();
  return m_SqueezeVolume->GetValue(unit);
}

bool SEBagValveMask::HasValvePositiveEndExpiredPressure() const
{
  return m_ValvePositiveEndExpiredPressure == nullptr ? false : m_ValvePositiveEndExpiredPressure->IsValid();
}
SEScalarPressure& SEBagValveMask::GetValvePositiveEndExpiredPressure()
{
  if (m_ValvePositiveEndExpiredPressure == nullptr)
    m_ValvePositiveEndExpiredPressure = new SEScalarPressure();
  return *m_ValvePositiveEndExpiredPressure;
}
double SEBagValveMask::GetValvePositiveEndExpiredPressure(const PressureUnit& unit) const
{
  if (m_ValvePositiveEndExpiredPressure == nullptr)
    return SEScalar::dNaN();
  return m_ValvePositiveEndExpiredPressure->GetValue(unit);
}

bool SEBagValveMask::HasBagResistance() const
{
  return m_BagResistance == nullptr ? false : m_BagResistance->IsValid();
}
SEScalarPressureTimePerVolume& SEBagValveMask::GetBagResistance()
{
  if (m_BagResistance == nullptr)
    m_BagResistance = new SEScalarPressureTimePerVolume();
  return *m_BagResistance;
}
double SEBagValveMask::GetBagResistance(const PressureTimePerVolumeUnit& unit) const
{
  if (m_BagResistance == nullptr)
    return SEScalar::dNaN();
  return m_BagResistance->GetValue(unit);
}

bool SEBagValveMask::HasFilterResistance() const
{
  return m_FilterResistance == nullptr ? false : m_FilterResistance->IsValid();
}
SEScalarPressureTimePerVolume& SEBagValveMask::GetFilterResistance()
{
  if (m_FilterResistance == nullptr)
    m_FilterResistance = new SEScalarPressureTimePerVolume();
  return *m_FilterResistance;
}
double SEBagValveMask::GetFilterResistance(const PressureTimePerVolumeUnit& unit) const
{
  if (m_FilterResistance == nullptr)
    return SEScalar::dNaN();
  return m_FilterResistance->GetValue(unit);
}

bool SEBagValveMask::HasFilterVolume() const
{
  return m_FilterVolume == nullptr ? false : m_FilterVolume->IsValid();
}
SEScalarVolume& SEBagValveMask::GetFilterVolume()
{
  if (m_FilterVolume == nullptr)
    m_FilterVolume = new SEScalarVolume();
  return *m_FilterVolume;
}
double SEBagValveMask::GetFilterVolume(const VolumeUnit& unit) const
{
  if (m_FilterVolume == nullptr)
    return SEScalar::dNaN();
  return m_FilterVolume->GetValue(unit);
}

bool SEBagValveMask::HasValveResistance() const
{
  return m_ValveResistance == nullptr ? false : m_ValveResistance->IsValid();
}
SEScalarPressureTimePerVolume& SEBagValveMask::GetValveResistance()
{
  if (m_ValveResistance == nullptr)
    m_ValveResistance = new SEScalarPressureTimePerVolume();
  return *m_ValveResistance;
}
double SEBagValveMask::GetValveResistance(const PressureTimePerVolumeUnit& unit) const
{
  if (m_ValveResistance == nullptr)
    return SEScalar::dNaN();
  return m_ValveResistance->GetValue(unit);
}

bool SEBagValveMask::HasFractionInspiredGas() const
{
  if (m_FractionInspiredGases.empty())
    return false;
  for (auto a : m_FractionInspiredGases)
    if (a->HasFractionAmount() && a->GetFractionAmount().IsPositive())
      return true;
  return false;
}
bool SEBagValveMask::HasFractionInspiredGas(const SESubstance& s) const
{
  for (const SESubstanceFraction* sf : m_FractionInspiredGases)
  {
    if (&s == &sf->GetSubstance())
      return sf->GetFractionAmount() > 0;
  }
  return false;
}
const std::vector<SESubstanceFraction*>& SEBagValveMask::GetFractionInspiredGases()
{
  return m_FractionInspiredGases;
}
const std::vector<const SESubstanceFraction*>& SEBagValveMask::GetFractionInspiredGases() const
{
  return m_cFractionInspiredGases;
}
SESubstanceFraction& SEBagValveMask::GetFractionInspiredGas(const SESubstance& s)
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
const SESubstanceFraction* SEBagValveMask::GetFractionInspiredGas(const SESubstance& s) const
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
void SEBagValveMask::RemoveFractionInspiredGas(const SESubstance& s)
{
  SESubstanceFraction& sf = GetFractionInspiredGas(s);
  sf.GetFractionAmount().SetValue(0);
}
void SEBagValveMask::RemoveFractionInspiredGases()
{
  for (SESubstanceFraction* sf : m_FractionInspiredGases)
    sf->GetFractionAmount().SetValue(0);
}

bool SEBagValveMask::HasConcentrationInspiredAerosol() const
{
  if (m_ConcentrationInspiredAerosols.empty())
    return false;
  for (auto a : m_ConcentrationInspiredAerosols)
    if (a->HasConcentration() && a->GetConcentration().IsPositive())
      return true;
  return false;
}
bool SEBagValveMask::HasConcentrationInspiredAerosol(const SESubstance& substance) const
{
  for (SESubstanceConcentration* sc : m_ConcentrationInspiredAerosols)
  {
    if (&substance == &sc->GetSubstance())
      return sc->GetConcentration().IsPositive();
  }
  return false;
}
const std::vector<SESubstanceConcentration*>& SEBagValveMask::GetConcentrationInspiredAerosols()
{
  return m_ConcentrationInspiredAerosols;
}
const std::vector<const SESubstanceConcentration*>& SEBagValveMask::GetConcentrationInspiredAerosols() const
{
  return m_cConcentrationInspiredAerosols;
}
SESubstanceConcentration& SEBagValveMask::GetConcentrationInspiredAerosol(const SESubstance& substance)
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
const SESubstanceConcentration* SEBagValveMask::GetConcentrationInspiredAerosol(const SESubstance& substance) const
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
void SEBagValveMask::RemoveConcentrationInspiredAerosol(const SESubstance& substance)
{
  SESubstanceConcentration& sc = GetConcentrationInspiredAerosol(substance);
  auto& unit = *sc.GetConcentration().GetUnit();
  sc.GetConcentration().SetValue(0, unit);
}
void SEBagValveMask::RemoveConcentrationInspiredAerosols()
{
  for (SESubstanceConcentration* sc : m_ConcentrationInspiredAerosols)
  {
    auto& unit = *sc->GetConcentration().GetUnit();
    sc->GetConcentration().SetValue(0, unit);
  }
}
