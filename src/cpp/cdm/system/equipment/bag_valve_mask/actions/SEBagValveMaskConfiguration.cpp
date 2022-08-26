/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "cdm/CommonDefs.h"
#include "cdm/system/equipment/bag_valve_mask/actions/SEBagValveMaskConfiguration.h"
#include "cdm/system/equipment/bag_valve_mask/SEBagValveMask.h"
#include "cdm/substance/SESubstance.h"
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/substance/SESubstanceConcentration.h"
#include "cdm/substance/SESubstanceFraction.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarPressureTimePerVolume.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/io/protobuf/PBEquipmentActions.h"

SEBagValveMaskConfiguration::SEBagValveMaskConfiguration(Logger* logger) : SEBagValveMaskAction(logger)
{
  m_ConfigurationFile = "";
  m_Configuration = nullptr;
  m_MergeType = eMergeType::Append;
}

SEBagValveMaskConfiguration::~SEBagValveMaskConfiguration()
{
  m_ConfigurationFile = "";
  SAFE_DELETE(m_Configuration);
  m_MergeType = eMergeType::Append;
}

void SEBagValveMaskConfiguration::Clear()
{
  SEBagValveMaskAction::Clear();
  m_ConfigurationFile = "";
  if (m_Configuration)
    m_Configuration->Clear();
  m_MergeType = eMergeType::Append;
}

void SEBagValveMaskConfiguration::Copy(const SEBagValveMaskConfiguration& src, const SESubstanceManager& subMgr, bool /*preserveState*/)
{// Using Bindings to make a copy
  PBEquipmentAction::Copy(src, *this, subMgr);
}

bool SEBagValveMaskConfiguration::IsValid() const
{
  return SEBagValveMaskAction::IsValid() && (HasConfiguration() || HasConfigurationFile());
}

bool SEBagValveMaskConfiguration::IsActive() const
{
  return SEBagValveMaskAction::IsActive();
}
void SEBagValveMaskConfiguration::Deactivate()
{
  SEBagValveMaskAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEBagValveMaskConfiguration::GetScalar(const std::string& name)
{
  return GetConfiguration().GetScalar(name);
}

bool SEBagValveMaskConfiguration::HasConfiguration() const
{
  return m_Configuration != nullptr;
}
SEBagValveMask& SEBagValveMaskConfiguration::GetConfiguration()
{
  if (m_Configuration == nullptr)
    m_Configuration = new SEBagValveMask(GetLogger());
  return *m_Configuration;
}
const SEBagValveMask* SEBagValveMaskConfiguration::GetConfiguration() const
{
  return m_Configuration;
}

std::string SEBagValveMaskConfiguration::GetConfigurationFile() const
{
  return m_ConfigurationFile;
}
void SEBagValveMaskConfiguration::SetConfigurationFile(const std::string& fileName)
{
  m_ConfigurationFile = fileName;
}
bool SEBagValveMaskConfiguration::HasConfigurationFile() const
{
  return !m_ConfigurationFile.empty();
}

void SEBagValveMaskConfiguration::SetMergeType(eMergeType m)
{
  m_MergeType = m;
}
eMergeType SEBagValveMaskConfiguration::GetMergeType() const
{
  return m_MergeType;
}

void SEBagValveMaskConfiguration::ToString(std::ostream &str) const
{
  SEBagValveMaskAction::ToString(str);
  if (HasConfigurationFile())
  {
    str << "\n\tConfiguration File: "; str << m_ConfigurationFile;
  }
  else if (HasConfiguration())
  {
    str << "\n\tConnection: " << eSwitch_Name(m_Configuration->GetConnection());
    str << "\n\tValvePositiveEndExpiredPressure: "; m_Configuration->HasValvePositiveEndExpiredPressure() ? str << m_Configuration->GetValvePositiveEndExpiredPressure() : str << "NaN";
    str << "\n\tBagResistance: "; m_Configuration->HasBagResistance() ? str << m_Configuration->GetBagResistance() : str << "NaN";
    str << "\n\tFilterResistance: "; m_Configuration->HasFilterResistance() ? str << m_Configuration->GetFilterResistance() : str << "NaN";
    str << "\n\tSealResistance: "; m_Configuration->HasSealResistance() ? str << m_Configuration->GetSealResistance() : str << "NaN";
    str << "\n\tFilterVolume: "; m_Configuration->HasFilterVolume() ? str << m_Configuration->GetFilterVolume() : str << "NaN";
    str << "\n\tConnectionVolume: "; m_Configuration->HasConnectionVolume() ? str << m_Configuration->GetConnectionVolume() : str << "NaN";
    str << "\n\tValveVolume: "; m_Configuration->HasValveVolume() ? str << m_Configuration->GetValveVolume() : str << "NaN";
    str << "\n\tValveResistance: "; m_Configuration->HasValveResistance() ? str << m_Configuration->GetValveResistance() : str << "NaN";
    
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
