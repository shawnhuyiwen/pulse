/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "cdm/system/environment/conditions/SEEnvironmentCondition.h"
class SEEnvironmentalConditions;

class CDM_DECL SEInitialEnvironmentalConditions : public SEEnvironmentCondition
{
  friend class PBEnvironmentCondition;//friend the serialization class
public:

  SEInitialEnvironmentalConditions(Logger* logger=nullptr);
  virtual ~SEInitialEnvironmentalConditions();

  static constexpr char const* Name = "Initial Environmental Conditions";
  std::string GetName() const override { return Name; }

  void Clear() override;
  virtual void Copy(const SEInitialEnvironmentalConditions& src, const SESubstanceManager& subMgr);

  bool IsValid() const override;
  bool IsActive() const override;

  virtual bool HasEnvironmentalConditions() const;
  virtual SEEnvironmentalConditions& GetEnvironmentalConditions();
  virtual const SEEnvironmentalConditions* GetEnvironmentalConditions() const;

  virtual std::string GetEnvironmentalConditionsFile() const;
  virtual void SetEnvironmentalConditionsFile(const std::string& fileName);
  virtual bool HasEnvironmentalConditionsFile() const;

protected:
  std::string                m_EnvironmentalConditionsFile;
  SEEnvironmentalConditions* m_EnvironmentalConditions;
};  
