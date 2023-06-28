/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/actions/SEChestCompression.h"
#include "cdm/properties/SEScalarForce.h"
#include "cdm/properties/SEScalarLength.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/io/protobuf/PBPatientActions.h"

SEChestCompression::SEChestCompression(Logger* logger) : SEPatientAction(logger)
{
  m_Force = nullptr;
  m_Depth = nullptr;
  m_CompressionPeriod = nullptr;
}

SEChestCompression::~SEChestCompression()
{
  SAFE_DELETE(m_Force);
  SAFE_DELETE(m_Depth);
  SAFE_DELETE(m_CompressionPeriod);
}

void SEChestCompression::Clear()
{
  SEPatientAction::Clear();
  INVALIDATE_PROPERTY(m_Force);
  INVALIDATE_PROPERTY(m_Depth);
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
  return SEPatientAction::IsValid() && (HasForce() || HasDepth()) && HasCompressionPeriod();
}

bool SEChestCompression::IsActive() const
{
  if (!SEPatientAction::IsActive())
    return false;
  bool hasForceMode = false;
  if (HasForce() && m_Force->IsPositive())
    hasForceMode = true;
  if (HasDepth() && m_Depth->IsPositive())
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
  if (name.compare("Depth") == 0)
    return &GetDepth();
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

bool SEChestCompression::HasDepth() const
{
  return m_Depth == nullptr ? false : m_Depth->IsValid();
}
SEScalarLength& SEChestCompression::GetDepth()
{
  if (m_Depth == nullptr)
    m_Depth = new SEScalarLength();
  return *m_Depth;
}
double SEChestCompression::GetDepth(const LengthUnit& unit) const
{
  if (m_Depth == nullptr)
    return SEScalar::dNaN();
  return m_Depth->GetValue(unit);
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
