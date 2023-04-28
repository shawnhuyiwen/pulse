/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/actions/SETubeThoracostomy.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/io/protobuf/PBPatientActions.h"

SETubeThoracostomy::SETubeThoracostomy(Logger* logger) : SEPatientAction(logger)
{
  m_Side = eSide::NullSide;
  m_FlowRate = nullptr;
}

SETubeThoracostomy::~SETubeThoracostomy()
{
  m_Side = eSide::NullSide;
  SAFE_DELETE(m_FlowRate);
}

void SETubeThoracostomy::Clear()
{
  SEPatientAction::Clear();
  m_Side= eSide::NullSide;
  INVALIDATE_PROPERTY(m_FlowRate);
}

void SETubeThoracostomy::Copy(const SETubeThoracostomy& src, bool /*preserveState*/)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}

bool SETubeThoracostomy::IsValid() const
{
  return SEPatientAction::IsValid() && HasSide();
}

bool SETubeThoracostomy::IsActive() const
{
  if (!SEPatientAction::IsActive())
    return false;
  if (HasFlowRate())
    return true;
  return false;
}
void SETubeThoracostomy::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SETubeThoracostomy::GetScalar(const std::string& name)
{
  if (name.compare("FlowRate") == 0)
    return &GetFlowRate();
  return nullptr;
}

eSide SETubeThoracostomy::GetSide() const
{
  return m_Side;
}
void SETubeThoracostomy::SetSide(eSide Side)
{
  m_Side = Side;
}
bool SETubeThoracostomy::HasSide() const
{
  return m_Side== eSide::NullSide ?false:true;
}
void SETubeThoracostomy::InvalidateSide()
{
  m_Side = eSide::NullSide;
}

bool SETubeThoracostomy::HasFlowRate() const
{
  return m_FlowRate==nullptr?false:m_FlowRate->IsValid();
}
SEScalarVolumePerTime& SETubeThoracostomy::GetFlowRate()
{
  if(m_FlowRate==nullptr)
    m_FlowRate=new SEScalarVolumePerTime();
  return *m_FlowRate;
}
double SETubeThoracostomy::GetFlowRate(const VolumePerTimeUnit& unit) const
{
  if (m_FlowRate == nullptr)
    return SEScalar::dNaN();
  return m_FlowRate->GetValue(unit);
}
