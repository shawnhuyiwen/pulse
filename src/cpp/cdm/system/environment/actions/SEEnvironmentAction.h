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
  virtual std::string GetActionType() const { return ActionType; }

  virtual void Clear();

  virtual bool IsValid() const;

  virtual void ToString(std::ostream &str) const { SEAction::ToString(str); }
};
