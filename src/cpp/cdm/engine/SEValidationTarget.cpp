/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include <numeric>

#include "SEValidationTarget.h"

#include "cdm/utils/GeneralMath.h"

SEValidationTarget::SEValidationTarget(const SEValidationTarget& vt) : SEDataRequest(vt)
{
  m_Type = vt.m_Type;
  m_RangeMin = vt.m_RangeMin;
  m_RangeMax = vt.m_RangeMax;
  m_TypeValue = vt.m_TypeValue;
  m_Error = vt.m_Error;
}

SEValidationTarget::SEValidationTarget(eValidationTargetType t, eDataRequest_Category category, const SEDecimalFormat* dfault) : SEDataRequest(category,dfault)
{
  m_Type = t;
}

void SEValidationTarget::Clear()
{
  m_RangeMin = SEScalar::dNaN();
  m_RangeMax = SEScalar::dNaN();
  m_TypeValue = SEScalar::dNaN();
  m_Error = SEScalar::dNaN();
  m_Data.clear();
}

eValidationTargetType SEValidationTarget::GetType() const
{
  return m_Type;
}

double SEValidationTarget::GetRangeMin() const
{
  return m_RangeMin;
}
void SEValidationTarget::SetRangeMin(double d)
{
  m_RangeMin = d;
}

double SEValidationTarget::GetRangeMax() const
{
  return m_RangeMax;
}
void SEValidationTarget::SetRangeMax(double d)
{
  m_RangeMax = d;
}

std::vector<double>& SEValidationTarget::GetData()
{
  return m_Data;
}

double SEValidationTarget::GetError() const
{
  return m_Error;
}
bool SEValidationTarget::ComputeError()
{
  m_TypeValue = SEScalar::dNaN();
  switch (m_Type)
  {
  case eValidationTargetType::Min:
  {
    m_TypeValue = *std::min_element(m_Data.begin(), m_Data.end());
    break;
  }
  case eValidationTargetType::Max:
  {
    m_TypeValue = *std::max_element(m_Data.begin(), m_Data.end());
    break;
  }
  case eValidationTargetType::Mean:
  {
    m_TypeValue = (double)(std::accumulate(m_Data.begin(), m_Data.end(),0) / m_Data.size());
    break;
  }
  default:
  {
    return false;
  }
  }
  // NOTE: We could use PercentTolerance too
  double minError = GeneralMath::PercentTolerance(m_RangeMin, m_TypeValue, 1e-9);
  double maxError = GeneralMath::PercentTolerance(m_RangeMax, m_TypeValue, 1e-9);
  // No error if we are in range
  if (m_TypeValue >= m_RangeMin && m_TypeValue <= m_RangeMax)
  {
    m_Error = 0;
    return true;
  }
  else if (m_TypeValue > m_RangeMax)
    m_Error = maxError;
  else if (m_TypeValue < m_RangeMin)
    m_Error = minError;
  else if (std::abs(m_Error) < 1e-15)
    m_Error = 0; // Close enough
  return true;
}
