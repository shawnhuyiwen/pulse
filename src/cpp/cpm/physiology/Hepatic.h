/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "controller/System.h"
#include "system/physiology/SEHepaticSystem.h"

/**
 * @copydoc Physiology_HepaticystemData
 */  
class PULSE_DECL Hepatic : public SEHepaticSystem, public PulseHepaticSystem, public PulseSystem
{
  friend class PulseData;
  friend class PBPulsePhysiology;//friend the serialization class
  friend class PulseEngineTest;
protected:
  Hepatic(PulseData& data);
  PulseData& m_data;

public:
  virtual ~Hepatic();

  void Clear();

  // Set members to a stable homeostatic state
  void Initialize();
  // Set pointers and other member varialbes common to both homeostatic initialization and loading a state
  void SetUp();

  void AtSteadyState();
  void PreProcess();
  void Process();
  void PostProcess();

protected:
};