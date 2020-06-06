/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "PulsePhysiologySystems.h"
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
  friend class PulseData;
  friend class PBPulseEquipment;//friend the serialization class
  friend class PulseEngineTest;
protected:
  MechanicalVentilator(PulseData& pc);
  PulseData& m_data;

public:
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

  void ApplyBaseline();
  void ApplyTarget();  
  void CalculateInspiration();
  void CalculateExpiration();
  void SetVentilatorPressure();
  void SetResistances();

  // Serializable member variables (Set in Initialize and in schema)
  bool                  m_Inhaling;
  double                m_CurrentBreathingCycleTime_s;
  double                m_PressureTarget_cmH2O;
  double                m_PressureBaseline_cmH2O;
  double                m_DriverPressure_cmH2O;

  // Stateless member variable (Set in SetUp())
  double                m_dt_s;

  SEGasCompartment*     m_Environment;
  SEGasCompartment*     m_Ventilator;
  SELiquidCompartment*  m_VentilatorAerosol;
  SEFluidCircuitPath*   m_EnvironmentToVentilator;
};
