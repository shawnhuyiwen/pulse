/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SEConsciousRespirationCommand.h"

class CDM_DECL SEForcedPause : public SEConsciousRespirationCommand
{
  friend class PBPatientAction;//friend the serialization class
  friend class SEConsciousRespiration;
  SEForcedPause(Logger* logger);
public:

  virtual ~SEForcedPause();

  virtual void Clear(); //clear memory
  virtual void Copy(const SEForcedPause& src);

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual bool HasPeriod() const;
  virtual SEScalarTime& GetPeriod();
  virtual double GetPeriod(const TimeUnit& unit) const;

  virtual void ToString(std::ostream &str) const;
protected:
  SEScalarTime*     m_Period;
};  