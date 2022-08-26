/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/conditions/SEPatientCondition.h"

class CDM_DECL SESepsis : public SEPatientCondition
{
  friend class PBPatientCondition;//friend the serialization class
public:

  SESepsis(Logger* logger=nullptr);
  virtual ~SESepsis();

  static constexpr char const* Name = "Sepsis";
  virtual std::string GetName() const { return Name; }

  virtual void Clear(); //clear memory
  virtual void Copy(const SESepsis& src);

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();
  virtual double GetSeverity() const;

protected:
  SEScalar0To1*           m_Severity;
};
