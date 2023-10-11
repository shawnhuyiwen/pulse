/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "cdm/patient/actions/SEPatientAction.h"
#include "cdm/patient/SENutrition.h"

class CDM_DECL SEConsumeNutrients : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEConsumeNutrients(Logger* logger=nullptr);
  virtual ~SEConsumeNutrients();

  static constexpr char const* Name = "Consume Nutrients";
  std::string GetName() const override { return Name; }

  void Clear() override;
  void Copy(const SEConsumeNutrients& src, bool /*preserveState*/=false);

  const SEScalar* GetScalar(const std::string& name) override;

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

  bool HasNutrition() const;
  SENutrition& GetNutrition();
  const SENutrition* GetNutrition() const;

  virtual std::string GetNutritionFile() const;
  virtual void SetNutritionFile(const std::string& fileName);
  virtual bool HasNutritionFile() const;

protected:
  SENutrition* m_Nutrition;
  std::string  m_NutritionFile;
};
