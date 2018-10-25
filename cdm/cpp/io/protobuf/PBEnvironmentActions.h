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
  static SEEnvironmentAction* Load(const cdm::AnyEnvironmentActionData& any, SESubstanceManager& subMgr);
  /** Create a new bind object, unload the action, put that in the bind object, and return said bind object */
  static cdm::AnyEnvironmentActionData* Unload(const SEEnvironmentAction& action);
  static void Serialize(const cdm::EnvironmentActionData& src, SEEnvironmentAction& dst);
  static void Serialize(const SEEnvironmentAction& src, cdm::EnvironmentActionData& dst);
  
  static void Load(const cdm::ChangeEnvironmentConditionsData& src, SEChangeEnvironmentConditions& dst);
  static cdm::ChangeEnvironmentConditionsData* Unload(const SEChangeEnvironmentConditions& src);
  static void Serialize(const cdm::ChangeEnvironmentConditionsData& src, SEChangeEnvironmentConditions& dst);
  static void Serialize(const SEChangeEnvironmentConditions& src, cdm::ChangeEnvironmentConditionsData& dst);
  static void Copy(const SEChangeEnvironmentConditions& src, SEChangeEnvironmentConditions& dst);

  static void Load(const cdm::ThermalApplicationData& src, SEThermalApplication& dst);
  static cdm::ThermalApplicationData* Unload(const SEThermalApplication& src);
  static void Serialize(const cdm::ThermalApplicationData& src, SEThermalApplication& dst);
  static void Serialize(const SEThermalApplication& src, cdm::ThermalApplicationData& dst);
  static void Copy(const SEThermalApplication& src, SEThermalApplication& dst);

};