/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "properties/SESegmentSigmoidal.h"

#include "properties/SEScalarPressure.h"
#include "properties/SEScalarPressureTimePerVolume.h"

SESegmentSigmoidal::SESegmentSigmoidal()
{
  m_LowerCorner = nullptr;
  m_UpperCorner = nullptr;
  m_BaselineCompliance = nullptr;
}

SESegmentSigmoidal::~SESegmentSigmoidal()
{
  SAFE_DELETE(m_LowerCorner);
  SAFE_DELETE(m_UpperCorner);
  SAFE_DELETE(m_BaselineCompliance);
}

void SESegmentSigmoidal::Clear()
{
  SESegment::Clear();
  INVALIDATE_PROPERTY(m_LowerCorner);
  INVALIDATE_PROPERTY(m_UpperCorner);
  INVALIDATE_PROPERTY(m_BaselineCompliance);
}

bool SESegmentSigmoidal::IsValid() const
{
  return SESegment::IsValid() &&
    HasLowerCorner() && HasUpperCorner() && HasBaselineCompliance();
}

bool SESegmentSigmoidal::HasLowerCorner() const
{
  return m_LowerCorner == nullptr ? false : m_LowerCorner->IsValid();
}
SEScalarPressure& SESegmentSigmoidal::GetLowerCorner()
{
  if (m_LowerCorner == nullptr)
    m_LowerCorner = new SEScalarPressure();
  return *m_LowerCorner;
}
double SESegmentSigmoidal::GetLowerCorner(const PressureUnit& unit) const
{
  if (m_LowerCorner == nullptr)
    return SEScalar::dNaN();
  return m_LowerCorner->GetValue(unit);
}

bool SESegmentSigmoidal::HasUpperCorner() const
{
  return m_UpperCorner == nullptr ? false : m_UpperCorner->IsValid();
}
SEScalarPressure& SESegmentSigmoidal::GetUpperCorner()
{
  if (m_UpperCorner == nullptr)
    m_UpperCorner = new SEScalarPressure();
  return *m_UpperCorner;
}
double SESegmentSigmoidal::GetUpperCorner(const PressureUnit& unit) const
{
  if (m_UpperCorner == nullptr)
    return SEScalar::dNaN();
  return m_UpperCorner->GetValue(unit);
}

bool SESegmentSigmoidal::HasBaselineCompliance() const
{
  return m_BaselineCompliance == nullptr ? false : m_BaselineCompliance->IsValid();
}
SEScalarPressureTimePerVolume& SESegmentSigmoidal::GetBaselineCompliance()
{
  if (m_BaselineCompliance == nullptr)
    m_BaselineCompliance = new SEScalarPressureTimePerVolume();
  return *m_BaselineCompliance;
}
double SESegmentSigmoidal::GetBaselineCompliance(const PressureTimePerVolumeUnit& unit) const
{
  if (m_BaselineCompliance == nullptr)
    return SEScalar::dNaN();
  return m_BaselineCompliance->GetValue(unit);
}

std::string SESegmentSigmoidal::ToString() const
{
  std::string str = "\n\t\tSigmoidal Segment" + SESegment::ToString();
  str += "\n\t\t\tLowerCorner: "; HasLowerCorner() ? str += m_LowerCorner->ToString() : str += "Not Set";
  str += "\n\t\t\tUpperCorner: "; HasUpperCorner() ? str += m_UpperCorner->ToString() : str += "Not Set";
  str += "\n\t\t\tBaselineCompliance: "; HasBaselineCompliance() ? str += m_BaselineCompliance->ToString() : str += "Not Set";
  return str;
}
void SESegmentSigmoidal::ToString(std::ostream& str) const
{
  str << ToString();
}
