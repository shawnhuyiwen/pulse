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
  std::string GetName() const override { return Name; }

  void Clear() override;
  virtual void Copy(const SEECMOConfiguration& src, const SESubstanceManager& subMgr, bool /*preserveState*/ = false);

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

  bool HasSettings() const;
  SEECMOSettings& GetSettings();
  const SEECMOSettings* GetSettings() const;

  virtual std::string GetSettingsFile() const;
  virtual void SetSettingsFile(const std::string& fileName);
  virtual bool HasSettingsFile() const;

  virtual eMergeType GetMergeType() const;
  virtual void SetMergeType(eMergeType m);

  const SEScalar* GetScalar(const std::string& name) override;

protected:

  std::string                       m_SettingsFile;
  SEECMOSettings*                   m_Settings;
  eMergeType                        m_MergeType;
};