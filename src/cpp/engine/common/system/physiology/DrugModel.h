/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "engine/common/system/Model.h"
#include "engine/common/system/Systems.h"

class SEFluidCircuitPath;
class SELiquidCompartment;
class SETissueCompartment;
class SESubstanceCompound;

namespace pulse
{
  class PBPhysiology;
  class EngineTest;
  /**
   * @brief
   * The drug system contains the physiologically based pharmacokinetic (PBPK) model and the pharmacodynamic (PD) model.
   * @details
   * Drug transvascular transport is modeled with a physiologically-based pharmacokinetic (PBPK) model,
   * and the physiologic effects on the body are modeled with a low-fidelity pharmacodynamic (PD) model.
   */
  class PULSE_DECL DrugModel : public DrugSystem, public Model
  {
    friend PBPhysiology;//friend the serialization class
    friend EngineTest;
  public:
    DrugModel(Data& data);
    virtual ~DrugModel();

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

    void AdministerSubstanceBolus();
    void AdministerSubstanceInfusion();
    void AdministerSubstanceCompoundInfusion();

    void CalculatePartitionCoefficients();
    void CalculateSubstanceClearance();
    void CalculatePlasmaSubstanceConcentration();
    void CalculateDrugEffects();

    // Stateless member variable (Set in SetUp)
    SESubstanceCompound* m_Saline;
    SESubstanceCompound* m_Blood;
    SELiquidCompartment* m_aortaVascular;
    SELiquidCompartment* m_venaCavaVascular;
    SELiquidCompartment* m_muscleIntracellular;
    SELiquidCompartment* m_liverVascular;
    SETissueCompartment* m_liverTissue;
    SEFluidCircuitPath* m_IVToVenaCava;
    SETissueCompartment* m_fatTissue;
  };
END_NAMESPACE