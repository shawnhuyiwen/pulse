/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SEConsciousRespirationCommand.h"

class CDM_DECL SEForcedExhale : public SEConsciousRespirationCommand
{
  friend class PBPatientAction;//friend the serialization class
  friend class SEConsciousRespiration;
  SEForcedExhale();
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

  virtual bool HasPeriod() const;
  virtual SEScalarTime& GetPeriod();
  virtual double GetPeriod(const TimeUnit& unit) const;

protected:
  SEScalar0To1* m_ExpiratoryReserveVolumeFraction;
  SEScalarTime* m_Period;
};  