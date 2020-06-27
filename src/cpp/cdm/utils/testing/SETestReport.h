/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SETestCase;
class SETestSuite;
class SETestErrorStatistics;

class CDM_DECL SETestReport : public Loggable
{
  friend class PBTestReport;//friend the serialization class
public:

  SETestReport(Logger* logger);
  virtual ~SETestReport();
  
  virtual void Reset(); //reset values
  virtual void Clear(); //clear memory

  bool SerializeToString(std::string& output, SerializationFormat m) const;
  bool SerializeToFile(const std::string& filename) const;
  bool SerializeFromString(const std::string& src, SerializationFormat m);
  bool SerializeFromFile(const std::string& filename);

  SETestSuite& CreateTestSuite();
  const std::vector<SETestSuite*>&  GetTestSuites() const;

protected: 
  std::vector<SETestSuite*> m_testSuite;
};