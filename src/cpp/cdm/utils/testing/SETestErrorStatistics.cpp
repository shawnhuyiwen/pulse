/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "utils/testing/SETestErrorStatistics.h"
#include "properties/SEFunction.h"

SETestErrorStatistics::SETestErrorStatistics(Logger* logger) : Loggable(logger)
{
  m_PercentToleranceVsNumErrorsHistogram = nullptr;
}

SETestErrorStatistics::~SETestErrorStatistics()
{
  Clear();
}

void SETestErrorStatistics::Clear()
{
  SAFE_DELETE(m_PercentToleranceVsNumErrorsHistogram);
}

void SETestErrorStatistics::Reset()
{
}

void SETestErrorStatistics::SetMinimumError(double MinimumError)
{
  m_MinimumError = MinimumError;
}

double SETestErrorStatistics::GetMinimumError() const
{
  return m_MinimumError;
}

void SETestErrorStatistics::SetMaximumError(double MaximumError)
{
  m_MaximumError = MaximumError;
}

double SETestErrorStatistics::GetMaximumError() const
{
  return m_MaximumError;
}

void SETestErrorStatistics::SetAverageError(double AverageError)
{
  m_AverageError = AverageError;
}

double SETestErrorStatistics::GetAverageError() const
{
  return m_AverageError;
}

void SETestErrorStatistics::SetStandardDeviation(double StandardDeviation)
{
  m_StandardDeviation = StandardDeviation;
}

double SETestErrorStatistics::GetStandardDeviation() const
{
  return m_StandardDeviation;
}

void SETestErrorStatistics::AddDifference(const std::string& difference)
{
  std::string n;
  for(unsigned int i=0; i<m_Differences.size(); i++)
  {
    n=m_Differences.at(i);
    if(n==difference)
      return;
  }
  m_Differences.push_back(difference);
}

const std::vector<std::string>* SETestErrorStatistics::GetDifferences() const
{
  return &m_Differences;
}

void SETestErrorStatistics::SetComputedPropertyID(const std::string& ComputedPropertyID)
{
  m_ComputedPropertyID = ComputedPropertyID;
}

std::string SETestErrorStatistics::GetComputedPropertyID() const
{
  return m_ComputedPropertyID;
}

void SETestErrorStatistics::SetExpectedPropertyID(const std::string& ExpectedPropertyID)
{
  m_ExpectedPropertyID = ExpectedPropertyID;
}

std::string SETestErrorStatistics::GetExpectedPropertyID() const
{
  return m_ExpectedPropertyID;
}

void SETestErrorStatistics::SetNumberOfErrors(int NumberOfErrors)
{
  m_NumberOfErrors = NumberOfErrors;
}

int SETestErrorStatistics::GetNumberOfErrors() const
{
  return m_NumberOfErrors;
}

void SETestErrorStatistics::SetPropertyName(const std::string& PropertyName)
{
  m_PropertyName = PropertyName;
}

std::string SETestErrorStatistics::GetPropertyName() const
{
  return m_PropertyName;
}

bool SETestErrorStatistics::HasPercentTolerancevsNumErrorsHistogram() const
{
  if(m_PercentToleranceVsNumErrorsHistogram==nullptr)
    return false;
  return true;
}

SEFunction&  SETestErrorStatistics::GetPercentToleranceVsNumErrorsHistogram()
{
  if(HasPercentTolerancevsNumErrorsHistogram())
    return *m_PercentToleranceVsNumErrorsHistogram;

  m_PercentToleranceVsNumErrorsHistogram = new SEFunction();
  m_PercentToleranceVsNumErrorsHistogram->GetIndependent().push_back(m_PercentTolerance);
  m_PercentToleranceVsNumErrorsHistogram->GetDependent().push_back(m_NumberOfErrors);
  return *m_PercentToleranceVsNumErrorsHistogram;
}

void SETestErrorStatistics::SetPercentToleranceVsNumErrorsHistogram(SEFunction* PercentToleranceVsNumErrors)
{
  m_PercentToleranceVsNumErrorsHistogram = PercentToleranceVsNumErrors;
}
