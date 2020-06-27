/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SESubstanceAdministration.h"
class SESubstance;

class CDM_DECL SESubstanceInfusion : public SESubstanceAdministration
{
  friend class PBPatientAction;//friend the serialization class
public:

  SESubstanceInfusion(const SESubstance& substance, Logger* logger=nullptr);
  virtual ~SESubstanceInfusion();

  virtual void Clear(); //clear memory
  virtual void Copy(const SESubstanceInfusion& src);

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual bool HasConcentration() const;
  virtual SEScalarMassPerVolume& GetConcentration();
  virtual double GetConcentration(const MassPerVolumeUnit& unit) const;

  virtual bool HasRate() const;
  virtual SEScalarVolumePerTime& GetRate();
  virtual double GetRate(const VolumePerTimeUnit& unit) const;

  virtual SESubstance& GetSubstance();
  virtual const SESubstance& GetSubstance() const;

  virtual void ToString(std::ostream &str) const;

protected:
  SEScalarMassPerVolume*                  m_Concentration;
  SEScalarVolumePerTime*                  m_Rate;
  const SESubstance&                      m_Substance;
};