/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SEAction;
class SEPatientActionCollection;
class SEEnvironmentActionCollection;
class SEEquipmentActionCollection;
class SESubstanceManager;

class CDM_DECL SEActionManager : public Loggable
{
  friend class PBEngine;//friend the serialization class
public:

  SEActionManager(Logger* logger=nullptr);
  ~SEActionManager();

  void Clear();
  
  bool SerializeToString(std::string& output, SerializationFormat m) const;
  bool SerializeToFile(const std::string& filename) const;
  bool SerializeFromString(const std::string& src, SerializationFormat m, SESubstanceManager& subMgr);
  bool SerializeFromFile(const std::string& filename, SESubstanceManager& subMgr);

  static bool SerializeFromString(const std::string& src, std::vector<SEAction*>& dst, SerializationFormat m, const SESubstanceManager& subMgr);

  bool ProcessAction(const SEAction& action, SESubstanceManager&);// Will make a copy

  SEPatientActionCollection&                 GetPatientActions()           { return *m_PatientActions; }
  SEEnvironmentActionCollection&             GetEnvironmentActions()       { return *m_EnvironmentActions; }
  SEEquipmentActionCollection&               GetEquipmentActions()         { return *m_EquipmentActions; }

  const SEPatientActionCollection&           GetPatientActions()           const { return *m_PatientActions; }
  const SEEnvironmentActionCollection&       GetEnvironmentActions()       const { return *m_EnvironmentActions; }
  const SEEquipmentActionCollection&         GetEquipmentActions()         const { return *m_EquipmentActions; }

  void GetAllActions(std::vector<const SEAction*>& v) const;

protected:

  SEPatientActionCollection*           m_PatientActions;
  SEEnvironmentActionCollection*       m_EnvironmentActions;
  SEEquipmentActionCollection*         m_EquipmentActions;

  std::stringstream m_ss;
};