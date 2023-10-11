/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEPatientAction.h"

class CDM_DECL SEPericardialEffusion : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEPericardialEffusion(Logger* logger=nullptr);
  virtual ~SEPericardialEffusion();

  static constexpr char const* Name = "Pericardial Effusion";
  std::string GetName() const override { return Name; }

  void Clear() override;
  void Copy(const SEPericardialEffusion& src, bool /*preserveState*/=false);

  const SEScalar* GetScalar(const std::string& name) override;

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

  virtual bool HasEffusionRate() const;
  virtual SEScalarVolumePerTime& GetEffusionRate();
  virtual double GetEffusionRate(const VolumePerTimeUnit& unit) const;

protected:
  SEScalarVolumePerTime* m_EffusionRate;
};
