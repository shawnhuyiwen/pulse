/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/actions/SEChestCompressionInstantaneous.h"
#include "cdm/properties/SEScalarForce.h"
#include "cdm/properties/SEScalarLength.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/io/protobuf/PBPatientActions.h"

SEChestCompressionInstantaneous::SEChestCompressionInstantaneous(Logger* logger) : SEPatientAction(logger)
{
  m_Force = nullptr;
  m_Depth = nullptr;
}

SEChestCompressionInstantaneous::~SEChestCompressionInstantaneous()
{
  SAFE_DELETE(m_Force);
  SAFE_DELETE(m_Depth);
}

void SEChestCompressionInstantaneous::Clear()
{
  SEPatientAction::Clear();
  INVALIDATE_PROPERTY(m_Force);
  INVALIDATE_PROPERTY(m_Depth);
}

void SEChestCompressionInstantaneous::Copy(const SEChestCompressionInstantaneous& src, bool /*preserveState*/)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}

bool SEChestCompressionInstantaneous::IsValid() const
{
  //Need exactly one of force or force scale
  return SEPatientAction::IsValid() && (HasForce() || HasDepth());
}

bool SEChestCompressionInstantaneous::IsActive() const
{
  if (!SEPatientAction::IsActive())
    return false;
  bool hasForceMode = false;
  if (HasForce() && m_Force->IsPositive())
    hasForceMode = true;
  if (HasDepth() && m_Depth->IsPositive())
    hasForceMode = true;
  return hasForceMode;
}
void SEChestCompressionInstantaneous::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEChestCompressionInstantaneous::GetScalar(const std::string& name)
{
  if (name.compare("Force") == 0)
    return &GetForce();
  if (name.compare("Depth") == 0)
    return &GetDepth();
  return nullptr;
}

bool SEChestCompressionInstantaneous::HasForce() const
{
  return m_Force == nullptr ? false : m_Force->IsValid();
}
SEScalarForce& SEChestCompressionInstantaneous::GetForce()
{
  if (m_Force == nullptr)
    m_Force = new SEScalarForce();
  return *m_Force;
}
double SEChestCompressionInstantaneous::GetForce(const ForceUnit& unit) const
{
  if (m_Force == nullptr)
    return SEScalar::dNaN();
  return m_Force->GetValue(unit);
}

bool SEChestCompressionInstantaneous::HasDepth() const
{
  return m_Depth == nullptr ? false : m_Depth->IsValid();
}
SEScalarLength& SEChestCompressionInstantaneous::GetDepth()
{
  if (m_Depth == nullptr)
    m_Depth = new SEScalarLength();
  return *m_Depth;
}
double SEChestCompressionInstantaneous::GetDepth(const LengthUnit& unit) const
{
  if (m_Depth == nullptr)
    return SEScalar::dNaN();
  return m_Depth->GetValue(unit);
}
