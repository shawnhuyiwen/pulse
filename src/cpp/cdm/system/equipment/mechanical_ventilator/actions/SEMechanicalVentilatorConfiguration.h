/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorAction.h"
class SEMechanicalVentilator;
class SESubstanceManager;

class CDM_DECL SEMechanicalVentilatorConfiguration : public SEMechanicalVentilatorAction
{
  friend class PBEquipmentAction;//friend the serialization class
public:

  SEMechanicalVentilatorConfiguration(Logger* logger=nullptr);
  virtual ~SEMechanicalVentilatorConfiguration();

  virtual void Clear();
  virtual void Copy(const SEMechanicalVentilatorConfiguration& src, const SESubstanceManager& subMgr, bool preserveState = false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  bool HasConfiguration() const;
  SEMechanicalVentilator& GetConfiguration();
  const SEMechanicalVentilator* GetConfiguration() const;

  virtual std::string GetConfigurationFile() const;
  virtual void SetConfigurationFile(const std::string& fileName);
  virtual bool HasConfigurationFile() const;

  virtual void ToString(std::ostream &str) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:

  std::string               m_ConfigurationFile;
  SEMechanicalVentilator*   m_Configuration;
};