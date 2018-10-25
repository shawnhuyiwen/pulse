/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "utils/testing/SETestCase.h"
#include "utils/testing/SETestErrorStatistics.h"
#include "properties/SEScalarTime.h"

SETestCase::SETestCase(Logger* logger) : SETestCase("default", logger)
{

}
SETestCase::SETestCase(const std::string& name, Logger* logger) : Loggable(logger)
{
  m_Name = name;
  m_Duration = new SEScalarTime();
}

SETestCase::~SETestCase()
{
  Clear();
  delete m_Duration;
}

void SETestCase::Clear()
{
  m_Failure.clear();
  m_Duration->SetValue(0, TimeUnit::s);
  DELETE_VECTOR(m_CaseEqualsErrors);
}

void SETestCase::Reset()
{
  m_Failure.clear();
  m_Duration->SetValue(0,TimeUnit::s);
  DELETE_VECTOR(m_CaseEqualsErrors);
}

void SETestCase::SetName(const std::string& Name)
{
  m_Name = Name;
}

std::string SETestCase::GetName() const
{
  return m_Name;
}

SEScalarTime& SETestCase::GetDuration()
{
  return *m_Duration;
}


void SETestCase::AddFailure(std::stringstream &err)
{
  AddFailure(err.str());
  err.str("");
  err.clear();
}
void SETestCase::AddFailure(const std::string& err)
{
  m_Failure.push_back(err);
  Error(err);
}
const std::vector<std::string>& SETestCase::GetFailures()
{
  return m_Failure;
}

SETestErrorStatistics& SETestCase::CreateErrorStatistic()
{
  SETestErrorStatistics* err = new SETestErrorStatistics(GetLogger());
  m_CaseEqualsErrors.push_back(err);
  return *err;
}
const std::vector<SETestErrorStatistics*>& SETestCase::GetErrorStatistics() const
{
  return m_CaseEqualsErrors;
}
