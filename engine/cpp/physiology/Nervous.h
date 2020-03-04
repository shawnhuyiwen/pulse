/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "controller/System.h"
#include "system/physiology/SENervousSystem.h"
/**
 * @brief 
 * The nervous class holds models of the peripheral and central nervous system. Currently, on the baroreceptor reflex is modeled.
 */  
class PULSE_DECL Nervous : public SENervousSystem, public PulseNervousSystem, public PulseSystem
{
  friend class PBPulsePhysiology;//friend the serialization class
  friend class PulseController;
  friend class PulseEngineTest;
protected:
  Nervous(PulseController& data);
  PulseController& m_data;

public:
  virtual ~Nervous();

  void Clear();

protected:
  // Set members to a stable homeostatic state
  void Initialize();
  // Set pointers and other member variables common to both homeostatic initialization and loading a state
  void SetUp();

  void AtSteadyState();
  void PreProcess();
  void Process();
  void PostProcess();

  void BaroreceptorFeedback();
  void CheckBrainStatus();
  void ChemoreceptorFeedback();
  void SetPupilEffects();

  // Serializable member variables (Set in Initialize and in schema)
  double m_ArterialOxygenBaseline_mmHg;
  double m_ArterialCarbonDioxideBaseline_mmHg;
  double m_LastMeanArterialPressure_mmHg;
  //Baroreceptor threshold parameters
  bool   m_BaroreceptorFeedbackStatus;
  double m_BaroreceptorActiveTime_s;
  double m_BaroreceptorMeanArterialPressureBaseline_mmHg;
  double m_TotalSympatheticFraction;
  bool   m_BaroreceptorSaturationStatus;
  double m_BaroreceptorSaturationTime_s;
  double m_BaroreceptorEffectivenessParameter;
  double previousBloodVolume_mL;

  // Stateless member variable (Set in SetUp())
  bool m_FeedbackActive; // An overall flag used to turn off baro/chemo receptors during stabilization
  double m_dt_s;

  // Configuration fractions representing the amount by which each quantity can change due to baroreceptors;
  double m_NormalizedGammaHeartRate;
  double m_NormalizedGammaElastance;
  double m_NormalizedGammaCompliance;
  double m_NormalizedGammaResistance;
  double m_NormalizedAlphaHeartRate;
  double m_NormalizedAlphaElastance;
  double m_NormalizedAlphaCompliance;
  double m_NormalizedAlphaResistance;
  double m_NormalizedBetaHeartRate;
};
