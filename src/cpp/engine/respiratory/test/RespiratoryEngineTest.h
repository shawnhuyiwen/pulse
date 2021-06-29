/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/utils/testing/SETestManager.h"

class ENGINE_TEST_DECL RespiratoryEngineTest : public SETestManager
{
public:
  RespiratoryEngineTest(Logger* logger = nullptr);
  virtual ~RespiratoryEngineTest();

  virtual bool RunTest(const std::string& testName, const std::string& sOutputDirectory) override;

protected:
  virtual void FillFunctionMap() override;
  typedef void(RespiratoryEngineTest::* testFunction)(const std::string&);
  std::map<std::string, testFunction> testMap;
  std::stringstream m_ss;

public:
  void SmokeTest(const std::string& sTestDirectory);
};
