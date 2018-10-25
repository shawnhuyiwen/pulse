/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL(AnyActionData)
CDM_BIND_DECL2(Action)
CDM_BIND_DECL2(AdvanceTime)
CDM_BIND_DECL2(SerializeState)
class SESubstanceManager;

class CDM_DECL PBAction
{
public:

  /** Create a new action based on the binding object, load that data into the new action, and return said action */
  static SEAction* Load(const cdm::AnyActionData& action, SESubstanceManager& subMgr);
  /** Create a new bind object, unload the action, put that in the bind object, and return said bind object */
  static cdm::AnyActionData* Unload(const SEAction& action);
  static SEAction* Copy(const SEAction& a, SESubstanceManager& subMgr);

  static void Serialize(const cdm::ActionData& src, SEAction& dst);
  static void Serialize(const SEAction& src, cdm::ActionData& dst);

  static void Load(const cdm::AdvanceTimeData& src, SEAdvanceTime& dst);
  static cdm::AdvanceTimeData* Unload(const SEAdvanceTime& src);
  static void Serialize(const cdm::AdvanceTimeData& src, SEAdvanceTime& dst);
  static void Serialize(const SEAdvanceTime& src, cdm::AdvanceTimeData& dst);

  static void Load(const cdm::SerializeStateData& src, SESerializeState& dst);
  static cdm::SerializeStateData* Unload(const SESerializeState& src);
  static void Serialize(const cdm::SerializeStateData& src, SESerializeState& dst);
  static void Serialize(const SESerializeState& src, cdm::SerializeStateData& dst);

};