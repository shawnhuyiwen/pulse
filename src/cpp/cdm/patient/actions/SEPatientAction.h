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
  virtual std::string GetActionType() const { return ActionType; }

  virtual void Clear(); //clear memory

  virtual bool IsValid() const;

  virtual void ToString(std::ostream& str) const { SEAction::ToString(str); }
};  