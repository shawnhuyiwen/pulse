/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "PulsePhysiologySystems.h"
#include "system/physiology/SEHepaticSystem.h"

/**
 * @copydoc Physiology_HepaticystemData
 */  
class PULSE_DECL Hepatic : public PulseHepaticSystem
{
  friend class PulseController;
  friend class PBPulsePhysiology;//friend the serialization class
  friend class PulseEngineTest;
public:
  Hepatic(PulseData& data);
  virtual ~Hepatic();

  void Clear();

  // Set members to a stable homeostatic state
  void Initialize();
  // Set pointers and other member varialbes common to both homeostatic initialization and loading a state
  void SetUp();

  void AtSteadyState();
  void PreProcess();
  void Process(bool solve_and_transport=true);
  void PostProcess(bool solve_and_transport=true);

protected:
  void ComputeExposedModelParameters() override;

};