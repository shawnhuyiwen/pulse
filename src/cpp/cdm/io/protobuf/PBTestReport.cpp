/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
PUSH_PROTO_WARNINGS()
#include "pulse/cdm/bind/TestReport.pb.h"
POP_PROTO_WARNINGS()
#include "io/protobuf/PBTestReport.h"
#include "io/protobuf/PBProperties.h"
#include "io/protobuf/PBUtils.h"
#include "utils/testing/SETestCase.h"
#include "utils/testing/SETestErrorStatistics.h"
#include "utils/testing/SETestReport.h"
#include "utils/testing/SETestSuite.h"
#include "utils/FileUtils.h"


void PBTestReport::Load(const CDM_BIND::TestCaseData& src, SETestCase& dst)
{
  dst.Clear();
  PBTestReport::Serialize(src, dst);
}
void PBTestReport::Serialize(const CDM_BIND::TestCaseData& src, SETestCase& dst)
{

  dst.m_Name = src.name();
  if (src.has_duration())
    PBProperty::Load(src.duration(), dst.GetDuration());

  SETestErrorStatistics* ex;
  for (int i = 0; i < src.errorstats_size(); i++)
  {
    auto eData = src.errorstats(i);
    ex = new SETestErrorStatistics(dst.GetLogger());
    PBTestReport::Load(eData, *ex);
    dst.m_CaseEqualsErrors.push_back(ex);
  }

  for (int i = 0; i < src.failure_size(); i++)
    dst.m_Failure.push_back(src.failure(i));
}
CDM_BIND::TestCaseData* PBTestReport::Unload(const SETestCase& src)
{
  CDM_BIND::TestCaseData* dst = new CDM_BIND::TestCaseData();
  PBTestReport::Serialize(src, *dst);
  return dst;
}
void PBTestReport::Serialize(const SETestCase& src, CDM_BIND::TestCaseData& dst)
{
  dst.set_name(src.m_Name);

  dst.set_allocated_duration(PBProperty::Unload(*src.m_Duration));

  for (std::string s : src.m_Failure)
    dst.mutable_failure()->Add(s.c_str());

  for (auto s : src.m_CaseEqualsErrors)
    dst.mutable_errorstats()->AddAllocated(PBTestReport::Unload(*s));
}

void PBTestReport::Load(const CDM_BIND::TestErrorStatisticsData& src, SETestErrorStatistics& dst)
{
  dst.Clear();
  PBTestReport::Serialize(src, dst);
}
void PBTestReport::Serialize(const CDM_BIND::TestErrorStatisticsData& src, SETestErrorStatistics& dst)
{
  dst.m_PropertyName = src.propertyname();
  dst.m_PercentTolerance = src.percenttolerance();
  dst.m_NumberOfErrors = src.numberoferrors();
  dst.m_ComputedPropertyID = src.computedpropertyid();
  dst.m_ExpectedPropertyID = src.expectedpropertyid();

  dst.m_MinimumError = src.minimumerror();
  dst.m_MaximumError = src.maximumerror();
  dst.m_AverageError = src.averageerror();
  dst.m_StandardDeviation = src.standarddeviation();

  if (src.has_percenttolerancevsnumerrors())
    PBProperty::Load(src.percenttolerancevsnumerrors(), dst.GetPercentToleranceVsNumErrorsHistogram());

  for (int i = 0; i < src.differences_size(); i++)
    dst.m_Differences.push_back(src.differences(i));
}
CDM_BIND::TestErrorStatisticsData* PBTestReport::Unload(const SETestErrorStatistics& src)
{
  CDM_BIND::TestErrorStatisticsData* dst = new CDM_BIND::TestErrorStatisticsData();
  PBTestReport::Serialize(src, *dst);
  return dst;
}
void PBTestReport::Serialize(const SETestErrorStatistics& src, CDM_BIND::TestErrorStatisticsData& dst)
{
  if (!src.m_PropertyName.empty())
    dst.set_propertyname(src.m_PropertyName);
  dst.set_percenttolerance(src.m_PercentTolerance);
  dst.set_numberoferrors(src.m_NumberOfErrors);
  if (!src.m_ComputedPropertyID.empty())
    dst.set_computedpropertyid(src.m_ComputedPropertyID);
  if (!src.m_ExpectedPropertyID.empty())
    dst.set_expectedpropertyid(src.m_ExpectedPropertyID);

  if (!std::isnan(src.m_MinimumError))
    dst.set_minimumerror(src.m_MinimumError);
  if (!std::isnan(src.m_MaximumError))
    dst.set_maximumerror(src.m_MaximumError);
  if (!std::isnan(src.m_AverageError))
    dst.set_averageerror(src.m_AverageError);
  if (!std::isnan(src.m_StandardDeviation))
    dst.set_standarddeviation(src.m_StandardDeviation);

  std::string dData;
  for (auto s : src.m_Differences)
    dst.mutable_differences()->Add(s.c_str());

  if (src.m_PercentToleranceVsNumErrorsHistogram != nullptr)
    dst.set_allocated_percenttolerancevsnumerrors(PBProperty::Unload(*src.m_PercentToleranceVsNumErrorsHistogram));
}


void PBTestReport::Load(const CDM_BIND::TestReportData& src, SETestReport& dst)
{
  dst.Clear();
  PBTestReport::Serialize(src, dst);
}
void PBTestReport::Serialize(const CDM_BIND::TestReportData& src, SETestReport& dst)
{
  SETestSuite* sx;
  for (int i = 0; i < src.testsuite_size(); i++)
  {
    sx = new SETestSuite(dst.GetLogger());
    PBTestReport::Load(src.testsuite(i), *sx);
    dst.m_testSuite.push_back(sx);
  }
}
CDM_BIND::TestReportData* PBTestReport::Unload(const SETestReport& src)
{
  CDM_BIND::TestReportData* dst = new CDM_BIND::TestReportData();
  PBTestReport::Serialize(src, *dst);
  return dst;
}
void PBTestReport::Serialize(const SETestReport& src, CDM_BIND::TestReportData& dst)
{
  for (auto ts : src.m_testSuite)
    dst.mutable_testsuite()->AddAllocated(PBTestReport::Unload(*ts));
}


void PBTestReport::Load(const CDM_BIND::TestSuiteData& src, SETestSuite& dst)
{
  dst.Clear();
  PBTestReport::Serialize(src, dst);
}
void PBTestReport::Serialize(const CDM_BIND::TestSuiteData& src, SETestSuite& dst)
{

  dst.m_Name = src.name();
  dst.m_Performed = src.performed();

  for (int i = 0; i < src.requirement_size(); i++)
    dst.m_Requirements.push_back(src.requirement(i));

  SETestErrorStatistics* ex;
  for (int i = 0; i < src.errorstats_size(); i++)
  {
    ex = new SETestErrorStatistics(dst.GetLogger());
    PBTestReport::Load(src.errorstats(i), *ex);
    dst.m_SuiteEqualError.push_back(ex);
  }

  SETestCase* tx;
  for (int i = 0; i < src.testcase_size(); i++)
  {
    tx = new SETestCase(dst.GetLogger());
    PBTestReport::Load(src.testcase(i), *tx);
    dst.m_TestCase.push_back(tx);
  }
}
CDM_BIND::TestSuiteData* PBTestReport::Unload(const SETestSuite& src)
{
  CDM_BIND::TestSuiteData* dst = new CDM_BIND::TestSuiteData();
  PBTestReport::Serialize(src, *dst);
  return dst;
}
void PBTestReport::Serialize(const SETestSuite& src, CDM_BIND::TestSuiteData& dst)
{
  dst.set_name(src.m_Name);
  dst.set_performed(src.m_Performed);
  dst.set_errors((google::protobuf::uint32)src.GetNumberOfErrors());
  dst.set_tests((google::protobuf::uint32)src.GetTestCases().size());

  for (std::string s : src.m_Requirements)
    dst.mutable_requirement()->Add(s.c_str());

  for (auto er : src.m_SuiteEqualError)
    dst.mutable_errorstats()->AddAllocated(PBTestReport::Unload(*er));

  for (auto tc : src.m_TestCase)
    dst.mutable_testcase()->AddAllocated(PBTestReport::Unload(*tc));
}


bool PBTestReport::SerializeToString(const SETestReport& src, std::string& output, SerializationFormat m)
{
  CDM_BIND::TestReportData data;
  PBTestReport::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBTestReport::SerializeToFile(const SETestReport& src, const std::string& filename)
{
  CDM_BIND::TestReportData data;
  PBTestReport::Serialize(src, data);
  return PBUtils::SerializeToFile(data, filename, src.GetLogger());
}
bool PBTestReport::SerializeFromString(const std::string& src, SETestReport& dst, SerializationFormat m)
{
  CDM_BIND::TestReportData data;
  if (!PBUtils::SerializeFromString(src, data, m, dst.GetLogger()))
    return false;
  PBTestReport::Load(data, dst);
  return true;
}
bool PBTestReport::SerializeFromFile(const std::string& filename, SETestReport& dst)
{
  CDM_BIND::TestReportData data;
  if (!PBUtils::SerializeFromFile(filename, data, dst.GetLogger()))
    return false;
  PBTestReport::Load(data, dst);
  return true;
}