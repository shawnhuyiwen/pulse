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
  virtual std::string GetName() const { return Name; }

  virtual void Clear(); //clear memory
  virtual void Copy(const SEConsumeMeal& src);

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  bool HasMeal() const;
  SEMeal& GetMeal();
  const SEMeal* GetMeal() const;

  virtual std::string GetMealFile() const;
  virtual void SetMealFile(const std::string& fileName);
  virtual bool HasMealFile() const;

  virtual void ToString(std::ostream &str) const;

protected:
  SEMeal*      m_Meal;
  std::string  m_MealFile;
};
