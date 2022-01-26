/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEPatientAction.h"
class SERespiratoryMechanics;

class CDM_DECL SERespiratoryMechanicsConfiguration : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SERespiratoryMechanicsConfiguration(Logger* logger=nullptr);
  virtual ~SERespiratoryMechanicsConfiguration();

  virtual void Clear();
  virtual void Copy(const SERespiratoryMechanicsConfiguration& src, bool /*preserveState*/=false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  bool HasSettings() const;
  SERespiratoryMechanics& GetSettings();
  const SERespiratoryMechanics* GetSettings() const;

  virtual std::string GetSettingsFile() const;
  virtual void SetSettingsFile(const std::string& fileName);
  virtual bool HasSettingsFile() const;

  virtual eAppliedRespiratoryCycle GetAppliedRespiratoryCycle() const;
  virtual void SetAppliedRespiratoryCycle(eAppliedRespiratoryCycle c);

  virtual eMergeType GetMergeType() const;
  virtual void SetMergeType(eMergeType m);

  virtual void ToString(std::ostream &str) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:
  eAppliedRespiratoryCycle m_AppliedRespiratoryCycle;
  eMergeType               m_MergeType;
  std::string              m_SettingsFile;
  SERespiratoryMechanics*  m_Settings;
};