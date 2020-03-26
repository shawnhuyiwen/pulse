/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SETestCase;
class SETestReport;
class SETestErrorStatistics;

class CDM_DECL SETestSuite : public Loggable
{
  friend class PBTestReport;//friend the serialization class
  friend SETestReport;
protected:
  SETestSuite(Logger* logger);
public:
  virtual ~SETestSuite();

  virtual void Reset(); //reset values
  virtual void Clear(); //clear memory

public:

  void                SetName(const std::string& Name);
  std::string         GetName() const;

  void                PerformSuite(bool Performed);
  bool                PerformedSuite() const;
  
  const SEScalarTime&  GetDuration() const;
          
  std::vector<std::string>&  GetRequirements();

  SETestCase& CreateTestCase();
  const std::vector<SETestCase*>&  GetTestCases() const;

  size_t GetNumberOfErrors() const;

protected:

  bool m_Performed;
  std::string m_Name;
  std::vector<std::string> m_Requirements;
  std::vector<SETestErrorStatistics*> m_SuiteEqualError;
  std::vector<SETestCase*> m_TestCase;
  mutable SEScalarTime* m_Time;
};