/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "cdm/system/environment/actions/SEEnvironmentAction.h"
class SESubstanceManager;
class SEEnvironmentalConditions;

class CDM_DECL SEChangeEnvironmentalConditions : public SEEnvironmentAction
{
  friend class PBEnvironmentAction;//friend the serialization class
public:

  SEChangeEnvironmentalConditions(Logger* logger=nullptr);
  virtual ~SEChangeEnvironmentalConditions();

  static constexpr char const* Name = "Change Environmental Conditions";
  virtual std::string GetName() const { return Name; }

  virtual void Clear();
  virtual void Copy(const SEChangeEnvironmentalConditions& src, const SESubstanceManager& subMgr, bool /*preserveState*/=false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual bool HasEnvironmentalConditions() const;
  virtual SEEnvironmentalConditions& GetEnvironmentalConditions();
  virtual const SEEnvironmentalConditions* GetEnvironmentalConditions() const;

  virtual std::string GetEnvironmentalConditionsFile() const;
  virtual void SetEnvironmentalConditionsFile(const std::string& fileName);
  virtual bool HasEnvironmentalConditionsFile() const;

  virtual eMergeType GetMergeType() const;
  virtual void SetMergeType(eMergeType m);

  virtual const SEScalar* GetScalar(const std::string& name);

protected:

  std::string                m_EnvironmentalConditionsFile;
  SEEnvironmentalConditions* m_EnvironmentalConditions;
  eMergeType                 m_MergeType;
};  
