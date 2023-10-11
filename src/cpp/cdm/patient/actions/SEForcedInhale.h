/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEConsciousRespirationCommand.h"

class CDM_DECL SEForcedInhale : public SEConsciousRespirationCommand
{
  friend class PBPatientAction;//friend the serialization class
  friend class SEConsciousRespiration;
  SEForcedInhale(Logger* logger);
public:

  virtual ~SEForcedInhale();

  static constexpr char const* Name = "Forced Inhale";
  std::string GetName() const override { return Name; }

  void Clear() override;
  void Copy(const SEForcedInhale& src);

  bool IsValid() const override;
  bool IsActive() const override;

  virtual bool HasInspiratoryCapacityFraction() const;
  virtual SEScalar0To1& GetInspiratoryCapacityFraction();
  virtual double GetInspiratoryCapacityFraction() const;

  virtual bool HasInhalePeriod() const;
  virtual SEScalarTime& GetInhalePeriod();
  virtual double GetInhalePeriod(const TimeUnit& unit) const;

  virtual bool HasHoldPeriod() const;
  virtual SEScalarTime& GetHoldPeriod();
  virtual double GetHoldPeriod(const TimeUnit& unit) const;

  virtual bool HasReleasePeriod() const;
  virtual SEScalarTime& GetReleasePeriod();
  virtual double GetReleasePeriod(const TimeUnit& unit) const;

protected:
  SEScalar0To1* m_InspiratoryCapacityFraction;
  SEScalarTime* m_InhalePeriod;
  SEScalarTime* m_HoldPeriod;
  SEScalarTime* m_ReleasePeriod;
};