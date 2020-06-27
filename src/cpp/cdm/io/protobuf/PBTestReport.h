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

  static void Load(const CDM_BIND::TestCaseData& src, SETestCase& dst);
  static CDM_BIND::TestCaseData* Unload(const SETestCase& src);
  static void Serialize(const CDM_BIND::TestCaseData& src, SETestCase& dst);
  static void Serialize(const SETestCase& src, CDM_BIND::TestCaseData& dst);

  static void Load(const CDM_BIND::TestErrorStatisticsData& src, SETestErrorStatistics& dst);
  static CDM_BIND::TestErrorStatisticsData* Unload(const SETestErrorStatistics& src);
  static void Serialize(const CDM_BIND::TestErrorStatisticsData& src, SETestErrorStatistics& dst);
  static void Serialize(const SETestErrorStatistics& src, CDM_BIND::TestErrorStatisticsData& dst);

  static void Load(const CDM_BIND::TestReportData& src, SETestReport& dst);
  static CDM_BIND::TestReportData* Unload(const SETestReport& src);
  static void Serialize(const CDM_BIND::TestReportData& src, SETestReport& dst);
  static void Serialize(const SETestReport& src, CDM_BIND::TestReportData& dst);

  static void Load(const CDM_BIND::TestSuiteData& src, SETestSuite& dst);
  static CDM_BIND::TestSuiteData* Unload(const SETestSuite& src);
  static void Serialize(const CDM_BIND::TestSuiteData& src, SETestSuite& dst);
  static void Serialize(const SETestSuite& src, CDM_BIND::TestSuiteData& dst);

  static bool SerializeToString(const SETestReport& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const SETestReport& src, const std::string& filename);
  static bool SerializeFromString(const std::string& src, SETestReport& dst, SerializationFormat m);
  static bool SerializeFromFile(const std::string& filename, SETestReport& dst);
};