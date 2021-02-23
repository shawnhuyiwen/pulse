/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEChestCompressionForce.h"
#include "properties/SEScalarForce.h"
#include "io/protobuf/PBPatientActions.h"

SEChestCompressionForce::SEChestCompressionForce(Logger* logger) : SEPatientAction(logger)
{
  m_Force = nullptr;
}

SEChestCompressionForce::~SEChestCompressionForce()
{
  SAFE_DELETE(m_Force);
}

void SEChestCompressionForce::Clear()
{
  SEPatientAction::Clear();
  INVALIDATE_PROPERTY(m_Force);
}

void SEChestCompressionForce::Copy(const SEChestCompressionForce& src, bool preserveState)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}

bool SEChestCompressionForce::IsValid() const
{
  return SEPatientAction::IsValid() && HasForce();
}

bool SEChestCompressionForce::IsActive() const
{
  if (!SEPatientAction::IsActive())
    return false;
  return !m_Force->IsZero();
}
void SEChestCompressionForce::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEChestCompressionForce::GetScalar(const std::string& name)
{
  if (name.compare("Force") == 0)
    return &GetForce();
  return nullptr;
}

bool SEChestCompressionForce::HasForce() const
{
  return m_Force == nullptr ? false : m_Force->IsValid();
}
SEScalarForce& SEChestCompressionForce::GetForce()
{
  if (m_Force == nullptr)
    m_Force = new SEScalarForce();
  return *m_Force;
}
double SEChestCompressionForce::GetForce(const ForceUnit& unit) const
{
  if (m_Force == nullptr)
    return SEScalar::dNaN();
  return m_Force->GetValue(unit);
}

void SEChestCompressionForce::ToString(std::ostream &str) const
{
  str << "Patient Action : Chest Compression";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tForce: "; HasForce() ? str << *m_Force : str << "NaN";
  str << std::flush;
}