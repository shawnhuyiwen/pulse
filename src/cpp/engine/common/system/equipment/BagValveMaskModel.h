/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "engine/common/system/Model.h"
#include "engine/common/system/Systems.h"

class SEGasCompartment;
class SELiquidCompartment;
class SEGasSubstanceQuantity;
class SEFluidCircuitNode;
class SEFluidCircuitPath;

namespace PULSE_ENGINE
{
  class PBEquipment;
  class EngineTest;
  /**
   * @brief
   * Generic Bag Valve Mask for positive pressure ventilation.
   */
  class PULSE_DECL BagValveMaskModel : public BagValveMaskSystem, public Model
  {
    friend PBEquipment;//friend the serialization class
    friend EngineTest;
  public:
    BagValveMaskModel(Data& pc);
    virtual ~BagValveMaskModel();

    void Clear();

    // Set members to a stable homeostatic state
    void Initialize();
    // Set pointers and other member varialbes common to both homeostatic initialization and loading a state
    void SetUp();

    void StateChange();

    void AtSteadyState() {}
    void PreProcess();
    void Process(bool solve_and_transport = true);
    void PostProcess(bool solve_and_transport = true);

  protected:

    void ComputeExposedModelParameters() override;

    void SetConnection();
    void SetConnection(eBagValveMask_Connection c);
    void InvalidateConnection();

    void CalculateInspiration();
    void CalculateExpiration();
    void SetSqeezeDriver();
    void CycleMode();
    void SetResistances();
    void SetVolumes();

    // Serializable member variables (Set in Initialize and in schema)
    eBreathState          m_CurrentBreathState;
    double                m_CurrentPeriodTime_s;
    double                m_SqueezeFlow_L_Per_s;
    double                m_SqueezePressure_cmH2O;

    // Stateless member variable (Set in SetUp())
    SEScalarFrequency* m_BreathFrequency;
    SEScalar* m_InspiratoryExpiratoryRatio;
    SEScalarPressure* m_SqueezePressure;
    SEScalarVolume* m_SqueezeVolume;
    SEScalarTime* m_ExpiratoryPeriod;
    SEScalarTime* m_InspiratoryPeriod;

    SEGasCompartment* m_Environment;
    SEGasCompartment* m_Reservoir;
    SELiquidCompartment* m_ReservoirAerosol;
    SEFluidCircuitNode* m_FilterNode;
    SEFluidCircuitNode* m_ConnectionNode;
    SEFluidCircuitNode* m_ValveNode;
    SEFluidCircuitPath* m_ReservoirToBag;
    SEFluidCircuitPath* m_BagToValve;
    SEFluidCircuitPath* m_ValveToFilter;
    SEFluidCircuitPath* m_FilterToConnection;
    SEFluidCircuitPath* m_ConnectionToEnvironment;
  };
}
