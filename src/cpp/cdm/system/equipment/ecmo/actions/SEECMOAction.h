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
  virtual std::string GetActionType() const { return ActionType; }

  virtual void Clear();

  virtual bool IsValid() const;

  virtual void ToString(std::ostream &str) const { SEAction::ToString(str); }
};  