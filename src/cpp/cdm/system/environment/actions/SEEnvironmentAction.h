/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "engine/SEAction.h"

class CDM_DECL SEEnvironmentAction : public SEAction
{
  friend class PBEnvironmentAction;//friend the serialization class
public:

  SEEnvironmentAction(Logger* logger);
  virtual ~SEEnvironmentAction();

  virtual void Clear();

  virtual bool IsValid() const;

  virtual void ToString(std::ostream &str) const = 0;
};  