/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "PulsePhysiologySystems.h"
#include "system/equipment/anesthesia_machine/SEAnesthesiaMachine.h"
class SEEquipmentActionCollection;
class SEGasCompartment;
class SEGasSubstanceQuantity;
class SEFluidCircuitNode;
class SEFluidCircuitPath;

/**
 * @brief 
 * Generic anesthesia machine for positive pressure ventilation.
 */    
class PULSE_DECL AnesthesiaMachine : public PulseAnesthesiaMachine
{
  friend class PulseController;
  friend class PBPulseEquipment;//friend the serialization class
  friend class PulseEngineTest;
public:
  AnesthesiaMachine(PulseData& pc);
  virtual ~AnesthesiaMachine();

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

  void CalculateScrubber();

  // Extending some functionality to these base class methods
  // We will update the Pulse Airway mode when these are called
  virtual void SetConnection(eAnesthesiaMachine_Connection c);
  virtual void InvalidateConnection();

  void CalculateSourceStatus();
  void CalculateEquipmentLeak();
  void SetConnection();
  void CalculateValveResistances();
  void CalculateVentilatorPressure();
  void CalculateGasSourceSubstances();
  void CalculateGasSourceResistance();
  void CalculateCyclePhase();
  void CheckReliefValve();

  // Serializable member variables (Set in Initialize and in schema)
  bool         m_inhaling; 
  double       m_inspirationTime_s;
  double       m_O2InletVolumeFraction;
  double       m_currentbreathingCycleTime_s;
  double       m_totalBreathingCycleTime_s;

  // Stateless member variable (Set in SetUp())
  double m_dValveOpenResistance_cmH2O_s_Per_L;
  double m_dValveClosedResistance_cmH2O_s_Per_L;
  double m_dSwitchOpenResistance_cmH2O_s_Per_L;
  double m_dSwitchClosedResistance_cmH2O_s_Per_L;
  SEEquipmentActionCollection*         m_actions;
  SEGasCompartment*                    m_ambient;
  SEGasSubstanceQuantity*              m_ambientCO2;
  SEGasSubstanceQuantity*              m_ambientN2;
  SEGasSubstanceQuantity*              m_ambientO2;
  SEGasCompartment*                    m_gasSource;
  SEGasSubstanceQuantity*              m_gasSourceCO2;
  SEGasSubstanceQuantity*              m_gasSourceN2;
  SEGasSubstanceQuantity*              m_gasSourceO2;
  SEGasCompartment*                    m_scrubber;
  SEGasSubstanceQuantity*              m_scubberCO2;
  SEGasSubstanceQuantity*              m_scrubberN2;
  SEFluidCircuitNode*                  m_nVentilator;
  SEFluidCircuitPath*                  m_pAnesthesiaConnectionToEnvironment;
  SEFluidCircuitPath*                  m_pYPieceToExpiratoryLimb;
  SEFluidCircuitPath*                  m_pGasSourceToGasInlet;
  SEFluidCircuitPath*                  m_pInspiratoryLimbToYPiece;
  SEFluidCircuitPath*                  m_pSelectorToReliefValve;
  SEFluidCircuitPath*                  m_pEnvironmentToReliefValve;
  SEFluidCircuitPath*                  m_pEnvironmentToVentilator;
  SEFluidCircuitPath*                  m_pExpiratoryLimbToSelector;
  SEFluidCircuitPath*                  m_pSelectorToScrubber;
  SEFluidCircuitPath*                  m_pEnvironmentToGasSource;
  SEFluidCircuitPath*                  m_pVentilatorToSelector;
};
