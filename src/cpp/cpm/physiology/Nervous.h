/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "PulsePhysiologySystems.h"
#include "system/physiology/SENervousSystem.h"
class SEFluidCircuitNode;
class SEFluidCircuitPath;
class SEFluidCircuit;

/**
 * @brief 
 * The nervous class holds models of the peripheral and central nervous system. Currently, on the baroreceptor reflex is modeled.
 */  
class PULSE_DECL Nervous : public PulseNervousSystem
{
  friend class PulseController;
  friend class PBPulsePhysiology;//friend the serialization class
  friend class PulseEngineTest;
public:
  Nervous(PulseData& data);
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
  void CerebralSpinalFluidUpdates();

  // Serializable member variables (Set in Initialize and in schema)
  bool   m_BaroreceptorFeedbackStatus;
  bool   m_BaroreceptorSaturationStatus;
  double m_ArterialOxygenBaseline_mmHg;
  double m_ArterialCarbonDioxideBaseline_mmHg;
  double m_BaroreceptorActiveTime_s;
  double m_BaroreceptorEffectivenessParameter;
  double m_BaroreceptorMeanArterialPressureBaseline_mmHg;
  double m_BaroreceptorSaturationTime_s;
  double m_LastMeanArterialPressure_mmHg;
  double m_PreviousBloodVolume_mL;
  double m_TotalSympatheticFraction;

  //SERIALIZE THESE AARON - should be system values
  double m_CSFAbsorptionRate_mLPermin;
  double m_CSFProductionRate_mlPermin;

  // Stateless member variable (Set in SetUp())
  // Configuration fractions representing the amount by which each quantity can change due to feedback;
  double m_NormalizedGammaHeartRate;
  double m_NormalizedGammaElastance;
  double m_NormalizedGammaCompliance;
  double m_NormalizedGammaResistance;
  double m_NormalizedAlphaHeartRate;
  double m_NormalizedAlphaElastance;
  double m_NormalizedAlphaCompliance;
  double m_NormalizedAlphaResistance;
  double m_NormalizedBetaHeartRate;

  SEFluidCircuitNode* m_IntracranialSpace;
  SEFluidCircuitPath* m_CSFProductAbsorptionPath;
  SEFluidCircuitPath* m_BrainVasculatureCompliancePath;
  SEFluidCircuitPath* m_BrainVasculatureResistancePath;
};
