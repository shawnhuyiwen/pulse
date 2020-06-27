/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SESubstanceBolus.h"
#include "substance/SESubstance.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarTime.h"
#include "io/protobuf/PBPatientActions.h"

SESubstanceBolus::SESubstanceBolus(const SESubstance& substance, Logger* logger) : SESubstanceAdministration(logger), m_Substance(substance), m_State(substance)
{
  m_AdminRoute=eSubstanceAdministration_Route::Intravenous;
  m_Dose=nullptr;
  m_Concentration=nullptr;
}

SESubstanceBolus::~SESubstanceBolus()
{
  Clear();
}

void SESubstanceBolus::Clear()
{
  SESubstanceAdministration::Clear();
  m_AdminRoute=eSubstanceAdministration_Route::Intravenous;
  SAFE_DELETE(m_Dose);
  SAFE_DELETE(m_Concentration);
  // m_Substance=nullptr; Keeping mapping!!
  m_State.Clear();
}

void SESubstanceBolus::Copy(const SESubstanceBolus& src)
{
  PBPatientAction::Copy(src, *this);
}

bool SESubstanceBolus::IsValid() const
{
  return SESubstanceAdministration::IsValid() && HasDose() && HasConcentration();
}

bool SESubstanceBolus::IsActive() const
{
  return IsValid();
}

eSubstanceAdministration_Route SESubstanceBolus::GetAdminRoute() const
{
  return m_AdminRoute;
}
void SESubstanceBolus::SetAdminRoute(eSubstanceAdministration_Route route)
{
  m_AdminRoute = route;
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
  return (SESubstance&)m_Substance;
}
const SESubstance& SESubstanceBolus::GetSubstance() const
{
  return (SESubstance&)m_Substance;
}

void SESubstanceBolus::ToString(std::ostream &str) const
{
  str << "Patient Action : Substance Bolus"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str  << "\n\tDose: "; HasDose()? str << *m_Dose : str << "No Dose Set";
  str  << "\n\tConcentration: "; HasConcentration()? str << *m_Concentration : str << "NaN";
  str << "\n\tSubstance: " << m_Substance.GetName();
  str  << "\n\tAdministration Route: " << eSubstanceAdministration_Route_Name(GetAdminRoute());
  str << std::flush;
}

SESubstanceBolusState::SESubstanceBolusState(const SESubstance& sub, Logger* logger) : Loggable(logger), m_Substance(sub)
{
  m_ElapsedTime = new SEScalarTime();
  m_AdministeredDose = new SEScalarVolume();
  Clear();
}
SESubstanceBolusState::~SESubstanceBolusState()
{
  delete m_ElapsedTime;
  delete m_AdministeredDose;
}

void SESubstanceBolusState::Clear()
{
  m_ElapsedTime->SetValue(0, TimeUnit::s);
  m_AdministeredDose->SetValue(0, VolumeUnit::mL);
}

double SESubstanceBolusState::GetElapsedTime(const TimeUnit& unit) const
{
  if (m_ElapsedTime == nullptr)
    return SEScalar::dNaN();
  return m_ElapsedTime->GetValue(unit);
}

double SESubstanceBolusState::GetAdministeredDose(const VolumeUnit& unit) const
{
  if (m_AdministeredDose == nullptr)
    return SEScalar::dNaN();
  return m_AdministeredDose->GetValue(unit);
}
