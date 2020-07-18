/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "system/equipment/SEEquipmentAction.h"
class SESubstanceManager;

class CDM_DECL SEInhalerAction : public SEEquipmentAction
{
  friend class PBEquipmentAction;//friend the serialization class
public:

  SEInhalerAction(Logger* logger);
  virtual ~SEInhalerAction();

  virtual void Clear();// Deletes all members
  
  virtual bool IsValid() const;

  virtual void ToString(std::ostream &str) const = 0;
};  