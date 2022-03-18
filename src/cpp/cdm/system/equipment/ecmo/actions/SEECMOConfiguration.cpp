/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "cdm/CommonDefs.h"
#include "cdm/system/equipment/ecmo/actions/SEECMOConfiguration.h"
#include "cdm/system/equipment/ecmo/SEECMOSettings.h"
#include "cdm/substance/SESubstance.h"
#include "cdm/substance/SESubstanceCompound.h"
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
    str << "\n\tInflowLocation: " << eECMO_CannulationLocation_Name(m_Settings->GetInflowLocation());
    str << "\n\tOutflowLocation: " << eECMO_CannulationLocation_Name(m_Settings->GetOutflowLocation());
    str << "\n\tOxygenatorVolume: "; m_Settings->HasOxygenatorVolume() ? str << m_Settings->GetOxygenatorVolume() : str << "NaN";
    str << "\n\tTransfusionFlow: "; m_Settings->HasTransfusionFlow() ? str << m_Settings->GetTransfusionFlow() : str << "NaN";
    str << "\n\tSubstanceCompound: "; m_Settings->HasSubstanceCompound() ? str << m_Settings->GetSubstanceCompound()->GetName() : str << "Not Provided";
    if (m_Settings->HasSubstanceConcentrations())
    {
      for (SESubstanceConcentration* sc : m_Settings->GetSubstanceConcentrations())
      {
        str << "\n\tSubstance : " << sc->GetSubstance().GetName() << " Concentration " << sc->GetConcentration();
      }
    }
  }
  str << "\n\tMergeType: " << eMergeType_Name(m_MergeType);
  str << std::flush;
}
