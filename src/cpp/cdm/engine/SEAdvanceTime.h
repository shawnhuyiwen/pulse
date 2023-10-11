/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/engine/SEAction.h"

class CDM_DECL SEAdvanceTime : public SEAction
{
  friend class PBAction;//friend the serialization class
public:

  SEAdvanceTime(Logger* logger=nullptr);
  virtual ~SEAdvanceTime();

  static constexpr char const* Name = "Advance Time";
  std::string GetName() const override { return Name; }

  void Clear() override; //clear memory

  bool IsValid() const override;

  virtual bool HasTime() const;
  virtual SEScalarTime& GetTime();
  virtual double GetTime(const TimeUnit& unit) const;

  const SEScalar* GetScalar(const std::string& name) override;

protected:

  SEScalarTime *m_Time;
};