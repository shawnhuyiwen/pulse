/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/engine/SECondition.h"

class CDM_DECL SEPatientCondition : public SECondition
{
  friend class PBPatientCondition;//friend the serialization class
public:

  SEPatientCondition(Logger* logger);
  virtual ~SEPatientCondition();

  static constexpr char const* ConditionType = "Patient";
  virtual std::string GetConditionType() const override { return ConditionType; }

  void Clear() override;

  bool IsValid() const override = 0;
  bool IsActive() const override = 0;
};
