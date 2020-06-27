/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SEPatientAction.h"

class CDM_DECL SEExercise : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEExercise(Logger* logger=nullptr);
  virtual ~SEExercise();

  virtual void Clear(); //clear memory
  virtual void Copy(const SEExercise& src);

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual bool HasIntensity() const;
  virtual SEScalar0To1& GetIntensity();
  virtual double GetIntensity() const;
  
  virtual void ToString(std::ostream &str) const;

protected:
  SEScalar0To1* m_Intensity;
};
