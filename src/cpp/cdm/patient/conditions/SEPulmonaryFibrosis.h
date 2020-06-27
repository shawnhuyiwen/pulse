//* Distributed under the Apache License, Version 2.0.
//See accompanying NOTICE file for details.

#pragma once
#include "patient/conditions/SEPatientCondition.h"

class CDM_DECL SEPulmonaryFibrosis : public SEPatientCondition
{
  friend class PBPatientCondition;//friend the serialization class
public:

  SEPulmonaryFibrosis(Logger* logger=nullptr);
  virtual ~SEPulmonaryFibrosis();

  virtual void Clear(); //clear memory
  virtual void Copy(const SEPulmonaryFibrosis& src);

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual std::string GetName() const { return "PulmonaryFibrosis"; }

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();
  virtual double GetSeverity() const;

  virtual void ToString(std::ostream &str) const;

protected:
  SEScalar0To1* m_Severity;
};
