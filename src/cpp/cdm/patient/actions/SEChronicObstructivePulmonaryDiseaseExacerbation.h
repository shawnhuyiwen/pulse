/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEPatientAction.h"
#include "cdm/system/physiology/SERespiratorySystem.h"

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

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual bool HasBronchitisSeverity() const;
  virtual SEScalar0To1& GetBronchitisSeverity();
  virtual double GetBronchitisSeverity() const;

  virtual bool HasEmphysemaSeverity() const;
  virtual LungImpairmentMap& GetEmphysemaSeverities();
  virtual const LungImpairmentMap& GetEmphysemaSeverities() const;
  virtual bool HasEmphysemaSeverity(eLungCompartment cmpt) const;
  virtual SEScalar0To1& GetEmphysemaSeverity(eLungCompartment cmpt);
  virtual const SEScalar0To1* GetEmphysemaSeverity(eLungCompartment cmpt) const;

protected:
  SEScalar0To1* m_BronchitisSeverity;
  LungImpairmentMap m_EmphysemaSeverities;
};
