/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorMode.h"
class SEMechanicalVentilator;
class SESubstanceManager;

class CDM_DECL SEMechanicalVentilatorContinuousPositiveAirwayPressure : public SEMechanicalVentilatorMode
{
  friend class PBEquipmentAction;//friend the serialization class
public:

  SEMechanicalVentilatorContinuousPositiveAirwayPressure(Logger* logger = nullptr);
  virtual ~SEMechanicalVentilatorContinuousPositiveAirwayPressure();

  void Clear() override;
  virtual void Copy(const SEMechanicalVentilatorContinuousPositiveAirwayPressure& src, bool /*preserveState*/ = false);
  virtual bool ToSettings(SEMechanicalVentilatorSettings& s, const SESubstanceManager& subMgr) override;

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

  virtual bool HasDeltaPressureSupport() const;
  virtual SEScalarPressure& GetDeltaPressureSupport();
  virtual double GetDeltaPressureSupport(const PressureUnit& unit) const;

  virtual bool HasFractionInspiredOxygen() const;
  virtual SEScalar0To1& GetFractionInspiredOxygen();
  virtual double GetFractionInspiredOxygen() const;

  virtual bool HasPositiveEndExpiredPressure() const;
  virtual SEScalarPressure& GetPositiveEndExpiredPressure();
  virtual double GetPositiveEndExpiredPressure(const PressureUnit& unit) const;

  virtual bool HasSlope() const;
  virtual SEScalarTime& GetSlope();
  virtual double GetSlope(const TimeUnit& unit) const;

  virtual eMechanicalVentilator_DriverWaveform GetInspirationWaveform() const;
  virtual void SetInspirationWaveform(eMechanicalVentilator_DriverWaveform w);

  virtual bool HasInspirationPatientTriggerFlow() const;
  virtual SEScalarVolumePerTime& GetInspirationPatientTriggerFlow();
  virtual double GetInspirationPatientTriggerFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasInspirationPatientTriggerPressure() const;
  virtual SEScalarPressure& GetInspirationPatientTriggerPressure();
  virtual double GetInspirationPatientTriggerPressure(const PressureUnit& unit) const;

  virtual eSwitch GetInspirationPatientTriggerRespiratoryModel() const;
  virtual void SetInspirationPatientTriggerRespiratoryModel(eSwitch c);

  virtual eMechanicalVentilator_DriverWaveform GetExpirationWaveform() const;
  virtual void SetExpirationWaveform(eMechanicalVentilator_DriverWaveform w);

  virtual bool HasExpirationCycleFlow() const;
  virtual SEScalarVolumePerTime& GetExpirationCycleFlow();
  virtual double GetExpirationCycleFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasExpirationCyclePressure() const;
  virtual SEScalarPressure& GetExpirationCyclePressure();
  virtual double GetExpirationCyclePressure(const PressureUnit& unit) const;

  virtual eSwitch GetExpirationCycleRespiratoryModel() const;
  virtual void SetExpirationCycleRespiratoryModel(eSwitch c);

  void ToString(std::ostream& str) const override;

  const SEScalar* GetScalar(const std::string& name) override;

protected:

  SEScalarPressure*                    m_DeltaPressureSupport;
  SEScalar0To1*                        m_FractionInspiredOxygen;
  SEScalarPressure*                    m_PositiveEndExpiredPressure;
  SEScalarTime*                        m_Slope;
  eMechanicalVentilator_DriverWaveform m_InspirationWaveform;
  SEScalarVolumePerTime*               m_InspirationPatientTriggerFlow;
  SEScalarPressure*                    m_InspirationPatientTriggerPressure;
  eSwitch                              m_InspirationPatientTriggerRespiratoryModel;
  eMechanicalVentilator_DriverWaveform m_ExpirationWaveform;
  SEScalarVolumePerTime*               m_ExpirationCycleFlow;
  SEScalarPressure*                    m_ExpirationCyclePressure;
  eSwitch                              m_ExpirationCycleRespiratoryModel;
};
