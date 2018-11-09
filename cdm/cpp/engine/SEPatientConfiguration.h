/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SEEngineConfiguration;
class SEPatient;
class SECondition;
class SESubstanceManager;

class CDM_DECL SEPatientConfiguration : public Loggable
{
  friend class PBEngine;//friend the serialization class
public:
  SEPatientConfiguration(Logger* logger);
  virtual ~SEPatientConfiguration();
  virtual void Clear(); //clear memory

  bool SerializeToString(std::string& output, SerializationFormat m) const;
  bool SerializeToFile(const std::string& filename, SerializationFormat m) const;
  bool SerializeFromString(const std::string& src, SerializationFormat m, SESubstanceManager& subMgr);
  bool SerializeFromFile(const std::string& filename, SerializationFormat m, SESubstanceManager& subMgr);

  virtual bool IsValid()const;

  virtual std::string GetPatientFile() const;
  virtual void SetPatientFile(const std::string& patientFile);
  virtual bool HasPatientFile() const;
  virtual void InvalidatePatientFile();

  virtual SEPatient& GetPatient();
  virtual const SEPatient* GetPatient() const;
  virtual bool HasPatient() const;
  virtual void InvalidatePatient();

  virtual std::vector<SECondition*>& GetConditions();
  virtual const std::vector<const SECondition*>& GetConditions() const;

protected:
  SEPatient*                      m_Patient;
  std::string                     m_PatientFile;
  std::vector<SECondition*>       m_Conditions;
  mutable std::vector<const SECondition*> m_cConditions;
};