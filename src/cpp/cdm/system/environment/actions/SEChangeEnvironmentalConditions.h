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
  std::string GetName() const override { return Name; }

  void Clear() override;
  virtual void Copy(const SEChangeEnvironmentalConditions& src, const SESubstanceManager& subMgr, bool /*preserveState*/=false);

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

  virtual bool HasEnvironmentalConditions() const;
  virtual SEEnvironmentalConditions& GetEnvironmentalConditions();
  virtual const SEEnvironmentalConditions* GetEnvironmentalConditions() const;

  virtual std::string GetEnvironmentalConditionsFile() const;
  virtual void SetEnvironmentalConditionsFile(const std::string& fileName);
  virtual bool HasEnvironmentalConditionsFile() const;

  virtual eMergeType GetMergeType() const;
  virtual void SetMergeType(eMergeType m);

  const SEScalar* GetScalar(const std::string& name) override;

protected:

  std::string                m_EnvironmentalConditionsFile;
  SEEnvironmentalConditions* m_EnvironmentalConditions;
  eMergeType                 m_MergeType;
};  
