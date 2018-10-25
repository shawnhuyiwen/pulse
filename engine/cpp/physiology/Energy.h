/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "controller/System.h"
#include "system/physiology/SEEnergySystem.h"
class SEPatient;
class SELiquidSubstanceQuantity;
class SEFluidCircuitPath;
class SEThermalCircuit;
class SEThermalCircuitNode;
class SEThermalCircuitPath;
class SEThermalCircuitCalculator;

/**
 * @brief @copydoc Physiology_EnergySystemData
 */  
class PULSE_DECL Energy : public SEEnergySystem, public PulseEnergySystem, public PulseSystem
{
  friend class PBPulsePhysiology;//friend the serialization class
  friend class PulseController;
  friend class PulseEngineTest;
protected:
  Energy(PulseController& data);
  PulseController& m_data;

public:
  ~Energy(void);

  void Clear();

protected:
  // Set members to a stable homeostatic state
  void Initialize();
  // Set pointers and other member varialbes common to both homeostatic initialization and loading a state
  void SetUp();

  void AtSteadyState();
  void PreProcess();
  void Process();
  void PostProcess();

  // Preprocess Methods
  void CalculateMetabolicHeatGeneration();
  void CalculateSweatRate();
  void UpdateHeatResistance();
  // Actions/Conditions
  void Exercise();
  // These are both part of the consume meal condition.
  //void Dehydration(double time); // Need to revisit
  //void Starvation(double time); // Need to revisit
  // Used in Reset & Starvation
  void CalculateBasalMetabolicRate();

  // Process Methods
  void CalculateVitalSigns();

  // Serializable member variables (Set in Initialize and in schema)
  double m_UsableEnergyStore_J;
  double m_PeakPowerEnergyStore_J;
  double m_MediumPowerEnergyStore_J;
  double m_EnduranceEnergyStore_J;
  SERunningAverage* m_BloodpH;
  SERunningAverage* m_BicarbonateMolarity_mmol_Per_L;

  // Stateless member variable (Set in SetUp())
  double                      m_dT_s;
  SEPatient*                  m_Patient;
  // Cmpts,Substance, and nodes
  SELiquidSubstanceQuantity*  m_AortaHCO3;  
  //Nodes
  SEThermalCircuitNode*       m_coreNode;
  SEThermalCircuitNode*       m_skinNode;
  //Paths
  SEThermalCircuitPath*       m_temperatureGroundToCorePath;
  SEThermalCircuitPath*       m_coreToSkinPath;
  SEFluidCircuitPath*         m_skinExtravascularToSweatingGroundPath;   
  //Circuits
  SEThermalCircuit*           m_InternalTemperatureCircuit;
  SEThermalCircuit*           m_TemperatureCircuit;
  SEThermalCircuitCalculator* m_circuitCalculator;
};
