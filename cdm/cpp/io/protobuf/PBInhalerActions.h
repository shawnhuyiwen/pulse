/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL2(InhalerConfiguration)
CDM_BIND_DECL2(InhalerAction)
CDM_BIND_DECL(AnyInhalerActionData)
class SESubstanceManager;

class CDM_DECL PBInhalerAction
{
public:
  /** Create a new action based on the binding object, load that data into the new action, and return said action */
  static SEInhalerAction* Load(const CDM_BIND::AnyInhalerActionData& any, SESubstanceManager& subMgr);
  /** Create a new bind object, unload the action, put that in the bind object, and return said bind object */
  static CDM_BIND::AnyInhalerActionData* Unload(const SEInhalerAction& action);
  static void Serialize(const CDM_BIND::InhalerActionData& src, SEInhalerAction& dst);
  static void Serialize(const SEInhalerAction& src, CDM_BIND::InhalerActionData& dst);

  static void Load(const CDM_BIND::InhalerConfigurationData& src, SEInhalerConfiguration& dst);
  static CDM_BIND::InhalerConfigurationData* Unload(const SEInhalerConfiguration& src);
  static void Serialize(const CDM_BIND::InhalerConfigurationData& src, SEInhalerConfiguration& dst);
  static void Serialize(const SEInhalerConfiguration& src, CDM_BIND::InhalerConfigurationData& dst);
  static void Copy(const SEInhalerConfiguration& src, SEInhalerConfiguration& dst);

};