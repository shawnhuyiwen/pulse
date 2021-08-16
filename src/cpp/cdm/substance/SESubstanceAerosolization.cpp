/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/substance/SESubstanceAerosolization.h"
#include "cdm/properties/SEScalarLength.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEHistogramFractionVsLength.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarNegative1To1.h"

SESubstanceAerosolization::SESubstanceAerosolization(Logger* logger) : Loggable(logger)
{
  m_BronchioleModifier = nullptr;
  m_InflammationCoefficient = nullptr;
  m_ParticulateSizeDistribution = nullptr;
}

SESubstanceAerosolization::~SESubstanceAerosolization()
{
  SAFE_DELETE(m_BronchioleModifier);
  SAFE_DELETE(m_InflammationCoefficient);
  SAFE_DELETE(m_ParticulateSizeDistribution);
}

void SESubstanceAerosolization::Clear()
{
  INVALIDATE_PROPERTY(m_BronchioleModifier);
  INVALIDATE_PROPERTY(m_InflammationCoefficient);
  INVALIDATE_PROPERTY(m_ParticulateSizeDistribution);
}

bool SESubstanceAerosolization::IsValid() const
{
  if (!HasBronchioleModifier())
    return false;
  if (!HasInflammationCoefficient())
    return false;
  if (!HasParticulateSizeDistribution())
    return false;
  return true;
}

const SEScalar* SESubstanceAerosolization::GetScalar(const std::string& name)
{
  if (name.compare("BronchioleModifier") == 0)
    return &GetBronchioleModifier();
  if (name.compare("InflammationCoefficient") == 0)
    return &GetInflammationCoefficient();
  return nullptr;
}


bool SESubstanceAerosolization::HasBronchioleModifier() const
{
  return (m_BronchioleModifier == nullptr) ? false : m_BronchioleModifier->IsValid();
}
SEScalarNegative1To1& SESubstanceAerosolization::GetBronchioleModifier()
{
  if (m_BronchioleModifier == nullptr)
    m_BronchioleModifier = new SEScalarNegative1To1();
  return *m_BronchioleModifier;
}
double SESubstanceAerosolization::GetBronchioleModifier() const
{
  if (m_BronchioleModifier == nullptr)
    return SEScalar::dNaN();
  return m_BronchioleModifier->GetValue();
}

bool SESubstanceAerosolization::HasInflammationCoefficient() const
{
  return (m_InflammationCoefficient == nullptr) ? false : m_InflammationCoefficient->IsValid();
}
SEScalar0To1& SESubstanceAerosolization::GetInflammationCoefficient()
{
  if (m_InflammationCoefficient == nullptr)
    m_InflammationCoefficient = new SEScalar0To1();
  return *m_InflammationCoefficient;
}
double SESubstanceAerosolization::GetInflammationCoefficient() const
{
  if (m_InflammationCoefficient == nullptr)
    return SEScalar::dNaN();
  return m_InflammationCoefficient->GetValue();
}

bool SESubstanceAerosolization::HasParticulateSizeDistribution() const
{
  return (m_ParticulateSizeDistribution == nullptr) ? false : m_ParticulateSizeDistribution->IsValid();
}
SEHistogramFractionVsLength& SESubstanceAerosolization::GetParticulateSizeDistribution()
{
  if (m_ParticulateSizeDistribution == nullptr)
    m_ParticulateSizeDistribution = new SEHistogramFractionVsLength();
  return *m_ParticulateSizeDistribution;
}
const SEHistogramFractionVsLength* SESubstanceAerosolization::GetParticulateSizeDistribution() const
{
  return m_ParticulateSizeDistribution;
}
