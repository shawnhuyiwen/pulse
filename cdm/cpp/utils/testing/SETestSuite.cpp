/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "utils/testing/SETestCase.h"
#include "utils/testing/SETestSuite.h"
#include "utils/testing/SETestErrorStatistics.h"
#include "properties/SEScalarTime.h"

SETestSuite::SETestSuite(Logger* logger) : Loggable(logger)
{
  m_Performed = true;
  m_Name = "";
  m_Time = new SEScalarTime();
}

SETestSuite::~SETestSuite()
{
  Clear();
  delete m_Time;
}

void SETestSuite::Clear()
{
  DELETE_VECTOR(m_SuiteEqualError);
  DELETE_VECTOR(m_TestCase);
}

void SETestSuite::Reset()
{
  for(unsigned int i = 0; i<m_TestCase.size(); i++)
  {
    m_TestCase.at(i)->Reset();
  }
  m_Performed = true;
  m_Name = "";
}

void SETestSuite::SetName(const std::string& Name)
{
  m_Name = Name;
}

std::string SETestSuite::GetName() const
{
  return m_Name;
}

void SETestSuite::PerformSuite(bool performed)
{
  m_Performed = performed;
}

bool SETestSuite::PerformedSuite() const
{
  return m_Performed;
}

const SEScalarTime& SETestSuite::GetDuration() const
{
  double time = 0;
  for (unsigned int i = 0; i < m_TestCase.size(); i++)
    time += m_TestCase.at(i)->GetDuration().GetValue(TimeUnit::s);
  m_Time->SetValue(time,TimeUnit::s);
  return *m_Time;
}

std::vector<std::string>& SETestSuite::GetRequirements()
{
  return m_Requirements;
}

SETestCase& SETestSuite::CreateTestCase()
{
  SETestCase* test = new SETestCase(GetLogger());
  m_TestCase.push_back(test);
  return *test;
}

const std::vector<SETestCase*>& SETestSuite::GetTestCases() const
{
  return m_TestCase;
}

size_t SETestSuite::GetNumberOfErrors() const
{
  size_t count = 0;
  for (unsigned int i = 0; i < m_TestCase.size(); i++)
    count += m_TestCase.at(i)->GetFailures().size()>0 ? 1 : 0;
  return count;
}
