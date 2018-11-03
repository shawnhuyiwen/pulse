/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
class SEAction;
class SEEnvironmentAction;
class SESubstanceManager;
class SEThermalApplication;
class SEChangeEnvironmentConditions;
CDM_BIND_DECL(ActionListData)
CDM_BIND_DECL(AnyEnvironmentActionData)

class CDM_DECL SEEnvironmentActionCollection : public Loggable
{
  friend class PBScenario;//friend the serialization class
  friend class SEActionManager;
protected:
  SEEnvironmentActionCollection(SESubstanceManager&);
public:
  ~SEEnvironmentActionCollection();

  bool HasChange() const;
  SEChangeEnvironmentConditions* GetChange();
  const SEChangeEnvironmentConditions* GetChange() const;
  void RemoveChange();

  bool HasThermalApplication() const;
  SEThermalApplication* GetThermalApplication();
  const SEThermalApplication* GetThermalApplication() const;
  void RemoveThermalApplication();

  void GetAllActions(std::vector<const SEAction*>& v) const;

protected:
  void Clear();
  bool ProcessAction(const SEEnvironmentAction& action);
  
  SEChangeEnvironmentConditions*  m_Change;
  SEThermalApplication*           m_ThermalApplication;
  // General
  SESubstanceManager&             m_Substances;
};
