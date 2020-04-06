/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "PulsePhysiologySystems.h"
#include "system/physiology/SEBloodChemistrySystem.h"
class SECompleteBloodCount;
class SEComprehensiveMetabolicPanel;
class SELiquidCompartment;
class SELiquidSubstanceQuantity;

/**
 * @brief @copydoc Physiology_BloodChemistrySystemData
 * The Blood Chemistry System holds the system-level blood substance data that is computed on the compartment level by other systems.
 * @details
 * The goal of the blood chemistry system in the Pulse engine is complete analysis on the blood at any given point during the simulation.
 * The blood chemistry system houses all of the blood concentrations and compositions needed to assess a patient's health. This system is under development
 * and will be improved in future releases to include more substances that can provide clinician level details and assessments, such as a CBC and blood panel.
 */           
class PULSE_DECL BloodChemistry : public PulseBloodChemistrySystem
{
  friend class PulseData;
  friend class PBPulsePhysiology;//friend the serialization class
  friend class PulseEngineTest;
protected:
  BloodChemistry(PulseData& data);
  PulseData& m_data;

public:
  virtual ~BloodChemistry();

  void Clear();

  // Set members to a stable homeostatic state
  void Initialize();
  // Set pointers and other member varialbes common to both homeostatic initialization and loading a state
  void SetUp();

  void AtSteadyState();
  void PreProcess();
  void Process(bool solve_and_transport=true);
  void PostProcess();

  bool CalculateCompleteBloodCount(SECompleteBloodCount& cbc) const;
  bool CalculateComprehensiveMetabolicPanel(SEComprehensiveMetabolicPanel& cmp) const;

protected:
  void ComputeExposedModelParameters() override;

  void CheckBloodGasLevels();
  // Serializable member variables (Set in Initialize and in schema)
  SERunningAverage* m_ArterialOxygen_mmHg;
  SERunningAverage* m_ArterialCarbonDioxide_mmHg;

  // Stateless member variable (Set in SetUp())
  double m_redBloodCellVolume_mL;
  double m_HbPerRedBloodCell_ug_Per_ct;
  SELiquidCompartment*       m_aorta;
  SELiquidSubstanceQuantity* m_aortaO2;
  SELiquidSubstanceQuantity* m_aortaCO2;
  SELiquidSubstanceQuantity* m_aortaCO;
  SELiquidSubstanceQuantity* m_aortaBicarbonate;
  SELiquidSubstanceQuantity* m_brainO2;
  SELiquidSubstanceQuantity* m_myocardiumO2;
  SELiquidSubstanceQuantity* m_pulmonaryArteriesO2;
  SELiquidSubstanceQuantity* m_pulmonaryArteriesCO2;
  SELiquidSubstanceQuantity* m_pulmonaryVeinsO2;
  SELiquidSubstanceQuantity* m_pulmonaryVeinsCO2;
  SELiquidCompartment*       m_venaCava;
  SELiquidSubstanceQuantity* m_venaCavaO2;
  SELiquidSubstanceQuantity* m_venaCavaCO2;
  SELiquidSubstanceQuantity* m_venaCavaAcetoacetate;
  SELiquidSubstanceQuantity* m_venaCavaAlbumin;
  SELiquidSubstanceQuantity* m_venaCavaBicarbonate;
  SELiquidSubstanceQuantity* m_venaCavaCalcium;
  SELiquidSubstanceQuantity* m_venaCavaChloride;
  SELiquidSubstanceQuantity* m_venaCavaCreatinine;
  SELiquidSubstanceQuantity* m_venaCavaEpinephrine;
  SELiquidSubstanceQuantity* m_venaCavaGlucose;
  SELiquidSubstanceQuantity* m_venaCavaInsulin;
  SELiquidSubstanceQuantity* m_venaCavaLactate;
  SELiquidSubstanceQuantity* m_venaCavaPotassium;
  SELiquidSubstanceQuantity* m_venaCavaSodium;
  SELiquidSubstanceQuantity* m_venaCavaTristearin;
  SELiquidSubstanceQuantity* m_venaCavaUrea;  
};
