/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "cdm/system/equipment/SEEquipmentAction.h"
class SESubstanceManager;

class CDM_DECL SEInhalerAction : public SEEquipmentAction
{
  friend class PBEquipmentAction;//friend the serialization class
public:

  SEInhalerAction(Logger* logger);
  virtual ~SEInhalerAction();

  static constexpr char const* ActionType = "Inhaler";
  virtual std::string GetActionType() const { return ActionType; }

  virtual void Clear();// Deletes all members
  
  virtual bool IsValid() const;
};
