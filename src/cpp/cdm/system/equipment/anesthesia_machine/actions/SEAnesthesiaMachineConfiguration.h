/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineAction.h"
class SEAnesthesiaMachine;
class SESubstanceManager;

class CDM_DECL SEAnesthesiaMachineConfiguration : public SEAnesthesiaMachineAction
{
  friend class PBEquipmentAction;//friend the serialization class
public:

  SEAnesthesiaMachineConfiguration(Logger* logger=nullptr);
  virtual ~SEAnesthesiaMachineConfiguration();

  virtual void Clear();
  virtual void Copy(const SEAnesthesiaMachineConfiguration& src, const SESubstanceManager& subMgr, bool preserveState=false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  bool HasConfiguration() const;
  SEAnesthesiaMachine& GetConfiguration();
  const SEAnesthesiaMachine* GetConfiguration() const;

  virtual std::string GetConfigurationFile() const;
  virtual void SetConfigurationFile(const std::string& fileName);
  virtual bool HasConfigurationFile() const;

  virtual void ToString(std::ostream &str) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:

  std::string          m_ConfigurationFile;
  SEAnesthesiaMachine* m_Configuration;
};