/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
class SEEnvironmentAction;
class SESubstanceManager;
class SEThermalApplication;
class SEChangeEnvironmentConditions;
CDM_BIND_DECL(ActionListData)
CDM_BIND_DECL(AnyEnvironmentActionData)

class CDM_DECL SEEnvironmentActionCollection : public Loggable
{
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

protected:
  void Clear();
  static void Serialize(const SEEnvironmentActionCollection& src, cdm::ActionListData& dst);
  bool ProcessAction(const SEEnvironmentAction& action, cdm::AnyEnvironmentActionData* any);
  
  SEChangeEnvironmentConditions*  m_Change;
  SEThermalApplication*           m_ThermalApplication;
  // General
  SESubstanceManager&             m_Substances;
};
