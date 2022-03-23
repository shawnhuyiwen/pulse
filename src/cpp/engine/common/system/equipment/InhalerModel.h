/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "engine/common/system/Model.h"
#include "engine/common/system/Systems.h"

class SEGasCompartment;
class SELiquidCompartment;
class SELiquidSubstanceQuantity;

namespace pulse
{
  class PBEquipment;
  class EngineTest;
  /**
  * @brief
  * Generic inhaler for substance administration.
  */
  class PULSE_DECL InhalerModel : public InhalerSystem, public Model
  {
    friend PBEquipment;//friend the serialization class
    friend EngineTest;
  public:
    InhalerModel(Data& pc);
    virtual ~InhalerModel();

    void Clear() override;

    // Set members to a stable homeostatic state
    void Initialize() override;
    // Set pointers and other member varialbes common to both homeostatic initialization and loading a state
    void SetUp() override;

    // main driver function responsible for calling the various ECG functions:
    void AtSteadyState() override {}
    void PreProcess() override;
    void Process(bool solve_and_transport = true) override;
    void PostProcess(bool solve_and_transport = true) override;

    void StateChange() override;
    void Administer();

  protected:
    void ComputeExposedModelParameters() override;

    // Serializable member variables (Set in Initialize and in schema)

    // Stateless member variable (Set in SetUp())
    SEGasCompartment* m_Mouthpiece;
    SELiquidCompartment* m_AerosolMouthpiece;
    SEGasCompartment* m_AmbientEnv;
    SELiquidSubstanceQuantity* m_InhalerDrug;
  };
END_NAMESPACE
