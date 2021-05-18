/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "patient/actions/SEImpairedAlveolarExchangeExacerbation.h"
#include "properties/SEScalarArea.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBPatientActions.h"

SEImpairedAlveolarExchangeExacerbation::SEImpairedAlveolarExchangeExacerbation(Logger* logger) : SEPatientAction(logger)
{
  m_ImpairedSurfaceArea = nullptr;
  m_ImpairedFraction = nullptr;
  m_Severity = nullptr;
}

SEImpairedAlveolarExchangeExacerbation::~SEImpairedAlveolarExchangeExacerbation()
{
  SAFE_DELETE(m_ImpairedSurfaceArea);
  SAFE_DELETE(m_ImpairedFraction);
  SAFE_DELETE(m_Severity);
}

void SEImpairedAlveolarExchangeExacerbation::Clear()
{
  INVALIDATE_PROPERTY(m_ImpairedSurfaceArea);
  INVALIDATE_PROPERTY(m_ImpairedFraction);
  INVALIDATE_PROPERTY(m_Severity);
}

void SEImpairedAlveolarExchangeExacerbation::Copy(const SEImpairedAlveolarExchangeExacerbation & src, bool preserveState)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}

bool SEImpairedAlveolarExchangeExacerbation::IsValid() const
{
  return HasImpairedFraction() || HasImpairedSurfaceArea() || HasSeverity();
}

bool SEImpairedAlveolarExchangeExacerbation::IsActive() const
{
  if (!SEPatientAction::IsActive())
    return false;
  if (GetImpairedFraction() > 0)
    return true;
  if (m_ImpairedSurfaceArea!=nullptr && m_ImpairedSurfaceArea->IsPositive())
    return true;
  if (HasSeverity())
    return true;
  return false;
}
void SEImpairedAlveolarExchangeExacerbation::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEImpairedAlveolarExchangeExacerbation::GetScalar(const std::string& name)
{
  if (name.compare("ImpairedSurfaceArea") == 0)
    return &GetImpairedSurfaceArea();
  if (name.compare("ImpairedFraction") == 0)
    return &GetImpairedFraction();
  if (name.compare("Severity") == 0)
    return &GetSeverity();
  return nullptr;
}

bool SEImpairedAlveolarExchangeExacerbation::HasImpairedSurfaceArea() const
{
  return m_ImpairedSurfaceArea == nullptr ? false : m_ImpairedSurfaceArea->IsValid();
}
SEScalarArea& SEImpairedAlveolarExchangeExacerbation::GetImpairedSurfaceArea()
{
  if (m_ImpairedSurfaceArea == nullptr)
    m_ImpairedSurfaceArea = new SEScalarArea();
  return *m_ImpairedSurfaceArea;
}
double SEImpairedAlveolarExchangeExacerbation::GetImpairedSurfaceArea(const AreaUnit& unit) const
{
  if (m_ImpairedSurfaceArea == nullptr)
    return SEScalar::dNaN();
  return m_ImpairedSurfaceArea->GetValue(unit);
}

bool SEImpairedAlveolarExchangeExacerbation::HasImpairedFraction() const
{
  return m_ImpairedFraction == nullptr ? false : m_ImpairedFraction->IsValid();
}
SEScalar0To1& SEImpairedAlveolarExchangeExacerbation::GetImpairedFraction()
{
  if (m_ImpairedFraction == nullptr)
    m_ImpairedFraction = new SEScalar0To1();
  return *m_ImpairedFraction;
}
double SEImpairedAlveolarExchangeExacerbation::GetImpairedFraction() const
{
  if (m_ImpairedFraction == nullptr)
    return SEScalar::dNaN();
  return m_ImpairedFraction->GetValue();
}

bool SEImpairedAlveolarExchangeExacerbation::HasSeverity() const
{
  return m_Severity == nullptr ? false : m_Severity->IsValid();
}
SEScalar0To1& SEImpairedAlveolarExchangeExacerbation::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
double SEImpairedAlveolarExchangeExacerbation::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SEImpairedAlveolarExchangeExacerbation::ToString(std::ostream &str) const
{
  str << "Impaired Alveoli Exchange Exacerbation:";
  str << "\n\tSeverity: "; HasSeverity() ? str << *m_Severity : str << "Not Provided";
  str << "\n\tImpairedFraction :"; HasImpairedFraction() ? str << *m_ImpairedFraction : str << "Not Provided";
  str << "\n\tImpairedSurfaceArea :";HasImpairedSurfaceArea() ? str << *m_ImpairedSurfaceArea : str << "Not Provided";

  str << std::flush;
}
