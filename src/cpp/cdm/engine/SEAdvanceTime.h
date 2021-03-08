/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/SEAction.h"

class CDM_DECL SEAdvanceTime : public SEAction
{
  friend class PBAction;//friend the serialization class
public:

  SEAdvanceTime(Logger* logger=nullptr);
  virtual ~SEAdvanceTime();

  virtual void Clear(); //clear memory

  virtual bool IsValid() const;

  virtual bool HasTime() const;
  virtual SEScalarTime& GetTime();
  virtual double GetTime(const TimeUnit& unit) const;

  virtual void ToString(std::ostream &str) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:

  SEScalarTime *m_Time;
};