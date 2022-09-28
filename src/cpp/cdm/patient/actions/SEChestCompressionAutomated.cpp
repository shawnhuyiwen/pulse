/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/actions/SEChestCompressionAutomated.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarForce.h"
#include "cdm/properties/SEScalarLength.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/io/protobuf/PBPatientActions.h"

SEChestCompressionAutomated::SEChestCompressionAutomated(Logger* logger) : SEPatientAction(logger)
{
  m_AppliedForceFraction = nullptr;
  m_CompressionFrequency = nullptr;
  m_Force = nullptr;
  m_Depth = nullptr;
}

SEChestCompressionAutomated::~SEChestCompressionAutomated()
{
  SAFE_DELETE(m_AppliedForceFraction);
  SAFE_DELETE(m_CompressionFrequency);
  SAFE_DELETE(m_Force);
  SAFE_DELETE(m_Depth);
}

void SEChestCompressionAutomated::Clear()
{
  SEPatientAction::Clear();
  INVALIDATE_PROPERTY(m_AppliedForceFraction);
  INVALIDATE_PROPERTY(m_CompressionFrequency);
  INVALIDATE_PROPERTY(m_Force);
  INVALIDATE_PROPERTY(m_Depth);
}

void SEChestCompressionAutomated::Copy(const SEChestCompressionAutomated& src, bool /*preserveState*/)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}

bool SEChestCompressionAutomated::IsValid() const
{
  return SEPatientAction::IsValid() && (HasForce() || HasDepth()) && HasCompressionFrequency();
}

bool SEChestCompressionAutomated::IsActive() const
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
  return m_CompressionFrequency->IsPositive();
}
void SEChestCompressionAutomated::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

bool SEChestCompressionAutomated::HasAppliedForceFraction() const
{
  return m_AppliedForceFraction == nullptr ? false : m_AppliedForceFraction->IsPositive();
}
SEScalar0To1& SEChestCompressionAutomated::GetAppliedForceFraction()
{
  if (m_AppliedForceFraction == nullptr)
    m_AppliedForceFraction = new SEScalar0To1();
  return *m_AppliedForceFraction;
}
double SEChestCompressionAutomated::GetAppliedForceFraction() const
{
  if (m_AppliedForceFraction == nullptr)
    return SEScalar::dNaN();

  return m_AppliedForceFraction->GetValue();
}

bool SEChestCompressionAutomated::HasCompressionFrequency() const
{
  return m_CompressionFrequency == nullptr ? false : m_CompressionFrequency->IsValid();
}
SEScalarFrequency& SEChestCompressionAutomated::GetCompressionFrequency()
{
  if (m_CompressionFrequency == nullptr)
    m_CompressionFrequency = new SEScalarFrequency();
  return *m_CompressionFrequency;
}
double SEChestCompressionAutomated::GetCompressionFrequency(const FrequencyUnit& unit) const
{
  if (m_CompressionFrequency == nullptr)
    return SEScalar::dNaN();
  return m_CompressionFrequency->GetValue(unit);
}

const SEScalar* SEChestCompressionAutomated::GetScalar(const std::string& name)
{
  if (name.compare("AppliedForceFraction") == 0)
    return &GetAppliedForceFraction();
  if (name.compare("CompressionFrequency") == 0)
    return &GetCompressionFrequency();
  if (name.compare("Force") == 0)
    return &GetForce();
  if (name.compare("Depth") == 0)
    return &GetDepth();
  return nullptr;
}

bool SEChestCompressionAutomated::HasForce() const
{
  return m_Force == nullptr ? false : m_Force->IsValid();
}
SEScalarForce& SEChestCompressionAutomated::GetForce()
{
  if (m_Force == nullptr)
    m_Force = new SEScalarForce();
  return *m_Force;
}
double SEChestCompressionAutomated::GetForce(const ForceUnit& unit) const
{
  if (m_Force == nullptr)
    return SEScalar::dNaN();
  return m_Force->GetValue(unit);
}

bool SEChestCompressionAutomated::HasDepth() const
{
  return m_Depth == nullptr ? false : m_Depth->IsValid();
}
SEScalarLength& SEChestCompressionAutomated::GetDepth()
{
  if (m_Depth == nullptr)
    m_Depth = new SEScalarLength();
  return *m_Depth;
}
double SEChestCompressionAutomated::GetDepth(const LengthUnit& unit) const
{
  if (m_Depth == nullptr)
    return SEScalar::dNaN();
  return m_Depth->GetValue(unit);
}
