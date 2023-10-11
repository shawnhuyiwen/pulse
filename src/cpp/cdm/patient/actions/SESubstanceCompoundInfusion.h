/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SESubstanceAdministration.h"
class SESubstanceCompound;

class CDM_DECL SESubstanceCompoundInfusion : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SESubstanceCompoundInfusion(const SESubstanceCompound& compound, Logger* logger=nullptr);
  virtual ~SESubstanceCompoundInfusion();

  static constexpr char const* Name = "Substance Compound Infusion";
  std::string GetName() const override { return Name; }

  void Clear() override;
  void Copy(const SESubstanceCompoundInfusion& src, bool preserveState=false);

  const SEScalar* GetScalar(const std::string& name) override;

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

  virtual bool HasBagVolume() const;
  virtual SEScalarVolume& GetBagVolume();
  virtual double GetBagVolume(const VolumeUnit& unit) const;

  virtual bool HasRate() const;
  virtual SEScalarVolumePerTime& GetRate();
  virtual double GetRate(const VolumePerTimeUnit& unit) const;

  virtual SESubstanceCompound& GetSubstanceCompound();
  virtual const SESubstanceCompound& GetSubstanceCompound() const;

protected:
  SEScalarVolume*                  m_BagVolume;
  SEScalarVolumePerTime*           m_Rate;
  const SESubstanceCompound&       m_Compound;
};
