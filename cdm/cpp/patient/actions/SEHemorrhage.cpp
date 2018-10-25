/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEHemorrhage.h"
#include "properties/SEScalarVolumePerTime.h"
#include "io/protobuf/PBPatientActions.h"

SEHemorrhage::SEHemorrhage() : SEPatientAction()
{
  m_Compartment = "";
  m_Rate=nullptr;
}

SEHemorrhage::~SEHemorrhage()
{
  Clear();
}

void SEHemorrhage::Clear()
{
  SEPatientAction::Clear();
  m_Compartment = "";
  SAFE_DELETE(m_Rate);
}

void SEHemorrhage::Copy(const SEHemorrhage& src)
{
  PBPatientAction::Copy(src, *this);
}

bool SEHemorrhage::IsValid() const
{
  return SEPatientAction::IsValid() && HasCompartment() && HasRate();
}

bool SEHemorrhage::IsActive() const
{
  return IsValid() ? !m_Rate->IsZero() : false;
}

std::string SEHemorrhage::GetCompartment() const
{
  return m_Compartment;
}

void SEHemorrhage::SetCompartment(const std::string& name)
{
  m_Compartment = name;
}

bool SEHemorrhage::HasCompartment() const
{
  return !m_Compartment.empty();
}

void SEHemorrhage::InvalidateCompartment()
{
  m_Compartment = "";
}

bool SEHemorrhage::HasRate() const
{
  return m_Rate==nullptr?false:m_Rate->IsValid();
}
SEScalarVolumePerTime& SEHemorrhage::GetRate()
{
  if(m_Rate==nullptr)
    m_Rate=new SEScalarVolumePerTime();
  return *m_Rate;
}
double SEHemorrhage::GetRate(const VolumePerTimeUnit& unit) const
{
  if (m_Rate == nullptr)
    return SEScalar::dNaN();
  return m_Rate->GetValue(unit);
}

void SEHemorrhage::ToString(std::ostream &str) const
{
  str << "Patient Action : Hemorrhage"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str << "\n\tRate: "; HasRate() ? str << *m_Rate : str << "NaN";
  str << "\n\tFor Compartment: "; HasCompartment()? str << GetCompartment() : str << "No Compartment Set"; 
  str << std::flush;
}
