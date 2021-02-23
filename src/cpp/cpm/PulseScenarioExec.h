/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "scenario/SEScenarioExec.h"
#include "PulseConfiguration.h"
class PulseEngine;
class PulseScenario;
class SEAutoSerialization;

/**
* @brief Overloaded scenario exector to run a %Pulse specific scenario
*/
class PULSE_DECL PulseScenarioExec : public SEScenarioExec
{
public:
  PulseScenarioExec(Logger* logger);
  virtual ~PulseScenarioExec();

  virtual bool ExecuteFile(PhysiologyEngine& engine, const std::string& scenarioFile, const std::string& resultsFile, std::string const& dataDir = "./");
  virtual bool Execute(PhysiologyEngine& engine, const std::string& scenario, SerializationFormat f, const std::string& resultsFile, std::string const& dataDir = "./");
  virtual bool Execute(PhysiologyEngine& engine, const PulseScenario& scenario, const std::string& resultsFile);

  static void Run(const std::string& scenarioFile);

protected:
  PulseScenario*             m_PulseScenario;
  double                     m_AutoSerializationPeriod_s;
  double                     m_AutoSerializationTime_s;
  eSwitch                    m_AutoSerializationTimeStamps;
  eSwitch                    m_AutoSerializationAfterActions;
  eSwitch                    m_AutoSerializationReload;
  std::string                m_AutoSerializationFileName;
  std::string                m_AutoSerializationDirectory;
  std::stringstream          m_AutoSerializationOutput;
  std::stringstream          m_AutoSerializationActions;

  virtual bool ProcessActions(PhysiologyEngine& engine, const SEScenario& scenario) override;
  virtual bool ProcessAction(PhysiologyEngine& engine, const SEAction& action) override;
  virtual void AdvanceEngine(PhysiologyEngine& engine) override;
};