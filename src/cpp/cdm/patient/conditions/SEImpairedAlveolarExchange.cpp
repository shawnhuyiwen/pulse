/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "patient/conditions/SEImpairedAlveolarExchange.h"
#include "properties/SEScalarArea.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBPatientConditions.h"

SEImpairedAlveolarExchange::SEImpairedAlveolarExchange(Logger* logger) : SEPatientCondition(logger)
{
  m_ImpairedSurfaceArea = nullptr;
  m_ImpairedFraction = nullptr;
  m_Severity = nullptr;
}

SEImpairedAlveolarExchange::~SEImpairedAlveolarExchange()
{
  SAFE_DELETE(m_ImpairedSurfaceArea);
  SAFE_DELETE(m_ImpairedFraction);
  SAFE_DELETE(m_Severity);
}

void SEImpairedAlveolarExchange::Clear()
{
  INVALIDATE_PROPERTY(m_ImpairedSurfaceArea);
  INVALIDATE_PROPERTY(m_ImpairedFraction);
  INVALIDATE_PROPERTY(m_Severity);
}

void SEImpairedAlveolarExchange::Copy(const SEImpairedAlveolarExchange& src)
{
  PBPatientCondition::Copy(src, *this);
}

bool SEImpairedAlveolarExchange::IsValid() const
{
  return HasImpairedFraction() || HasImpairedSurfaceArea() || HasSeverity();
}

bool SEImpairedAlveolarExchange::IsActive() const
{
  if (!IsValid())
    return false;
  if (GetImpairedFraction() > 0)
    return true;
  if (m_ImpairedSurfaceArea!=nullptr && m_ImpairedSurfaceArea->IsPositive())
    return true;
  if (HasSeverity())
    return true;
  return false;
}

bool SEImpairedAlveolarExchange::HasImpairedSurfaceArea() const
{
  return m_ImpairedSurfaceArea == nullptr ? false : m_ImpairedSurfaceArea->IsValid();
}
SEScalarArea& SEImpairedAlveolarExchange::GetImpairedSurfaceArea()
{
  if (m_ImpairedSurfaceArea == nullptr)
    m_ImpairedSurfaceArea = new SEScalarArea();
  return *m_ImpairedSurfaceArea;
}
double SEImpairedAlveolarExchange::GetImpairedSurfaceArea(const AreaUnit& unit) const
{
  if (m_ImpairedSurfaceArea == nullptr)
    return SEScalar::dNaN();
  return m_ImpairedSurfaceArea->GetValue(unit);
}

bool SEImpairedAlveolarExchange::HasImpairedFraction() const
{
  return m_ImpairedFraction == nullptr ? false : m_ImpairedFraction->IsValid();
}
SEScalar0To1& SEImpairedAlveolarExchange::GetImpairedFraction()
{
  if (m_ImpairedFraction == nullptr)
    m_ImpairedFraction = new SEScalar0To1();
  return *m_ImpairedFraction;
}
double SEImpairedAlveolarExchange::GetImpairedFraction() const
{
  if (m_ImpairedFraction == nullptr)
    return SEScalar::dNaN();
  return m_ImpairedFraction->GetValue();
}

bool SEImpairedAlveolarExchange::HasSeverity() const
{
  return m_Severity == nullptr ? false : m_Severity->IsValid();
}
SEScalar0To1& SEImpairedAlveolarExchange::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
double SEImpairedAlveolarExchange::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SEImpairedAlveolarExchange::ToString(std::ostream &str) const
{
  str << "Impaired Alveoli Exchange :";
  str << "\n\tSeverity: "; HasSeverity() ? str << *m_Severity : str << "Not Provided";
  str << "\n\tImpairedFraction :"; HasImpairedFraction() ? str << *m_ImpairedFraction : str << "Not Provided";
  str << "\n\tImpairedSurfaceArea :";HasImpairedSurfaceArea() ? str << *m_ImpairedSurfaceArea : str << "Not Provided";

  str << std::flush;
}
