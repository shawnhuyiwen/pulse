/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/actions/SEChestCompression.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarForce.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/io/protobuf/PBPatientActions.h"

SEChestCompression::SEChestCompression(Logger* logger) : SEPatientAction(logger)
{
  m_Force = nullptr;
  m_ForceScale = nullptr;
  m_CompressionPeriod = nullptr;
}

SEChestCompression::~SEChestCompression()
{
  SAFE_DELETE(m_Force);
  SAFE_DELETE(m_ForceScale);
  SAFE_DELETE(m_CompressionPeriod);
}

void SEChestCompression::Clear()
{
  SEPatientAction::Clear();
  INVALIDATE_PROPERTY(m_Force);
  INVALIDATE_PROPERTY(m_ForceScale);
  INVALIDATE_PROPERTY(m_CompressionPeriod);
}

void SEChestCompression::Copy(const SEChestCompression& src, bool /*preserveState*/)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}

bool SEChestCompression::IsValid() const
{
  //Need exactly one of force or force scale
  return SEPatientAction::IsValid() && (HasForce() || HasForceScale()) && HasCompressionPeriod();
}

bool SEChestCompression::IsActive() const
{
  if (!SEPatientAction::IsActive())
    return false;
  bool hasForceMode = false;
  if (HasForce() && m_Force->IsPositive())
    hasForceMode = true;
  if (HasForceScale() && m_ForceScale->IsPositive())
    hasForceMode = true;
  if (!hasForceMode)
    return false;
  return m_CompressionPeriod->IsPositive();
}
void SEChestCompression::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEChestCompression::GetScalar(const std::string& name)
{
  if (name.compare("Force") == 0)
    return &GetForce();
  if (name.compare("ForceScale") == 0)
    return &GetForceScale();
  if (name.compare("CompressionPeriod") == 0)
    return &GetCompressionPeriod();
  return nullptr;
}

bool SEChestCompression::HasForce() const
{
  return m_Force == nullptr ? false : m_Force->IsValid();
}
SEScalarForce& SEChestCompression::GetForce()
{
  if (m_Force == nullptr)
    m_Force = new SEScalarForce();
  return *m_Force;
}
double SEChestCompression::GetForce(const ForceUnit& unit) const
{
  if (m_Force == nullptr)
    return SEScalar::dNaN();
  return m_Force->GetValue(unit);
}

bool SEChestCompression::HasForceScale() const
{
  return m_ForceScale == nullptr ? false : m_ForceScale->IsValid();
}
SEScalar0To1& SEChestCompression::GetForceScale()
{
  if (m_ForceScale == nullptr)
    m_ForceScale = new SEScalar0To1();
  return *m_ForceScale;
}
double SEChestCompression::GetForceScale() const
{
  if (m_ForceScale == nullptr)
    return SEScalar::dNaN();
  return m_ForceScale->GetValue();
}

bool SEChestCompression::HasCompressionPeriod() const
{
  return m_CompressionPeriod == nullptr ? false : m_CompressionPeriod->IsValid();
}
SEScalarTime& SEChestCompression::GetCompressionPeriod()
{
  if (m_CompressionPeriod == nullptr)
    m_CompressionPeriod = new SEScalarTime();
  return *m_CompressionPeriod;
}
double SEChestCompression::GetCompressionPeriod(const TimeUnit& unit) const
{
  if (m_CompressionPeriod == nullptr)
    return SEScalar::dNaN();
  return m_CompressionPeriod->GetValue(unit);
}
