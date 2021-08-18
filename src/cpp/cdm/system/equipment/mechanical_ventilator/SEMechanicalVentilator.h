/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/system/equipment/SEEquipment.h"
#include "cdm/system/equipment/mechanical_ventilator/SEMechanicalVentilatorSettings.h"

class CDM_DECL SEMechanicalVentilator : public SEEquipment
{
  friend class PBMechanicalVentilator;//friend the serialization class
public:

  SEMechanicalVentilator(Logger* logger);
  virtual ~SEMechanicalVentilator();

  virtual void Clear();

protected:
  /** @name StateChange
  *   @brief - This method is called when ever there is a state change
  *            Specically a new file has been loaded, configuration action, or the system reset
  *            Engine specific methodology can then update their logic.
  */
  virtual void StateChange() {};

public:

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool HasAirwayPressure() const;
  virtual SEScalarPressure& GetAirwayPressure();
  virtual double GetAirwayPressure(const PressureUnit& unit) const;

  virtual bool HasEndTidalCarbonDioxideFraction() const;
  virtual SEScalar0To1& GetEndTidalCarbonDioxideFraction();
  virtual double GetEndTidalCarbonDioxideFraction() const;

  virtual bool HasEndTidalCarbonDioxidePressure() const;
  virtual SEScalarPressure& GetEndTidalCarbonDioxidePressure();
  virtual double GetEndTidalCarbonDioxidePressure(const PressureUnit& unit) const;

  virtual bool HasEndTidalOxygenFraction() const;
  virtual SEScalar0To1& GetEndTidalOxygenFraction();
  virtual double GetEndTidalOxygenFraction() const;

  virtual bool HasEndTidalOxygenPressure() const;
  virtual SEScalarPressure& GetEndTidalOxygenPressure();
  virtual double GetEndTidalOxygenPressure(const PressureUnit& unit) const;

  virtual bool HasExpiratoryFlow() const;
  virtual SEScalarVolumePerTime& GetExpiratoryFlow();
  virtual double GetExpiratoryFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasExpiratoryTidalVolume() const;
  virtual SEScalarVolume& GetExpiratoryTidalVolume();
  virtual double GetExpiratoryTidalVolume(const VolumeUnit& unit) const;

  virtual bool HasInspiratoryExpiratoryRatio() const;
  virtual SEScalar& GetInspiratoryExpiratoryRatio();
  virtual double GetInspiratoryExpiratoryRatio() const;

  virtual bool HasInspiratoryFlow() const;
  virtual SEScalarVolumePerTime& GetInspiratoryFlow();
  virtual double GetInspiratoryFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasInspiratoryTidalVolume() const;
  virtual SEScalarVolume& GetInspiratoryTidalVolume();
  virtual double GetInspiratoryTidalVolume(const VolumeUnit& unit) const;

  virtual bool HasIntrinsicPositiveEndExpiredPressure() const;
  virtual SEScalarPressure& GetIntrinsicPositiveEndExpiredPressure();
  virtual double GetIntrinsicPositiveEndExpiredPressure(const PressureUnit& unit) const;

  virtual bool HasLeakFractionOrSeverity() const;
  virtual SEScalar0To1& GetLeakFractionOrSeverity();
  virtual double GetLeakFractionOrSeverity() const;

  virtual bool HasMeanAirwayPressure() const;
  virtual SEScalarPressure& GetMeanAirwayPressure();
  virtual double GetMeanAirwayPressure(const PressureUnit& unit) const;

  virtual bool HasPeakInspiratoryPressure() const;
  virtual SEScalarPressure& GetPeakInspiratoryPressure();
  virtual double GetPeakInspiratoryPressure(const PressureUnit& unit) const;

  virtual bool HasPlateauPressure() const;
  virtual SEScalarPressure& GetPlateauPressure();
  virtual double GetPlateauPressure(const PressureUnit& unit) const;

  virtual bool HasPositiveEndExpiratoryPressure() const;
  virtual SEScalarPressure& GetPositiveEndExpiratoryPressure();
  virtual double GetPositiveEndExpiratoryPressure(const PressureUnit& unit) const;

  virtual bool HasPulmonaryCompliance() const;
  virtual SEScalarVolumePerPressure& GetPulmonaryCompliance();
  virtual double GetPulmonaryCompliance(const VolumePerPressureUnit& unit) const;

  virtual bool HasPulmonaryElastance() const;
  virtual SEScalarPressurePerVolume& GetPulmonaryElastance();
  virtual double GetPulmonaryElastance(const PressurePerVolumeUnit& unit) const;

  virtual bool HasRelativeTotalLungVolume() const;
  virtual SEScalarVolume& GetRelativeTotalLungVolume();
  virtual double GetRelativeTotalLungVolume(const VolumeUnit& unit) const;

  virtual bool HasRespirationRate() const;
  virtual SEScalarFrequency& GetRespirationRate();
  virtual double GetRespirationRate(const FrequencyUnit& unit) const;

  virtual bool HasTidalVolume() const;
  virtual SEScalarVolume& GetTidalVolume();
  virtual double GetTidalVolume(const VolumeUnit& unit) const;

  virtual bool HasTotalLungVolume() const;
  virtual SEScalarVolume& GetTotalLungVolume();
  virtual double GetTotalLungVolume(const VolumeUnit& unit) const;

  virtual bool HasTotalPulmonaryVentilation() const;
  virtual SEScalarVolumePerTime& GetTotalPulmonaryVentilation();
  virtual double GetTotalPulmonaryVentilation(const VolumePerTimeUnit& unit) const;

  virtual bool HasSettings() const;
  virtual SEMechanicalVentilatorSettings& GetSettings();
  virtual const SEMechanicalVentilatorSettings* GetSettings() const;
  virtual void RemoveSettings();

protected:

  SEScalarPressure*                  m_AirwayPressure;
  SEScalar0To1*                      m_EndTidalCarbonDioxideFraction;
  SEScalarPressure*                  m_EndTidalCarbonDioxidePressure;
  SEScalar0To1*                      m_EndTidalOxygenFraction;
  SEScalarPressure*                  m_EndTidalOxygenPressure;
  SEScalarVolumePerTime*             m_ExpiratoryFlow;
  SEScalarVolume*                    m_ExpiratoryTidalVolume;
  SEScalar*                          m_InspiratoryExpiratoryRatio;
  SEScalarVolumePerTime*             m_InspiratoryFlow;
  SEScalarVolume*                    m_InspiratoryTidalVolume;
  SEScalarPressure*                  m_IntrinsicPositiveEndExpiredPressure;
  SEScalar0To1*                      m_LeakFractionOrSeverity;
  SEScalarPressure*                  m_MeanAirwayPressure;
  SEScalarPressure*                  m_PeakInspiratoryPressure;
  SEScalarPressure*                  m_PlateauPressure;
  SEScalarPressure*                  m_PositiveEndExpiratoryPressure;
  SEScalarVolumePerPressure*         m_PulmonaryCompliance;
  SEScalarPressurePerVolume*         m_PulmonaryElastance;
  SEScalarVolume*                    m_RelativeTotalLungVolume;
  SEScalarFrequency*                 m_RespirationRate;
  SEScalarVolume*                    m_TidalVolume;
  SEScalarVolume*                    m_TotalLungVolume;
  SEScalarVolumePerTime*             m_TotalPulmonaryVentilation;

  SEMechanicalVentilatorSettings*    m_Settings;
};
