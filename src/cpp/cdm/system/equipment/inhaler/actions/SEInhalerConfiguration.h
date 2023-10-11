/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once

#include "cdm/system/equipment/inhaler/actions/SEInhalerAction.h"
class SEInhaler;

class CDM_DECL SEInhalerConfiguration : public SEInhalerAction
{
  friend class PBEquipmentAction;//friend the serialization class
public:

  SEInhalerConfiguration(Logger* logger=nullptr);
  virtual ~SEInhalerConfiguration();

  static constexpr char const* Name = "Configuration";
  std::string GetName() const override { return Name; }

  void Clear() override;
  virtual void Copy(const SEInhalerConfiguration& src, const SESubstanceManager& subMgr, bool /*preserveState*/ = false);

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

  bool HasConfiguration() const;
  SEInhaler& GetConfiguration();
  const SEInhaler* GetConfiguration() const;

  virtual std::string GetConfigurationFile() const;
  virtual void SetConfigurationFile(const std::string& fileName);
  virtual bool HasConfigurationFile() const;

  virtual eMergeType GetMergeType() const;
  virtual void SetMergeType(eMergeType m);

  const SEScalar* GetScalar(const std::string& name) override;

protected:

  std::string m_ConfigurationFile;
  SEInhaler*  m_Configuration;
  eMergeType  m_MergeType;
};
