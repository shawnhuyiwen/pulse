/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEPatientAction.h"

class CDM_DECL SEExercise : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEExercise(Logger* logger=nullptr);
  virtual ~SEExercise();

  static constexpr char const* Name = "Exercise";
  std::string GetName() const override { return Name; }

  void Clear() override;
  void Copy(const SEExercise& src, bool /*preserveState*/=false);

  const SEScalar* GetScalar(const std::string& name) override;

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

  virtual bool HasIntensity() const;
  virtual SEScalar0To1& GetIntensity();
  virtual double GetIntensity() const;

protected:
  SEScalar0To1* m_Intensity;
};
