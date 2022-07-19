/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/actions/SEChestCompressionAutomated.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarForce.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/io/protobuf/PBPatientActions.h"

SEChestCompressionAutomated::SEChestCompressionAutomated(Logger* logger) : SEPatientAction(logger)
{
  m_CompressionFrequency = nullptr;
  m_Force = nullptr;
  m_ForceScale = nullptr;
}

SEChestCompressionAutomated::~SEChestCompressionAutomated()
{
  SAFE_DELETE(m_CompressionFrequency);
  SAFE_DELETE(m_Force);
  SAFE_DELETE(m_ForceScale);
}

void SEChestCompressionAutomated::Clear()
{
  SEPatientAction::Clear();
  INVALIDATE_PROPERTY(m_CompressionFrequency);
  INVALIDATE_PROPERTY(m_Force);
  INVALIDATE_PROPERTY(m_ForceScale);
}

void SEChestCompressionAutomated::Copy(const SEChestCompressionAutomated& src, bool /*preserveState*/)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}

bool SEChestCompressionAutomated::IsValid() const
{
  return SEPatientAction::IsValid() && (HasForce() != HasForceScale()) && HasCompressionFrequency();
}

bool SEChestCompressionAutomated::IsActive() const
{
  if (!SEPatientAction::IsActive())
    return false;
  if (HasForce())
    return !m_Force->IsZero();
  else if(HasForceScale())
    return !m_ForceScale->IsZero();

  return false;
}
void SEChestCompressionAutomated::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
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
  if (name.compare("CompressionFrequency") == 0)
    return &GetCompressionFrequency();
  if (name.compare("Force") == 0)
    return &GetForce();
  if (name.compare("ForceScale") == 0)
    return &GetForceScale();
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

bool SEChestCompressionAutomated::HasForceScale() const
{
  return m_ForceScale == nullptr ? false : m_ForceScale->IsValid();
}
SEScalar0To1& SEChestCompressionAutomated::GetForceScale()
{
  if (m_ForceScale == nullptr)
    m_ForceScale = new SEScalar0To1();
  return *m_ForceScale;
}
double SEChestCompressionAutomated::GetForceScale() const
{
  if (m_ForceScale == nullptr)
    return SEScalar::dNaN();
  
  return m_ForceScale->GetValue();
}

void SEChestCompressionAutomated::ToString(std::ostream &str) const
{
  str << "Patient Action : Automated Chest Compressions";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tCompressionFrequency: "; HasCompressionFrequency() ? str << *m_CompressionFrequency : str << "NaN";
  str << "\n\tForce: "; HasForce() ? str << *m_Force : str << "NaN";
  str << "\n\tForceScale: "; HasForceScale() ? str << *m_ForceScale : str << "NaN";
  str << std::flush;
}