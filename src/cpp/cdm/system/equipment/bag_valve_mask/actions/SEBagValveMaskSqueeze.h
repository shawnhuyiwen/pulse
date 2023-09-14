/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "cdm/system/equipment/bag_valve_mask/actions/SEBagValveMaskAction.h"
class SEBagValveMask;
class SESubstanceManager;

class CDM_DECL SEBagValveMaskSqueeze : public SEBagValveMaskAction
{
  friend class PBEquipmentAction;//friend the serialization class
public:

  SEBagValveMaskSqueeze(Logger* logger=nullptr);
  virtual ~SEBagValveMaskSqueeze();

  static constexpr char const* Name = "Squeeze";
  std::string GetName() const override { return Name; }

  void Clear() override;
  virtual void Copy(const SEBagValveMaskSqueeze& src);

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

  virtual bool HasSqueezePressure() const;
  virtual SEScalarPressure& GetSqueezePressure();
  virtual double GetSqueezePressure(const PressureUnit& unit) const;

  virtual bool HasSqueezeVolume() const;
  virtual SEScalarVolume& GetSqueezeVolume();
  virtual double GetSqueezeVolume(const VolumeUnit& unit) const;

  virtual bool HasExpiratoryPeriod() const;
  virtual SEScalarTime& GetExpiratoryPeriod();
  virtual double GetExpiratoryPeriod(const TimeUnit& unit) const;

  virtual bool HasInspiratoryPeriod() const;
  virtual SEScalarTime& GetInspiratoryPeriod();
  virtual double GetInspiratoryPeriod(const TimeUnit& unit) const;

  const SEScalar* GetScalar(const std::string& name) override;

protected:

  SEScalarPressure* m_SqueezePressure;
  SEScalarVolume*   m_SqueezeVolume;
  SEScalarTime*     m_ExpiratoryPeriod;
  SEScalarTime*     m_InspiratoryPeriod;
};