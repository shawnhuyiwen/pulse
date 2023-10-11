/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/conditions/SEPatientCondition.h"

class CDM_DECL SEPulmonaryShunt : public SEPatientCondition
{
  friend class PBPatientCondition;//friend the serialization class
public:

  SEPulmonaryShunt(Logger* logger=nullptr);
  virtual ~SEPulmonaryShunt();

  static constexpr char const* Name = "Pulmonary Shunt";
  std::string GetName() const override { return Name; }

  void Clear() override;
  void Copy(const SEPulmonaryShunt& src);

  bool IsValid() const override;
  bool IsActive() const override;

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();
  virtual double GetSeverity() const;

protected:
  SEScalar0To1* m_Severity;
};
