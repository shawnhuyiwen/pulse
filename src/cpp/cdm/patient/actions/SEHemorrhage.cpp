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
  m_Compartment = "";
  m_FlowRate=nullptr;
  m_Severity = nullptr;
  m_TotalBloodLost = nullptr;
  m_Type = eHemorrhage_Type::External;
}

SEHemorrhage::~SEHemorrhage()
{
  m_Compartment = "";
  SAFE_DELETE(m_FlowRate);
  SAFE_DELETE(m_Severity);
  SAFE_DELETE(m_TotalBloodLost);
  m_Type = eHemorrhage_Type::External;
}

void SEHemorrhage::Clear()
{
  SEPatientAction::Clear();
  m_Compartment = "";
  INVALIDATE_PROPERTY(m_FlowRate);
  INVALIDATE_PROPERTY(m_Severity);
  INVALIDATE_PROPERTY(m_TotalBloodLost);
  m_Type = eHemorrhage_Type::External;
}

void SEHemorrhage::Copy(const SEHemorrhage& src, bool preserveState)
{
  static double v;
  static const VolumeUnit* vu;
  if (preserveState)
  {
    vu = nullptr;
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

eHemorrhage_Type SEHemorrhage::GetType() const
{
  return m_Type;
}
void SEHemorrhage::SetType(eHemorrhage_Type Type)
{
  m_Type = Type;
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

void SEHemorrhage::SetExternal(const ExternalCompartment& c)
{
  SetType(eHemorrhage_Type::External);
  SetCompartment(c.value);
}

void SEHemorrhage::SetInternal(const InternalCompartment& c)
{
  SetType(eHemorrhage_Type::Internal);
  SetCompartment(c.value);
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

void SEHemorrhage::ToString(std::ostream &str) const
{
  str << "Patient Action : Hemorrhage";
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str << "\n\tType: " << eHemorrhage_Type_Name(GetType());
  str << "\n\tFor Compartment: "; HasCompartment()? str << GetCompartment() : str << "No Compartment Set";
  str << "\n\tFlowRate: "; HasFlowRate() ? str << *m_FlowRate : str << "Not Set";
  str << "\n\tSeverity: "; HasSeverity() ? str << *m_Severity : str << "Not Set";
  str << "\n\tTotalBloodLost: "; HasTotalBloodLost() ? str << *m_TotalBloodLost : str << "Not Set";
  str << std::flush;
}

const SEHemorrhage::ExternalCompartment SEHemorrhage::ExternalCompartment::RightLeg("RightLeg");
const SEHemorrhage::ExternalCompartment SEHemorrhage::ExternalCompartment::LeftLeg("LeftLeg");
const SEHemorrhage::ExternalCompartment SEHemorrhage::ExternalCompartment::RightArm("RightArm");
const SEHemorrhage::ExternalCompartment SEHemorrhage::ExternalCompartment::LeftArm("LeftArm");
const SEHemorrhage::ExternalCompartment SEHemorrhage::ExternalCompartment::Skin("Skin");
const SEHemorrhage::ExternalCompartment SEHemorrhage::ExternalCompartment::Muscle("Muscle");
const SEHemorrhage::ExternalCompartment SEHemorrhage::ExternalCompartment::Brain("Brain");
const SEHemorrhage::ExternalCompartment SEHemorrhage::ExternalCompartment::LeftKidney("LeftKidney");
const SEHemorrhage::ExternalCompartment SEHemorrhage::ExternalCompartment::RightKidney("RightKidney");
const SEHemorrhage::ExternalCompartment SEHemorrhage::ExternalCompartment::Liver("Liver");
const SEHemorrhage::ExternalCompartment SEHemorrhage::ExternalCompartment::Spleen("Spleen");
const SEHemorrhage::ExternalCompartment SEHemorrhage::ExternalCompartment::Splanchnic("Splanchnic");
const SEHemorrhage::ExternalCompartment SEHemorrhage::ExternalCompartment::SmallIntestine("SmallIntestine");
const SEHemorrhage::ExternalCompartment SEHemorrhage::ExternalCompartment::LargeIntestine("LargeIntestine");
const SEHemorrhage::ExternalCompartment SEHemorrhage::ExternalCompartment::Aorta("Aorta");
const SEHemorrhage::ExternalCompartment SEHemorrhage::ExternalCompartment::VenaCava("VenaCava");
SEHemorrhage::ExternalCompartment::ExternalCompartment(const std::string& v) : value(v) {}

const SEHemorrhage::InternalCompartment SEHemorrhage::InternalCompartment::LeftKidney("LeftKidney");
const SEHemorrhage::InternalCompartment SEHemorrhage::InternalCompartment::RightKidney("RightKidney");
const SEHemorrhage::InternalCompartment SEHemorrhage::InternalCompartment::Liver("Liver");
const SEHemorrhage::InternalCompartment SEHemorrhage::InternalCompartment::Spleen("Spleen");
const SEHemorrhage::InternalCompartment SEHemorrhage::InternalCompartment::Splanchnic("Splanchnic");
const SEHemorrhage::InternalCompartment SEHemorrhage::InternalCompartment::SmallIntestine("SmallIntestine");
const SEHemorrhage::InternalCompartment SEHemorrhage::InternalCompartment::LargeIntestine("LargeIntestine");
const SEHemorrhage::InternalCompartment SEHemorrhage::InternalCompartment::Aorta("Aorta");
const SEHemorrhage::InternalCompartment SEHemorrhage::InternalCompartment::VenaCava("VenaCava");
SEHemorrhage::InternalCompartment::InternalCompartment(const std::string& v) : value(v) {}
