/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SEAction;
class SECondition;
class SEDataRequestManager;
class SEPatientConfiguration;
class SESubstanceManager;

class CDM_DECL SEScenario : public Loggable
{
  friend class PBScenario;//friend the serialization class
public:

  SEScenario(Logger* logger=nullptr, std::string const& dataDir="./");
  virtual ~SEScenario();
  
  virtual void Clear(); //clear memory
  virtual void Copy(const SEScenario& src);

  bool SerializeToString(std::string& output, SerializationFormat m) const;
  bool SerializeToFile(const std::string& filename) const;
  bool SerializeFromString(const std::string& src, SerializationFormat m);
  bool SerializeFromFile(const std::string& filename);

  bool IsValid() const;

  virtual std::string GetName() const;
  virtual void SetName(const std::string& name);
  virtual bool HasName() const;

  virtual std::string GetDescription() const;
  virtual void SetDescription(const std::string& desc);
  virtual bool HasDescription() const;

  virtual std::string GetEngineStateFile() const;
  virtual void SetEngineStateFile(const std::string& file);
  virtual bool HasEngineStateFile() const;
  
  virtual SEPatientConfiguration& GetPatientConfiguration();
  virtual const SEPatientConfiguration* GetPatientConfiguration() const;
  virtual bool HasPatientConfiguration() const;

  virtual void AddAction(const SEAction& action);
  virtual const std::vector<SEAction*>& GetActions();
  virtual const std::vector<const SEAction*>& GetActions() const;

  virtual SEDataRequestManager& GetDataRequestManager() { return *m_DataRequestMgr; }
  virtual const SEDataRequestManager& GetDataRequestManager() const { return *m_DataRequestMgr; }

  virtual SESubstanceManager& GetSubstanceManager() { return *m_SubMgr; }
  virtual const SESubstanceManager& GetSubstanceManager() const { return *m_SubMgr; }

protected:
  SESubstanceManager*                         m_SubMgr;
  std::string                                 m_Name;
  std::string                                 m_Description;
  std::string                                 m_EngineStateFile;
  SEPatientConfiguration*                     m_PatientConfiguration;
  SEDataRequestManager*                       m_DataRequestMgr;
  std::vector<SEAction*>                      m_Actions;
};
