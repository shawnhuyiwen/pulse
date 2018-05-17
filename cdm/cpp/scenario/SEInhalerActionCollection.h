/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

class SEInhalerAction;
class SESubstanceManager;
class SEInhaler;
class SEInhalerConfiguration;
CDM_BIND_DECL(ActionListData)
CDM_BIND_DECL(AnyInhalerActionData)

class CDM_DECL SEInhalerActionCollection : public Loggable
{
  friend class SEActionManager;
protected:
  SEInhalerActionCollection(SESubstanceManager&);
public:
  ~SEInhalerActionCollection();

  // STATE ACTION
  bool HasConfiguration() const;
  SEInhalerConfiguration* GetConfiguration();
  const SEInhalerConfiguration* GetConfiguration() const;
  void RemoveConfiguration();
  
protected:
  void Clear();
  static void Serialize(const SEInhalerActionCollection& src, cdm::ActionListData& dst);
  bool ProcessAction(const SEInhalerAction& action, cdm::AnyInhalerActionData* any);

  SEInhalerConfiguration*   m_Configuration;
  // General
  SESubstanceManager& m_Substances;
};
