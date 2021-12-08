/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SEEngineConfiguration;
class SEPatient;
class SEConditionManager;
class SESubstanceManager;

class CDM_DECL SEPatientConfiguration : public Loggable
{
  friend class PBEngine;//friend the serialization class
public:
  explicit SEPatientConfiguration(Logger* logger=nullptr);
  virtual ~SEPatientConfiguration();

  SEPatientConfiguration(const SEPatientConfiguration&) = delete;
  SEPatientConfiguration operator=(const SEPatientConfiguration&) = delete;

  virtual void Clear(); //clear memory

  bool SerializeToString(std::string& output, eSerializationFormat m) const;
  bool SerializeToFile(const std::string& filename) const;
  bool SerializeFromString(const std::string& src, eSerializationFormat m, const SESubstanceManager& subMgr);
  bool SerializeFromFile(const std::string& filename, const SESubstanceManager& subMgr);

  virtual bool IsValid()const;

  virtual std::string GetDataRoot() const;
  virtual void SetDataRoot(const std::string& rootDir);

  virtual std::string GetPatientFile() const;
  virtual void SetPatientFile(const std::string& patientFile);
  virtual bool HasPatientFile() const;

  virtual SEPatient& GetPatient();
  virtual const SEPatient* GetPatient() const;
  virtual bool HasPatient() const;

  virtual SEConditionManager& GetConditions();
  virtual const SEConditionManager* GetConditions() const;
  virtual bool HasConditions() const;

protected:
  std::string                     m_DataRoot;
  SEPatient*                      m_Patient;
  std::string                     m_PatientFile;
  SEConditionManager*             m_Conditions;
};