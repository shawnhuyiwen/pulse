/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorMode.h"

// Keep enums in sync with appropriate schema/cdm/MechanicalVentilatorActions.proto file !!
enum class eMechanicalVentilator_VolumeControlMode { AssistedControl = 0, ContinuousMandatoryVentilation };
extern const std::string& eMechanicalVentilator_VolumeControlMode_Name(eMechanicalVentilator_VolumeControlMode m);

class CDM_DECL SEMechanicalVentilatorVolumeControl : public SEMechanicalVentilatorMode
{
  friend class PBEquipmentAction;//friend the serialization class
public:

  SEMechanicalVentilatorVolumeControl(Logger* logger = nullptr);
  virtual ~SEMechanicalVentilatorVolumeControl();

  virtual void Clear();
  virtual void Copy(const SEMechanicalVentilatorVolumeControl& src, bool /*preserveState*/ = false);
  virtual bool ToSettings(SEMechanicalVentilatorSettings& s, const SESubstanceManager& subMgr) override;

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual eMechanicalVentilator_VolumeControlMode GetMode() const;
  virtual void SetMode(eMechanicalVentilator_VolumeControlMode c);

  virtual bool HasFlow() const;
  virtual SEScalarVolumePerTime& GetFlow();
  virtual double GetFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasFractionInspiredOxygen() const;
  virtual SEScalar0To1& GetFractionInspiredOxygen();
  virtual double GetFractionInspiredOxygen() const;

  virtual bool HasInspiratoryPeriod() const;
  virtual SEScalarTime& GetInspiratoryPeriod();
  virtual double GetInspiratoryPeriod(const TimeUnit& unit) const;

  virtual bool HasPositiveEndExpiredPressure() const;
  virtual SEScalarPressure& GetPositiveEndExpiredPressure();
  virtual double GetPositiveEndExpiredPressure(const PressureUnit& unit) const;

  virtual bool HasRespirationRate() const;
  virtual SEScalarFrequency& GetRespirationRate();
  virtual double GetRespirationRate(const FrequencyUnit& unit) const;

  virtual bool HasTidalVolume() const;
  virtual SEScalarVolume& GetTidalVolume();
  virtual double GetTidalVolume(const VolumeUnit& unit) const;

  virtual void ToString(std::ostream& str) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:

  eMechanicalVentilator_VolumeControlMode m_Mode;
  SEScalarVolumePerTime*                  m_Flow;
  SEScalar0To1*                           m_FractionInspiredOxygen;
  SEScalarTime*                           m_InspiratoryPeriod;
  SEScalarPressure*                       m_PositiveEndExpiredPressure;
  SEScalarFrequency*                      m_RespirationRate;
  SEScalarVolume*                         m_TidalVolume;
};
