/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SEPatientAction.h"

class CDM_DECL SEChronicObstructivePulmonaryDiseaseExacerbation : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEChronicObstructivePulmonaryDiseaseExacerbation();
  virtual ~SEChronicObstructivePulmonaryDiseaseExacerbation();

  virtual void Clear(); //clear memory
  virtual void Copy(const SEChronicObstructivePulmonaryDiseaseExacerbation& src);

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual bool HasBronchitisSeverity() const;
  virtual SEScalar0To1& GetBronchitisSeverity();
  virtual double GetBronchitisSeverity() const;

  virtual bool HasEmphysemaSeverity() const;
  virtual SEScalar0To1& GetEmphysemaSeverity();
  virtual double GetEmphysemaSeverity() const;

  virtual void ToString(std::ostream &str) const;

protected:
  SEScalar0To1*     m_BronchitisSeverity;
  SEScalar0To1*     m_EmphysemaSeverity;
};
