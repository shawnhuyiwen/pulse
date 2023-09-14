/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "cdm/system/equipment/bag_valve_mask/actions/SEBagValveMaskAction.h"
class SEBagValveMask;
class SESubstanceManager;

class CDM_DECL SEBagValveMaskAutomated : public SEBagValveMaskAction
{
  friend class PBEquipmentAction;//friend the serialization class
public:

  SEBagValveMaskAutomated(Logger* logger=nullptr);
  virtual ~SEBagValveMaskAutomated();

  static constexpr char const* Name = "Automated";
  std::string GetName() const override { return Name; }

  void Clear() override;
  virtual void Copy(const SEBagValveMaskAutomated& src);

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

  virtual bool HasBreathFrequency() const;
  virtual SEScalarFrequency& GetBreathFrequency();
  virtual double GetBreathFrequency(const FrequencyUnit& unit) const;

  virtual bool HasInspiratoryExpiratoryRatio() const;
  virtual SEScalar& GetInspiratoryExpiratoryRatio();
  virtual double GetInspiratoryExpiratoryRatio() const;

  virtual bool HasSqueezePressure() const;
  virtual SEScalarPressure& GetSqueezePressure();
  virtual double GetSqueezePressure(const PressureUnit& unit) const;

  virtual bool HasSqueezeVolume() const;
  virtual SEScalarVolume& GetSqueezeVolume();
  virtual double GetSqueezeVolume(const VolumeUnit& unit) const;

  const SEScalar* GetScalar(const std::string& name) override;

protected:

  SEScalarFrequency*   m_BreathFrequency;
  SEScalar*            m_InspiratoryExpiratoryRatio;
  SEScalarPressure*    m_SqueezePressure;
  SEScalarVolume*      m_SqueezeVolume;
};