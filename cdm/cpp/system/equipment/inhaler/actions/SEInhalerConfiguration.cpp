/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/inhaler/actions/SEInhalerConfiguration.h"
#include "system/equipment/inhaler/SEInhaler.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarVolume.h"
#include "io/protobuf/PBInhalerActions.h"

SEInhalerConfiguration::SEInhalerConfiguration(SESubstanceManager& substances) : SEInhalerAction(), m_Substances(substances)
{
  m_Configuration = nullptr;
  InvalidateConfigurationFile();
}

SEInhalerConfiguration::~SEInhalerConfiguration()
{
  Clear();
}

void SEInhalerConfiguration::Clear()
{
  SEInhalerAction::Clear();
  InvalidateConfigurationFile();
  SAFE_DELETE(m_Configuration);
}

void SEInhalerConfiguration::Copy(const SEInhalerConfiguration& src)
{// Using Bindings to make a copy
  PBInhalerAction::Copy(src, *this);
}

bool SEInhalerConfiguration::IsValid() const
{
  return SEInhalerAction::IsValid() && (HasConfiguration() || HasConfigurationFile());
}

bool SEInhalerConfiguration::HasConfiguration() const
{
  return m_Configuration != nullptr;
}
SEInhaler& SEInhalerConfiguration::GetConfiguration()
{
  m_ConfigurationFile = "";
  if (m_Configuration == nullptr)
    m_Configuration = new SEInhaler(m_Substances);
  return *m_Configuration;
}
const SEInhaler* SEInhalerConfiguration::GetConfiguration() const
{
  return m_Configuration;
}

std::string SEInhalerConfiguration::GetConfigurationFile() const
{
  return m_ConfigurationFile;
}
void SEInhalerConfiguration::SetConfigurationFile(const std::string& fileName)
{
  if (m_Configuration != nullptr)
    SAFE_DELETE(m_Configuration);
  m_ConfigurationFile = fileName;
}
bool SEInhalerConfiguration::HasConfigurationFile() const
{
  return m_ConfigurationFile.empty() ? false : true;
}
void SEInhalerConfiguration::InvalidateConfigurationFile()
{
  m_ConfigurationFile = "";
}


void SEInhalerConfiguration::ToString(std::ostream &str) const
{
  str << "Inhaler Configuration";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  if (HasConfigurationFile())
    str << "\n\tConfiguration File: "; str << m_ConfigurationFile;
  if (HasConfiguration())
  {
    str << "\n\tState: " << eSwitch_Name(m_Configuration->GetState());
    str << "\n\tMetered Dose: "; m_Configuration->HasMeteredDose() ? str << m_Configuration->GetMeteredDose() : str << "NaN";
    str << "\n\tNozzle Loss: "; m_Configuration->HasNozzleLoss() ? str << m_Configuration->GetNozzleLoss() : str << "NaN";
    str << "\n\tSpacerVolume: "; m_Configuration->HasSpacerVolume() ? str << m_Configuration->GetSpacerVolume() : str << "NaN";
    str << "\n\tSubstance: "; m_Configuration->HasSubstance() ? str << m_Configuration->GetSubstance()->GetName() : str << "Not Set";    
  }
  str << std::flush;
}

