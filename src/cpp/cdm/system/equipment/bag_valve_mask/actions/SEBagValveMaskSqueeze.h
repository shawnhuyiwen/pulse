/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "system/equipment/bag_valve_mask/actions/SEBagValveMaskAction.h"
class SEBagValveMask;
class SESubstanceManager;

class CDM_DECL SEBagValveMaskSqueeze : public SEBagValveMaskAction
{
  friend class PBEquipmentAction;//friend the serialization class
public:

  SEBagValveMaskSqueeze(Logger* logger=nullptr);
  virtual ~SEBagValveMaskSqueeze();

  virtual void Clear();
  virtual void Copy(const SEBagValveMaskSqueeze& src, const SESubstanceManager& subMgr, bool preserveState = false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

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

  virtual void ToString(std::ostream &str) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:

  SEScalarPressure* m_SqueezePressure;
  SEScalarVolume*   m_SqueezeVolume;
  SEScalarTime*     m_ExpiratoryPeriod;
  SEScalarTime*     m_InspiratoryPeriod;
};