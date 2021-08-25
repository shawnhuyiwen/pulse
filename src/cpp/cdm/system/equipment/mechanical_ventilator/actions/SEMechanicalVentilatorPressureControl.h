/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorMode.h"

// Keep enums in sync with appropriate schema/cdm/MechanicalVentilatorActions.proto file !!
enum class eMechanicalVentilator_PressureControlMode { AssistedControl = 0, ContinuousMandatoryVentilation };
extern const std::string& eMechanicalVentilator_PressureControlMode_Name(eMechanicalVentilator_PressureControlMode m);

class CDM_DECL SEMechanicalVentilatorPressureControl : public SEMechanicalVentilatorMode
{
  friend class PBEquipmentAction;//friend the serialization class
public:

  SEMechanicalVentilatorPressureControl(Logger* logger = nullptr);
  virtual ~SEMechanicalVentilatorPressureControl();

  virtual void Clear();
  virtual void Copy(const SEMechanicalVentilatorPressureControl& src, bool /*preserveState*/ = false);
  virtual bool ToSettings(SEMechanicalVentilatorSettings& s, const SESubstanceManager& subMgr) override;

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual eMechanicalVentilator_PressureControlMode GetMode() const;
  virtual void SetMode(eMechanicalVentilator_PressureControlMode c);

  virtual bool HasFractionInspiredOxygen() const;
  virtual SEScalar0To1& GetFractionInspiredOxygen();
  virtual double GetFractionInspiredOxygen() const;

  virtual bool HasInspiratoryPeriod() const;
  virtual SEScalarTime& GetInspiratoryPeriod();
  virtual double GetInspiratoryPeriod(const TimeUnit& unit) const;

  virtual bool HasInspiratoryPressure() const;
  virtual SEScalarPressure& GetInspiratoryPressure();
  virtual double GetInspiratoryPressure(const PressureUnit& unit) const;

  virtual bool HasPositiveEndExpiredPressure() const;
  virtual SEScalarPressure& GetPositiveEndExpiredPressure();
  virtual double GetPositiveEndExpiredPressure(const PressureUnit& unit) const;

  virtual bool HasRespirationRate() const;
  virtual SEScalarFrequency& GetRespirationRate();
  virtual double GetRespirationRate(const FrequencyUnit& unit) const;

  virtual bool HasSlope() const;
  virtual SEScalarTime& GetSlope();
  virtual double GetSlope(const TimeUnit& unit) const;

  virtual void ToString(std::ostream& str) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:

  eMechanicalVentilator_PressureControlMode m_Mode;
  SEScalar0To1*                             m_FractionInspiredOxygen;
  SEScalarTime*                             m_InspiratoryPeriod;
  SEScalarPressure*                         m_InspiratoryPressure;
  SEScalarPressure*                         m_PositiveEndExpiredPressure;
  SEScalarFrequency*                        m_RespirationRate;
  SEScalarTime*                             m_Slope;
};
