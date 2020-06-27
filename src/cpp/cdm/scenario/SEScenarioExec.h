/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

class SEAction;
class SEScenario;
class PhysiologyEngine;
class SEEngineConfiguration;

class CDM_DECL SEScenarioExec : public Loggable
{
public:
  SEScenarioExec(Logger* logger);
  virtual ~SEScenarioExec();


  virtual void Cancel();

  virtual bool ExecuteFile(PhysiologyEngine& pe, const std::string& scenarioFile, const std::string& resultsFile, const SEEngineConfiguration* ec=nullptr, std::string const& dataDir="./");
  virtual bool Execute(PhysiologyEngine& pe, const std::string& scenario, SerializationFormat format, const std::string& resultsFile, const SEEngineConfiguration* ec = nullptr, std::string const& dataDir = "./");
  virtual bool Execute(PhysiologyEngine& pe, const SEScenario& scenario,      const std::string& resultsFile, const SEEngineConfiguration* ec=nullptr);
  
protected:

  virtual bool ProcessActions(PhysiologyEngine& pe, const SEScenario& scenario);
  /// This does not include advance time actions
  /// To override default functionality with those 
  /// actions override the ProcessActions method
  virtual bool ProcessAction(PhysiologyEngine& pe, const SEAction& action);
  virtual void AdvanceEngine(PhysiologyEngine& pe);

  bool                  m_Cancel;
  SEScenario*           m_Scenario;
  std::stringstream     m_ss;
};