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
  m_TotalBloodVolume = nullptr;
}

SEHemothorax::~SEHemothorax()
{
  m_Side = eSide::NullSide;
  SAFE_DELETE(m_Severity);
  SAFE_DELETE(m_FlowRate);
  SAFE_DELETE(m_TotalBloodVolume);
}

void SEHemothorax::Clear()
{
  SEPatientAction::Clear();
  m_Side = eSide::NullSide;
  INVALIDATE_PROPERTY(m_Severity);
  INVALIDATE_PROPERTY(m_FlowRate);
  INVALIDATE_PROPERTY(m_TotalBloodVolume);
}

void SEHemothorax::Copy(const SEHemothorax& src, bool preserveState)
{
  double v = SEScalar::dNaN();
  const VolumeUnit* vu = nullptr;
  if (preserveState)
  {
    if (HasTotalBloodVolume())
    {
      vu = GetTotalBloodVolume().GetUnit();
      v = GetTotalBloodVolume(*vu);
    }
  }
  PBPatientAction::Copy(src, *this);
  // Put back any state
  if (preserveState && vu != nullptr)
    GetTotalBloodVolume().SetValue(v, *vu);
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
  // Keep the other variables, as they are state
}

const SEScalar* SEHemothorax::GetScalar(const std::string& name)
{
  if (name.compare("Severity") == 0)
    return &GetSeverity();
  if (name.compare("FlowRate") == 0)
    return &GetFlowRate();
  if (name.compare("TotalBloodVolume") == 0)
    return &GetTotalBloodVolume();
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

bool SEHemothorax::HasTotalBloodVolume() const
{
  return m_TotalBloodVolume == nullptr ? false : m_TotalBloodVolume->IsValid();
}
SEScalarVolume& SEHemothorax::GetTotalBloodVolume()
{
  if (m_TotalBloodVolume == nullptr)
    m_TotalBloodVolume = new SEScalarVolume();
  return *m_TotalBloodVolume;
}
double SEHemothorax::GetTotalBloodVolume(const VolumeUnit& unit) const
{
  if (m_TotalBloodVolume == nullptr)
    return SEScalar::dNaN();
  return m_TotalBloodVolume->GetValue(unit);
}
