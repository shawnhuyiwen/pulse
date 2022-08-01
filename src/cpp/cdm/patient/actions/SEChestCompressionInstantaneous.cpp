/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/actions/SEChestCompressionInstantaneous.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarForce.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/io/protobuf/PBPatientActions.h"

SEChestCompressionInstantaneous::SEChestCompressionInstantaneous(Logger* logger) : SEPatientAction(logger)
{
  m_Force = nullptr;
  m_ForceScale = nullptr;
}

SEChestCompressionInstantaneous::~SEChestCompressionInstantaneous()
{
  SAFE_DELETE(m_Force);
  SAFE_DELETE(m_ForceScale);
}

void SEChestCompressionInstantaneous::Clear()
{
  SEPatientAction::Clear();
  INVALIDATE_PROPERTY(m_Force);
  INVALIDATE_PROPERTY(m_ForceScale);
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
  return SEPatientAction::IsValid() && (HasForce() || HasForceScale());
}

bool SEChestCompressionInstantaneous::IsActive() const
{
  if (!SEPatientAction::IsActive())
    return false;
  bool hasForceMode = false;
  if (HasForce() && m_Force->IsPositive())
    hasForceMode = true;
  if (HasForceScale() && m_ForceScale->IsPositive())
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
  if (name.compare("ForceScale") == 0)
    return &GetForceScale();
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

bool SEChestCompressionInstantaneous::HasForceScale() const
{
  return m_ForceScale == nullptr ? false : m_ForceScale->IsValid();
}
SEScalar0To1& SEChestCompressionInstantaneous::GetForceScale()
{
  if (m_ForceScale == nullptr)
    m_ForceScale = new SEScalar0To1();
  return *m_ForceScale;
}
double SEChestCompressionInstantaneous::GetForceScale() const
{
  if (m_ForceScale == nullptr)
    return SEScalar::dNaN();
  return m_ForceScale->GetValue();
}

void SEChestCompressionInstantaneous::ToString(std::ostream &str) const
{
  str << "Patient Action : Chest Compression Instantaneous";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tForce: "; HasForce() ? str << *m_Force : str << "NaN";
  str << "\n\tForceScale: "; HasForceScale() ? str << *m_ForceScale : str << "NaN";
  str << std::flush;
}