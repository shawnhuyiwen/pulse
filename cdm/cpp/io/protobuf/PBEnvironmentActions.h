/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL2(ChangeEnvironmentConditions)
CDM_BIND_DECL2(EnvironmentAction)
CDM_BIND_DECL2(ThermalApplication)
CDM_BIND_DECL(AnyEnvironmentActionData)
class SESubstanceManager;

class CDM_DECL PBEnvironmentAction
{
public:

  /** Create a new action based on the binding object, load that data into the new action, and return said action */
  static SEEnvironmentAction* Load(const CDM_BIND::AnyEnvironmentActionData& any, SESubstanceManager& subMgr);
  /** Create a new bind object, unload the action, put that in the bind object, and return said bind object */
  static CDM_BIND::AnyEnvironmentActionData* Unload(const SEEnvironmentAction& action);
  static void Serialize(const CDM_BIND::EnvironmentActionData& src, SEEnvironmentAction& dst);
  static void Serialize(const SEEnvironmentAction& src, CDM_BIND::EnvironmentActionData& dst);
  
  static void Load(const CDM_BIND::ChangeEnvironmentConditionsData& src, SEChangeEnvironmentConditions& dst);
  static CDM_BIND::ChangeEnvironmentConditionsData* Unload(const SEChangeEnvironmentConditions& src);
  static void Serialize(const CDM_BIND::ChangeEnvironmentConditionsData& src, SEChangeEnvironmentConditions& dst);
  static void Serialize(const SEChangeEnvironmentConditions& src, CDM_BIND::ChangeEnvironmentConditionsData& dst);
  static void Copy(const SEChangeEnvironmentConditions& src, SEChangeEnvironmentConditions& dst);

  static void Load(const CDM_BIND::ThermalApplicationData& src, SEThermalApplication& dst);
  static CDM_BIND::ThermalApplicationData* Unload(const SEThermalApplication& src);
  static void Serialize(const CDM_BIND::ThermalApplicationData& src, SEThermalApplication& dst);
  static void Serialize(const SEThermalApplication& src, CDM_BIND::ThermalApplicationData& dst);
  static void Copy(const SEThermalApplication& src, SEThermalApplication& dst);

};