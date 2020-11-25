/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "SEEnvironmentCondition.h"
class SEEnvironmentalConditions;

class CDM_DECL SEInitialEnvironmentalConditions : public SEEnvironmentCondition
{
  friend class PBEnvironmentCondition;//friend the serialization class
public:

  SEInitialEnvironmentalConditions(Logger* logger=nullptr);
  virtual ~SEInitialEnvironmentalConditions();

  virtual void Clear();
  virtual void Copy(const SEInitialEnvironmentalConditions& src, const SESubstanceManager& subMgr);

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual std::string GetName() const{ return "InitialEnvironmentalConditions"; }

  virtual bool HasEnvironmentalConditions() const;
  virtual SEEnvironmentalConditions& GetEnvironmentalConditions();
  virtual const SEEnvironmentalConditions* GetEnvironmentalConditions() const;

  virtual std::string GetEnvironmentalConditionsFile() const;
  virtual void SetEnvironmentalConditionsFile(const std::string& fileName);
  virtual bool HasEnvironmentalConditionsFile() const;

  virtual void ToString(std::ostream &str) const;

protected:
  std::string                m_EnvironmentalConditionsFile;
  SEEnvironmentalConditions* m_EnvironmentalConditions;
};  
