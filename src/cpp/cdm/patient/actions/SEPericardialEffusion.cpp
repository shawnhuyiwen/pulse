/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEPericardialEffusion.h"
#include "properties/SEScalarVolumePerTime.h"
#include "io/protobuf/PBPatientActions.h"

SEPericardialEffusion::SEPericardialEffusion() : SEPatientAction()
{
  m_EffusionRate = nullptr;
}

SEPericardialEffusion::~SEPericardialEffusion()
{
  Clear();
}

void SEPericardialEffusion::Clear()
{
  SEPatientAction::Clear();
  SAFE_DELETE(m_EffusionRate);
}

void SEPericardialEffusion::Copy(const SEPericardialEffusion& src)
{
  PBPatientAction::Copy(src, *this);
}

bool SEPericardialEffusion::IsValid() const
{
  return SEPatientAction::IsValid() && HasEffusionRate();
}

bool SEPericardialEffusion::IsActive() const
{
  return IsValid() ? !m_EffusionRate->IsZero() : false;
}

bool SEPericardialEffusion::HasEffusionRate() const
{
  return m_EffusionRate == nullptr ? false : m_EffusionRate->IsValid();
}
SEScalarVolumePerTime& SEPericardialEffusion::GetEffusionRate()
{
  if (m_EffusionRate == nullptr)
    m_EffusionRate = new SEScalarVolumePerTime();
  return *m_EffusionRate;
}
double SEPericardialEffusion::GetEffusionRate(const VolumePerTimeUnit& unit) const
{
  if (m_EffusionRate == nullptr)
    return SEScalar::dNaN();
  return m_EffusionRate->GetValue(unit);
}

void SEPericardialEffusion::ToString(std::ostream &str) const
{
  str << "Patient Action : Pericardial Effusion"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str << "\n\tEffusion Rate: "; HasEffusionRate() ? str << *m_EffusionRate : str << "NaN";
  str << std::flush;
}