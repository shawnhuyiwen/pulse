/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "system/equipment/SEEquipmentAction.h"

class CDM_DECL SEMechanicalVentilatorAction : public SEEquipmentAction
{
  friend class PBEquipmentAction;//friend the serialization class
public:

  SEMechanicalVentilatorAction(Logger* logger);
  virtual ~SEMechanicalVentilatorAction();

  virtual void Clear();

  virtual bool IsValid() const;

  virtual void ToString(std::ostream &str) const = 0;
};  