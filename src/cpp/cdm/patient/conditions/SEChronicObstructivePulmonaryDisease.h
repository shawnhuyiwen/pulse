/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/conditions/SEPatientCondition.h"
#include "cdm/system/physiology/SERespiratorySystem.h"

class CDM_DECL SEChronicObstructivePulmonaryDisease : public SEPatientCondition
{
  friend class PBPatientCondition;//friend the serialization class
public:

  SEChronicObstructivePulmonaryDisease(Logger* logger=nullptr);
  virtual ~SEChronicObstructivePulmonaryDisease();

  static constexpr char const* Name = "Chronic Obstructive Pulmonary Disease";
  virtual std::string GetName() const { return Name; }

  virtual void Clear(); //clear memory
  virtual void Copy(const SEChronicObstructivePulmonaryDisease& src);

  virtual bool IsValid() const;
  virtual bool IsActive() const;

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
  SEScalar0To1*     m_BronchitisSeverity;
  LungImpairmentMap m_EmphysemaSeverities;
};
