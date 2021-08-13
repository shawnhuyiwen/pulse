/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "CommonDataModel.h"
#include "properties/SESegmentConstant.h"

#include "properties/SEScalarVolumePerPressure.h"

SESegmentConstant::SESegmentConstant()
{
  m_Compliance = nullptr;
}

SESegmentConstant::~SESegmentConstant()
{
  SAFE_DELETE(m_Compliance);
}

void SESegmentConstant::Clear()
{
  SESegment::Clear();
  INVALIDATE_PROPERTY(m_Compliance);
}

bool SESegmentConstant::IsValid() const
{
  return SESegment::IsValid() && HasCompliance();
}

bool SESegmentConstant::HasCompliance() const
{
  return m_Compliance == nullptr ? false : m_Compliance->IsValid();
}
SEScalarVolumePerPressure& SESegmentConstant::GetCompliance()
{
  if (m_Compliance == nullptr)
    m_Compliance = new SEScalarVolumePerPressure();
  return *m_Compliance;
}
double SESegmentConstant::GetCompliance(const VolumePerPressureUnit& unit) const
{
  if (m_Compliance == nullptr)
    return SEScalar::dNaN();
  return m_Compliance->GetValue(unit);
}

std::string SESegmentConstant::ToString() const
{
  std::string str = "\n\t\tConstant Segment" + SESegment::ToString();
  str += "\n\t\t\tCompliance: "; HasCompliance() ? str += m_Compliance->ToString() : str += "Not Set";
  return str;
}
void SESegmentConstant::ToString(std::ostream& str) const
{
  str << ToString();
}
