/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "system/equipment/SEEquipmentAction.h"

class CDM_DECL SEAnesthesiaMachineAction : public SEEquipmentAction
{
  friend class PBEquipmentAction;//friend the serialization class
public:

  SEAnesthesiaMachineAction(Logger* logger);
  virtual ~SEAnesthesiaMachineAction();

  virtual void Clear();

  virtual bool IsValid() const;

  virtual void ToString(std::ostream &str) const = 0;
};  