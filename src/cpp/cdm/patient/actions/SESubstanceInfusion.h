/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SESubstanceAdministration.h"
class SESubstance;

class CDM_DECL SESubstanceInfusion : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SESubstanceInfusion(const SESubstance& substance, Logger* logger=nullptr);
  virtual ~SESubstanceInfusion();

  static constexpr char const* Name = "Substance Infusion";
  virtual std::string GetName() const { return Name; }

  virtual void Clear(); //clear memory
  virtual void Copy(const SESubstanceInfusion& src, bool /*preserveState*/=false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual bool HasConcentration() const;
  virtual SEScalarMassPerVolume& GetConcentration();
  virtual double GetConcentration(const MassPerVolumeUnit& unit) const;

  virtual bool HasRate() const;
  virtual SEScalarVolumePerTime& GetRate();
  virtual double GetRate(const VolumePerTimeUnit& unit) const;

  virtual bool HasVolume() const;
  virtual SEScalarVolume& GetVolume();
  virtual double GetVolume(const VolumeUnit& unit) const;

  virtual SESubstance& GetSubstance();
  virtual const SESubstance& GetSubstance() const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:
  SEScalarMassPerVolume*                  m_Concentration;
  SEScalarVolumePerTime*                  m_Rate;
  SEScalarVolume*                         m_Volume;
  const SESubstance&                      m_Substance;
};
