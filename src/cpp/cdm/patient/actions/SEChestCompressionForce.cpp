/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEChestCompressionForce.h"
#include "properties/SEScalarForce.h"
#include "io/protobuf/PBPatientActions.h"

SEChestCompressionForce::SEChestCompressionForce(Logger* logger) : SEChestCompression(logger)
{
  m_Force = nullptr;
}

SEChestCompressionForce::~SEChestCompressionForce()
{
  Clear();
}

void SEChestCompressionForce::Clear()
{
  SEChestCompression::Clear();
  SAFE_DELETE(m_Force);
}

void SEChestCompressionForce::Copy(const SEChestCompressionForce& src)
{
  PBPatientAction::Copy(src, *this);
}

bool SEChestCompressionForce::IsValid() const
{
  return SEChestCompression::IsValid() && HasForce();
}

bool SEChestCompressionForce::IsActive() const
{
  return IsValid() ? !m_Force->IsZero() : false;
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