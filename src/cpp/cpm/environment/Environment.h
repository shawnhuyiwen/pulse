/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "PulsePhysiologySystems.h"
#include "system/environment/SEEnvironment.h"
class SEPatient;
class SEGasCompartment;
class SELiquidCompartment;
class SEThermalCircuit;
class SEThermalCircuitNode;
class SEThermalCircuitPath;

/**
 * @brief The %Environment class characterizes the environment and manages interactions between the body its surroundings.
 */  
class PULSE_DECL Environment : public PulseEnvironmentSystem
{
  friend class PBPulseEnvironment;//friend the serialization class
  friend class PulseEngineTest;
public:
  Environment(PulseData& data);
  virtual ~Environment();

  void Clear();

  // Set members to a stable homeostatic state
  void Initialize();
  // Set pointers and other member varialbes common to both homeostatic initialization and loading a state
  void SetUp();

  void AtSteadyState();
  void PreProcess();
  void Process(bool solve_and_transport = true);
  void PostProcess(bool solve_and_transport = true);

  void StateChange();

protected:
  void ComputeExposedModelParameters() override;

  void ProcessActions();
  void CalculateSupplementalValues();
  /**/double AntoineEquation(double dTemperature_C);
  void CalculateRadiation();
  void CalculateConvection();
  void CalculateEvaporation();
  void CalculateRespiration();

  // Serializable member variables (Set in Initialize and in schema)

  // Stateless member variable (Calculated in Supplemental Method and used in other methods)
  double m_dLewisRelation;
  double m_dHeatOfVaporizationOfWater_J_Per_kg;
  double m_dWaterVaporPressureInAmbientAir_Pa;
  double m_dWaterVaporPressureAtSkin_Pa;
  //Prandtl number calculation
  double m_WaterSpecificHeat_J_Per_kg_K;
  double m_WaterViscosity_N_s_Per_m2;
  double m_WaterThermalConductivity_W_Per_m_K;
  //Grashof number calculation
  double m_ThermalExpansion_Per_K;
  // For Convection calculations
  double m_PatientEquivalentDiameter_m;

  // Stateless member variable (Set in SetUp())
  // Compartments
  SEGasCompartment*              m_AmbientGases;
  SELiquidCompartment*           m_AmbientAerosols;
  //Circuits                     
  SEThermalCircuit*              m_EnvironmentCircuit;
  //Nodes                        
  SEThermalCircuitNode*          m_ThermalEnvironment;
  SEThermalCircuitNode*          m_SkinNode;
  SEThermalCircuitNode*          m_ClothingNode;
  SEThermalCircuitNode*          m_EnclosureNode;
  //Paths                        
  SEThermalCircuitPath*          m_SkinToClothing;
  SEThermalCircuitPath*          m_ActiveHeatTransferRatePath;
  SEThermalCircuitPath*          m_ActiveTemperaturePath;
  SEThermalCircuitPath*          m_ActiveSwitchPath;
  SEThermalCircuitPath*          m_ClothingToEnclosurePath;
  SEThermalCircuitPath*          m_GroundToEnclosurePath;
  SEThermalCircuitPath*          m_ClothingToEnvironmentPath;
  SEThermalCircuitPath*          m_GroundToEnvironmentPath;
  SEThermalCircuitPath*          m_EnvironmentSkinToGroundPath;
  SEThermalCircuitPath*          m_EnvironmentCoreToGroundPath;
};
