/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineAction.h"
class SEAnesthesiaMachine;
class SESubstanceManager;

class CDM_DECL SEAnesthesiaMachineConfiguration : public SEAnesthesiaMachineAction
{
  friend class PBEquipmentAction;//friend the serialization class
public:

  SEAnesthesiaMachineConfiguration(Logger* logger=nullptr);
  virtual ~SEAnesthesiaMachineConfiguration();

  static constexpr char const* Name = "Configuration";
  std::string GetName() const override{ return Name; }

  void Clear() override;
  virtual void Copy(const SEAnesthesiaMachineConfiguration& src, const SESubstanceManager& subMgr, bool /*preserveState*/=false);

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

  bool HasConfiguration() const;
  SEAnesthesiaMachine& GetConfiguration();
  const SEAnesthesiaMachine* GetConfiguration() const;

  virtual std::string GetConfigurationFile() const;
  virtual void SetConfigurationFile(const std::string& fileName);
  virtual bool HasConfigurationFile() const;

  virtual eMergeType GetMergeType() const;
  virtual void SetMergeType(eMergeType m);

  virtual const SEScalar* GetScalar(const std::string& name) override;

protected:

  std::string          m_ConfigurationFile;
  SEAnesthesiaMachine* m_Configuration;
  eMergeType           m_MergeType;
};