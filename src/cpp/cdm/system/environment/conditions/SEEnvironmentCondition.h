/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "cdm/engine/SECondition.h"

class CDM_DECL SEEnvironmentCondition : public SECondition
{
public:
  friend class PBEnvironmentCondition;//friend the serialization class

  SEEnvironmentCondition(Logger* logger);
  virtual ~SEEnvironmentCondition();

  static constexpr char const* ConditionType = "Environment";
  std::string GetConditionType() const override { return ConditionType; }

  void Clear() override;

  bool IsValid() const override = 0;
  bool IsActive() const override = 0;

};
