/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "system/environment/actions/SEEnvironmentAction.h"
class SESubstanceManager;
class SEEnvironmentalConditions;

class CDM_DECL SEChangeEnvironmentConditions : public SEEnvironmentAction
{
  friend class PBEnvironmentAction;//friend the serialization class
public:

  SEChangeEnvironmentConditions(SESubstanceManager& substances);
  virtual ~SEChangeEnvironmentConditions();

  virtual void Clear();
  virtual void Copy(const SEChangeEnvironmentConditions& src);

  virtual bool IsValid() const;

  virtual bool HasConditions() const;
  virtual SEEnvironmentalConditions& GetConditions();
  virtual const SEEnvironmentalConditions* GetConditions() const;

  virtual std::string GetConditionsFile() const;
  virtual void SetConditionsFile(const std::string& fileName);
  virtual bool HasConditionsFile() const;
  virtual void InvalidateConditionsFile();
  
  virtual void ToString(std::ostream &str) const;

protected:
  SESubstanceManager&  m_Substances;

  std::string                m_ConditionsFile;
  SEEnvironmentalConditions* m_Conditions;
};  
