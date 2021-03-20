/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "utils/Logger.h"
#include "utils/FileUtils.h"

class SEAction;
class SEScenario;
class PhysiologyEngine;
class SEEngineConfiguration;

class CDM_DECL SEScenarioExec
{
public:
  SEScenarioExec();
  virtual ~SEScenarioExec();

  virtual void Clear();

  eSwitch GetLogToConsole() const { return m_LogToConsole; }
  void SetLogToConsole(eSwitch s) { m_LogToConsole = s; }

  std::string GetLogFilename() const { return m_LogFilename; }
  void SetLogFilename(const std::string& fn) { m_LogFilename = fn; }

  std::string GetDataRequestCSVFilename() const { return m_DataRequestCSVFilename; }
  void SetDataRequestCSVFilename(const std::string& fn) { m_DataRequestCSVFilename = fn; }

  std::string GetDataRootDirectory() const { return m_DataRootDirectory; }
  void SetDataRootDirectory(const std::string& fn) { m_DataRootDirectory = fn; }

  std::string GetEngineConfigurationContent() const { return m_EngineConfigurationContent; }
  void SetEngineConfigurationContent(const std::string& ecc)
  {
    m_EngineConfigurationContent = ecc;
    m_EngineConfigurationFilename = "";
  }
  std::string GetEngineConfigurationFilename() const { return m_EngineConfigurationFilename; }
  void SetEngineConfigurationFilename(const std::string& fn)
  {
    m_EngineConfigurationContent = "";
    m_EngineConfigurationFilename = fn;
  }

  std::string GetScenarioContent() const { return m_ScenarioContent; }
  void SetScenarioContent(const std::string& sc)
  {
    m_ScenarioContent = sc;
    m_ScenarioFilename = "";
  }
  std::string GetScenarioFilename() const { return m_ScenarioFilename; }
  void SetScenarioFilename(const std::string& fn)
  {
    m_ScenarioContent = "";
    m_ScenarioFilename = fn;
  }

  SerializationFormat GetContentFormat() const { return m_ContentFormat; }
  void SetContentFormat(SerializationFormat s) { m_ContentFormat = s; }

  std::string GetSerializationDirectory() const { return m_SerializationDirectory; }
  void SetSerializationDirectory(const std::string& d) { m_SerializationDirectory = d; }

  std::string GetAutoSerializeFilename() const { return m_AutoSerializeFilename; }
  void SetAutoSerializeFilename(const std::string& fn)
  {
    // Remove any path from the provided filename (add directory via SetSerializationDirectory)
    SplitFilenameExt(fn, m_AutoSerializeBaseFilename, m_AutoSerializeFilenameExt);
    m_AutoSerializeFilename = m_AutoSerializeBaseFilename + m_AutoSerializeFilenameExt;
  }

  eSwitch GetAutoSerializeAfterActions() const { return m_AutoSerializeAfterActions; }
  void SetAutoSerializeAfterActions(eSwitch s) { m_AutoSerializeAfterActions = s; }

  double GetAutoSerializePeriod_s() const { return m_AutoSerializePeriod_s; }
  void SetAutoSerializePeriod_s(double s) { m_AutoSerializePeriod_s = s; }

  eSwitch GetTimeStampSerializedStates() const { return m_TimeStampSerializedStates; }
  void SetTimeStampSerializedStates(eSwitch s) { m_TimeStampSerializedStates = s; }

  eSwitch GetReloadSerializedState() const { return m_ReloadSerializedState; }
  void SetReloadSerializedState(eSwitch s) { m_ReloadSerializedState = s; }

  bool SerializeToString(std::string& output, SerializationFormat m, Logger* logger=nullptr) const;
  bool SerializeFromString(const std::string& src, SerializationFormat m, Logger* logger=nullptr);

  bool Execute(PhysiologyEngine& pe, SEScenario& sce);

protected:

  bool ProcessActions(PhysiologyEngine& pe, SEScenario& sce);
  /// This does not include advance time actions
  /// To override default functionality with those 
  /// actions override the ProcessActions method
  bool ProcessAction(PhysiologyEngine& pe, SEAction& action);
  void AdvanceEngine(PhysiologyEngine& pe);

  eSwitch     m_LogToConsole;
  std::string m_LogFilename;

  std::string m_DataRequestCSVFilename;
  std::string m_DataRootDirectory;

  // Note, its up to the deriving exec's to merge configs
  // The basic config is empty
  std::string m_EngineConfigurationContent;
  std::string m_EngineConfigurationFilename;

  std::string m_ScenarioContent;
  std::string m_ScenarioFilename;

  // For both the EC and Scenrio Content
  SerializationFormat m_ContentFormat = SerializationFormat::JSON;

  // Settings for serialization
  bool                       m_SaveNextStep;
  std::string                m_SerializationDirectory;
  std::string                m_AutoSerializeFilename;
  std::string                m_AutoSerializeBaseFilename;
  std::string                m_AutoSerializeFilenameExt;
  eSwitch                    m_AutoSerializeAfterActions;
  double                     m_AutoSerializePeriod_s;
  double                     m_AutoSerializeTime_s;
  eSwitch                    m_TimeStampSerializedStates;
  eSwitch                    m_ReloadSerializedState;
  std::stringstream          m_SerializationOutput;
  std::stringstream          m_SerializationActions;
};