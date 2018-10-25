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
  static SEInhalerAction* Load(const cdm::AnyInhalerActionData& any, SESubstanceManager& subMgr);
  /** Create a new bind object, unload the action, put that in the bind object, and return said bind object */
  static cdm::AnyInhalerActionData* Unload(const SEInhalerAction& action);
  static void Serialize(const cdm::InhalerActionData& src, SEInhalerAction& dst);
  static void Serialize(const SEInhalerAction& src, cdm::InhalerActionData& dst);

  static void Load(const cdm::InhalerConfigurationData& src, SEInhalerConfiguration& dst);
  static cdm::InhalerConfigurationData* Unload(const SEInhalerConfiguration& src);
  static void Serialize(const cdm::InhalerConfigurationData& src, SEInhalerConfiguration& dst);
  static void Serialize(const SEInhalerConfiguration& src, cdm::InhalerConfigurationData& dst);
  static void Copy(const SEInhalerConfiguration& src, SEInhalerConfiguration& dst);

};