/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
class SEAction;
class SEEnvironmentAction;
class SESubstanceManager;
class SEThermalApplication;
class SEChangeEnvironmentalConditions;
CDM_BIND_DECL(ActionListData)
CDM_BIND_DECL(AnyEnvironmentActionData)

class CDM_DECL SEEnvironmentActionCollection : public Loggable
{
  friend class PBEngine;//friend the serialization class
  friend class SEActionManager;
protected:
  SEEnvironmentActionCollection(SESubstanceManager&);
public:
  ~SEEnvironmentActionCollection();

  bool HasChangeEnvironmentalConditions() const;
  SEChangeEnvironmentalConditions* GetChangeEnvironmentalConditions();
  const SEChangeEnvironmentalConditions* GetChangeEnvironmentalConditions() const;
  void RemoveChangeEnvironmentalConditions();

  bool HasThermalApplication() const;
  SEThermalApplication* GetThermalApplication();
  const SEThermalApplication* GetThermalApplication() const;
  void RemoveThermalApplication();

  void GetAllActions(std::vector<const SEAction*>& v) const;

protected:
  void Clear();
  bool ProcessAction(const SEEnvironmentAction& action);
  
  SEChangeEnvironmentalConditions*  m_ChangeEnvironmentalConditions;
  SEThermalApplication*             m_ThermalApplication;
  // General
  SESubstanceManager&             m_Substances;
};
