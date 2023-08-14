/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEConsciousRespirationCommand.h"

class CDM_DECL SEForcedPause : public SEConsciousRespirationCommand
{
  friend class PBPatientAction;//friend the serialization class
  friend class SEConsciousRespiration;
  SEForcedPause(Logger* logger);
public:

  virtual ~SEForcedPause();

  static constexpr char const* Name = "Forced Pause";
  std::string GetName() const override { return Name; }

  void Clear() override;
  void Copy(const SEForcedPause& src);

  bool IsValid() const override;
  bool IsActive() const override;

  virtual bool HasPeriod() const;
  virtual SEScalarTime& GetPeriod();
  virtual double GetPeriod(const TimeUnit& unit) const;

protected:
  SEScalarTime*     m_Period;
};  