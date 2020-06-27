/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/conditions/SEPatientCondition.h"

class CDM_DECL SELobarPneumonia : public SEPatientCondition
{
  friend class PBPatientCondition;//friend the serialization class
public:

  SELobarPneumonia(Logger* logger=nullptr);
  virtual ~SELobarPneumonia();

  virtual void Clear(); //clear memory
  virtual void Copy(const SELobarPneumonia& src);

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual std::string GetName() const { return "LobarPneumonia"; }

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();
  virtual double GetSeverity() const;

  virtual bool HasLeftLungAffected() const;
  virtual SEScalar0To1& GetLeftLungAffected();
  virtual double GetLeftLungAffected() const;

  virtual bool HasRightLungAffected() const;
  virtual SEScalar0To1& GetRightLungAffected();
  virtual double GetRightLungAffected() const;

  virtual void ToString(std::ostream &str) const;

protected:

  SEScalar0To1*     m_Severity;
  SEScalar0To1*     m_LeftLungAffected;
  SEScalar0To1*     m_RightLungAffected;
};
