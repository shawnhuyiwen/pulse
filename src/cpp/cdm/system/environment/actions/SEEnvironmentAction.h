/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "cdm/engine/SEAction.h"

class CDM_DECL SEEnvironmentAction : public SEAction
{
  friend class PBEnvironmentAction;//friend the serialization class
public:

  SEEnvironmentAction(Logger* logger);
  virtual ~SEEnvironmentAction();

  static constexpr char const* ActionType = "Environmental";
  std::string GetActionType() const override { return ActionType; }

  void Clear() override;

  bool IsValid() const override;
};
