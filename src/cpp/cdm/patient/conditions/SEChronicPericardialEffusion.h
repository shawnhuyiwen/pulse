/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/conditions/SEPatientCondition.h"

class CDM_DECL SEChronicPericardialEffusion : public SEPatientCondition
{
  friend class PBPatientCondition;//friend the serialization class
public:

  SEChronicPericardialEffusion(Logger* logger=nullptr);
  virtual ~SEChronicPericardialEffusion();

  static constexpr char const* Name = "Chronic Pericardial Effusion";
  std::string GetName() const override { return Name; }

  void Clear() override;
  void Copy(const SEChronicPericardialEffusion& src);

  bool IsValid() const override;
  bool IsActive() const override;

  virtual bool HasAccumulatedVolume() const;
  virtual SEScalarVolume& GetAccumulatedVolume();
  virtual double GetAccumulatedVolume(const VolumeUnit& unit) const;

protected:

  SEScalarVolume* m_AccumulatedVolume;
};