/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "system/SESystem.h"

   // Keep enums in sync with appropriate schema/cdm/PhysiologyEnums.proto file !!
enum class eBreathState { Inhale = 0, Pause, Exhale };
extern const std::string& eBreathState_Name(eBreathState m);

class CDM_DECL SERespiratorySystem : public SESystem
{
  friend class PBPhysiology;//friend the serialization class
public:

  SERespiratorySystem(Logger* logger);
  virtual ~SERespiratorySystem();

  virtual void Clear();// Deletes all members
  
  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool HasAlveolarArterialGradient() const;
  virtual SEScalarPressure& GetAlveolarArterialGradient();
  virtual double GetAlveolarArterialGradient(const PressureUnit& unit) const;

  virtual bool HasAlveolarDeadSpace() const;
  virtual SEScalarVolume& GetAlveolarDeadSpace();
  virtual double GetAlveolarDeadSpace(const VolumeUnit& unit) const;

  virtual bool HasAnatomicDeadSpace() const;
  virtual SEScalarVolume& GetAnatomicDeadSpace();
  virtual double GetAnatomicDeadSpace(const VolumeUnit& unit) const;

  virtual bool HasCarricoIndex() const;
  virtual SEScalarPressure& GetCarricoIndex();
  virtual double GetCarricoIndex(const PressureUnit& unit) const;

  virtual bool HasChestWallCompliance() const;
  virtual SEScalarVolumePerPressure& GetChestWallCompliance();
  virtual double GetChestWallCompliance(const VolumePerPressureUnit& unit) const;

  virtual bool HasElasticWorkOfBreathing() const;
  virtual SEScalarEnergy& GetElasticWorkOfBreathing();
  virtual double GetElasticWorkOfBreathing(const EnergyUnit& unit) const;

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

  virtual bool HasExpiratoryPulmonaryResistance() const;
  virtual SEScalarPressureTimePerVolume& GetExpiratoryPulmonaryResistance();
  virtual double GetExpiratoryPulmonaryResistance(const PressureTimePerVolumeUnit& unit) const;

  virtual bool HasFractionOfInsipredOxygen() const;
  virtual SEScalar0To1& GetFractionOfInsipredOxygen();
  virtual double GetFractionOfInsipredOxygen() const;

  virtual bool HasImposedPowerOfBreathing() const;
  virtual SEScalarPower& GetImposedPowerOfBreathing();
  virtual double GetImposedPowerOfBreathing(const PowerUnit& unit) const;

  virtual bool HasImposedWorkOfBreathing() const;
  virtual SEScalarEnergy& GetImposedWorkOfBreathing();
  virtual double GetImposedWorkOfBreathing(const EnergyUnit& unit) const;

  virtual bool HasInspiratoryExpiratoryRatio() const;
  virtual SEScalar& GetInspiratoryExpiratoryRatio();
  virtual double GetInspiratoryExpiratoryRatio() const;

  virtual bool HasInspiratoryFlow() const;
  virtual SEScalarVolumePerTime& GetInspiratoryFlow();
  virtual double GetInspiratoryFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasInspiratoryPulmonaryResistance() const;
  virtual SEScalarPressureTimePerVolume& GetInspiratoryPulmonaryResistance();
  virtual double GetInspiratoryPulmonaryResistance(const PressureTimePerVolumeUnit& unit) const;

  virtual bool HasIntrapleuralPressure() const;
  virtual SEScalarPressure& GetIntrapleuralPressure();
  virtual double GetIntrapleuralPressure(const PressureUnit& unit) const;

  virtual bool HasIntrapulmonaryPressure() const;
  virtual SEScalarPressure& GetIntrapulmonaryPressure();
  virtual double GetIntrapulmonaryPressure(const PressureUnit& unit) const;

  virtual bool HasLungCompliance() const;
  virtual SEScalarVolumePerPressure& GetLungCompliance();
  virtual double GetLungCompliance(const VolumePerPressureUnit& unit) const;

  virtual bool HasMaximalInspiratoryPressure() const;
  virtual SEScalarPressure& GetMaximalInspiratoryPressure();
  virtual double GetMaximalInspiratoryPressure(const PressureUnit& unit) const;
  
  virtual bool HasMeanAirwayPressure() const;
  virtual SEScalarPressure& GetMeanAirwayPressure();
  virtual double GetMeanAirwayPressure(const PressureUnit& unit) const;

  virtual bool HasOxygenationIndex() const;
  virtual SEScalar& GetOxygenationIndex();
  virtual double GetOxygenationIndex() const;

  virtual bool HasOxygenSaturationIndex() const;
  virtual SEScalarPressure& GetOxygenSaturationIndex();
  virtual double GetOxygenSaturationIndex(const PressureUnit& unit) const;

  virtual bool HasPatientPowerOfBreathing() const;
  virtual SEScalarPower& GetPatientPowerOfBreathing();
  virtual double GetPatientPowerOfBreathing(const PowerUnit& unit) const;

  virtual bool HasPatientWorkOfBreathing() const;
  virtual SEScalarEnergy& GetPatientWorkOfBreathing();
  virtual double GetPatientWorkOfBreathing(const EnergyUnit& unit) const;

  virtual bool HasPeakInspiratoryPressure() const;
  virtual SEScalarPressure& GetPeakInspiratoryPressure();
  virtual double GetPeakInspiratoryPressure(const PressureUnit& unit) const;

  virtual bool HasPhysiologicDeadSpace() const;
  virtual SEScalarVolume& GetPhysiologicDeadSpace();
  virtual double GetPhysiologicDeadSpace(const VolumeUnit& unit) const;

  virtual bool HasPositiveEndExpiratoryPressure() const;
  virtual SEScalarPressure& GetPositiveEndExpiratoryPressure();
  virtual double GetPositiveEndExpiratoryPressure(const PressureUnit& unit) const;

  virtual bool HasPulmonaryCompliance() const;
  virtual SEScalarVolumePerPressure& GetPulmonaryCompliance();
  virtual double GetPulmonaryCompliance(const VolumePerPressureUnit& unit) const;

  virtual bool HasPulmonaryElastance() const;
  virtual SEScalarPressurePerVolume& GetPulmonaryElastance();
  virtual double GetPulmonaryElastance(const PressurePerVolumeUnit& unit) const;

  virtual bool HasResistiveExpiratoryWorkOfBreathing() const;
  virtual SEScalarEnergy& GetResistiveExpiratoryWorkOfBreathing();
  virtual double GetResistiveExpiratoryWorkOfBreathing(const EnergyUnit& unit) const;

  virtual bool HasResistiveInspiratoryWorkOfBreathing() const;
  virtual SEScalarEnergy& GetResistiveInspiratoryWorkOfBreathing();
  virtual double GetResistiveInspiratoryWorkOfBreathing(const EnergyUnit& unit) const;

  virtual bool HasRespirationRate() const;
  virtual SEScalarFrequency& GetRespirationRate();
  virtual double GetRespirationRate(const FrequencyUnit& unit) const;

  virtual bool HasRespiratoryMuscleFatigue() const;
  virtual SEScalar0To1& GetRespiratoryMuscleFatigue();
  virtual double GetRespiratoryMuscleFatigue() const;

  virtual bool HasRespiratoryMusclePressure() const;
  virtual SEScalarPressure& GetRespiratoryMusclePressure();
  virtual double GetRespiratoryMusclePressure(const PressureUnit& unit) const;

  virtual bool HasSaturationAndFractionOfInspiredOxygenRatio() const;
  virtual SEScalar& GetSaturationAndFractionOfInspiredOxygenRatio();
  virtual double GetSaturationAndFractionOfInspiredOxygenRatio() const;

  virtual bool HasSpecificVentilation() const;
  virtual SEScalar& GetSpecificVentilation();
  virtual double GetSpecificVentilation() const;

  virtual bool HasTidalVolume() const;
  virtual SEScalarVolume& GetTidalVolume();
  virtual double GetTidalVolume(const VolumeUnit& unit) const;

  virtual bool HasTotalAlveolarVentilation() const;
  virtual SEScalarVolumePerTime& GetTotalAlveolarVentilation();
  virtual double GetTotalAlveolarVentilation(const VolumePerTimeUnit& unit) const;

  virtual bool HasTotalDeadSpaceVentilation() const;
  virtual SEScalarVolumePerTime& GetTotalDeadSpaceVentilation();
  virtual double GetTotalDeadSpaceVentilation(const VolumePerTimeUnit& unit) const;

  virtual bool HasTotalLungVolume() const;
  virtual SEScalarVolume& GetTotalLungVolume();
  virtual double GetTotalLungVolume(const VolumeUnit& unit) const;

  virtual bool HasTotalPowerOfBreathing() const;
  virtual SEScalarPower& GetTotalPowerOfBreathing();
  virtual double GetTotalPowerOfBreathing(const PowerUnit& unit) const;

  virtual bool HasTotalPulmonaryVentilation() const;
  virtual SEScalarVolumePerTime& GetTotalPulmonaryVentilation();
  virtual double GetTotalPulmonaryVentilation(const VolumePerTimeUnit& unit) const;

  virtual bool HasTotalWorkOfBreathing() const;
  virtual SEScalarEnergy& GetTotalWorkOfBreathing();
  virtual double GetTotalWorkOfBreathing(const EnergyUnit& unit) const;

  virtual bool HasTransairwayPressure() const;
  virtual SEScalarPressure& GetTransairwayPressure();
  virtual double GetTransairwayPressure(const PressureUnit& unit) const;

  virtual bool HasTransalveolarPressure() const;
  virtual SEScalarPressure& GetTransalveolarPressure();
  virtual double GetTransalveolarPressure(const PressureUnit& unit) const;

  virtual bool HasTransChestWallPressure() const;
  virtual SEScalarPressure& GetTransChestWallPressure();
  virtual double GetTransChestWallPressure(const PressureUnit& unit) const;

  virtual bool HasTransMusclePressure() const;
  virtual SEScalarPressure& GetTransMusclePressure();
  virtual double GetTransMusclePressure(const PressureUnit& unit) const;

  virtual bool HasTranspulmonaryPressure() const;
  virtual SEScalarPressure& GetTranspulmonaryPressure();
  virtual double GetTranspulmonaryPressure(const PressureUnit& unit) const;

  virtual bool HasTransrespiratoryPressure() const;
  virtual SEScalarPressure& GetTransrespiratoryPressure();
  virtual double GetTransrespiratoryPressure(const PressureUnit& unit) const;

  virtual bool HasTransthoracicPressure() const;
  virtual SEScalarPressure& GetTransthoracicPressure();
  virtual double GetTransthoracicPressure(const PressureUnit& unit) const;


protected:

  SEScalarPressure*              m_AlveolarArterialGradient;
  SEScalarVolume*                m_AlveolarDeadSpace;
  SEScalarVolume*                m_AnatomicDeadSpace;
  SEScalarPressure*              m_CarricoIndex;
  SEScalarVolumePerPressure*     m_ChestWallCompliance;
  SEScalarEnergy*                m_ElasticWorkOfBreathing;
  SEScalar0To1*                  m_EndTidalCarbonDioxideFraction;
  SEScalarPressure*              m_EndTidalCarbonDioxidePressure;
  SEScalar0To1*                  m_EndTidalOxygenFraction;
  SEScalarPressure*              m_EndTidalOxygenPressure;
  SEScalarVolumePerTime*         m_ExpiratoryFlow;
  SEScalarPressureTimePerVolume* m_ExpiratoryPulmonaryResistance;
  SEScalar0To1*                  m_FractionOfInsipredOxygen;
  SEScalarPower*                 m_ImposedPowerOfBreathing;
  SEScalarEnergy*                m_ImposedWorkOfBreathing;
  SEScalar*                      m_InspiratoryExpiratoryRatio;
  SEScalarVolumePerTime*         m_InspiratoryFlow;
  SEScalarPressureTimePerVolume* m_InspiratoryPulmonaryResistance;
  SEScalarPressure*              m_IntrapleuralPressure;
  SEScalarPressure*              m_IntrapulmonaryPressure;
  SEScalarVolumePerPressure*     m_LungCompliance;
  SEScalarPressure*              m_MaximalInspiratoryPressure;
  SEScalarPressure*              m_MeanAirwayPressure;
  SEScalar*                      m_OxygenationIndex;
  SEScalarPressure*              m_OxygenSaturationIndex;
  SEScalarPower*                 m_PatientPowerOfBreathing;
  SEScalarEnergy*                m_PatientWorkOfBreathing;
  SEScalarPressure*              m_PeakInspiratoryPressure;
  SEScalarVolume*                m_PhysiologicDeadSpace;
  SEScalarPressure*              m_PositiveEndExpiratoryPressure;
  SEScalarVolumePerPressure*     m_PulmonaryCompliance;
  SEScalarPressurePerVolume*     m_PulmonaryElastance;
  SEScalarEnergy*                m_ResistiveExpiratoryWorkOfBreathing;
  SEScalarEnergy*                m_ResistiveInspiratoryWorkOfBreathing;
  SEScalarFrequency*             m_RespirationRate;
  SEScalar0To1*                  m_RespiratoryMuscleFatigue;
  SEScalarPressure*              m_RespiratoryMusclePressure;
  SEScalar*                      m_SaturationAndFractionOfInspiredOxygenRatio;
  SEScalar*                      m_SpecificVentilation;
  SEScalarVolume*                m_TidalVolume;
  SEScalarVolumePerTime*         m_TotalAlveolarVentilation;
  SEScalarVolumePerTime*         m_TotalDeadSpaceVentilation;
  SEScalarVolume*                m_TotalLungVolume;
  SEScalarPower*                 m_TotalPowerOfBreathing;
  SEScalarVolumePerTime*         m_TotalPulmonaryVentilation;
  SEScalarEnergy*                m_TotalWorkOfBreathing;
  SEScalarPressure*              m_TransairwayPressure;
  SEScalarPressure*              m_TransalveolarPressure;
  SEScalarPressure*              m_TransChestWallPressure;
  SEScalarPressure*              m_TransMusclePressure;
  SEScalarPressure*              m_TranspulmonaryPressure;
  SEScalarPressure*              m_TransrespiratoryPressure;
  SEScalarPressure*              m_TransthoracicPressure;
};