/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "engine/SECondition.h"

class CDM_DECL SEEnvironmentCondition : public SECondition
{
public:
  friend class PBEnvironmentCondition;//friend the serialization class

  SEEnvironmentCondition(Logger* logger);
  virtual ~SEEnvironmentCondition();

  virtual void Clear();

  virtual bool IsValid() const = 0;
  virtual bool IsActive() const = 0;

public:
  virtual void ToString(std::ostream &str) const = 0;
};  
