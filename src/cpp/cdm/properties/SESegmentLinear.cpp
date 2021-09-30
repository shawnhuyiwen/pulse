/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SESegmentLinear.h"

#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarPressurePerVolume.h"

SESegmentLinear::SESegmentLinear()
{
  m_Slope = nullptr;
  m_YIntercept = nullptr;
}

SESegmentLinear::~SESegmentLinear()
{
  SAFE_DELETE(m_Slope);
  SAFE_DELETE(m_YIntercept);
}

void SESegmentLinear::Clear()
{
  SESegment::Clear();
  INVALIDATE_PROPERTY(m_Slope);
  INVALIDATE_PROPERTY(m_YIntercept);
}

bool SESegmentLinear::IsValid() const
{
  return SESegment::IsValid() && HasSlope() && HasYIntercept();
}

bool SESegmentLinear::HasSlope() const
{
  return m_Slope == nullptr ? false : m_Slope->IsValid();
}
SEScalarPressurePerVolume& SESegmentLinear::GetSlope()
{
  if (m_Slope == nullptr)
    m_Slope = new SEScalarPressurePerVolume();
  return *m_Slope;
}
double SESegmentLinear::GetSlope(const PressurePerVolumeUnit& unit) const
{
  if (m_Slope == nullptr)
    return SEScalar::dNaN();
  return m_Slope->GetValue(unit);
}

bool SESegmentLinear::HasYIntercept() const
{
  return m_YIntercept == nullptr ? false : m_YIntercept->IsValid();
}
SEScalarPressure& SESegmentLinear::GetYIntercept()
{
  if (m_YIntercept == nullptr)
    m_YIntercept = new SEScalarPressure();
  return *m_YIntercept;
}
double SESegmentLinear::GetYIntercept(const PressureUnit& unit) const
{
  if (m_YIntercept == nullptr)
    return SEScalar::dNaN();
  return m_YIntercept->GetValue(unit);
}

std::string SESegmentLinear::ToString() const
{
  std::string str = "\n\t\tLinear Segment" + SESegment::ToString();
  str += "\n\t\t\tSlope: "; HasSlope() ? str += m_Slope->ToString() : str += "Not Set";
  str += "\n\t\t\tYIntercept: "; HasYIntercept() ? str += m_YIntercept->ToString() : str += "Not Set";
  return str;
}
void SESegmentLinear::ToString(std::ostream& str) const
{
  str << ToString();
}
