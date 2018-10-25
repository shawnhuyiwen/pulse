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
  double m_ArterialOxygenSetPoint_mmHg; /// \todo Aaron these should be baselines.
  double m_ArterialCarbonDioxideSetPoint_mmHg;

  // Stateless member variable (Set in SetUp())
  bool m_FeedbackActive;
  double m_dt_s;
  // Configuration fractions representing the amount by which each quantity can change due to baroreceptors;
  double m_normalizedGammaHeartRate;
  double m_normalizedGammaElastance;
  double m_normalizedGammaCompliance;
  double m_normalizedGammaResistance;
  double m_normalizedAlphaHeartRate;
  double m_normalizedAlphaElastance;
  double m_normalizedAlphaCompliance;
  double m_normalizedAlphaResistance;
  double m_normalizedBetaHeartRate;
};
