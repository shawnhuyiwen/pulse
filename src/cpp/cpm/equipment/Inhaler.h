/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "PulsePhysiologySystems.h"
#include "system/equipment/inhaler/SEInhaler.h"
class SEGasCompartment;
class SELiquidCompartment;
class SELiquidSubstanceQuantity;

/**
* @brief 
* Generic inhaler for substance administration.
*/
class PULSE_DECL Inhaler : public PulseInhaler
{
  friend class PulseController;
  friend class PBPulseEquipment;//friend the serialization class
  friend class PulseEngineTest;
public:
  Inhaler(PulseData& pc);
  virtual ~Inhaler();

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

  void StateChange();
  void Administer();

protected:
  void ComputeExposedModelParameters() override;

  // Serializable member variables (Set in Initialize and in schema)

  // Stateless member variable (Set in SetUp())
  SEGasCompartment          *m_Mouthpiece;
  SELiquidCompartment       *m_AerosolMouthpiece;
  SEGasCompartment          *m_AmbientEnv;
  SELiquidSubstanceQuantity *m_InhalerDrug;
};
