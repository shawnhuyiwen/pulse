/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDataModel.h"
#include "cdm/properties/SESegmentParabolic.h"

#include "cdm/properties/SEScalar.h"

SESegmentParabolic::SESegmentParabolic()
{
  m_Coefficient1 = nullptr;
  m_Coefficient2 = nullptr;
  m_Coefficient3 = nullptr;
  m_Coefficient4 = nullptr;
}

SESegmentParabolic::~SESegmentParabolic()
{
  SAFE_DELETE(m_Coefficient1);
  SAFE_DELETE(m_Coefficient2);
  SAFE_DELETE(m_Coefficient3);
  SAFE_DELETE(m_Coefficient4);
}

void SESegmentParabolic::Clear()
{
  SESegment::Clear();
  INVALIDATE_PROPERTY(m_Coefficient1);
  INVALIDATE_PROPERTY(m_Coefficient2);
  INVALIDATE_PROPERTY(m_Coefficient3);
  INVALIDATE_PROPERTY(m_Coefficient4);
}

bool SESegmentParabolic::IsValid() const
{
  return SESegment::IsValid() &&
    HasCoefficient1() && HasCoefficient2() &&
    HasCoefficient3() && HasCoefficient4();
}

bool SESegmentParabolic::HasCoefficient1() const
{
  return m_Coefficient1 == nullptr ? false : m_Coefficient1->IsValid();
}
SEScalar& SESegmentParabolic::GetCoefficient1()
{
  if (m_Coefficient1 == nullptr)
    m_Coefficient1 = new SEScalar();
  return *m_Coefficient1;
}
double SESegmentParabolic::GetCoefficient1() const
{
  if (m_Coefficient1 == nullptr)
    return SEScalar::dNaN();
  return m_Coefficient1->GetValue();
}

bool SESegmentParabolic::HasCoefficient2() const
{
  return m_Coefficient2 == nullptr ? false : m_Coefficient2->IsValid();
}
SEScalar& SESegmentParabolic::GetCoefficient2()
{
  if (m_Coefficient2 == nullptr)
    m_Coefficient2 = new SEScalar();
  return *m_Coefficient2;
}
double SESegmentParabolic::GetCoefficient2() const
{
  if (m_Coefficient2 == nullptr)
    return SEScalar::dNaN();
  return m_Coefficient2->GetValue();
}

bool SESegmentParabolic::HasCoefficient3() const
{
  return m_Coefficient3 == nullptr ? false : m_Coefficient3->IsValid();
}
SEScalar& SESegmentParabolic::GetCoefficient3()
{
  if (m_Coefficient3 == nullptr)
    m_Coefficient3 = new SEScalar();
  return *m_Coefficient3;
}
double SESegmentParabolic::GetCoefficient3() const
{
  if (m_Coefficient3 == nullptr)
    return SEScalar::dNaN();
  return m_Coefficient3->GetValue();
}

bool SESegmentParabolic::HasCoefficient4() const
{
  return m_Coefficient4 == nullptr ? false : m_Coefficient4->IsValid();
}
SEScalar& SESegmentParabolic::GetCoefficient4()
{
  if (m_Coefficient4 == nullptr)
    m_Coefficient4 = new SEScalar();
  return *m_Coefficient4;
}
double SESegmentParabolic::GetCoefficient4() const
{
  if (m_Coefficient4 == nullptr)
    return SEScalar::dNaN();
  return m_Coefficient4->GetValue();
}

std::string SESegmentParabolic::ToString() const
{
  std::string str = "\n\t\tParabolic Segment" + SESegment::ToString();
  str += "\n\t\t\tCoefficient1: "; HasCoefficient1() ? str += m_Coefficient1->ToString() : str += "Not Set";
  str += "\n\t\t\tCoefficient2: "; HasCoefficient2() ? str += m_Coefficient2->ToString() : str += "Not Set";
  str += "\n\t\t\tCoefficient3: "; HasCoefficient3() ? str += m_Coefficient3->ToString() : str += "Not Set";
  str += "\n\t\t\tCoefficient4: "; HasCoefficient4() ? str += m_Coefficient4->ToString() : str += "Not Set";
  return str;
}
void SESegmentParabolic::ToString(std::ostream& str) const
{
  str << ToString();
}
