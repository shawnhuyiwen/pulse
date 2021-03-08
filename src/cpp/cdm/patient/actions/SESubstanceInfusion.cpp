/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SESubstanceInfusion.h"
#include "substance/SESubstance.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalarMassPerVolume.h"
#include "io/protobuf/PBPatientActions.h"

SESubstanceInfusion::SESubstanceInfusion(const SESubstance& substance, Logger* logger) : SEPatientAction(logger), m_Substance(substance)
{
  m_Rate=nullptr;
  m_Concentration=nullptr;
}

SESubstanceInfusion::~SESubstanceInfusion()
{
  SAFE_DELETE(m_Rate);
  SAFE_DELETE(m_Concentration);
}

void SESubstanceInfusion::Clear()
{
  SEPatientAction::Clear();
  INVALIDATE_PROPERTY(m_Rate);
  INVALIDATE_PROPERTY(m_Concentration);
}

void SESubstanceInfusion::Copy(const SESubstanceInfusion& src, bool preserveState)
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
  return !m_Rate->IsZero() && !m_Concentration->IsZero();
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
  str << "Patient Action : Substance Infusion"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str << "\n\tRate: "; HasRate() ? str << *m_Rate : str << "NaN";
  str << "\n\tConcentration: "; HasConcentration() ? str << *m_Concentration : str << "NaN";
  str << "\n\tSubstance: " << m_Substance.GetName();
  str  <<std::flush;
}
