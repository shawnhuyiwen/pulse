/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "engine/common/system/Model.h"
#include "engine/common/system/Systems.h"

class SEEquipmentActionCollection;
class SEGasCompartment;
class SELiquidCompartment;
class SEGasSubstanceQuantity;
class SEFluidCircuitNode;
class SEFluidCircuitPath;

namespace pulse
{
  class PBEquipment;
  class EngineTest;
  /**
   * @brief
   * Generic ECMO.
   */
  class PULSE_DECL ECMOModel : public ECMOSystem, public Model
  {
    friend PBEquipment;//friend the serialization class
    friend EngineTest;
  public:
    ECMOModel(Data& pc);
    virtual ~ECMOModel();

    void Clear() override;

    // Set members to a stable homeostatic state
    void Initialize() override;
    // Set pointers and other member varialbes common to both homeostatic initialization and loading a state
    void SetUp() override;

    void StateChange() override;

    void AtSteadyState() override {}
    void PreProcess() override;
    void Process(bool solve_and_transport = true) override;
    void PostProcess(bool solve_and_transport = true) override;

  protected:
    void ComputeExposedModelParameters() override;
    void DisconnectECMO();

    // Serializable member variables (Set in Initialize and in schema)

    // Stateless member variable (Set in SetUp())
    eECMO_CannulationLocation m_CurrentInflowLocation;
    eECMO_CannulationLocation m_CurrentOutflowLocation;

    SELiquidCompartment*     m_InflowCmpt;
    SELiquidCompartmentLink* m_lVascularToBloodSamplingPort;
    SELiquidCompartment*     m_cBloodSamplingPort;
    SELiquidCompartmentLink* m_lBloodSamplingPortToOxygenator;
    SELiquidCompartment*     m_cOxygenator;
    SELiquidCompartmentLink* m_lOxygenatorToVascular;
    SELiquidCompartment*     m_OutflowCmpt;


    SEFluidCircuitNode*      m_InflowNode;
    SEFluidCircuitPath*      m_pVascularToBloodSamplingPort;
    SEFluidCircuitNode*      m_nBloodSamplingPort;
    SEFluidCircuitPath*      m_pBloodSamplingPortToGround;
    SEFluidCircuitPath*      m_pGroundToOxygenator;
    SEFluidCircuitNode*      m_nOxygenator;
    SEFluidCircuitPath*      m_pOxygenatorToVascular;
    SEFluidCircuitNode*      m_OutflowNode;
  };
END_NAMESPACE
