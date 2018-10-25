/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SEPatientAction.h"

class CDM_DECL SEPericardialEffusion : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEPericardialEffusion();
  virtual ~SEPericardialEffusion();

  virtual void Clear(); //clear memory
  virtual void Copy(const SEPericardialEffusion& src);

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual bool HasEffusionRate() const;
  virtual SEScalarVolumePerTime& GetEffusionRate();
  virtual double GetEffusionRate(const VolumePerTimeUnit& unit) const;
  
  virtual void ToString(std::ostream &str) const;

protected:
  SEScalarVolumePerTime* m_EffusionRate;
};      