/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "patient/actions/SEPatientAction.h"
#include "patient/SENutrition.h"

class CDM_DECL SEConsumeNutrients : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEConsumeNutrients(Logger* logger=nullptr);
  virtual ~SEConsumeNutrients();

  virtual void Clear(); //clear memory
  virtual void Copy(const SEConsumeNutrients& src, bool preserveState=false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  bool HasNutrition() const;
  SENutrition& GetNutrition();
  const SENutrition* GetNutrition() const;

  virtual std::string GetNutritionFile() const;
  virtual void SetNutritionFile(const std::string& fileName);
  virtual bool HasNutritionFile() const;

  virtual void ToString(std::ostream &str) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:
  SENutrition* m_Nutrition;
  std::string  m_NutritionFile;
};
