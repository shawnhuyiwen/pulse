/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEChestCompressionForceScale.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarTime.h"
#include "io/protobuf/PBPatientActions.h"

SEChestCompressionForceScale::SEChestCompressionForceScale(Logger* logger) : SEPatientAction(logger)
{
  m_ForceScale = nullptr;
  m_ForcePeriod = nullptr;
}

SEChestCompressionForceScale::~SEChestCompressionForceScale()
{
  SAFE_DELETE(m_ForceScale);
  SAFE_DELETE(m_ForcePeriod);
}

void SEChestCompressionForceScale::Clear()
{
  SEPatientAction::Clear();
  INVALIDATE_PROPERTY(m_ForceScale);
  INVALIDATE_PROPERTY(m_ForcePeriod);
}

void SEChestCompressionForceScale::Copy(const SEChestCompressionForceScale& src, bool preserveState)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}

bool SEChestCompressionForceScale::IsValid() const
{
  return SEPatientAction::IsValid() && HasForceScale();
}

bool SEChestCompressionForceScale::IsActive() const
{
  if (!SEPatientAction::IsActive())
    return false;
  return !m_ForceScale->IsZero();
}
void SEChestCompressionForceScale::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEChestCompressionForceScale::GetScalar(const std::string& name)
{
  if (name.compare("ForceScale") == 0)
    return &GetForceScale();
  if (name.compare("ForcePeriod") == 0)
    return &GetForcePeriod();
  return nullptr;
}

bool SEChestCompressionForceScale::HasForceScale() const
{
  return m_ForceScale == nullptr ? false : m_ForceScale->IsValid();
}
SEScalar0To1& SEChestCompressionForceScale::GetForceScale()
{
  if (m_ForceScale == nullptr)
    m_ForceScale = new SEScalar0To1();
  return *m_ForceScale;
}
double SEChestCompressionForceScale::GetForceScale() const
{
  if (m_ForceScale == nullptr)
    return SEScalar::dNaN();
  return m_ForceScale->GetValue();
}

bool SEChestCompressionForceScale::HasForcePeriod() const
{
  return m_ForcePeriod == nullptr ? false : m_ForcePeriod->IsValid();
}
SEScalarTime& SEChestCompressionForceScale::GetForcePeriod()
{
  if (m_ForcePeriod == nullptr)
    m_ForcePeriod = new SEScalarTime();
  return *m_ForcePeriod;
}
double SEChestCompressionForceScale::GetForcePeriod(const TimeUnit& unit) const
{
  if (m_ForcePeriod == nullptr)
    return SEScalar::dNaN();
  return m_ForcePeriod->GetValue(unit);
}

void SEChestCompressionForceScale::ToString(std::ostream &str) const
{
  str << "Patient Action : Chest Compression";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tForceScale: "; HasForceScale() ? str << *m_ForceScale : str << "NaN";
  str << "\n\tForcePeriod: "; HasForcePeriod() ? str << *m_ForcePeriod : str << "NaN";
  str << std::flush;
}