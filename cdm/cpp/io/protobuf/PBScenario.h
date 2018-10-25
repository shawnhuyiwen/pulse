/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL(ActionListData)
CDM_BIND_DECL(ConditionListData)
CDM_BIND_DECL2(DataRequest)
CDM_BIND_DECL2(DataRequestManager)
CDM_BIND_DECL2(DecimalFormat)
CDM_BIND_DECL2(Scenario)
CDM_BIND_DECL2(ScenarioInitialParameters)
class SEActionManager;
class SEConditionManager;
class SESubstanceManager;
class SEAnesthesiaMachineActionCollection;
class SEEnvironmentActionCollection;
class SEInhalerActionCollection;
class SEPatientActionCollection;

class CDM_DECL PBScenario
{
public:

  static void Load(const cdm::ConditionListData& src, SEConditionManager& dst);
  static cdm::ConditionListData* Unload(const SEConditionManager& src);
  static void Serialize(const cdm::ConditionListData& src, SEConditionManager& dst);
  static void Serialize(const SEConditionManager& src, cdm::ConditionListData& dst);

  static void Load(const cdm::ActionListData& src, SEActionManager& dst);
  static cdm::ActionListData* Unload(const SEActionManager& src);
  static void Serialize(const cdm::ActionListData& src, SEActionManager& dst);
  static void Serialize(const SEActionManager& src, cdm::ActionListData& dst);
  static void Serialize(const SEAnesthesiaMachineActionCollection& src, cdm::ActionListData& dst);
  static void Serialize(const SEEnvironmentActionCollection& src, cdm::ActionListData& dst);
  static void Serialize(const SEInhalerActionCollection& src, cdm::ActionListData& dst);
  static void Serialize(const SEPatientActionCollection& src, cdm::ActionListData& dst);

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

  static void Load(const cdm::DecimalFormatData& src, SEDecimalFormat& dst);
  static cdm::DecimalFormatData* Unload(const SEDecimalFormat& src);
  static void Serialize(const cdm::DecimalFormatData& src, SEDecimalFormat& dst);
  static void Serialize(const SEDecimalFormat& src, cdm::DecimalFormatData& dst);

  static void Load(const cdm::ScenarioData& src, SEScenario& dst);
  static cdm::ScenarioData* Unload(const SEScenario& src);
  static void Serialize(const cdm::ScenarioData& src, SEScenario& dst);
  static void Serialize(const SEScenario& src, cdm::ScenarioData& dst);

  static void Load(const cdm::ScenarioInitialParametersData& src, SEScenarioInitialParameters& dst);
  static cdm::ScenarioInitialParametersData* Unload(const SEScenarioInitialParameters& src);
  static void Serialize(const cdm::ScenarioInitialParametersData& src, SEScenarioInitialParameters& dst);
  static void Serialize(const SEScenarioInitialParameters& src, cdm::ScenarioInitialParametersData& dst);

  static bool SerializeToString(const SEScenario& src, std::string& output, SerializationMode m);
  static bool SerializeToFile(const SEScenario& src, const std::string& filename, SerializationMode m);
  static bool SerializeFromString(const std::string& src, SEScenario& dst, SerializationMode m);
  static bool SerializeFromFile(const std::string& filename, SEScenario& dst, SerializationMode m);

  static bool SerializeToString(const SEConditionManager& src, std::string& output, SerializationMode m);
  static bool SerializeToFile(const SEConditionManager& src, const std::string& filename, SerializationMode m);
  static bool SerializeFromString(const std::string& src, SEConditionManager& dst, SerializationMode m);
  static bool SerializeFromFile(const std::string& filename, SEConditionManager& dst, SerializationMode m);

  static bool SerializeToString(const SEActionManager& src, std::string& output, SerializationMode m);
  static bool SerializeToFile(const SEActionManager& src, const std::string& filename, SerializationMode m);
  static bool SerializeFromString(const std::string& src, SEActionManager& dst, SerializationMode m);
  static bool SerializeFromFile(const std::string& filename, SEActionManager& dst, SerializationMode m);

  static bool SerializeToString(const SEDataRequestManager& src, std::string& output, SerializationMode m);
  static bool SerializeToFile(const SEDataRequestManager& src, const std::string& filename, SerializationMode m);
  static bool SerializeFromString(const std::string& src, SEDataRequestManager& dst, SerializationMode m, const SESubstanceManager& subMgr);
  static bool SerializeFromFile(const std::string& filename, SEDataRequestManager& dst, SerializationMode m, const SESubstanceManager& subMgr);
};