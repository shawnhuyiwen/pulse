/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "PulsePhysiologySystems.h"
#include "system/physiology/SENervousSystem.h"
/**
 * @brief 
 * The nervous class holds models of the peripheral and central nervous system. Currently, on the baroreceptor reflex is modeled.
 */  
class PULSE_DECL Nervous : public PulseNervousSystem
{
  friend class PulseData;
  friend class PBPulsePhysiology;//friend the serialization class
  friend class PulseEngineTest;
protected:
  Nervous(PulseData& data);
  PulseData& m_data;

public:
  virtual ~Nervous();

  void Clear();

  // Set members to a stable homeostatic state
  void Initialize();
  // Set pointers and other member variables common to both homeostatic initialization and loading a state
  void SetUp();

  void AtSteadyState();
  void PreProcess();
  void Process(bool solve_and_transport=true);
  void PostProcess(bool solve_and_transport=true);

protected:
  void ComputeExposedModelParameters() override;

  void BaroreceptorFeedback();
  void CheckBrainStatus();
  void ChemoreceptorFeedback();
  void SetPupilEffects();

  // Serializable member variables (Set in Initialize and in schema)
  double m_ArterialOxygenBaseline_mmHg;
  double m_ArterialCarbonDioxideBaseline_mmHg;
  //Baroreceptor threshold parameters
  bool   m_BaroreceptorFeedbackStatus;
  double m_BaroreceptorActiveTime_s;
  double m_BaroreceptorMeanArterialPressureBaseline_mmHg;

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
