/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/environment/actions/SEChangeEnvironmentalConditions.h"
#include "system/environment/SEEnvironmentalConditions.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceFraction.h"
#include "substance/SESubstanceConcentration.h"
#include "substance/SESubstanceManager.h"

#include "properties/SEScalar0To1.h"
#include "properties/SEScalarHeatConductancePerArea.h"
#include "properties/SEScalarLengthPerTime.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarPower.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarTemperature.h"
#include "properties/SEScalarHeatResistanceArea.h"
#include "io/protobuf/PBEnvironmentActions.h"


SEChangeEnvironmentalConditions::SEChangeEnvironmentalConditions(Logger* logger) : SEEnvironmentAction(logger)
{
  m_EnvironmentalConditions = nullptr;
  m_EnvironmentalConditionsFile = "";
}

SEChangeEnvironmentalConditions::~SEChangeEnvironmentalConditions()
{
  m_EnvironmentalConditionsFile = "";
  SAFE_DELETE(m_EnvironmentalConditions);
}

void SEChangeEnvironmentalConditions::Clear()
{
  SEEnvironmentAction::Clear();
  m_EnvironmentalConditionsFile = "";
  if (m_EnvironmentalConditions)
    m_EnvironmentalConditions->Clear();
}

void SEChangeEnvironmentalConditions::Copy(const SEChangeEnvironmentalConditions& src, const SESubstanceManager& subMgr, bool preserveState)
{// Using Bindings to make a copy
  PBEnvironmentAction::Copy(src, *this, subMgr);
}

bool SEChangeEnvironmentalConditions::IsValid() const
{
  return SEEnvironmentAction::IsValid() && (HasEnvironmentalConditions() || HasEnvironmentalConditionsFile());
}

bool SEChangeEnvironmentalConditions::IsActive() const
{
  return SEEnvironmentAction::IsActive();
}
void SEChangeEnvironmentalConditions::Deactivate()
{
  SEEnvironmentAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEChangeEnvironmentalConditions::GetScalar(const std::string& name)
{
  return GetEnvironmentalConditions().GetScalar(name);
}

bool SEChangeEnvironmentalConditions::HasEnvironmentalConditions() const
{
  return m_EnvironmentalConditions != nullptr;
}
SEEnvironmentalConditions& SEChangeEnvironmentalConditions::GetEnvironmentalConditions()
{
  if (m_EnvironmentalConditions == nullptr)
    m_EnvironmentalConditions = new SEEnvironmentalConditions(GetLogger());
  return *m_EnvironmentalConditions;
}
const SEEnvironmentalConditions* SEChangeEnvironmentalConditions::GetEnvironmentalConditions() const
{
  return m_EnvironmentalConditions;
}

std::string SEChangeEnvironmentalConditions::GetEnvironmentalConditionsFile() const
{
  return m_EnvironmentalConditionsFile;
}
void SEChangeEnvironmentalConditions::SetEnvironmentalConditionsFile(const std::string& fileName)
{
  m_EnvironmentalConditionsFile = fileName;
}
bool SEChangeEnvironmentalConditions::HasEnvironmentalConditionsFile() const
{
  return !m_EnvironmentalConditionsFile.empty();
}

void SEChangeEnvironmentalConditions::ToString(std::ostream &str) const
{
  str << "Environment Action : Change Environmental Conditions"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  if (HasEnvironmentalConditionsFile())
  {
    str << "\n\tEnvironmental Conditions File: "; str << m_EnvironmentalConditionsFile;
  }
  else if (HasEnvironmentalConditions())
  {
    str << "\n\tSurroundingType: " << eSurroundingType_Name(m_EnvironmentalConditions->GetSurroundingType());
    str << "\n\tAir Velocity: ";  m_EnvironmentalConditions->HasAirVelocity() ? str << m_EnvironmentalConditions->GetAirVelocity() : str << "Not Set";
    str << "\n\tAmbient Temperature: ";  m_EnvironmentalConditions->HasAmbientTemperature() ? str << m_EnvironmentalConditions->GetAmbientTemperature() : str << "Not Set";
    str << "\n\tAtmospheric Pressure: ";  m_EnvironmentalConditions->HasAtmosphericPressure() ? str << m_EnvironmentalConditions->GetAtmosphericPressure() : str << "Not Set";
    str << "\n\tClothing Resistance: ";  m_EnvironmentalConditions->HasClothingResistance() ? str << m_EnvironmentalConditions->GetClothingResistance() : str << "Not Set";
    str << "\n\tEmissivity: ";  m_EnvironmentalConditions->HasEmissivity() ? str << m_EnvironmentalConditions->GetEmissivity() : str << "Not Set";
    str << "\n\tMean Radiant Temperature: ";  m_EnvironmentalConditions->HasMeanRadiantTemperature() ? str << m_EnvironmentalConditions->GetMeanRadiantTemperature() : str << "Not Set";
    str << "\n\tRelative Humidity: ";  m_EnvironmentalConditions->HasRelativeHumidity() ? str << m_EnvironmentalConditions->GetRelativeHumidity() : str << "Not Set";
    str << "\n\tRespiration Ambient Temperature: ";  m_EnvironmentalConditions->HasRespirationAmbientTemperature() ? str << m_EnvironmentalConditions->GetRespirationAmbientTemperature() : str << "Not Set";
    if (m_EnvironmentalConditions->HasAmbientGas())
    {
      for (SESubstanceFraction* sf : m_EnvironmentalConditions->GetAmbientGases())
      {
        str << "\n\tSubstance : " << sf->GetSubstance().GetName() << " Fraction Amount " << sf->GetFractionAmount();
      }
    }
    if (m_EnvironmentalConditions->HasAmbientAerosol())
    {
      for (SESubstanceConcentration* sc : m_EnvironmentalConditions->GetAmbientAerosols())
      {
        str << "\n\tSubstance : " << sc->GetSubstance().GetName() << " Concentration " << sc->GetConcentration();
      }
    }
  }
  str << std::flush;
}
