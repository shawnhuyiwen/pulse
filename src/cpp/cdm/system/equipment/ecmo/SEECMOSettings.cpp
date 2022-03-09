/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/system/equipment/ecmo/SEECMOSettings.h"
#include "cdm/system/equipment/ecmo/actions/SEECMOConfiguration.h"

#include "cdm/substance/SESubstance.h"
#include "cdm/substance/SESubstanceCompound.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/io/protobuf/PBECMO.h"

SEECMOSettings::SEECMOSettings(Logger* logger) : Loggable(logger)
{
  m_InflowLocation = eECMO_CannulationLocation::NullCannulationLocation;
  m_OutflowLocation = eECMO_CannulationLocation::NullCannulationLocation;
  m_OxygenatorVolume = nullptr;
  m_TransfusionFlow = nullptr;
}

SEECMOSettings::~SEECMOSettings()
{
  m_InflowLocation = eECMO_CannulationLocation::NullCannulationLocation;
  m_OutflowLocation = eECMO_CannulationLocation::NullCannulationLocation;
  SAFE_DELETE(m_OxygenatorVolume);
  SAFE_DELETE(m_TransfusionFlow);

  DELETE_VECTOR(m_SubstanceConcentrations);
  m_SubstanceConcentrations.clear();
}

void SEECMOSettings::Clear()
{
  m_InflowLocation = eECMO_CannulationLocation::NullCannulationLocation;
  m_OutflowLocation = eECMO_CannulationLocation::NullCannulationLocation;
  INVALIDATE_PROPERTY(m_OxygenatorVolume);
  INVALIDATE_PROPERTY(m_TransfusionFlow);

  RemoveSubstanceConcentrations();
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
  if(from.m_InflowLocation != eECMO_CannulationLocation::NullCannulationLocation)
    SetInflowLocation(from.m_InflowLocation);
  if (from.m_OutflowLocation != eECMO_CannulationLocation::NullCannulationLocation)
    SetOutflowLocation(from.m_OutflowLocation);
  COPY_PROPERTY(OxygenatorVolume);
  COPY_PROPERTY(TransfusionFlow);

  for (SESubstanceConcentration* sc : from.m_SubstanceConcentrations)
  {
    SESubstanceConcentration& mine = GetSubstanceConcentration(sc->GetSubstance());
    mine.GetConcentration().Set(sc->GetConcentration());
  }
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

  if (name == "OxygenatorVolume")
    return &GetOxygenatorVolume();
  if (name == "TransfusionFlow")
    return &GetTransfusionFlow();

  // I did not support for getting a specific concentration scalars due to lack of coffee
  return nullptr;
}

void SEECMOSettings::SetInflowLocation(eECMO_CannulationLocation c)
{
  m_InflowLocation = c;
}
eECMO_CannulationLocation SEECMOSettings::GetInflowLocation() const
{
  return m_InflowLocation;
}

void SEECMOSettings::SetOutflowLocation(eECMO_CannulationLocation c)
{
  m_OutflowLocation = c;
}
eECMO_CannulationLocation SEECMOSettings::GetOutflowLocation() const
{
  return m_OutflowLocation;
}

bool SEECMOSettings::HasOxygenatorVolume() const
{
  return m_OxygenatorVolume == nullptr ? false : m_OxygenatorVolume->IsValid();
}
SEScalarVolume& SEECMOSettings::GetOxygenatorVolume()
{
  if (m_OxygenatorVolume == nullptr)
    m_OxygenatorVolume = new SEScalarVolume();
  return *m_OxygenatorVolume;
}
double SEECMOSettings::GetOxygenatorVolume(const VolumeUnit& unit) const
{
  if (m_OxygenatorVolume == nullptr)
    return SEScalar::dNaN();
  return m_OxygenatorVolume->GetValue(unit);
}

bool SEECMOSettings::HasTransfusionFlow() const
{
  return m_TransfusionFlow == nullptr ? false : m_TransfusionFlow->IsValid();
}
SEScalarVolumePerTime& SEECMOSettings::GetTransfusionFlow()
{
  if (m_TransfusionFlow == nullptr)
    m_TransfusionFlow = new SEScalarVolumePerTime();
  return *m_TransfusionFlow;
}
double SEECMOSettings::GetTransfusionFlow(const VolumePerTimeUnit& unit) const
{
  if (m_TransfusionFlow == nullptr)
    return SEScalar::dNaN();
  return m_TransfusionFlow->GetValue(unit);
}

bool SEECMOSettings::HasSubstanceConcentrations() const
{
  if (m_SubstanceConcentrations.empty())
    return false;
  for (auto a : m_SubstanceConcentrations)
    if (a->HasConcentration() && a->GetConcentration().IsPositive())
      return true;
  return false;
}
bool SEECMOSettings::HasSubstanceConcentration(const SESubstance& substance) const
{
  for (SESubstanceConcentration* sc : m_SubstanceConcentrations)
  {
    if (&substance == &sc->GetSubstance())
      return sc->GetConcentration().IsPositive();
  }
  return false;
}
const std::vector<SESubstanceConcentration*>& SEECMOSettings::GetSubstanceConcentrations()
{
  return m_SubstanceConcentrations;
}
const std::vector<const SESubstanceConcentration*>& SEECMOSettings::GetSubstanceConcentrations() const
{
  return std::vector<const SESubstanceConcentration*>(m_SubstanceConcentrations.begin(), m_SubstanceConcentrations.end());
}
SESubstanceConcentration& SEECMOSettings::GetSubstanceConcentration(const SESubstance& substance)
{
  for (SESubstanceConcentration* sc : m_SubstanceConcentrations)
  {
    if (&substance == &sc->GetSubstance())
      return *sc;
  }
  SESubstanceConcentration* sc = new SESubstanceConcentration(substance);
  sc->GetConcentration().SetValue(0, MassPerVolumeUnit::ug_Per_L);
  m_SubstanceConcentrations.push_back(sc);
  return *sc;
}
const SESubstanceConcentration* SEECMOSettings::GetSubstanceConcentration(const SESubstance& substance) const
{
  const SESubstanceConcentration* sc = nullptr;
  for (unsigned int i = 0; i < m_SubstanceConcentrations.size(); i++)
  {
    sc = m_SubstanceConcentrations[i];
    if (&substance == &sc->GetSubstance())
      return sc;
  }
  return sc;
}
void SEECMOSettings::RemoveSubstanceConcentration(const SESubstance& substance)
{
  SESubstanceConcentration& sc = GetSubstanceConcentration(substance);
  auto& unit = *sc.GetConcentration().GetUnit();
  sc.GetConcentration().SetValue(0, unit);
}
void SEECMOSettings::RemoveSubstanceConcentrations()
{
  for (SESubstanceConcentration* sc : m_SubstanceConcentrations)
  {
    auto& unit = *sc->GetConcentration().GetUnit();
    sc->GetConcentration().SetValue(0, unit);
  }
}

void SEECMOSettings::ApplyCompoundConcentrations(const SESubstanceCompound& compound)
{
  for (const SESubstanceConcentration* sc : compound.GetComponents())
  {
    SESubstanceConcentration& mine = GetSubstanceConcentration(sc->GetSubstance());
    mine.GetConcentration().Set(*sc->GetConcentration());
  }
}
