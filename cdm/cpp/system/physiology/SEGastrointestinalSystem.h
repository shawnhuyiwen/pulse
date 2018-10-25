/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "system/SESystem.h"
class SENutrition;

class CDM_DECL SEGastrointestinalSystem : public SESystem
{
  friend class PBPhysiology;//friend the serialization class
public:

  SEGastrointestinalSystem(Logger* logger);
  virtual ~SEGastrointestinalSystem();

  virtual void Clear();// Deletes all members

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool HasChymeAbsorptionRate() const;
  virtual SEScalarVolumePerTime& GetChymeAbsorptionRate();
  virtual double GetChymeAbsorptionRate(const VolumePerTimeUnit& unit) const;

  virtual bool HasStomachContents() const;
  virtual SENutrition& GetStomachContents();
  virtual const SENutrition* GetStomachContents() const;
  virtual void RemoveStomachContents();

protected:

  SEScalarVolumePerTime* m_ChymeAbsorptionRate;
  SENutrition*           m_StomachContents;
};
