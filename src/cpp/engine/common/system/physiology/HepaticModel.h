/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "engine/common/system/Model.h"
#include "engine/common/system/Systems.h"

namespace pulse
{
  class PBPhysiology;
  class EngineTest;
  /**
   * @copydoc Physiology_HepaticystemData
   */
  class PULSE_DECL HepaticModel : public HepaticSystem, public Model
  {
    friend PBPhysiology;//friend the serialization class
    friend EngineTest;
  public:
    HepaticModel(Data& data);
    virtual ~HepaticModel();

    void Clear() override;

    // Set members to a stable homeostatic state
    void Initialize() override;
    // Set pointers and other member varialbes common to both homeostatic initialization and loading a state
    void SetUp() override;

    void AtSteadyState() override;
    void PreProcess() override;
    void Process(bool solve_and_transport = true) override;
    void PostProcess(bool solve_and_transport = true) override;

  protected:
    void ComputeExposedModelParameters() override;

  };
END_NAMESPACE