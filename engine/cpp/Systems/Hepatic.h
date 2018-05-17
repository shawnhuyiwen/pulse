/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "Controller/System.h"
#include "system/physiology/SEHepaticSystem.h"
PULSE_BIND_DECL(HepaticSystemData)

/**
 * @copydoc Physiology_HepaticystemData
 */  
class PULSE_DECL Hepatic : public SEHepaticSystem, public PulseHepaticSystem, public PulseSystem
{
  friend class PulseController;
  friend class PulseEngineTest;
protected:
  Hepatic(PulseController& data);
  PulseController& m_data;

public:
  virtual ~Hepatic();

  void Clear();

  static void Load(const pulse::HepaticSystemData& src, Hepatic& dst);
  static pulse::HepaticSystemData* Unload(const Hepatic& src);
protected:
  static void Serialize(const pulse::HepaticSystemData& src, Hepatic& dst);
  static void Serialize(const Hepatic& src, pulse::HepaticSystemData& dst);

  // Set members to a stable homeostatic state
  void Initialize();
  // Set pointers and other member varialbes common to both homeostatic initialization and loading a state
  void SetUp();

  void AtSteadyState();
  void PreProcess();
  void Process();
  void PostProcess();

};