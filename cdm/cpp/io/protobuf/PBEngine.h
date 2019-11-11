/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL(LogMessagesData)
CDM_BIND_DECL(ActionListData)
CDM_BIND_DECL(ConditionListData)
CDM_BIND_DECL2(DataRequest)
CDM_BIND_DECL2(DataRequestManager)
CDM_BIND_DECL2(DecimalFormat)
CDM_BIND_DECL2(AutoSerialization)
CDM_BIND_DECL2(DynamicStabilization)
CDM_BIND_DECL2(DynamicStabilizationEngineConvergence)
CDM_BIND_DECL2(TimedStabilization)
CDM_BIND_DECL2(PatientConfiguration)
class LogMessages;
class SEAction;
class SEActionManager;
class SEConditionManager;
class SESubstanceManager;
class SEAnesthesiaMachineActionCollection;
class SEEnvironmentActionCollection;
class SEInhalerActionCollection;
class SEPatientActionCollection;

class CDM_DECL PBEngine
{
public:

  static void Load(const cdm::LogMessagesData& src, LogMessages& dst);
  static cdm::LogMessagesData* Unload(const LogMessages& src);
  static void Serialize(const cdm::LogMessagesData& src, LogMessages& dst);
  static void Serialize(const LogMessages& src, cdm::LogMessagesData& dst); 
  static bool SerializeToString(const LogMessages& src, std::string& output, SerializationFormat m, Logger* logger);
  static bool SerializeFromString(const std::string& src, LogMessages& dst, SerializationFormat m, Logger* logger);

  static void Load(const cdm::PatientConfigurationData& src, SEPatientConfiguration& dst, SESubstanceManager& subMgr);
  static cdm::PatientConfigurationData* Unload(const SEPatientConfiguration& src);
  static void Serialize(const cdm::PatientConfigurationData& src, SEPatientConfiguration& dst, SESubstanceManager& subMgr);
  static void Serialize(const SEPatientConfiguration& src, cdm::PatientConfigurationData& dst);
  static bool SerializeToString(const SEPatientConfiguration& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const SEPatientConfiguration& src, const std::string& filename, SerializationFormat m);
  static bool SerializeFromString(const std::string& src, SEPatientConfiguration& dst, SerializationFormat m, SESubstanceManager& subMgr);
  static bool SerializeFromFile(const std::string& filename, SEPatientConfiguration& dst, SerializationFormat m, SESubstanceManager& subMgr);

  static void Load(const cdm::ConditionListData& src, SEConditionManager& dst);
  static cdm::ConditionListData* Unload(const SEConditionManager& src);
  static void Serialize(const cdm::ConditionListData& src, SEConditionManager& dst);
  static void Serialize(const SEConditionManager& src, cdm::ConditionListData& dst);

  static bool SerializeToString(const SEConditionManager& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const SEConditionManager& src, const std::string& filename, SerializationFormat m);
  static bool SerializeFromString(const std::string& src, SEConditionManager& dst, SerializationFormat m);
  static bool SerializeFromFile(const std::string& filename, SEConditionManager& dst, SerializationFormat m);

  static void Load(const cdm::ActionListData& src, SEActionManager& dst);
  static cdm::ActionListData* Unload(const SEActionManager& src);
  static void Serialize(const cdm::ActionListData& src, SEActionManager& dst);
  static void Serialize(const SEActionManager& src, cdm::ActionListData& dst);
  static void Serialize(const SEAnesthesiaMachineActionCollection& src, cdm::ActionListData& dst);
  static void Serialize(const SEEnvironmentActionCollection& src, cdm::ActionListData& dst);
  static void Serialize(const SEInhalerActionCollection& src, cdm::ActionListData& dst);
  static void Serialize(const SEPatientActionCollection& src, cdm::ActionListData& dst);

  static bool SerializeToString(const SEActionManager& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const SEActionManager& src, const std::string& filename, SerializationFormat m);
  static bool SerializeFromString(const std::string& src, SEActionManager& dst, SerializationFormat m);
  static bool SerializeFromFile(const std::string& filename, SEActionManager& dst, SerializationFormat m);

  static void Load(const cdm::ActionListData& src, std::vector<SEAction*>& dst, SESubstanceManager& subMgr);
  static void Serialize(const cdm::ActionListData& src, std::vector<SEAction*>& dst, SESubstanceManager& subMgr);
  static bool SerializeFromString(const std::string& src, std::vector<SEAction*>& dst, SerializationFormat m, SESubstanceManager& subMgr);

  static void Load(const cdm::DataRequestData& src, SEDataRequest& dst);
  static cdm::DataRequestData* Unload(const SEDataRequest& src);
  static void Serialize(const cdm::DataRequestData& src, SEDataRequest& dst);
  static void Serialize(const SEDataRequest& src, cdm::DataRequestData& dst);
  static void Copy(const SEDataRequest& src, SEDataRequest& dst);

  static void Load(const cdm::DataRequestManagerData& src, SEDataRequestManager& dst, const SESubstanceManager& subMgr);
  static cdm::DataRequestManagerData* Unload(const SEDataRequestManager& src);
  static void Serialize(const cdm::DataRequestManagerData& src, SEDataRequestManager& dst, const SESubstanceManager& subMgr);
  static void Serialize(const SEDataRequestManager& src, cdm::DataRequestManagerData& dst);
  static void Copy(const SEDataRequestManager& src, SEDataRequestManager& dst, const SESubstanceManager& subMgr);

  static bool SerializeToString(const SEDataRequestManager& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const SEDataRequestManager& src, const std::string& filename, SerializationFormat m);
  static bool SerializeFromString(const std::string& src, SEDataRequestManager& dst, SerializationFormat m, const SESubstanceManager& subMgr);
  static bool SerializeFromFile(const std::string& filename, SEDataRequestManager& dst, SerializationFormat m, const SESubstanceManager& subMgr);

  static void Load(const cdm::DecimalFormatData& src, SEDecimalFormat& dst);
  static cdm::DecimalFormatData* Unload(const SEDecimalFormat& src);
  static void Serialize(const cdm::DecimalFormatData& src, SEDecimalFormat& dst);
  static void Serialize(const SEDecimalFormat& src, cdm::DecimalFormatData& dst);

  static void Load(const cdm::AutoSerializationData& src, SEAutoSerialization& dst);
  static cdm::AutoSerializationData* Unload(const SEAutoSerialization& src);
  static void Serialize(const cdm::AutoSerializationData& src, SEAutoSerialization& dst);
  static void Serialize(const SEAutoSerialization& src, cdm::AutoSerializationData& dst);

  static void Load(const cdm::DynamicStabilizationData& src, SEDynamicStabilization& dst);
  static cdm::DynamicStabilizationData* Unload(const SEDynamicStabilization& src);
  static void Serialize(const cdm::DynamicStabilizationData& src, SEDynamicStabilization& dst);
  static void Serialize(const SEDynamicStabilization& src, cdm::DynamicStabilizationData& dst);

  static bool SerializeToString(const SEDynamicStabilization& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const SEDynamicStabilization& src, const std::string& filename, SerializationFormat);
  static bool SerializeFromString(const std::string& src, SEDynamicStabilization& dst, SerializationFormat m);
  static bool SerializeFromFile(const std::string& filename, SEDynamicStabilization& dst, SerializationFormat m);

  static void Load(const cdm::DynamicStabilizationEngineConvergenceData& src, SEDynamicStabilizationEngineConvergence& dst);
  static cdm::DynamicStabilizationEngineConvergenceData* Unload(const SEDynamicStabilizationEngineConvergence& src);
  static void Serialize(const cdm::DynamicStabilizationEngineConvergenceData& src, SEDynamicStabilizationEngineConvergence& dst);
  static void Serialize(const SEDynamicStabilizationEngineConvergence& src, cdm::DynamicStabilizationEngineConvergenceData& dst);

  static void Load(const cdm::TimedStabilizationData& src, SETimedStabilization& dst);
  static cdm::TimedStabilizationData* Unload(const SETimedStabilization& src);
  static void Serialize(const cdm::TimedStabilizationData& src, SETimedStabilization& dst);
  static void Serialize(const SETimedStabilization& src, cdm::TimedStabilizationData& dst);

  static bool SerializeToString(const SETimedStabilization& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const SETimedStabilization& src, const std::string& filename, SerializationFormat m);
  static bool SerializeFromString(const std::string& src, SETimedStabilization& dst, SerializationFormat m);
  static bool SerializeFromFile(const std::string& filename, SETimedStabilization& dst, SerializationFormat m);
};