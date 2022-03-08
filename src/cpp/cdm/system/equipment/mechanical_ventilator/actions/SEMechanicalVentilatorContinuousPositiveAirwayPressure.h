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

  void ToString(std::ostream& str) const override;

  const SEScalar* GetScalar(const std::string& name) override;

protected:

  SEScalarPressure* m_DeltaPressureSupport;
  SEScalar0To1*     m_FractionInspiredOxygen;
  SEScalarPressure* m_PositiveEndExpiredPressure;
  SEScalarTime*     m_Slope;
};
