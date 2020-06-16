/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/conditions/SEPatientCondition.h"

class CDM_DECL SEPulmonaryShunt : public SEPatientCondition
{
  friend class PBPatientCondition;//friend the serialization class
public:

  SEPulmonaryShunt(Logger* logger=nullptr);
  virtual ~SEPulmonaryShunt();

  virtual void Clear(); //clear memory
  virtual void Copy(const SEPulmonaryShunt& src);

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual std::string GetName() const{ return "PulmonaryShunt"; }

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();
  virtual double GetSeverity() const;

  virtual void ToString(std::ostream &str) const;

protected:
  SEScalar0To1* m_Severity;
};
