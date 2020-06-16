/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL2(ChangeEnvironmentalConditions)
CDM_BIND_DECL2(EnvironmentAction)
CDM_BIND_DECL2(ThermalApplication)
CDM_BIND_DECL(AnyEnvironmentActionData)
class SESubstanceManager;

class CDM_DECL PBEnvironmentAction
{
public:

  /** Create a new action based on the binding object, load that data into the new action, and return said action */
  static SEEnvironmentAction* Load(const CDM_BIND::AnyEnvironmentActionData& any, const SESubstanceManager& subMgr);
  /** Create a new bind object, unload the action, put that in the bind object, and return said bind object */
  static CDM_BIND::AnyEnvironmentActionData* Unload(const SEEnvironmentAction& action);
  static void Serialize(const CDM_BIND::EnvironmentActionData& src, SEEnvironmentAction& dst);
  static void Serialize(const SEEnvironmentAction& src, CDM_BIND::EnvironmentActionData& dst);
  
  static void Load(const CDM_BIND::ChangeEnvironmentalConditionsData& src, SEChangeEnvironmentalConditions& dst, const SESubstanceManager& subMgr);
  static CDM_BIND::ChangeEnvironmentalConditionsData* Unload(const SEChangeEnvironmentalConditions& src);
  static void Serialize(const CDM_BIND::ChangeEnvironmentalConditionsData& src, SEChangeEnvironmentalConditions& dst, const SESubstanceManager& subMgr);
  static void Serialize(const SEChangeEnvironmentalConditions& src, CDM_BIND::ChangeEnvironmentalConditionsData& dst);
  static void Copy(const SEChangeEnvironmentalConditions& src, SEChangeEnvironmentalConditions& dst, const SESubstanceManager& subMgr);

  static void Load(const CDM_BIND::ThermalApplicationData& src, SEThermalApplication& dst);
  static CDM_BIND::ThermalApplicationData* Unload(const SEThermalApplication& src);
  static void Serialize(const CDM_BIND::ThermalApplicationData& src, SEThermalApplication& dst);
  static void Serialize(const SEThermalApplication& src, CDM_BIND::ThermalApplicationData& dst);
  static void Copy(const SEThermalApplication& src, SEThermalApplication& dst);

};