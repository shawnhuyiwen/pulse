/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "cdm/system/equipment/SEEquipmentAction.h"

class CDM_DECL SEAnesthesiaMachineAction : public SEEquipmentAction
{
  friend class PBEquipmentAction;//friend the serialization class
public:

  SEAnesthesiaMachineAction(Logger* logger);
  virtual ~SEAnesthesiaMachineAction();

  static constexpr char const* ActionType = "Anesthesia Machine";
  std::string GetActionType() const override { return ActionType; }

  void Clear() override;

  bool IsValid() const override;
};
