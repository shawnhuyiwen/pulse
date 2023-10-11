/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "cdm/system/equipment/bag_valve_mask/actions/SEBagValveMaskAction.h"
class SEBagValveMask;
class SESubstanceManager;

class CDM_DECL SEBagValveMaskInstantaneous : public SEBagValveMaskAction
{
  friend class PBEquipmentAction;//friend the serialization class
public:

  SEBagValveMaskInstantaneous(Logger* logger=nullptr);
  virtual ~SEBagValveMaskInstantaneous();

  static constexpr char const* Name = "Instantaneous";
  std::string GetName() const override { return Name; }

  void Clear() override;
  virtual void Copy(const SEBagValveMaskInstantaneous& src);

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

  virtual bool HasFlow() const;
  virtual SEScalarVolumePerTime& GetFlow();
  virtual double GetFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasPressure() const;
  virtual SEScalarPressure& GetPressure();
  virtual double GetPressure(const PressureUnit& unit) const;

  const SEScalar* GetScalar(const std::string& name) override;

protected:

  SEScalarVolumePerTime* m_Flow;
  SEScalarPressure*      m_Pressure;
};