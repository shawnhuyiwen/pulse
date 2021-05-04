/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "PulsePhysiologySystems.h"
#include "system/physiology/SERespiratorySystem.h"
#include "system/equipment/bag_valve_mask/SEBagValveMask.h"
class SEEquipmentActionCollection;
class SEGasCompartment;
class SELiquidCompartment;
class SEGasSubstanceQuantity;
class SEFluidCircuitNode;
class SEFluidCircuitPath;

/**
 * @brief 
 * Generic mechanical ventilator for positive pressure ventilation.
 */
class PULSE_DECL BagValveMask : public PulseBagValveMask
{
  friend class PulseController;
  friend class PBPulseEquipment;//friend the serialization class
  friend class PulseEngineTest;
public:
  BagValveMask(PulseData& pc);
  virtual ~BagValveMask();

  void Clear();

  // Set members to a stable homeostatic state
  void Initialize();
  // Set pointers and other member varialbes common to both homeostatic initialization and loading a state
  void SetUp();

  void StateChange();

  void AtSteadyState() {}
  void PreProcess();
  void Process(bool solve_and_transport=true);
  void PostProcess(bool solve_and_transport=true);

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

  // Serializable member variables (Set in Initialize and in schema)
  eBreathState          m_CurrentBreathState;
  double                m_CurrentPeriodTime_s;
  double                m_SqueezeFlow_L_Per_s;
  double                m_SqueezePressure_cmH2O;

  // Stateless member variable (Set in SetUp())
  SEGasCompartment*     m_Environment;
  SEGasCompartment*     m_Reservoir;
  SELiquidCompartment*  m_ReservoirAerosol;
  SEFluidCircuitNode*   m_ReservoirNode;
  SEFluidCircuitNode*   m_ConnectionNode;
  SEFluidCircuitNode*   m_AmbientNode;
  SEFluidCircuitPath*   m_ReservoirToBag;
  SEFluidCircuitPath*   m_ValveToEnvironment;
  SEFluidCircuitPath*   m_EnvironmentToPositiveEndExpiratoryPressurePort;
  SEFluidCircuitPath*   m_ConnectionToEnvironment;
  double                m_DefaultOpenResistance_cmH2O_s_Per_L;
  double                m_DefaultClosedResistance_cmH2O_s_Per_L;
};
