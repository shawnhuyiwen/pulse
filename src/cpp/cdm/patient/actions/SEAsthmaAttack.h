/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SEPatientAction.h"

class CDM_DECL SEAsthmaAttack : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEAsthmaAttack();
  virtual ~SEAsthmaAttack();

  virtual void Clear(); //clear memory
  virtual void Copy(const SEAsthmaAttack& src);

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();
  virtual double GetSeverity() const;

  virtual void ToString(std::ostream &str) const;

protected:
  SEScalar0To1*           m_Severity;
};
