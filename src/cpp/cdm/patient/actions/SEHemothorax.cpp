/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/actions/SEHemothorax.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/io/protobuf/PBPatientActions.h"

SEHemothorax::SEHemothorax(Logger* logger) : SEPatientAction(logger)
{
  m_Side=eSide::NullSide;
  m_Severity = nullptr;
  m_FlowRate = nullptr;
  m_TargetVolume = nullptr;
  m_BloodVolume = nullptr;
}

SEHemothorax::~SEHemothorax()
{
  m_Side = eSide::NullSide;
  SAFE_DELETE(m_Severity);
  SAFE_DELETE(m_FlowRate);
  SAFE_DELETE(m_TargetVolume);
  SAFE_DELETE(m_BloodVolume);
}

void SEHemothorax::Clear()
{
  SEPatientAction::Clear();
  m_Side = eSide::NullSide;
  INVALIDATE_PROPERTY(m_Severity);
  INVALIDATE_PROPERTY(m_FlowRate);
  INVALIDATE_PROPERTY(m_TargetVolume);
  INVALIDATE_PROPERTY(m_BloodVolume);
}

void SEHemothorax::Copy(const SEHemothorax& src, bool preserveState)
{
  double v = SEScalar::dNaN();
  const VolumeUnit* vu = nullptr;
  if (preserveState)
  {
    if (HasBloodVolume())
    {
      vu = GetBloodVolume().GetUnit();
      v = GetBloodVolume(*vu);
    }
  }
  PBPatientAction::Copy(src, *this);
  // Put back any state
  if (preserveState && vu != nullptr)
    GetBloodVolume().SetValue(v, *vu);
}

bool SEHemothorax::IsValid() const
{
  return SEPatientAction::IsValid() && HasSide() && (HasFlowRate() || HasSeverity());
}

bool SEHemothorax::IsActive() const
{
  if (!SEPatientAction::IsActive())
    return false;
  if (HasSeverity())
    return true;
  if (HasFlowRate())
    return true;
  return false;
}
void SEHemothorax::Deactivate()
{
  SEPatientAction::Deactivate();
  INVALIDATE_PROPERTY(m_FlowRate);
  INVALIDATE_PROPERTY(m_Severity);
  INVALIDATE_PROPERTY(m_TargetVolume);
  // Keep the other variables, as they are state
}

const SEScalar* SEHemothorax::GetScalar(const std::string& name)
{
  if (name.compare("Severity") == 0)
    return &GetSeverity();
  if (name.compare("FlowRate") == 0)
    return &GetFlowRate();
  if (name.compare("TargetVolume") == 0)
    return &GetTargetVolume();
  if (name.compare("BloodVolume") == 0)
    return &GetBloodVolume();
  return nullptr;
}

eSide SEHemothorax::GetSide() const
{
  return m_Side;
}
void SEHemothorax::SetSide(eSide Side)
{
  m_Side = Side;
}
bool SEHemothorax::HasSide() const
{
  return m_Side== eSide::NullSide ?false:true;
}
void SEHemothorax::InvalidateSide()
{
  m_Side = eSide::NullSide;
}

bool SEHemothorax::HasSeverity() const
{
  return m_Severity == nullptr ? false : m_Severity->IsValid();
}
SEScalar0To1& SEHemothorax::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
double SEHemothorax::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

bool SEHemothorax::HasFlowRate() const
{
  return m_FlowRate==nullptr?false:m_FlowRate->IsValid();
}
SEScalarVolumePerTime& SEHemothorax::GetFlowRate()
{
  if(m_FlowRate==nullptr)
    m_FlowRate=new SEScalarVolumePerTime();
  return *m_FlowRate;
}
double SEHemothorax::GetFlowRate(const VolumePerTimeUnit& unit) const
{
  if (m_FlowRate == nullptr)
    return SEScalar::dNaN();
  return m_FlowRate->GetValue(unit);
}

bool SEHemothorax::HasTargetVolume() const
{
  return m_TargetVolume == nullptr ? false : m_TargetVolume->IsValid();
}
SEScalarVolume& SEHemothorax::GetTargetVolume()
{
  if (m_TargetVolume == nullptr)
    m_TargetVolume = new SEScalarVolume();
  return *m_TargetVolume;
}
double SEHemothorax::GetTargetVolume(const VolumeUnit& unit) const
{
  if (m_TargetVolume == nullptr)
    return SEScalar::dNaN();
  return m_TargetVolume->GetValue(unit);
}

bool SEHemothorax::HasBloodVolume() const
{
  return m_BloodVolume == nullptr ? false : m_BloodVolume->IsValid();
}
SEScalarVolume& SEHemothorax::GetBloodVolume()
{
  if (m_BloodVolume == nullptr)
    m_BloodVolume = new SEScalarVolume();
  return *m_BloodVolume;
}
double SEHemothorax::GetBloodVolume(const VolumeUnit& unit) const
{
  if (m_BloodVolume == nullptr)
    return SEScalar::dNaN();
  return m_BloodVolume->GetValue(unit);
}
