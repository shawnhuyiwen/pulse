/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "cdm/system/equipment/ecmo/actions/SEECMOAction.h"
class SEECMOSettings;
class SESubstanceManager;

class CDM_DECL SEECMOConfiguration : public SEECMOAction
{
  friend class PBEquipmentAction;//friend the serialization class
public:

  SEECMOConfiguration(Logger* logger=nullptr);
  virtual ~SEECMOConfiguration();

  static constexpr char const* Name = "Configuration";
  virtual std::string GetName() const { return Name; }

  virtual void Clear();
  virtual void Copy(const SEECMOConfiguration& src, const SESubstanceManager& subMgr, bool /*preserveState*/ = false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  bool HasSettings() const;
  SEECMOSettings& GetSettings();
  const SEECMOSettings* GetSettings() const;

  virtual std::string GetSettingsFile() const;
  virtual void SetSettingsFile(const std::string& fileName);
  virtual bool HasSettingsFile() const;

  virtual eMergeType GetMergeType() const;
  virtual void SetMergeType(eMergeType m);

  virtual const SEScalar* GetScalar(const std::string& name);

protected:

  std::string                       m_SettingsFile;
  SEECMOSettings*                   m_Settings;
  eMergeType                        m_MergeType;
};