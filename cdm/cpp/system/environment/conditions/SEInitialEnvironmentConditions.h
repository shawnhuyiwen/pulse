/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "SEEnvironmentCondition.h"
class SEEnvironmentalConditions;

class CDM_DECL SEInitialEnvironmentConditions : public SEEnvironmentCondition
{
  friend class PBEnvironmentCondition;//friend the serialization class
public:

  SEInitialEnvironmentConditions(SESubstanceManager& substances);
  virtual ~SEInitialEnvironmentConditions();

  virtual void Clear();
  virtual void Copy(const SEInitialEnvironmentConditions& src);

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual std::string GetName() const{ return "InitialEnvironment"; }

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
