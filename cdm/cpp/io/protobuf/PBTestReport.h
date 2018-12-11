/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL2(TestCase)
CDM_BIND_DECL2(TestErrorStatistics)
CDM_BIND_DECL2(TestReport)
CDM_BIND_DECL2(TestSuite)

class CDM_DECL PBTestReport
{
public:

  static void Load(const cdm::TestCaseData& src, SETestCase& dst);
  static cdm::TestCaseData* Unload(const SETestCase& src);
  static void Serialize(const cdm::TestCaseData& src, SETestCase& dst);
  static void Serialize(const SETestCase& src, cdm::TestCaseData& dst);

  static void Load(const cdm::TestErrorStatisticsData& src, SETestErrorStatistics& dst);
  static cdm::TestErrorStatisticsData* Unload(const SETestErrorStatistics& src);
  static void Serialize(const cdm::TestErrorStatisticsData& src, SETestErrorStatistics& dst);
  static void Serialize(const SETestErrorStatistics& src, cdm::TestErrorStatisticsData& dst);

  static void Load(const cdm::TestReportData& src, SETestReport& dst);
  static cdm::TestReportData* Unload(const SETestReport& src);
  static void Serialize(const cdm::TestReportData& src, SETestReport& dst);
  static void Serialize(const SETestReport& src, cdm::TestReportData& dst);

  static void Load(const cdm::TestSuiteData& src, SETestSuite& dst);
  static cdm::TestSuiteData* Unload(const SETestSuite& src);
  static void Serialize(const cdm::TestSuiteData& src, SETestSuite& dst);
  static void Serialize(const SETestSuite& src, cdm::TestSuiteData& dst);

  static bool SerializeToString(const SETestReport& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const SETestReport& src, const std::string& filename, SerializationFormat m);
  static bool SerializeFromString(const std::string& src, SETestReport& dst, SerializationFormat m);
  static bool SerializeFromFile(const std::string& filename, SETestReport& dst, SerializationFormat m);
};