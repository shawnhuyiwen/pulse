/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SESubstanceBolus.h"
#include "substance/SESubstance.h"
#include "bind/cdm/PatientActions.pb.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarTime.h"
#include "bind/cdm/PatientActionEnums.pb.h"

const std::string& eSubstanceAdministration_Route_Name(eSubstanceAdministration_Route m)
{
  return cdm::eSubstanceAdministration_Route_Name((cdm::eSubstanceAdministration_Route)m);
}

SESubstanceBolus::SESubstanceBolus(const SESubstance& substance) : SESubstanceAdministration(), m_Substance(substance), m_State(substance)
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

bool SESubstanceBolus::IsValid() const
{
  return SESubstanceAdministration::IsValid() && HasDose() && HasConcentration();
}

bool SESubstanceBolus::IsActive() const
{
  return IsValid();
}

void SESubstanceBolus::Load(const cdm::SubstanceBolusData& src, SESubstanceBolus& dst)
{
  SESubstanceBolus::Serialize(src, dst);
}
void SESubstanceBolus::Serialize(const cdm::SubstanceBolusData& src, SESubstanceBolus& dst)
{
  SEPatientAction::Serialize(src.patientaction(), dst);
  dst.SetAdminRoute((eSubstanceAdministration_Route)src.administrationroute());
  if (src.has_dose())
    SEScalarVolume::Load(src.dose(), dst.GetDose());
  if (src.has_concentration())
    SEScalarMassPerVolume::Load(src.concentration(), dst.GetConcentration());
  if (src.has_state())
    SESubstanceBolusState::Load(src.state(), dst.m_State);
}

cdm::SubstanceBolusData* SESubstanceBolus::Unload(const SESubstanceBolus& src)
{
  cdm::SubstanceBolusData* dst = new cdm::SubstanceBolusData();
  SESubstanceBolus::Serialize(src, *dst);
  return dst;
}
void SESubstanceBolus::Serialize(const SESubstanceBolus& src, cdm::SubstanceBolusData& dst)
{
  SEPatientAction::Serialize(src, *dst.mutable_patientaction());
  dst.set_substance(src.m_Substance.GetName());
  dst.set_administrationroute((cdm::eSubstanceAdministration_Route)src.m_AdminRoute);
  if(src.HasDose())
    dst.set_allocated_dose(SEScalarVolume::Unload(*src.m_Dose));
  if (src.HasConcentration())
    dst.set_allocated_concentration(SEScalarMassPerVolume::Unload(*src.m_Concentration));
  dst.set_allocated_state(SESubstanceBolusState::Unload(src.m_State));
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

SESubstanceBolusState::SESubstanceBolusState(const SESubstance& sub) : m_Substance(sub)
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

void SESubstanceBolusState::Load(const cdm::SubstanceBolusData_StateData& src, SESubstanceBolusState& dst)
{
  SESubstanceBolusState::Serialize(src, dst);
}
void SESubstanceBolusState::Serialize(const cdm::SubstanceBolusData_StateData& src, SESubstanceBolusState& dst)
{
  if (src.has_elapsedtime())
    SEScalarTime::Load(src.elapsedtime(), dst.GetElapsedTime());
  if (src.has_administereddose())
    SEScalarVolume::Load(src.administereddose(), dst.GetAdministeredDose());
}

cdm::SubstanceBolusData_StateData* SESubstanceBolusState::Unload(const SESubstanceBolusState& src)
{
  cdm::SubstanceBolusData_StateData* dst = new cdm::SubstanceBolusData_StateData();
  SESubstanceBolusState::Serialize(src, *dst);
  return dst;
}

void SESubstanceBolusState::Serialize(const SESubstanceBolusState& src, cdm::SubstanceBolusData_StateData& dst)
{
  dst.set_substance(src.m_Substance.GetName());
  dst.set_allocated_elapsedtime(SEScalarTime::Unload(*src.m_ElapsedTime));
  dst.set_allocated_administereddose(SEScalarVolume::Unload(*src.m_AdministeredDose));
}