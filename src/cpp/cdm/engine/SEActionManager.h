/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SEAction;
class SEPatientActionCollection;
class SEAnesthesiaMachineActionCollection;
class SEEnvironmentActionCollection;
class SEInhalerActionCollection;
class SESubstanceManager;

class CDM_DECL SEActionManager : public Loggable
{
  friend class PBEngine;//friend the serialization class
public:

  SEActionManager(SESubstanceManager&);
  ~SEActionManager();

  void Clear();
  
  bool SerializeToString(std::string& output, SerializationFormat m) const;
  bool SerializeToFile(const std::string& filename, SerializationFormat m) const;
  bool SerializeFromString(const std::string& src, SerializationFormat m);
  bool SerializeFromFile(const std::string& filename, SerializationFormat m);

  static bool SerializeFromString(const std::string& src, std::vector<SEAction*>& dst, SerializationFormat m, SESubstanceManager& subMgr);

  bool ProcessAction(const SEAction& action);// Will make a copy

  SEPatientActionCollection&                 GetPatientActions()           { return *m_PatientActions; }
  SEEnvironmentActionCollection&             GetEnvironmentActions()       { return *m_EnvironmentActions; }
  SEAnesthesiaMachineActionCollection&       GetAnesthesiaMachineActions() { return *m_AnesthesiaMachineActions; }
  SEInhalerActionCollection&                 GetInhalerActions()           { return *m_InhalerActions; }

  const SEPatientActionCollection&           GetPatientActions()           const { return *m_PatientActions; }
  const SEEnvironmentActionCollection&       GetEnvironmentActions()       const { return *m_EnvironmentActions; }
  const SEAnesthesiaMachineActionCollection& GetAnesthesiaMachineActions() const { return *m_AnesthesiaMachineActions; }
  const SEInhalerActionCollection&           GetInhalerActions()           const { return *m_InhalerActions; }

  void GetAllActions(std::vector<const SEAction*>& v) const;

protected:

  SESubstanceManager&                  m_Substances;
  SEPatientActionCollection*           m_PatientActions;
  SEEnvironmentActionCollection*       m_EnvironmentActions;
  SEAnesthesiaMachineActionCollection* m_AnesthesiaMachineActions;
  SEInhalerActionCollection*           m_InhalerActions;

  std::stringstream m_ss;
};