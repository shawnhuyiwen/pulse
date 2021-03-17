/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "PulsePhysiologySystems.h"
#include "system/physiology/SERespiratorySystem.h"
#include "substance/SESubstanceTransport.h"
class SEPatient;
class SEPatientActionCollection;
class SEGasCompartment;
class SEGasSubstanceQuantity;
class SELiquidCompartment;
class SELiquidSubstanceQuantity;
class SEFluidCircuit;
class SEFluidCircuitNode;
class SEFluidCircuitPath;
class SEFluidCircuitCalculator;
class SEConsciousRespirationCommand;
class SEPulmonaryFunctionTest;

/**
* @brief The %Respiratory System class handles the analysis and storage of
* data related the respiratory physiology and pathology.
* @details The goal of the Respiratory System class is to handle the analysis
* and storage of data representing the ventilation and gas exchange of the human
* respiratory system. The class handles the analysis of changes in the lungs volume
* and pressure data during the flow and transfer of gases between the lungs, the
* blood and the outside environment. The class updates the volume and volume fractions
* of gases in the lungs, and ensures the integration and flow of data between the
* respiratory system and the anesthesia machine during mechanical ventilation.
*/
class PULSE_DECL Respiratory : public PulseRespiratorySystem
{
  friend class PulseController;
  friend class PBPulsePhysiology;//friend the serialization class
  friend class PulseEngineTest;
public:
  Respiratory(PulseData& data);
  virtual ~Respiratory();

  virtual void Clear();

  // Set members to a stable homeostatic state
  virtual void Initialize();
  // Set pointers and other member varialbes common to both homeostatic initialization and loading a state
  virtual void SetUp();

  virtual void AtSteadyState();
  virtual void PreProcess();
  virtual void Process(bool solve_and_transport=true);
  virtual void PostProcess(bool solve_and_transport=true);
  bool CalculatePulmonaryFunctionTest(SEPulmonaryFunctionTest& pft) const;

protected:
  void ComputeExposedModelParameters() override;

  //Tuning
  void TuneCircuit();

  //PreProcess
  void CalculateWork();
  void CalculateFatigue();
  void UpdateChestWallCompliances();
  void UpdateVolumes();
  void UpdateResistances();
  void UpdateAlveolarCompliances();
  void UpdateInspiratoryExpiratoryRatio();
  void UpdateDiffusion();
  void UpdatePulmonaryCapillary();
  void UpdatePulmonaryShunt();
  //Overrides
  void SetRespiratoryResistance();
  void SetRespiratoryCompliance();

  //Actions
  void Pneumothorax();
  void DoLeftNeedleDecompression(double dFlowResistance);
  void DoRightNeedleDecompression(double dFlowResistance);
  void MechanicalVentilation();
  void SupplementalOxygen();
  // Driver
  void RespiratoryDriver();
  /**/void SetBreathCycleFractions();
  /**/void ConsciousRespiration();
  /**/double VolumeToDriverPressure(double TargetVolume);
  /**/void ModifyDriverPressure();
  // Aerosol Deposition and various Effects
  void ProcessAerosolSubstances();

  //Process
  void CalculateVitalSigns();
  void Debugging(SEFluidCircuit& RespiratoryCircuit);

  // Serializable member variables (Set in Initialize and in schema)

  //   CalculateVitalSigns()
  bool   m_BreathingCycle;
  bool   m_NotBreathing;
  double m_TopBreathTotalVolume_L;
  double m_TopBreathAlveoliVolume_L;
  double m_TopBreathPleuralVolume_L;
  double m_TopBreathAlveoliPressure_cmH2O;
  double m_TopBreathDriverPressure_cmH2O;
  double m_TopBreathPleuralPressure_cmH2O;
  double m_LastCardiacCycleBloodPH;
  double m_TopCarinaO2;
  double m_TopBreathElapsedTime_min;
  double m_BottomBreathElapsedTime_min;
  double m_BottomBreathTotalVolume_L;
  double m_BottomBreathAlveoliVolume_L;
  double m_BottomBreathPleuralVolume_L;
  double m_BottomBreathAlveoliPressure_cmH2O;
  double m_BottomBreathDriverPressure_cmH2O;
  double m_BottomBreathPleuralPressure_cmH2O;
  double m_PeakAlveolarPressure_cmH2O;
  double m_MaximalAlveolarPressure_cmH2O;
  SERunningAverage* m_BloodPHRunningAverage;
  SERunningAverage* m_MeanAirwayPressure_cmH2O;

  // Respiratory Driver
  double m_ArterialO2PartialPressure_mmHg;
  double m_ArterialCO2PartialPressure_mmHg;
  double m_BreathingCycleTime_s;
  double m_DriverPressure_cmH2O;
  double m_ElapsedBreathingCycleTime_min;
  double m_IERatioScaleFactor;
  double m_PeakInspiratoryPressure_cmH2O;
  double m_PeakExpiratoryPressure_cmH2O;
  double m_PreviousTargetAlveolarVentilation_L_Per_min;
  double m_VentilationFrequency_Per_min;
  double m_VentilationToTidalVolumeSlope;
  SERunningAverage* m_ArterialO2RunningAverage_mmHg;
  SERunningAverage* m_ArterialCO2RunningAverage_mmHg;

  // Muscle Pressure Waveform
  double m_ExpiratoryHoldFraction;
  double m_ExpiratoryReleaseFraction;
  double m_ExpiratoryRiseFraction;
  double m_InspiratoryHoldFraction;
  double m_InspiratoryReleaseFraction;
  double m_InspiratoryRiseFraction;
  double m_InspiratoryToExpiratoryPauseFraction;

  // Conscious Respiration
  bool m_ActiveConsciousRespirationCommand;

  // Disease States
  double m_leftAlveoliDecrease_L;
  double m_rightAlveoliDecrease_L;

  // Overrides
  double m_RespiratoryResistanceOverride_cmH2O_s_Per_L;
  double m_RespiratoryComplianceOverride_L_Per_cmH2O;

  // Stateless member variable (Set in SetUp())
  // Respiratory Driver
  double m_MaxDriverPressure_cmH2O;
  // Configuration parameters
  double m_CentralControlGainConstant;
  double m_DefaultOpenResistance_cmH2O_s_Per_L;
  double m_DefaultClosedResistance_cmH2O_s_Per_L;
  double m_PeripheralControlGainConstant;
  double m_RespOpenResistance_cmH2O_s_Per_L;
  double m_RespClosedResistance_cmH2O_s_Per_L;
  double m_VentilationTidalVolumeIntercept;
  double m_VentilatoryOcclusionPressure_cmH2O;
  double m_MinimumAllowableTidalVolume_L;
  double m_MinimumAllowableInpiratoryAndExpiratoryPeriod_s;
  // State between functions (i.e. shared between methods in preprocess, set to a default value at the start of preprocess)
  double m_AverageLocalTissueBronchodilationEffects;

  // Patient
  SEPatientActionCollection* m_PatientActions;
  //Compartments
  SEGasCompartment* m_Environment;
  SELiquidCompartment* m_AerosolAirway;
  SELiquidCompartment* m_AerosolCarina;
  SELiquidCompartment* m_AerosolLeftAnatomicDeadSpace;
  SELiquidCompartment* m_AerosolLeftAlveolarDeadSpace;
  SELiquidCompartment* m_AerosolLeftAlveoli;
  SELiquidCompartment* m_AerosolRightAnatomicDeadSpace;
  SELiquidCompartment* m_AerosolRightAlveolarDeadSpace;
  SELiquidCompartment* m_AerosolRightAlveoli;
  SELiquidCompartment* m_LeftLungExtravascular;
  SELiquidCompartment* m_RightLungExtravascular;
  SEGasCompartment* m_Lungs;
  SEGasCompartment* m_LeftLung;
  SEGasCompartment* m_RightLung;
  SEGasCompartment* m_Carina;
  SEGasSubstanceQuantity* m_CarinaO2;
  SELiquidSubstanceQuantity* m_AortaO2;
  SELiquidSubstanceQuantity* m_AortaCO2;
  SEGasSubstanceQuantity* m_LeftAlveoliO2;
  SEGasSubstanceQuantity* m_RightAlveoliO2;
  std::vector<SELiquidCompartment*> m_AerosolEffects;
  SEGasCompartment* m_MechanicalVentilationConnection;
  SELiquidCompartment* m_MechanicalVentilationAerosolConnection;
  SEGasCompartment* m_PleuralCavity;
  //Circuits
  SEFluidCircuit* m_RespiratoryCircuit;
  //Nodes
  SEFluidCircuitNode* m_Airway;
  SEFluidCircuitNode* m_LeftAlveoli;
  SEFluidCircuitNode* m_LeftAnatomicDeadSpace;
  SEFluidCircuitNode* m_LeftAlveolarDeadSpace;
  SEFluidCircuitNode* m_LeftPleural;
  SEFluidCircuitNode* m_RespiratoryMuscle;
  SEFluidCircuitNode* m_RightAlveoli;
  SEFluidCircuitNode* m_RightAnatomicDeadSpace;
  SEFluidCircuitNode* m_RightAlveolarDeadSpace;
  SEFluidCircuitNode* m_RightPleural;
  SEFluidCircuitNode* m_Ambient;
  SEFluidCircuitNode* m_Stomach;
  //Paths
  SEFluidCircuitPath* m_CarinaToLeftAnatomicDeadSpace;
  SEFluidCircuitPath* m_CarinaToRightAnatomicDeadSpace;
  SEFluidCircuitPath* m_LeftAnatomicDeadSpaceToLeftAlveolarDeadSpace;
  SEFluidCircuitPath* m_RightAnatomicDeadSpaceToRightAlveolarDeadSpace;
  SEFluidCircuitPath* m_LeftAlveolarDeadSpaceToLeftAlveoli;
  SEFluidCircuitPath* m_RightAlveolarDeadSpaceToRightAlveoli;
  SEFluidCircuitPath* m_LeftPleuralToRespiratoryMuscle;
  SEFluidCircuitPath* m_RightPleuralToRespiratoryMuscle;
  SEFluidCircuitPath* m_DriverPressurePath;
  SEFluidCircuitPath* m_AirwayToCarina;
  SEFluidCircuitPath* m_AirwayToStomach;
  SEFluidCircuitPath* m_EnvironmentToLeftChestLeak;
  SEFluidCircuitPath* m_EnvironmentToRightChestLeak;
  SEFluidCircuitPath* m_LeftAlveoliLeakToLeftPleural;
  SEFluidCircuitPath* m_RightAlveoliLeakToRightPleural;
  SEFluidCircuitPath* m_LeftPleuralToEnvironment;
  SEFluidCircuitPath* m_RightPleuralToEnvironment;
  SEFluidCircuitPath* m_LeftAlveoliToLeftPleuralConnection;
  SEFluidCircuitPath* m_RightAlveoliToRightPleuralConnection;
  SEFluidCircuitPath* m_LeftPulmonaryCapillary;
  SEFluidCircuitPath* m_RightPulmonaryCapillary;
  SEFluidCircuitPath* m_LeftPulmonaryArteriesToVeins;
  SEFluidCircuitPath* m_RightPulmonaryArteriesToVeins;
  SEFluidCircuitPath* m_ConnectionToAirway;
  SEFluidCircuitPath* m_GroundToConnection;

  SEFluidCircuitCalculator* m_Calculator;
  SEGasTransporter* m_GasTransporter;
  SELiquidTransporter* m_AerosolTransporter;
};
