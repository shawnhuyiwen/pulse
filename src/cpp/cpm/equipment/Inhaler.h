/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "controller/System.h"
#include "system/equipment/inhaler/SEInhaler.h"
class SEGasCompartment;
class SELiquidCompartment;
class SELiquidSubstanceQuantity;

/**
* @brief 
* Generic inhaler for substance administration.
*/
class PULSE_DECL Inhaler : public SEInhaler, public PulseInhaler, public PulseSystem
{
  friend class PulseData;
  friend class PBPulseEquipment;//friend the serialization class
  friend class PulseEngineTest;
protected:
  Inhaler(PulseData& pc);
  PulseData& m_data;

public:
  virtual ~Inhaler();

  void Clear();

  // Set members to a stable homeostatic state
  void Initialize();
  // Set pointers and other member varialbes common to both homeostatic initialization and loading a state
  void SetUp();

  // main driver function responsible for calling the various ECG functions:
  void AtSteadyState() {}
  void PreProcess();
  void Process();
  void PostProcess();

  void StateChange();
  void Administer();

protected:
  // Serializable member variables (Set in Initialize and in schema)

  // Stateless member variable (Set in SetUp())
  double                     m_dt_s;
  SEGasCompartment          *m_Mouthpiece;
  SELiquidCompartment       *m_AerosolMouthpiece;
  SEGasCompartment          *m_AmbientEnv;
  SELiquidSubstanceQuantity *m_InhalerDrug;
};
