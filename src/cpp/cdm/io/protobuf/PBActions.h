/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL(AnyActionData)
CDM_BIND_DECL2(Action)
CDM_BIND_DECL2(AdvanceTime)
CDM_BIND_DECL2(SerializeState)
CDM_BIND_DECL2(Overrides)
class SESubstanceManager;

class CDM_DECL PBAction
{
public:

  /** Create a new action based on the binding object, load that data into the new action, and return said action */
  static SEAction* Load(const CDM_BIND::AnyActionData& action, const SESubstanceManager& subMgr);
  /** Create a new bind object, unload the action, put that in the bind object, and return said bind object */
  static CDM_BIND::AnyActionData* Unload(const SEAction& action);
  static SEAction* Copy(const SEAction& a, const SESubstanceManager& subMgr);

  static void Serialize(const CDM_BIND::ActionData& src, SEAction& dst);
  static void Serialize(const SEAction& src, CDM_BIND::ActionData& dst);

  static void Load(const CDM_BIND::AdvanceTimeData& src, SEAdvanceTime& dst);
  static CDM_BIND::AdvanceTimeData* Unload(const SEAdvanceTime& src);
  static void Serialize(const CDM_BIND::AdvanceTimeData& src, SEAdvanceTime& dst);
  static void Serialize(const SEAdvanceTime& src, CDM_BIND::AdvanceTimeData& dst);

  static void Load(const CDM_BIND::SerializeStateData& src, SESerializeState& dst);
  static CDM_BIND::SerializeStateData* Unload(const SESerializeState& src);
  static void Serialize(const CDM_BIND::SerializeStateData& src, SESerializeState& dst);
  static void Serialize(const SESerializeState& src, CDM_BIND::SerializeStateData& dst);

  static void Load(const CDM_BIND::OverridesData& src, SEOverrides& dst);
  static CDM_BIND::OverridesData* Unload(const SEOverrides& src);
  static void Serialize(const CDM_BIND::OverridesData& src, SEOverrides& dst);
  static void Serialize(const SEOverrides& src, CDM_BIND::OverridesData& dst);
  static void Copy(const SEOverrides& src, SEOverrides& dst);
};