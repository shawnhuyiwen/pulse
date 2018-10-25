/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SESubstanceInfusion.h"
#include "substance/SESubstance.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalarMassPerVolume.h"
#include "io/protobuf/PBPatientActions.h"

SESubstanceInfusion::SESubstanceInfusion(const SESubstance& substance) : SESubstanceAdministration(), m_Substance(substance)
{

  m_Rate=nullptr;
  m_Concentration=nullptr;
}

SESubstanceInfusion::~SESubstanceInfusion()
{
  Clear();
}

void SESubstanceInfusion::Clear()
{
  SESubstanceAdministration::Clear();
  m_Rate=nullptr;
  m_Concentration=nullptr;  
}

void SESubstanceInfusion::Copy(const SESubstanceInfusion& src)
{
  PBPatientAction::Copy(src, *this);
}

bool SESubstanceInfusion::IsValid() const
{
  return SESubstanceAdministration::IsValid() && HasRate() && HasConcentration();
}

bool SESubstanceInfusion::IsActive() const
{
  return IsValid() ? !m_Rate->IsZero() : false;
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
  return (SESubstance&)m_Substance;
}
const SESubstance& SESubstanceInfusion::GetSubstance() const
{
  return (SESubstance&)m_Substance;
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