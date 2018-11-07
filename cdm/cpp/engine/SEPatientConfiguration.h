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
  friend class SEScenario;
protected:
  SEPatientConfiguration(SESubstanceManager& subMgr);
public:
  virtual ~SEPatientConfiguration();
  virtual void Clear(); //clear memory

public:

  virtual bool IsValid()const;

  virtual std::string GetPatientFile() const;
  virtual void SetPatientFile(const std::string& patientFile);
  virtual bool HasPatientFile() const;
  virtual void InvalidatePatientFile();

  virtual SEPatient& GetPatient();
  virtual const SEPatient* GetPatient() const;
  virtual bool HasPatient() const;
  virtual void InvalidatePatient();

  virtual const std::vector<SECondition*>& GetConditions() const;

protected:
  SESubstanceManager&            m_SubMgr;
  SEPatient*                     m_Patient;
  std::string                    m_PatientFile;
  std::vector<SECondition*>      m_Conditions;
};