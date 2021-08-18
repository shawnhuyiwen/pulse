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
  virtual SEScalarVolumePerTime& GetExpiratoryTidalVolume();
  virtual double GetExpiratoryTidalVolume(const VolumeUnit& unit) const;

  virtual bool HasInspiratoryExpiratoryRatio() const;
  virtual SEScalar& GetInspiratoryExpiratoryRatio();
  virtual double GetInspiratoryExpiratoryRatio() const;

  virtual bool HasInspiratoryFlow() const;
  virtual SEScalarVolumePerTime& GetInspiratoryFlow();
  virtual double GetInspiratoryFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasSettings() const;
  virtual SEMechanicalVentilatorSettings& GetSettings();
  virtual const SEMechanicalVentilatorSettings* GetSettings() const;
  virtual void RemoveSettings();

protected:
  SEMechanicalVentilatorSettings* m_Settings;
};
