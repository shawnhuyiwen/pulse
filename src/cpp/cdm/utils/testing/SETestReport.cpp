/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "utils/testing/SETestReport.h"
#include "utils/testing/SETestCase.h"
#include "utils/testing/SETestSuite.h"
#include "utils/testing/SETestErrorStatistics.h"
#include "io/protobuf/PBTestReport.h"

SETestReport::SETestReport(Logger* logger) : Loggable(logger)
{
}

SETestReport::~SETestReport()
{
  Clear();
}

void SETestReport::Clear()
{
  DELETE_VECTOR(m_testSuite);
}

void SETestReport::Reset()
{
}

bool SETestReport::SerializeToString(std::string& output, SerializationFormat m) const
{
  return PBTestReport::SerializeToString(*this, output, m);
}
bool SETestReport::SerializeToFile(const std::string& filename) const
{
  return PBTestReport::SerializeToFile(*this, filename);
}
bool SETestReport::SerializeFromString(const std::string& src, SerializationFormat m)
{
  return PBTestReport::SerializeFromString(src, *this, m);
}
bool SETestReport::SerializeFromFile(const std::string& filename)
{
  return PBTestReport::SerializeFromFile(filename, *this);
}

SETestSuite& SETestReport::CreateTestSuite()
{
  SETestSuite* suite = new SETestSuite(GetLogger());
  m_testSuite.push_back(suite);
  return *suite;
}
const std::vector<SETestSuite*>& SETestReport::GetTestSuites() const
{
  return m_testSuite;
}