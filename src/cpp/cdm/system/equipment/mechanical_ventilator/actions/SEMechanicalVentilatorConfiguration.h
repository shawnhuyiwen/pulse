/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorAction.h"
class SEMechanicalVentilatorSettings;
class SESubstanceManager;

class CDM_DECL SEMechanicalVentilatorConfiguration : public SEMechanicalVentilatorAction
{
  friend class PBEquipmentAction;//friend the serialization class
public:

  SEMechanicalVentilatorConfiguration(Logger* logger=nullptr);
  virtual ~SEMechanicalVentilatorConfiguration();

  virtual void Clear();
  virtual void Copy(const SEMechanicalVentilatorConfiguration& src, const SESubstanceManager& subMgr, bool /*preserveState*/ = false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  bool HasSettings() const;
  SEMechanicalVentilatorSettings& GetSettings();
  const SEMechanicalVentilatorSettings* GetSettings() const;

  virtual std::string GetSettingsFile() const;
  virtual void SetSettingsFile(const std::string& fileName);
  virtual bool HasSettingsFile() const;

  virtual eMergeType GetMergeType() const;
  virtual void SetMergeType(eMergeType m);

  virtual void ToString(std::ostream &str) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:

  std::string                       m_SettingsFile;
  SEMechanicalVentilatorSettings*   m_Settings;
  eMergeType                        m_MergeType;
};