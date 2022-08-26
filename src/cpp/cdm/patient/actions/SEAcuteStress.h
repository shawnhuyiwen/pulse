/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEPatientAction.h"

class CDM_DECL SEAcuteStress : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEAcuteStress(Logger* logger=nullptr);
  virtual ~SEAcuteStress();

  static constexpr char const* Name = "Acute Stress";
  virtual std::string GetName() const { return Name; }

  virtual void Clear();
  virtual void Copy(const SEAcuteStress& src, bool /*preserveState*/=false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();
  virtual double GetSeverity() const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:
  SEScalar0To1*           m_Severity;
};
