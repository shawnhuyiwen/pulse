/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/conditions/SEPatientCondition.h"

class CDM_DECL SEChronicAnemia : public SEPatientCondition
{
  friend class PBPatientCondition;//friend the serialization class
public:

  SEChronicAnemia(Logger* logger=nullptr);
  virtual ~SEChronicAnemia();

  static constexpr char const* Name = "Chronic Anemia";
  std::string GetName() const override { return Name; }

  void Clear() override;
  void Copy(const SEChronicAnemia& src);

  bool IsValid() const override;
  bool IsActive() const override;

  virtual bool HasReductionFactor() const;
  virtual SEScalar0To1& GetReductionFactor();
  virtual double GetReductionFactor() const;

protected:
  SEScalar0To1*           m_ReductionFactor;
};
