/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/conditions/SEPatientCondition.h"

class CDM_DECL SEChronicObstructivePulmonaryDisease : public SEPatientCondition
{
  friend class PBPatientCondition;//friend the serialization class
public:

  SEChronicObstructivePulmonaryDisease(Logger* logger=nullptr);
  virtual ~SEChronicObstructivePulmonaryDisease();

  static constexpr char const* Name = "COPD";
  virtual std::string GetName() const { return Name; }

  virtual void Clear(); //clear memory
  virtual void Copy(const SEChronicObstructivePulmonaryDisease& src);

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
