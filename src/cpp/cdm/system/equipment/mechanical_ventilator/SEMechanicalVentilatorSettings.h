/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/CommonDefs.h"
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/substance/SESubstanceConcentration.h"
#include "cdm/substance/SESubstanceFraction.h"

class SEMechanicalVentilatorConfiguration;

// Keep enums in sync with appropriate schema/cdm/MechanicalVentilator.proto file !!
enum class eMechanicalVentilator_DriverWaveform { NullDriverWaveform = 0, Square, Exponential, Ramp, Sinusoidal, Sigmoidal };
extern const std::string& eMechanicalVentilator_DriverWaveform_Name(eMechanicalVentilator_DriverWaveform m);

class CDM_DECL SEMechanicalVentilatorSettings : public Loggable
{
  friend class PBMechanicalVentilator;//friend the serialization class
public:

  SEMechanicalVentilatorSettings(Logger* logger);
  virtual ~SEMechanicalVentilatorSettings();

  virtual void Clear();

  bool SerializeToString(std::string& output, eSerializationFormat m) const;
  bool SerializeToFile(const std::string& filename) const;
  bool SerializeFromString(const std::string& src, eSerializationFormat m, const SESubstanceManager& subMgr);
  bool SerializeFromFile(const std::string& filename, const SESubstanceManager& subMgr);

  virtual void Merge(const SEMechanicalVentilatorSettings& from, SESubstanceManager& subMgr);
  virtual void ProcessConfiguration(SEMechanicalVentilatorConfiguration& config, SESubstanceManager& subMgr);

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual eSwitch GetConnection() const;
  virtual void SetConnection(eSwitch c);

  virtual bool HasConnectionVolume() const;
  virtual SEScalarVolume& GetConnectionVolume();
  virtual double GetConnectionVolume(const VolumeUnit& unit) const;

  virtual bool HasCompliance() const;
  virtual SEScalarVolumePerPressure& GetCompliance();
  virtual double GetCompliance(const VolumePerPressureUnit& unit) const;

  virtual bool HasDriverDampingParameter() const;
  virtual SEScalarFrequency& GetDriverDampingParameter();
  virtual double GetDriverDampingParameter(const FrequencyUnit& unit) const;

  virtual bool HasPositiveEndExpiredPressure() const;
  virtual SEScalarPressure& GetPositiveEndExpiredPressure();
  virtual double GetPositiveEndExpiredPressure(const PressureUnit& unit) const;

  virtual bool HasFunctionalResidualCapacity() const;
  virtual SEScalarPressure& GetFunctionalResidualCapacity();
  virtual double GetFunctionalResidualCapacity(const PressureUnit& unit) const;

  virtual bool HasExpirationCycleFlow() const;
  virtual SEScalarVolumePerTime& GetExpirationCycleFlow();
  virtual double GetExpirationCycleFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasExpirationCyclePressure() const;
  virtual SEScalarPressure& GetExpirationCyclePressure();
  virtual double GetExpirationCyclePressure(const PressureUnit& unit) const;

  virtual bool HasExpirationCycleVolume() const;
  virtual SEScalarVolume& GetExpirationCycleVolume();
  virtual double GetExpirationCycleVolume(const VolumeUnit& unit) const;

  virtual bool HasExpirationCycleTime() const;
  virtual SEScalarTime& GetExpirationCycleTime();
  virtual double GetExpirationCycleTime(const TimeUnit& unit) const;

  virtual eSwitch GetExpirationCycleRespiratoryModel() const;
  virtual void SetExpirationCycleRespiratoryModel(eSwitch c);

  virtual bool HasExpirationTubeResistance() const;
  virtual SEScalarPressureTimePerVolume& GetExpirationTubeResistance();
  virtual double GetExpirationTubeResistance(const PressureTimePerVolumeUnit& unit) const;

  virtual bool HasExpirationValveResistance() const;
  virtual SEScalarPressureTimePerVolume& GetExpirationValveResistance();
  virtual double GetExpirationValveResistance(const PressureTimePerVolumeUnit& unit) const;

  virtual eMechanicalVentilator_DriverWaveform GetExpirationWaveform() const;
  virtual void SetExpirationWaveform(eMechanicalVentilator_DriverWaveform w);

  virtual bool HasExpirationWaveformPeriod() const;
  virtual SEScalarTime& GetExpirationWaveformPeriod();
  virtual double GetExpirationWaveformPeriod(const TimeUnit& unit) const;

  virtual bool HasInspirationLimitFlow() const;
  virtual SEScalarVolumePerTime& GetInspirationLimitFlow();
  virtual double GetInspirationLimitFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasInspirationLimitPressure() const;
  virtual SEScalarPressure& GetInspirationLimitPressure();
  virtual double GetInspirationLimitPressure(const PressureUnit& unit) const;

  virtual bool HasInspirationLimitVolume() const;
  virtual SEScalarVolume& GetInspirationLimitVolume();
  virtual double GetInspirationLimitVolume(const VolumeUnit& unit) const;

  virtual bool HasInspirationPauseTime() const;
  virtual SEScalarTime& GetInspirationPauseTime();
  virtual double GetInspirationPauseTime(const TimeUnit& unit) const;

  virtual bool HasPeakInspiratoryPressure() const;
  virtual SEScalarPressure& GetPeakInspiratoryPressure();
  virtual double GetPeakInspiratoryPressure(const PressureUnit& unit) const;

  virtual bool HasInspirationTargetFlow() const;
  virtual SEScalarVolumePerTime& GetInspirationTargetFlow();
  virtual double GetInspirationTargetFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasInspirationMachineTriggerTime() const;
  virtual SEScalarTime& GetInspirationMachineTriggerTime();
  virtual double GetInspirationMachineTriggerTime(const TimeUnit& unit) const;

  virtual bool HasInspirationPatientTriggerFlow() const;
  virtual SEScalarVolumePerTime& GetInspirationPatientTriggerFlow();
  virtual double GetInspirationPatientTriggerFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasInspirationPatientTriggerPressure() const;
  virtual SEScalarPressure& GetInspirationPatientTriggerPressure();
  virtual double GetInspirationPatientTriggerPressure(const PressureUnit& unit) const;

  virtual eSwitch GetInspirationPatientTriggerRespiratoryModel() const;
  virtual void SetInspirationPatientTriggerRespiratoryModel(eSwitch c);

  virtual bool HasInspirationTubeResistance() const;
  virtual SEScalarPressureTimePerVolume& GetInspirationTubeResistance();
  virtual double GetInspirationTubeResistance(const PressureTimePerVolumeUnit& unit) const;

  virtual bool HasInspirationValveResistance() const;
  virtual SEScalarPressureTimePerVolume& GetInspirationValveResistance();
  virtual double GetInspirationValveResistance(const PressureTimePerVolumeUnit& unit) const;

  virtual bool HasExpirationLimbVolume() const;
  virtual SEScalarVolume& GetExpirationLimbVolume();
  virtual double GetExpirationLimbVolume(const VolumeUnit& unit) const;

  virtual bool HasExpirationValveVolume() const;
  virtual SEScalarVolume& GetExpirationValveVolume();
  virtual double GetExpirationValveVolume(const VolumeUnit& unit) const;

  virtual bool HasInspirationLimbVolume() const;
  virtual SEScalarVolume& GetInspirationLimbVolume();
  virtual double GetInspirationLimbVolume(const VolumeUnit& unit) const;

  virtual bool HasInspirationValveVolume() const;
  virtual SEScalarVolume& GetInspirationValveVolume();
  virtual double GetInspirationValveVolume(const VolumeUnit& unit) const;

  virtual bool HasYPieceVolume() const;
  virtual SEScalarVolume& GetYPieceVolume();
  virtual double GetYPieceVolume(const VolumeUnit& unit) const;


  virtual eMechanicalVentilator_DriverWaveform GetInspirationWaveform() const;
  virtual void SetInspirationWaveform(eMechanicalVentilator_DriverWaveform w);

  virtual bool HasInspirationWaveformPeriod() const;
  virtual SEScalarTime& GetInspirationWaveformPeriod();
  virtual double GetInspirationWaveformPeriod(const TimeUnit& unit) const;

  bool HasFractionInspiredGas() const;
  bool HasFractionInspiredGas(const SESubstance& substance) const;
  const std::vector<SESubstanceFraction*>& GetFractionInspiredGases();
  const std::vector<const SESubstanceFraction*>& GetFractionInspiredGases() const;
  SESubstanceFraction& GetFractionInspiredGas(const SESubstance& substance);
  const SESubstanceFraction* GetFractionInspiredGas(const SESubstance& substance) const;
  void RemoveFractionInspiredGas(const SESubstance& substance);
  void RemoveFractionInspiredGases();

  bool HasConcentrationInspiredAerosol() const;
  bool HasConcentrationInspiredAerosol(const SESubstance& substance) const;
  const std::vector<SESubstanceConcentration*>& GetConcentrationInspiredAerosols();
  const std::vector<const SESubstanceConcentration*>& GetConcentrationInspiredAerosols() const;
  SESubstanceConcentration& GetConcentrationInspiredAerosol(const SESubstance& substance);
  const SESubstanceConcentration* GetConcentrationInspiredAerosol(const SESubstance& substance) const;
  void RemoveConcentrationInspiredAerosol(const SESubstance& substance);
  void RemoveConcentrationInspiredAerosols();

protected:
  
  eSwitch                                      m_Connection;
  SEScalarVolume*                              m_ConnectionVolume;
  SEScalarVolumePerPressure*                   m_Compliance;
  SEScalarFrequency*                           m_DriverDampingParameter;

  // Expiratory Baseline Properties (Only set 1)
  SEScalarPressure*                            m_PositiveEndExpiredPressure;
  SEScalarPressure*                            m_FunctionalResidualCapacity;

  // Expriatory Cycle Properties (Only Set 1)
  SEScalarVolumePerTime*                       m_ExpirationCycleFlow;
  SEScalarPressure*                            m_ExpirationCyclePressure;
  SEScalarVolume*                              m_ExpirationCycleVolume;
  SEScalarTime*                                m_ExpirationCycleTime;
  eSwitch                                      m_ExpirationCycleRespiratoryModel;

  SEScalarPressureTimePerVolume*               m_ExpirationTubeResistance;
  SEScalarPressureTimePerVolume*               m_ExpirationValveResistance;
  SEScalarVolume*                              m_ExpirationLimbVolume;
  SEScalarVolume*                              m_ExpirationValveVolume;
  eMechanicalVentilator_DriverWaveform         m_ExpirationWaveform;
  SEScalarTime*                                m_ExpirationWaveformPeriod;

  // Inspiratory Limit Properties (Only set 1)
  SEScalarVolumePerTime*                       m_InspirationLimitFlow;
  SEScalarPressure*                            m_InspirationLimitPressure;
  SEScalarVolume*                              m_InspirationLimitVolume;

  SEScalarTime*                                m_InspirationPauseTime;

  // Inspiratory Target Properties (Only set 1)
  SEScalarPressure*                            m_PeakInspiratoryPressure;
  SEScalarVolumePerTime*                       m_InspirationTargetFlow;

  // Inspiratory Machine Trigger Properties (Only set 1)
  SEScalarTime*                                m_InspirationMachineTriggerTime;

  // Inspiratory Patient Trigger Properties (Only set 1)
  SEScalarVolumePerTime*                       m_InspirationPatientTriggerFlow;
  SEScalarPressure*                            m_InspirationPatientTriggerPressure;
  eSwitch                                      m_InspirationPatientTriggerRespiratoryModel;

  SEScalarPressureTimePerVolume*               m_InspirationTubeResistance;
  SEScalarPressureTimePerVolume*               m_InspirationValveResistance;
  SEScalarVolume*                              m_InspirationLimbVolume;
  SEScalarVolume*                              m_InspirationValveVolume;
  eMechanicalVentilator_DriverWaveform         m_InspirationWaveform;
  SEScalarTime*                                m_InspirationWaveformPeriod;

  SEScalarVolume*                              m_YPieceVolume;

  std::vector<SESubstanceFraction*>            m_FractionInspiredGases;
  std::vector<const SESubstanceFraction*>      m_cFractionInspiredGases;

  std::vector<SESubstanceConcentration*>       m_ConcentrationInspiredAerosols;
  std::vector<const SESubstanceConcentration*> m_cConcentrationInspiredAerosols;
};
