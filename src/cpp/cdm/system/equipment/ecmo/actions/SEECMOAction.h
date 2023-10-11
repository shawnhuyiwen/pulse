/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "cdm/system/equipment/SEEquipmentAction.h"

class CDM_DECL SEECMOAction : public SEEquipmentAction
{
  friend class PBEquipmentAction;//friend the serialization class
public:

  SEECMOAction(Logger* logger);
  virtual ~SEECMOAction();

  static constexpr char const* ActionType = "ECMO";
  std::string GetActionType() const override { return ActionType; }

  void Clear() override;

  bool IsValid() const override;
};
