/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/engine/SEAction.h"

class CDM_DECL SEPatientAction : public SEAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEPatientAction(Logger* logger=nullptr);
  virtual ~SEPatientAction();

  static constexpr char const* ActionType = "Patient";
  std::string GetActionType() const override { return ActionType; }

  void Clear() override;

  bool IsValid() const override;
};  