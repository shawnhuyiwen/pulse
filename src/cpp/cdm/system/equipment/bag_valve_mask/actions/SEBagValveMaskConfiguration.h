/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "system/equipment/bag_valve_mask/actions/SEBagValveMaskAction.h"
class SEBagValveMask;
class SESubstanceManager;

class CDM_DECL SEBagValveMaskConfiguration : public SEBagValveMaskAction
{
  friend class PBEquipmentAction;//friend the serialization class
public:

  SEBagValveMaskConfiguration(Logger* logger=nullptr);
  virtual ~SEBagValveMaskConfiguration();

  virtual void Clear();
  virtual void Copy(const SEBagValveMaskConfiguration& src, const SESubstanceManager& subMgr, bool preserveState = false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  bool HasConfiguration() const;
  SEBagValveMask& GetConfiguration();
  const SEBagValveMask* GetConfiguration() const;

  virtual std::string GetConfigurationFile() const;
  virtual void SetConfigurationFile(const std::string& fileName);
  virtual bool HasConfigurationFile() const;

  virtual void ToString(std::ostream &str) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:

  std::string       m_ConfigurationFile;
  SEBagValveMask*   m_Configuration;
};