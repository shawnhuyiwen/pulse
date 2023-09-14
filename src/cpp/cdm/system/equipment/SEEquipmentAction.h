/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "cdm/engine/SEAction.h"

class CDM_DECL SEEquipmentAction : public SEAction
{
  friend class PBEquipmentAction;//friend the serialization class
public:

  SEEquipmentAction(Logger* logger);
  virtual ~SEEquipmentAction();

  void Clear() override;

  bool IsValid() const override;
};  