/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "engine/common/system/Model.h"
#include "engine/common/system/Systems.h"

class SEElectroCardioGramWaveform;

namespace pulse
{
  class PBEquipment;
  class EngineTest;
  /**
  * @brief
  * Generic ECG machine to assess the heart rhythm.
  */
  class PULSE_DECL ElectroCardioGramModel : public ElectroCardioGramSystem, public Model
  {
    friend PBEquipment;//friend the serialization class
    friend EngineTest;
  public:
    ElectroCardioGramModel(Data& pc);
    virtual ~ElectroCardioGramModel();

    void Clear();

    // Set members to a stable homeostatic state
    void Initialize();
    // Set pointers and other member varialbes common to both homeostatic initialization and loading a state
    void SetUp();

    // main driver function responsible for calling the various ECG functions:
    void AtSteadyState() {}
    void PreProcess();
    void Process(bool solve_and_transport = true);
    void PostProcess(bool solve_and_transport = true);

  protected:
    void ComputeExposedModelParameters() override;

    // Serializable member variables (Set in Initialize and in schema)

    // Stateless member variable (Set in SetUp())
  };
END_NAMESPACE
