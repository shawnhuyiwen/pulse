/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SEAction;
class SEInhalerAction;
class SESubstanceManager;
class SEInhaler;
class SEInhalerConfiguration;
CDM_BIND_DECL(ActionListData)
CDM_BIND_DECL(AnyInhalerActionData)

class CDM_DECL SEInhalerActionCollection : public Loggable
{
  friend class PBScenario;//friend the serialization class
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

  void GetAllActions(std::vector<const SEAction*>& v) const;
  
protected:
  void Clear();
  bool ProcessAction(const SEInhalerAction& action);

  SEInhalerConfiguration*   m_Configuration;
  // General
  SESubstanceManager& m_Substances;
};
