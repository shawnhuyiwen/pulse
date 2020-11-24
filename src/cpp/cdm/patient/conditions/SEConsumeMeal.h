/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/conditions/SEPatientCondition.h"
#include "patient/SEMeal.h"

class CDM_DECL SEConsumeMeal : public SEPatientCondition
{
  friend class PBPatientCondition;//friend the serialization class
public:

  SEConsumeMeal(Logger* logger=nullptr);
  virtual ~SEConsumeMeal();

  virtual void Clear(); //clear memory
  virtual void Copy(const SEConsumeMeal& src);

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual std::string GetName() const { return "ConsumeMeal"; }

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
