/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SEConsciousRespirationCommand.h"

class CDM_DECL SEForcedExhale : public SEConsciousRespirationCommand
{
  friend class PBPatientAction;//friend the serialization class
  friend class SEConsciousRespiration;
  SEForcedExhale(Logger* logger);
public:

  virtual ~SEForcedExhale();

  virtual void Clear(); //clear memory
  virtual void Copy(const SEForcedExhale& src);

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual void ToString(std::ostream &str) const;

  virtual bool HasExpiratoryReserveVolumeFraction() const;
  virtual SEScalar0To1& GetExpiratoryReserveVolumeFraction();
  virtual double GetExpiratoryReserveVolumeFraction() const;

  virtual bool HasExhalePeriod() const;
  virtual SEScalarTime& GetExhalePeriod();
  virtual double GetExhalePeriod(const TimeUnit& unit) const;

  virtual bool HasHoldPeriod() const;
  virtual SEScalarTime& GetHoldPeriod();
  virtual double GetHoldPeriod(const TimeUnit& unit) const;

  virtual bool HasReleasePeriod() const;
  virtual SEScalarTime& GetReleasePeriod();
  virtual double GetReleasePeriod(const TimeUnit& unit) const;

protected:
  SEScalar0To1* m_ExpiratoryReserveVolumeFraction;
  SEScalarTime* m_ExhalePeriod;
  SEScalarTime* m_HoldPeriod;
  SEScalarTime* m_ReleasePeriod;
};  