/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/conditions/SEPatientCondition.h"

class CDM_DECL SEChronicAnemia : public SEPatientCondition
{
  friend class PBPatientCondition;//friend the serialization class
public:

  SEChronicAnemia();
  virtual ~SEChronicAnemia();

  virtual void Clear(); //clear memory
  virtual void Copy(const SEChronicAnemia& src);

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual std::string GetName() const{ return "ChronicAnemia"; }

  virtual bool HasReductionFactor() const;
  virtual SEScalar0To1& GetReductionFactor();
  virtual double GetReductionFactor() const;

  virtual void ToString(std::ostream &str) const;

protected:
  SEScalar0To1*           m_ReductionFactor;
};      