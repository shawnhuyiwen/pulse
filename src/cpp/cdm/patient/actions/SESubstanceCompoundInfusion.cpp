/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SESubstanceCompoundInfusion.h"
#include "substance/SESubstanceCompound.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalarVolume.h"
#include "io/protobuf/PBPatientActions.h"

SESubstanceCompoundInfusion::SESubstanceCompoundInfusion(const SESubstanceCompound& compound, Logger* logger) : SEPatientAction(logger), m_Compound(compound)
{
  m_Rate=nullptr;
  m_BagVolume=nullptr;
}

SESubstanceCompoundInfusion::~SESubstanceCompoundInfusion()
{
  SAFE_DELETE(m_Rate);
  SAFE_DELETE(m_BagVolume);
  // m_Compound=nullptr; Keeping mapping!!
}

void SESubstanceCompoundInfusion::Clear()
{
  SEPatientAction::Clear();
  INVALIDATE_PROPERTY(m_Rate);
  INVALIDATE_PROPERTY(m_BagVolume);
}

void SESubstanceCompoundInfusion::Copy(const SESubstanceCompoundInfusion& src, bool preserveState)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}

bool SESubstanceCompoundInfusion::IsValid() const
{
  return SEPatientAction::IsValid() && HasRate() && HasBagVolume();
}

bool SESubstanceCompoundInfusion::IsActive() const
{
  if (!SEPatientAction::IsActive())
    return false;
  return !m_Rate->IsZero();
}
void SESubstanceCompoundInfusion::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SESubstanceCompoundInfusion::GetScalar(const std::string& name)
{
  if (name.compare("Rate") == 0)
    return &GetRate();
  if (name.compare("BagVolume") == 0)
    return &GetBagVolume();
  return nullptr;
}

bool SESubstanceCompoundInfusion::HasRate() const
{
  return m_Rate==nullptr?false:m_Rate->IsValid();
}
SEScalarVolumePerTime& SESubstanceCompoundInfusion::GetRate()
{
  if(m_Rate==nullptr)
    m_Rate=new SEScalarVolumePerTime();
  return *m_Rate;
}
double SESubstanceCompoundInfusion::GetRate(const VolumePerTimeUnit& unit) const
{
  if (m_Rate == nullptr)
    return SEScalar::dNaN();
  return m_Rate->GetValue(unit);
}


bool SESubstanceCompoundInfusion::HasBagVolume() const
{
  return m_BagVolume==nullptr?false:m_BagVolume->IsValid();
}
SEScalarVolume& SESubstanceCompoundInfusion::GetBagVolume()
{
  if(m_BagVolume==nullptr)
    m_BagVolume=new SEScalarVolume();
  return *m_BagVolume;
}
double SESubstanceCompoundInfusion::GetBagVolume(const VolumeUnit& unit) const
{
  if (m_BagVolume == nullptr)
    return SEScalar::dNaN();
  return m_BagVolume->GetValue(unit);
}

SESubstanceCompound& SESubstanceCompoundInfusion::GetSubstanceCompound()
{
  return const_cast<SESubstanceCompound&>(m_Compound);
}
const SESubstanceCompound& SESubstanceCompoundInfusion::GetSubstanceCompound() const
{
  return m_Compound;
}

void SESubstanceCompoundInfusion::ToString(std::ostream &str) const
{
  str << "Patient Action : Compound Infusion"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str  << "\n\tRate: "; HasRate()? str << *m_Rate : str <<"NaN";
  str  << "\n\tBag Volume: "; HasBagVolume()? str << *m_BagVolume : str << "NaN";
  str  << "\n\tSubstance Compound: " << m_Compound.GetName();
  str  << std::flush;
}
