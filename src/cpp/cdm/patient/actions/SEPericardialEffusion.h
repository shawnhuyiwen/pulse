/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SEPatientAction.h"

class CDM_DECL SEPericardialEffusion : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEPericardialEffusion(Logger* logger=nullptr);
  virtual ~SEPericardialEffusion();

  virtual void Clear(); //clear memory
  virtual void Copy(const SEPericardialEffusion& src, bool preserveState=false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual bool HasEffusionRate() const;
  virtual SEScalarVolumePerTime& GetEffusionRate();
  virtual double GetEffusionRate(const VolumePerTimeUnit& unit) const;
  
  virtual void ToString(std::ostream &str) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:
  SEScalarVolumePerTime* m_EffusionRate;
};
