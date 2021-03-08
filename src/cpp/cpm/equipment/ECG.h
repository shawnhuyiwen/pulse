/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "PulsePhysiologySystems.h"
#include "system/equipment/electrocardiogram/SEElectroCardioGram.h"
class SEElectroCardioGramWaveformInterpolator;

/**
* @brief 
* Generic ECG machine to assess the heart rhythm.
*/
class PULSE_DECL ECG : public PulseElectroCardioGram
{
  friend class PulseController;
  friend class PBPulseEquipment;//friend the serialization class
  friend class PulseEngineTest;
public:
  ECG(PulseData& pc);
  virtual ~ECG();

  void Clear();

  // Set members to a stable homeostatic state
  void Initialize();
  // Set pointers and other member varialbes common to both homeostatic initialization and loading a state
  void SetUp();

  // main driver function responsible for calling the various ECG functions:
  void AtSteadyState() {}
  void PreProcess();
  void Process(bool solve_and_transport=true);
  void PostProcess(bool solve_and_transport=true);

protected:
  void ComputeExposedModelParameters() override;

  // Serializable member variables (Set in Initialize and in schema)
  double m_heartRhythmTime_s;
  double m_heartRhythmPeriod_s;
  SEElectroCardioGramWaveformInterpolator* m_interpolator;

  // Stateless member variable (Set in SetUp())
};
