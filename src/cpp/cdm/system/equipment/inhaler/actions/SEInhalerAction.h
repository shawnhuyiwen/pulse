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
  std::string GetActionType() const override { return ActionType; }

  void Clear() override;// Deletes all members
  
  bool IsValid() const override;
};
