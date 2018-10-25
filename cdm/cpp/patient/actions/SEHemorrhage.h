/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SEPatientAction.h"

class CDM_DECL SEHemorrhage : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEHemorrhage();
  virtual ~SEHemorrhage();

  virtual void Clear(); //clear memory
  virtual void Copy(const SEHemorrhage& src);

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual std::string GetCompartment() const;
  virtual void SetCompartment(const std::string& name);
  virtual bool HasCompartment() const;
  virtual void InvalidateCompartment();

  virtual bool HasRate() const;
  virtual SEScalarVolumePerTime& GetRate();
  virtual double GetRate(const VolumePerTimeUnit& unit) const;

  virtual void ToString(std::ostream &str) const;

protected:
  std::string             m_Compartment;
  SEScalarVolumePerTime*  m_Rate;
  
};    