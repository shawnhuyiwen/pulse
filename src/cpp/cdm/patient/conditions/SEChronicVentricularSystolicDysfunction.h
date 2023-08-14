/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/conditions/SEPatientCondition.h"

class CDM_DECL SEChronicVentricularSystolicDysfunction : public SEPatientCondition
{
  friend class PBPatientCondition;//friend the serialization class
public:

  SEChronicVentricularSystolicDysfunction(Logger* logger=nullptr);
  virtual ~SEChronicVentricularSystolicDysfunction();

  static constexpr char const* Name = "Chronic Ventricular Systolic Dysfunction";
  std::string GetName() const override { return Name; }

  void Clear() override;
  void Copy(const SEChronicVentricularSystolicDysfunction& src);

  bool IsValid() const override;
  bool IsActive() const override;

protected:
};
