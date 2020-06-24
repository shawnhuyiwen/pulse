/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL2(InitialEnvironmentalConditions)
CDM_BIND_DECL2(EnvironmentCondition)
CDM_BIND_DECL(AnyEnvironmentConditionData)
class SESubstanceManager;

class CDM_DECL PBEnvironmentCondition
{
public:

  /** Create a new condition based on the binding object, load that data into the new condition, and return said condition */
  static SEEnvironmentCondition* Load(const CDM_BIND::AnyEnvironmentConditionData& any, const SESubstanceManager& subMgr);
  /** Create a new bind object, unload the action, put that in the bind object, and return said bind object */
  static CDM_BIND::AnyEnvironmentConditionData* Unload(const SEEnvironmentCondition& condition);
  static void Serialize(const CDM_BIND::EnvironmentConditionData& src, SEEnvironmentCondition& dst);
  static void Serialize(const SEEnvironmentCondition& src, CDM_BIND::EnvironmentConditionData& dst);

  static void Load(const CDM_BIND::InitialEnvironmentalConditionsData& src, SEInitialEnvironmentalConditions& dst, const SESubstanceManager& subMgr);
  static CDM_BIND::InitialEnvironmentalConditionsData* Unload(const SEInitialEnvironmentalConditions& src);
  static void Serialize(const CDM_BIND::InitialEnvironmentalConditionsData& src, SEInitialEnvironmentalConditions& dst, const SESubstanceManager& subMgr);
  static void Serialize(const SEInitialEnvironmentalConditions& src, CDM_BIND::InitialEnvironmentalConditionsData& dst);
  static void Copy(const SEInitialEnvironmentalConditions& src, SEInitialEnvironmentalConditions& dst, const SESubstanceManager& subMgr);

};