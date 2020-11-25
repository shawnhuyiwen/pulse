/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/conditions/SEChronicPericardialEffusion.h"
#include "properties/SEScalarVolume.h"
#include "io/protobuf/PBPatientConditions.h"

SEChronicPericardialEffusion::SEChronicPericardialEffusion(Logger* logger) : SEPatientCondition(logger)
{
  m_AccumulatedVolume = nullptr;
}

SEChronicPericardialEffusion::~SEChronicPericardialEffusion()
{
  Clear();
  SAFE_DELETE(m_AccumulatedVolume);
}

void SEChronicPericardialEffusion::Clear()
{
  SEPatientCondition::Clear();
  INVALIDATE_PROPERTY(m_AccumulatedVolume);
}

void SEChronicPericardialEffusion::Copy(const SEChronicPericardialEffusion& src)
{
  PBPatientCondition::Copy(src, *this);
}

bool SEChronicPericardialEffusion::IsValid() const
{
  return HasAccumulatedVolume();
}
bool SEChronicPericardialEffusion::IsActive() const
{
  if (!IsValid())
    return false;
  return (m_AccumulatedVolume != nullptr && m_AccumulatedVolume->IsPositive());
}

bool SEChronicPericardialEffusion::HasAccumulatedVolume() const
{
  return m_AccumulatedVolume == nullptr ? false : m_AccumulatedVolume->IsValid();
}
SEScalarVolume& SEChronicPericardialEffusion::GetAccumulatedVolume()
{
  if (m_AccumulatedVolume == nullptr)
    m_AccumulatedVolume = new SEScalarVolume();
  return *m_AccumulatedVolume;
}
double SEChronicPericardialEffusion::GetAccumulatedVolume(const VolumeUnit& unit) const
{
  if (m_AccumulatedVolume == nullptr)
    return SEScalar::dNaN();
  return m_AccumulatedVolume->GetValue(unit);
}


void SEChronicPericardialEffusion::ToString(std::ostream &str) const
{
  str << "Patient Condition : Pericardial Effusion";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tAccumulatedVolume: "; HasAccumulatedVolume() ? str << *m_AccumulatedVolume : str << "NaN";
  str << std::flush;
}