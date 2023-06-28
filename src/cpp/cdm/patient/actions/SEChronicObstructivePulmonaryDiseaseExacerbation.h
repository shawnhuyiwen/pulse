/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEPatientAction.h"

class CDM_DECL SEChronicObstructivePulmonaryDiseaseExacerbation : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEChronicObstructivePulmonaryDiseaseExacerbation(Logger* logger=nullptr);
  virtual ~SEChronicObstructivePulmonaryDiseaseExacerbation();

  static constexpr char const* Name = "COPD Exacerbation";
  virtual std::string GetName() const { return Name; }

  virtual void Clear(); //clear memory
  virtual void Copy(const SEChronicObstructivePulmonaryDiseaseExacerbation& src, bool /*preserveState*/=false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual bool HasBronchitisSeverity() const;
  virtual SEScalar0To1& GetBronchitisSeverity();
  virtual double GetBronchitisSeverity() const;

  virtual bool HasEmphysemaSeverity() const;
  virtual SEScalar0To1& GetEmphysemaSeverity();
  virtual double GetEmphysemaSeverity() const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:
  SEScalar0To1*     m_BronchitisSeverity;
  SEScalar0To1*     m_EmphysemaSeverity;
};
