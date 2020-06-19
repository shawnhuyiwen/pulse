/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "engine/SEAction.h"

class CDM_DECL SEEquipmentAction : public SEAction
{
  friend class PBEquipmentAction;//friend the serialization class
public:

  SEEquipmentAction(Logger* logger);
  virtual ~SEEquipmentAction();

  virtual void Clear();

  virtual bool IsValid() const;

  virtual void ToString(std::ostream &str) const = 0;
};  