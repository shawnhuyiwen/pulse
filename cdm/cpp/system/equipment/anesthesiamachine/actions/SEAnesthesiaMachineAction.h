/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "scenario/SEAction.h"

class CDM_DECL SEAnesthesiaMachineAction : public SEAction
{
  friend class PBAnesthesiaMachineAction;//friend the serialization class
public:

  SEAnesthesiaMachineAction();
  virtual ~SEAnesthesiaMachineAction();

  virtual void Clear();

  virtual bool IsValid() const;

  virtual void ToString(std::ostream &str) const = 0;
};  