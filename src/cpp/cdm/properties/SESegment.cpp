/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "CommonDataModel.h"
#include "properties/SESegment.h"

#include "properties/SEScalarVolume.h"

SESegment::SESegment()
{
  m_BeginVolume = nullptr;
  m_EndVolume = nullptr;
}

SESegment::~SESegment()
{
  SAFE_DELETE(m_BeginVolume);
  SAFE_DELETE(m_EndVolume);
}

void SESegment::Clear()
{
  INVALIDATE_PROPERTY(m_BeginVolume);
  INVALIDATE_PROPERTY(m_EndVolume);
}

bool SESegment::IsValid() const
{
  return HasBeginVolume() && HasEndVolume();
}

bool SESegment::HasBeginVolume() const
{
  return m_BeginVolume == nullptr ? false : m_BeginVolume->IsValid();
}
SEScalarVolume& SESegment::GetBeginVolume()
{
  if (m_BeginVolume == nullptr)
    m_BeginVolume = new SEScalarVolume();
  return *m_BeginVolume;
}
double SESegment::GetBeginVolume(const VolumeUnit& unit) const
{
  if (m_BeginVolume == nullptr)
    return SEScalar::dNaN();
  return m_BeginVolume->GetValue(unit);
}

bool SESegment::HasEndVolume() const
{
  return m_EndVolume == nullptr ? false : m_EndVolume->IsValid();
}
SEScalarVolume& SESegment::GetEndVolume()
{
  if (m_EndVolume == nullptr)
    m_EndVolume = new SEScalarVolume();
  return *m_EndVolume;
}
double SESegment::GetEndVolume(const VolumeUnit& unit) const
{
  if (m_EndVolume == nullptr)
    return SEScalar::dNaN();
  return m_EndVolume->GetValue(unit);
}

std::string SESegment::ToString() const
{
  std::string str;
  str += "\n\t\t\tBeginVolume: "; HasBeginVolume() ? str += m_BeginVolume->ToString() : str += "Not Set";
  str += "\n\t\t\tEndVolume: "; HasEndVolume() ? str += m_EndVolume->ToString() : str += "Not Set";
  return str;
}
void SESegment::ToString(std::ostream& str) const
{
  str << ToString();
}