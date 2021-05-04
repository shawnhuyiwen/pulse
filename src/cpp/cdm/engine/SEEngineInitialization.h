/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SEPatientConfiguration;
class SEDataRequestManager;
class SESubstanceManager;

class CDM_DECL SEEngineInitialization : public Loggable
{
  friend class PBEngine;//friend the serialization class
public:
  explicit SEEngineInitialization(Logger* logger=nullptr);
  virtual ~SEEngineInitialization();

  SEEngineInitialization(const SEEngineInitialization&) = delete;
  SEEngineInitialization operator=(const SEEngineInitialization&) = delete;

  virtual void Copy(const SEEngineInitialization& from, const SESubstanceManager& subMgr);
  virtual void Clear(); //clear memory

  bool SerializeToString(std::string& output, SerializationFormat m) const;
  bool SerializeFromString(const std::string& src, SerializationFormat m, const SESubstanceManager& subMgr);
  static bool SerializeFromString(const std::string& src, std::vector<SEEngineInitialization*>& dst, SerializationFormat m, const SESubstanceManager& subMgr);

  virtual bool IsValid()const;

  virtual int GetID() const;
  virtual void SetID(int id);

  virtual bool HasPatientConfiguration() const;
  virtual SEPatientConfiguration& GetPatientConfiguration();
  virtual const SEPatientConfiguration* GetPatientConfiguration() const;

  virtual bool HasStateFilename() const;
  virtual std::string GetStateFilename() const;
  virtual void SetStateFilename(const std::string& filename);

  virtual bool HasState() const;
  virtual std::string GetState() const;
  virtual void SetState(const std::string& s, SerializationFormat fmt);
  virtual SerializationFormat GetStateFormat() const;

  virtual bool HasDataRequestManager() const;
  virtual SEDataRequestManager& GetDataRequestManager();
  virtual const SEDataRequestManager* GetDataRequestManager() const;

  virtual bool HasLogFilename() const;
  virtual std::string GetLogFilename() const;
  virtual void SetLogFilename(const std::string& filename);

  virtual bool KeepLogMessages() const;
  virtual void KeepLogMessages(bool b);

  virtual bool KeepEventChanges() const;
  virtual void KeepEventChanges(bool b);

protected:
  int                             m_ID;
  SEPatientConfiguration*         m_PatientConfiguration;
  std::string                     m_StateFilename;
  std::string                     m_State;
  SerializationFormat             m_StateFormat;
  SEDataRequestManager*           m_DataRequestManager;
  std::string                     m_LogFilename;
  bool                            m_KeepLogMessages;
  bool                            m_KeepEventChanges;
};
