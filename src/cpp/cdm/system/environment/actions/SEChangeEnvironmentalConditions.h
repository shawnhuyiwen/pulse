/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "system/environment/actions/SEEnvironmentAction.h"
class SESubstanceManager;
class SEEnvironmentalConditions;

class CDM_DECL SEChangeEnvironmentalConditions : public SEEnvironmentAction
{
  friend class PBEnvironmentAction;//friend the serialization class
public:

  SEChangeEnvironmentalConditions(Logger* logger=nullptr);
  virtual ~SEChangeEnvironmentalConditions();

  virtual void Clear();
  virtual void Copy(const SEChangeEnvironmentalConditions& src, const SESubstanceManager& subMgr, bool preserveState=false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual bool HasEnvironmentalConditions() const;
  virtual SEEnvironmentalConditions& GetEnvironmentalConditions();
  virtual const SEEnvironmentalConditions* GetEnvironmentalConditions() const;

  virtual std::string GetEnvironmentalConditionsFile() const;
  virtual void SetEnvironmentalConditionsFile(const std::string& fileName);
  virtual bool HasEnvironmentalConditionsFile() const;
  
  virtual void ToString(std::ostream &str) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:

  std::string                m_EnvironmentalConditionsFile;
  SEEnvironmentalConditions* m_EnvironmentalConditions;
};  
