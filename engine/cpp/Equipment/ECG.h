/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "controller/System.h"
#include "system/equipment/electrocardiogram/SEElectroCardioGram.h"
class SEElectroCardioGramWaveformInterpolator;

/**
* @brief 
* Generic ECG machine to assess the heart rhythm.
*/
class PULSE_DECL ECG : public SEElectroCardioGram, public PulseElectroCardioGram, public PulseSystem
{
  friend class PBPulseEquipment;//friend the serialization class
  friend class PulseController;
  friend class PulseEngineTest;
protected:
  ECG(PulseController& pc);
  PulseController& m_data;

public:
  virtual ~ECG();

  void Clear();

protected:
  // Set members to a stable homeostatic state
  void Initialize();
  // Set pointers and other member varialbes common to both homeostatic initialization and loading a state
  void SetUp();

  // main driver function responsible for calling the various ECG functions:
  void PreProcess();
  void Process();
  void PostProcess();

  // Serializable member variables (Set in Initialize and in schema)
  double m_heartRhythmTime_s;
  double m_heartRhythmPeriod_s;
  SEElectroCardioGramWaveformInterpolator* m_interpolator;

  // Stateless member variable (Set in SetUp())
  double m_dt_s;
};
