/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL2(Condition)
CDM_BIND_DECL(AnyConditionData)
class SESubstanceManager;

class CDM_DECL PBCondition
{
public:

  /** Create a new action based on the binding object, load that data into the new action, and return said action */
  static SECondition* Load(const cdm::AnyConditionData& condition, SESubstanceManager& subMgr);
  /** Create a new bind object, unload the action, put that in the bind object, and return said bind object */
  static cdm::AnyConditionData* Unload(const SECondition& condition);
  static void Serialize(const cdm::ConditionData& src, SECondition& dst);
  static void Serialize(const SECondition& src, cdm::ConditionData& dst);
};