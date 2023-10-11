/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/conditions/SEPatientCondition.h"
#include "cdm/patient/SEMeal.h"

class CDM_DECL SEConsumeMeal : public SEPatientCondition
{
  friend class PBPatientCondition;//friend the serialization class
public:

  SEConsumeMeal(Logger* logger=nullptr);
  virtual ~SEConsumeMeal();

  static constexpr char const* Name = "Consume Meal";
  std::string GetName() const override { return Name; }

  void Clear() override;
  void Copy(const SEConsumeMeal& src);

  bool IsValid() const override;
  bool IsActive() const override;

  bool HasMeal() const;
  SEMeal& GetMeal();
  const SEMeal* GetMeal() const;

  virtual std::string GetMealFile() const;
  virtual void SetMealFile(const std::string& fileName);
  virtual bool HasMealFile() const;

protected:
  SEMeal*      m_Meal;
  std::string  m_MealFile;
};
