/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "engine/SEAction.h"
class SESubstanceManager;

class CDM_DECL SEInhalerAction : public SEAction
{
  friend class PBInhalerAction;//friend the serialization class
public:

  SEInhalerAction();
  virtual ~SEInhalerAction();

  virtual void Clear();// Deletes all members
  
  virtual bool IsValid() const;

  virtual void ToString(std::ostream &str) const = 0;
};  