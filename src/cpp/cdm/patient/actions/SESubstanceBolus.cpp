/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SESubstanceBolus.h"
#include "substance/SESubstance.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarTime.h"
#include "io/protobuf/PBPatientActions.h"

SESubstanceBolus::SESubstanceBolus(const SESubstance& substance, Logger* logger) : SEPatientAction(logger), m_Substance(substance)
{
  m_AdminRoute=eSubstanceAdministration_Route::Intravenous;
  m_AdminDuration=nullptr;
  m_Dose=nullptr;
  m_Concentration=nullptr;
  m_TotalInfusedDose = nullptr;
}

SESubstanceBolus::~SESubstanceBolus()
{
  m_AdminRoute = eSubstanceAdministration_Route::Intravenous;
  SAFE_DELETE(m_AdminDuration);
  SAFE_DELETE(m_Dose);
  SAFE_DELETE(m_Concentration);
  SAFE_DELETE(m_TotalInfusedDose);
}

void SESubstanceBolus::Clear()
{
  SEPatientAction::Clear();
  m_AdminRoute=eSubstanceAdministration_Route::Intravenous;
  INVALIDATE_PROPERTY(m_AdminDuration);
  INVALIDATE_PROPERTY(m_Dose);
  INVALIDATE_PROPERTY(m_Concentration);
  INVALIDATE_PROPERTY(m_TotalInfusedDose);
}

void SESubstanceBolus::Copy(const SESubstanceBolus& src, bool preserveState)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}

bool SESubstanceBolus::IsValid() const
{
  return SEPatientAction::IsValid() && HasDose() && HasConcentration();
}

bool SESubstanceBolus::IsActive() const
{
  if (!SEPatientAction::IsActive())
    return false;
  return !m_Dose->IsZero() && !m_Concentration->IsZero();
}
void SESubstanceBolus::Activate()
{
  SEPatientAction::Activate();
  if(!HasTotalInfusedDose())
    GetTotalInfusedDose().SetValue(0, VolumeUnit::mL);
}
void SESubstanceBolus::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SESubstanceBolus::GetScalar(const std::string& name)
{
  if (name.compare("AdminDuration") == 0)
    return &GetAdminDuration();
  if (name.compare("TotalInfusedDose") == 0)
    return &GetTotalInfusedDose();
  if (name.compare("Dose") == 0)
    return &GetDose();
  if (name.compare("Concentration") == 0)
    return &GetConcentration();
  return nullptr;
}

eSubstanceAdministration_Route SESubstanceBolus::GetAdminRoute() const
{
  return m_AdminRoute;
}
void SESubstanceBolus::SetAdminRoute(eSubstanceAdministration_Route route)
{
  m_AdminRoute = route;
}

bool SESubstanceBolus::HasAdminDuration() const
{
  return m_AdminDuration == nullptr ? false : m_AdminDuration->IsValid();
}
SEScalarTime& SESubstanceBolus::GetAdminDuration()
{
  if (m_AdminDuration == nullptr)
    m_AdminDuration = new SEScalarTime();
  return *m_AdminDuration;
}
double SESubstanceBolus::GetAdminDuration(const TimeUnit& unit) const
{
  if (m_AdminDuration == nullptr)
    return SEScalar::dNaN();
  return m_AdminDuration->GetValue(unit);
}

bool SESubstanceBolus::HasTotalInfusedDose() const
{
  return m_TotalInfusedDose == nullptr ? false : m_TotalInfusedDose->IsValid();
}
SEScalarVolume& SESubstanceBolus::GetTotalInfusedDose()
{
  if (m_TotalInfusedDose == nullptr)
    m_TotalInfusedDose = new SEScalarVolume();
  return *m_TotalInfusedDose;
}
double SESubstanceBolus::GetTotalInfusedDose(const VolumeUnit& unit) const
{
  if (m_TotalInfusedDose == nullptr)
    return SEScalar::dNaN();
  return m_TotalInfusedDose->GetValue(unit);
}

bool SESubstanceBolus::HasDose() const
{
  return m_Dose==nullptr?false:m_Dose->IsValid();
}
SEScalarVolume& SESubstanceBolus::GetDose()
{
  if(m_Dose==nullptr)
    m_Dose=new SEScalarVolume();
  return *m_Dose;
}
double SESubstanceBolus::GetDose(const VolumeUnit& unit) const
{
  if (m_Dose == nullptr)
    return SEScalar::dNaN();
  return m_Dose->GetValue(unit);
}

bool SESubstanceBolus::HasConcentration() const
{
  return m_Concentration==nullptr?false:m_Concentration->IsValid();
}
SEScalarMassPerVolume& SESubstanceBolus::GetConcentration()
{
  if(m_Concentration==nullptr)
    m_Concentration=new SEScalarMassPerVolume();
  return *m_Concentration;
}
double SESubstanceBolus::GetConcentration(const MassPerVolumeUnit& unit) const
{
  if (m_Concentration == nullptr)
    return SEScalar::dNaN();
  return m_Concentration->GetValue(unit);
}

SESubstance& SESubstanceBolus::GetSubstance()
{
  return const_cast<SESubstance&>(m_Substance);
}
const SESubstance& SESubstanceBolus::GetSubstance() const
{
  return m_Substance;
}

void SESubstanceBolus::ToString(std::ostream &str) const
{
  str << "Patient Action : Substance Bolus"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str << "\n\tSubstance: " << m_Substance.GetName();
  str << "\n\tAdministration Route: " << eSubstanceAdministration_Route_Name(GetAdminRoute());
  str << "\n\tAdministration Duration: "; HasAdminDuration() ? str << *m_AdminDuration : str << "No Administration Duration Set";
  str << "\n\tDose: "; HasDose()? str << *m_Dose : str << "No Dose Set";
  str << "\n\tConcentration: "; HasConcentration()? str << *m_Concentration : str << "NaN";
  str << "\n\tTotalInfusedDose: "; HasTotalInfusedDose() ? str << *m_TotalInfusedDose : str << "NaN";
  str << std::flush;
}
