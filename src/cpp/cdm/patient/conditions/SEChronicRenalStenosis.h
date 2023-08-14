/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/conditions/SEPatientCondition.h"

class CDM_DECL SEChronicRenalStenosis : public SEPatientCondition
{
  friend class PBPatientCondition;//friend the serialization class
public:

  SEChronicRenalStenosis(Logger* logger=nullptr);
  virtual ~SEChronicRenalStenosis();

  static constexpr char const* Name = "Chronic Renal Stenosis";
  std::string GetName() const override { return Name; }

  void Clear() override;
  void Copy(const SEChronicRenalStenosis& src);

  bool IsValid() const override;
  bool IsActive() const override;

  virtual bool HasLeftKidneySeverity() const;
  virtual SEScalar0To1& GetLeftKidneySeverity();
  virtual double GetLeftKidneySeverity() const;

  virtual bool HasRightKidneySeverity() const;
  virtual SEScalar0To1& GetRightKidneySeverity();
  virtual double GetRightKidneySeverity() const;

protected:

  SEScalar0To1*           m_LeftKidneySeverity;
  SEScalar0To1*           m_RightKidneySeverity;
};