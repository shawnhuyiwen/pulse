/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/


#pragma once
#include "Controller/System.h"
#include "system/equipment/inhaler/SEInhaler.h"
class SEGasCompartment;
class SELiquidCompartment;
class SELiquidSubstanceQuantity;
PULSE_BIND_DECL(InhalerData)

/**
* @brief 
* Generic inhaler for substance administration.
*/
class PULSE_DECL Inhaler : public SEInhaler, public PulseInhaler, public PulseSystem
{
  friend class PulseController;
  friend class PulseEngineTest;
protected:
  Inhaler(PulseController& pc);
  PulseController& m_data;

public:
  virtual ~Inhaler();

  void Clear();

  static void Load(const pulse::InhalerData& src, Inhaler& dst);
  static pulse::InhalerData* Unload(const Inhaler& src);
protected:
  static void Serialize(const pulse::InhalerData& src, Inhaler& dst);
  static void Serialize(const Inhaler& src, pulse::InhalerData& dst);

  // Set members to a stable homeostatic state
  void Initialize();
  // Set pointers and other member varialbes common to both homeostatic initialization and loading a state
  void SetUp();

  // main driver function responsible for calling the various ECG functions:
  void PreProcess();
  void Process();
  void PostProcess();

  void StateChange();
  void Administer();

  // Serializable member variables (Set in Initialize and in schema)

  // Stateless member variable (Set in SetUp())
  double                     m_dt_s;
  SEGasCompartment          *m_Mouthpiece;
  SELiquidCompartment       *m_AerosolMouthpiece;
  SEGasCompartment          *m_AmbientEnv;
  SELiquidSubstanceQuantity *m_InhalerDrug;
};
