/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SESubstanceAdministration.h"
class SESubstanceCompound;

class CDM_DECL SESubstanceCompoundInfusion : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SESubstanceCompoundInfusion(const SESubstanceCompound& compound, Logger* logger=nullptr);
  virtual ~SESubstanceCompoundInfusion();

  virtual void Clear(); //clear memory
  virtual void Copy(const SESubstanceCompoundInfusion& src, bool preserveState=false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual bool HasBagVolume() const;
  virtual SEScalarVolume& GetBagVolume();
  virtual double GetBagVolume(const VolumeUnit& unit) const;

  virtual bool HasRate() const;
  virtual SEScalarVolumePerTime& GetRate();
  virtual double GetRate(const VolumePerTimeUnit& unit) const;

  virtual SESubstanceCompound& GetSubstanceCompound();
  virtual const SESubstanceCompound& GetSubstanceCompound() const;

  virtual void ToString(std::ostream &str) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:
  SEScalarVolume*                  m_BagVolume;
  SEScalarVolumePerTime*           m_Rate;
  const SESubstanceCompound&       m_Compound;
};
