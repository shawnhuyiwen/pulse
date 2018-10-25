/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SEConsciousRespirationCommand.h"

class CDM_DECL SEForcedInhale : public SEConsciousRespirationCommand
{
  friend class PBPatientAction;//friend the serialization class
  friend class SEConsciousRespiration;
  SEForcedInhale();
public:

  virtual ~SEForcedInhale();

  virtual void Clear(); //clear memory
  virtual void Copy(const SEForcedInhale& src);

  virtual bool IsValid() const;
  virtual bool IsActive() const;

public:

  virtual bool HasInspiratoryCapacityFraction() const;
  virtual SEScalar0To1& GetInspiratoryCapacityFraction();
  virtual double GetInspiratoryCapacityFraction() const;

  virtual bool HasPeriod() const;
  virtual SEScalarTime& GetPeriod();
  virtual double GetPeriod(const TimeUnit& unit) const;

  virtual void ToString(std::ostream &str) const;
protected:
  SEScalar0To1* m_InspiratoryCapacityFraction;
  SEScalarTime* m_Period;
};