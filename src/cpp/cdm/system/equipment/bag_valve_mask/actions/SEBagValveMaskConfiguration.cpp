/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/bag_valve_mask/actions/SEBagValveMaskConfiguration.h"
#include "system/equipment/bag_valve_mask/SEBagValveMask.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"
#include "substance/SESubstanceConcentration.h"
#include "substance/SESubstanceFraction.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarPressureTimePerVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarTime.h"
#include "io/protobuf/PBEquipmentActions.h"

SEBagValveMaskConfiguration::SEBagValveMaskConfiguration(Logger* logger) : SEBagValveMaskAction(logger)
{
  m_ConfigurationFile = "";
  m_Configuration = nullptr;
}

SEBagValveMaskConfiguration::~SEBagValveMaskConfiguration()
{
  m_ConfigurationFile = "";
  SAFE_DELETE(m_Configuration);
}

void SEBagValveMaskConfiguration::Clear()
{
  SEBagValveMaskAction::Clear();
  m_ConfigurationFile = "";
  if (m_Configuration)
    m_Configuration->Clear();
}

void SEBagValveMaskConfiguration::Copy(const SEBagValveMaskConfiguration& src, const SESubstanceManager& subMgr, bool preserveState)
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

void SEBagValveMaskConfiguration::ToString(std::ostream &str) const
{
  str << "Bag Valve Mask Configuration";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  if (HasConfigurationFile())
  {
    str << "\n\tConfiguration File: "; str << m_ConfigurationFile;
  }
  else if (HasConfiguration())
  {
    str << "\n\tConnection: " << eBagValveMask_Connection_Name(m_Configuration->GetConnection());
    str << "\n\tValvePositiveEndExpiredPressure: "; m_Configuration->HasValvePositiveEndExpiredPressure() ? str << m_Configuration->GetValvePositiveEndExpiredPressure() : str << "NaN";
    str << "\n\tBagResistance: "; m_Configuration->HasBagResistance() ? str << m_Configuration->GetBagResistance() : str << "NaN";
    str << "\n\tFilterResistance: "; m_Configuration->HasFilterResistance() ? str << m_Configuration->GetFilterResistance() : str << "NaN";
    str << "\n\tFilterVolume: "; m_Configuration->HasFilterVolume() ? str << m_Configuration->GetFilterVolume() : str << "NaN";
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
  str << std::flush;
}
