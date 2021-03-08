/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "PulsePhysiologySystems.h"
#include "system/physiology/SERespiratorySystem.h"
#include "system/equipment/mechanical_ventilator/SEMechanicalVentilator.h"
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
class PULSE_DECL MechanicalVentilator : public PulseMechanicalVentilator
{
  friend class PulseController;
  friend class PBPulseEquipment;//friend the serialization class
  friend class PulseEngineTest;
public:
  MechanicalVentilator(PulseData& pc);
  virtual ~MechanicalVentilator();

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
  void SetConnection(eMechanicalVentilator_Connection c);
  void InvalidateConnection();

  void CalculateInspiration();
  void CalculatePause();
  void CalculateExpiration();
  void SetVentilatorDriver();
  void CycleMode();
  void SetResistances();

  // Serializable member variables (Set in Initialize and in schema)
  double                m_CurrentPeriodTime_s;
  double                m_DriverPressure_cmH2O;
  double                m_DriverFlow_L_Per_s;
  double                m_CurrentInspiratoryVolume_L;
  eBreathState          m_CurrentBreathState;

  // Stateless member variable (Set in SetUp())
  SEGasCompartment*     m_Environment;
  SEGasCompartment*     m_Ventilator;
  SELiquidCompartment*  m_VentilatorAerosol;
  SEFluidCircuitNode*   m_VentilatorNode;
  SEFluidCircuitNode*   m_ConnectionNode;
  SEFluidCircuitNode*   m_AmbientNode;
  SEFluidCircuitPath*   m_EnvironmentToVentilator;
  SEFluidCircuitPath*   m_YPieceToConnection;
};
