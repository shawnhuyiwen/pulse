/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/actions/SESubstanceInfusion.h"
#include "cdm/substance/SESubstance.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/io/protobuf/PBPatientActions.h"

SESubstanceInfusion::SESubstanceInfusion(const SESubstance& substance, Logger* logger) : SEPatientAction(logger), m_Substance(substance)
{
  m_Rate=nullptr;
  m_Concentration=nullptr;
  m_Volume=nullptr;
}

SESubstanceInfusion::~SESubstanceInfusion()
{
  SAFE_DELETE(m_Rate);
  SAFE_DELETE(m_Concentration);
  SAFE_DELETE(m_Volume);
}

void SESubstanceInfusion::Clear()
{
  SEPatientAction::Clear();
  INVALIDATE_PROPERTY(m_Rate);
  INVALIDATE_PROPERTY(m_Concentration);
  INVALIDATE_PROPERTY(m_Volume);
}

void SESubstanceInfusion::Copy(const SESubstanceInfusion& src, bool /*preserveState*/)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}

bool SESubstanceInfusion::IsValid() const
{
  return SEPatientAction::IsValid() && HasRate() && HasConcentration();
}

bool SESubstanceInfusion::IsActive() const
{
  if (!SEPatientAction::IsActive())
    return false;
  if (m_Rate->IsZero())
    return false;
  if (m_Concentration->IsZero())
    return false;
  if (HasVolume() && m_Volume->IsZero())
    return false;
  return true;
}
void SESubstanceInfusion::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SESubstanceInfusion::GetScalar(const std::string& name)
{
  if (name.compare("Rate") == 0)
    return &GetRate();
  if (name.compare("Concentration") == 0)
    return &GetConcentration();
  if (name.compare("Volume") == 0)
    return &GetVolume();
  return nullptr;
}

bool SESubstanceInfusion::HasRate() const
{
  return m_Rate==nullptr?false:m_Rate->IsValid();
}
SEScalarVolumePerTime& SESubstanceInfusion::GetRate()
{
  if(m_Rate==nullptr)
    m_Rate=new SEScalarVolumePerTime();
  return *m_Rate;
}
double SESubstanceInfusion::GetRate(const VolumePerTimeUnit& unit) const
{
  if (m_Rate == nullptr)
    return SEScalar::dNaN();
  return m_Rate->GetValue(unit);
}

bool SESubstanceInfusion::HasConcentration() const
{
  return m_Concentration==nullptr?false:m_Concentration->IsValid();
}
SEScalarMassPerVolume& SESubstanceInfusion::GetConcentration()
{
  if(m_Concentration==nullptr)
    m_Concentration=new SEScalarMassPerVolume();
  return *m_Concentration;
}
double SESubstanceInfusion::GetConcentration(const MassPerVolumeUnit& unit) const
{
  if (m_Concentration == nullptr)
    return SEScalar::dNaN();
  return m_Concentration->GetValue(unit);
}

bool SESubstanceInfusion::HasVolume() const
{
  return m_Volume == nullptr ? false : m_Volume->IsValid();
}
SEScalarVolume& SESubstanceInfusion::GetVolume()
{
  if (m_Volume == nullptr)
    m_Volume = new SEScalarVolume();
  return *m_Volume;
}
double SESubstanceInfusion::GetVolume(const VolumeUnit& unit) const
{
  if (m_Volume == nullptr)
    return SEScalar::dNaN();
  return m_Volume->GetValue(unit);
}

SESubstance& SESubstanceInfusion::GetSubstance()
{
  return const_cast<SESubstance&>(m_Substance);
}
const SESubstance& SESubstanceInfusion::GetSubstance() const
{
  return m_Substance;
}

void SESubstanceInfusion::ToString(std::ostream &str) const
{
  SEPatientAction::ToString(str);
  str << "\n\tRate: "; HasRate() ? str << *m_Rate : str << "NaN";
  str << "\n\tConcentration: "; HasConcentration() ? str << *m_Concentration : str << "NaN";
  str << "\n\tVolume: "; HasVolume() ? str << *m_Volume : str << "NaN";
  str << "\n\tSubstance: " << m_Substance.GetName();
  str  <<std::flush;
}
