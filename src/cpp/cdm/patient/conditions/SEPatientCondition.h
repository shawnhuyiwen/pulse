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
  virtual std::string GetConditionType() const { return ConditionType; }

  virtual void Clear(); //clear memory

  virtual bool IsValid() const = 0;
  virtual bool IsActive() const = 0;
};
