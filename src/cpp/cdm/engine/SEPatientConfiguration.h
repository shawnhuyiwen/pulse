/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SEEngineConfiguration;
class SEPatient;
class SEConditionManager;
class SESubstanceManager;
class SEScalarProperty;

class CDM_DECL SEPatientConfiguration : public Loggable
{
  friend class PBEngine;//friend the serialization class
public:
  explicit SEPatientConfiguration(Logger* logger=nullptr);
  virtual ~SEPatientConfiguration();

  SEPatientConfiguration(const SEPatientConfiguration&) = delete;
  SEPatientConfiguration operator=(const SEPatientConfiguration&) = delete;

  virtual void Clear(); //clear memory

  bool SerializeToString(std::string& output, SerializationFormat m) const;
  bool SerializeToFile(const std::string& filename) const;
  bool SerializeFromString(const std::string& src, SerializationFormat m, const SESubstanceManager& subMgr);
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

  virtual bool HasOverride() const;
  virtual void AddScalarOverride(const std::string& name, double value);
  virtual void AddScalarOverride(const std::string& name, double value, std::string unit);
  virtual void AddScalarOverride(const std::string& name, double value, const CCompoundUnit& unit);
  virtual std::vector<SEScalarProperty>& GetScalarOverrides();
  virtual const std::vector<SEScalarProperty>& GetScalarOverrides() const;
  virtual void RemoveOverrides();

protected:
  std::string                     m_DataRoot;
  SEPatient*                      m_Patient;
  std::string                     m_PatientFile;
  SEConditionManager*             m_Conditions;
  std::vector<SEScalarProperty>   m_ScalarOverrides;
};