/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include <numeric>

#include "SEValidationTarget.h"

#include "cdm/utils/GeneralMath.h"

SEValidationTarget::SEValidationTarget(Logger* logger) : Loggable(logger)
{

}

SEValidationTarget::Type SEValidationTarget::GetType() const
{
  return m_Type;
}
void SEValidationTarget::SetType(SEValidationTarget::Type t)
{
  m_Type = t;
}

SEDataRequest* SEValidationTarget::GetDataRequest() const
{
  return m_DataRequest;
}
void SEValidationTarget::SetDataRequest(SEDataRequest* dr)
{
  m_DataRequest = dr;
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
  m_Value = SEScalar::dNaN();
  switch (m_Type)
  {
  case Type::Min:
  {
    m_Value = *std::min_element(m_Data.begin(), m_Data.end());
    break;
  }
  case Type::Max:
  {
    m_Value = *std::max_element(m_Data.begin(), m_Data.end());
    break;
  }
  case Type::Mean:
  {
    m_Value = std::reduce(m_Data.begin(), m_Data.end()) / m_Data.size();
    break;
  }
  default:
  {
    Error("Unsupported validation target type!");
    return false;
  }
  }
  // NOTE: We could use PercentTolerance too
  double minError = GeneralMath::PercentDifference(m_RangeMin, m_Value);
  double maxError = GeneralMath::PercentDifference(m_RangeMax, m_Value);
  // No error if we are in range
  if (m_Value >= m_RangeMin && m_Value <= m_RangeMax)
  {
    m_Error = 0;
    return true;
  }
  // Keep the bigger error
  m_Error = (minError > maxError) ? minError : maxError;
  if (std::abs(m_Error) < 1e-15)
    m_Error = 0; // Close enough
  return true;
}
