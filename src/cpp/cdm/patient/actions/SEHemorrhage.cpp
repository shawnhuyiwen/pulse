/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/actions/SEHemorrhage.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/io/protobuf/PBPatientActions.h"

SEHemorrhage::SEHemorrhage(Logger* logger) : SEPatientAction(logger)
{
  m_Type = eHemorrhage_Type::External;
  m_Compartment = eHemorrhage_Compartment::None;
  m_CompartmentName = "";
  m_FlowRate=nullptr;
  m_Severity = nullptr;
  m_TotalBloodLost = nullptr;
}

SEHemorrhage::~SEHemorrhage()
{

  m_Type = eHemorrhage_Type::External;
  m_Compartment = eHemorrhage_Compartment::None;
  m_CompartmentName = "";
  SAFE_DELETE(m_FlowRate);
  SAFE_DELETE(m_Severity);
  SAFE_DELETE(m_TotalBloodLost);
}

void SEHemorrhage::Clear()
{
  SEPatientAction::Clear();
  m_Type = eHemorrhage_Type::External;
  m_Compartment = eHemorrhage_Compartment::None;
  m_CompartmentName = "";
  INVALIDATE_PROPERTY(m_FlowRate);
  INVALIDATE_PROPERTY(m_Severity);
  INVALIDATE_PROPERTY(m_TotalBloodLost);
}

void SEHemorrhage::Copy(const SEHemorrhage& src, bool preserveState)
{
  double v = SEScalar::dNaN();
  const VolumeUnit* vu = nullptr;
  if (preserveState)
  {
    if (HasTotalBloodLost())
    {
      vu = GetTotalBloodLost().GetUnit();
      v = GetTotalBloodLost(*vu);
    }
  }
  PBPatientAction::Copy(src, *this);
  // Put back any state
  if (preserveState && vu != nullptr)
    GetTotalBloodLost().SetValue(v, *vu);
}

bool SEHemorrhage::IsValid() const
{
  return SEPatientAction::IsValid() && HasCompartment() && (HasFlowRate() || HasSeverity());
}

bool SEHemorrhage::IsActive() const
{
  if (!SEPatientAction::IsActive())
    return false;
  if (HasSeverity())
    return true;
  if (HasFlowRate())
    return true;
  return false;
}
void SEHemorrhage::Deactivate()
{
  SEPatientAction::Deactivate();
  INVALIDATE_PROPERTY(m_FlowRate);
  INVALIDATE_PROPERTY(m_Severity);
  // Keep the other variables, as they are state
}

const SEScalar* SEHemorrhage::GetScalar(const std::string& name)
{
  if (name.compare("FlowRate") == 0)
    return &GetFlowRate();
  if (name.compare("Severity") == 0)
    return &GetSeverity();
  if (name.compare("TotalBloodLost") == 0)
    return &GetTotalBloodLost();
  return nullptr;
}

bool SEHemorrhage::HasCompartment() const
{
  return m_Compartment != eHemorrhage_Compartment::None;
}
eHemorrhage_Compartment SEHemorrhage::GetCompartment() const
{
  return m_Compartment;
}
std::string SEHemorrhage::GetCompartmentName() const
{
  return m_CompartmentName;
}
void SEHemorrhage::SetCompartment(eHemorrhage_Compartment cmpt)
{
  m_Compartment = cmpt;
  m_CompartmentName = eHemorrhage_Compartment_Name(cmpt);
}

bool SEHemorrhage::HasFlowRate() const
{
  return m_FlowRate==nullptr?false:m_FlowRate->IsValid();
}
SEScalarVolumePerTime& SEHemorrhage::GetFlowRate()
{
  if(m_FlowRate==nullptr)
    m_FlowRate=new SEScalarVolumePerTime();
  return *m_FlowRate;
}
double SEHemorrhage::GetFlowRate(const VolumePerTimeUnit& unit) const
{
  if (m_FlowRate == nullptr)
    return SEScalar::dNaN();
  return m_FlowRate->GetValue(unit);
}

bool SEHemorrhage::HasSeverity() const
{
  return m_Severity == nullptr ? false : m_Severity->IsValid();
}
SEScalar0To1& SEHemorrhage::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
double SEHemorrhage::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

bool SEHemorrhage::HasTotalBloodLost() const
{
  return m_TotalBloodLost == nullptr ? false : m_TotalBloodLost->IsValid();
}
SEScalarVolume& SEHemorrhage::GetTotalBloodLost()
{
  if (m_TotalBloodLost == nullptr)
    m_TotalBloodLost = new SEScalarVolume();
  return *m_TotalBloodLost;
}
double SEHemorrhage::GetTotalBloodLost(const VolumeUnit& unit) const
{
  if (m_TotalBloodLost == nullptr)
    return SEScalar::dNaN();
  return m_TotalBloodLost->GetValue(unit);
}

bool ValidHemorrhageCompartment(const std::string& cmpt)
{
  if (cmpt == "Aorta") { return true; }
  if (cmpt == "Brain") { return true; }
  if (cmpt == "Muscle") { return true; }
  if (cmpt == "LargeIntestine") { return true; }
  if (cmpt == "LeftArm") { return true; }
  if (cmpt == "LeftKidney") { return true; }
  if (cmpt == "LeftLeg") { return true; }
  if (cmpt == "Liver") { return true; }
  if (cmpt == "RightArm") { return true; }
  if (cmpt == "RightKidney") { return true; }
  if (cmpt == "RightLeg") { return true; }
  if (cmpt == "Skin") { return true; }
  if (cmpt == "SmallIntestine") { return true; }
  if (cmpt == "Splanchnic") { return true; }
  if (cmpt == "Spleen") { return true; }
  if (cmpt == "VenaCava") { return true; }

  return false;
}
