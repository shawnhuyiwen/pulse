/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL2(InitialEnvironmentConditions)
CDM_BIND_DECL2(EnvironmentCondition)
CDM_BIND_DECL(AnyEnvironmentConditionData)
class SESubstanceManager;

class CDM_DECL PBEnvironmentCondition
{
public:

  /** Create a new condition based on the binding object, load that data into the new condition, and return said condition */
  static SEEnvironmentCondition* Load(const cdm::AnyEnvironmentConditionData& any, SESubstanceManager& subMgr);
  /** Create a new bind object, unload the action, put that in the bind object, and return said bind object */
  static cdm::AnyEnvironmentConditionData* Unload(const SEEnvironmentCondition& condition);
  static void Serialize(const cdm::EnvironmentConditionData& src, SEEnvironmentCondition& dst);
  static void Serialize(const SEEnvironmentCondition& src, cdm::EnvironmentConditionData& dst);

  static void Load(const cdm::InitialEnvironmentConditionsData& src, SEInitialEnvironmentConditions& dst);
  static cdm::InitialEnvironmentConditionsData* Unload(const SEInitialEnvironmentConditions& src);
  static void Serialize(const cdm::InitialEnvironmentConditionsData& src, SEInitialEnvironmentConditions& dst);
  static void Serialize(const SEInitialEnvironmentConditions& src, cdm::InitialEnvironmentConditionsData& dst);
  static void Copy(const SEInitialEnvironmentConditions& src, SEInitialEnvironmentConditions& dst);

};