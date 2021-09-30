/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "engine/common/system/Model.h"
#include "engine/common/system/Systems.h"

class SEArterialBloodGasTest;
class SECompleteBloodCount;
class SEComprehensiveMetabolicPanel;
class SELiquidCompartment;
class SELiquidSubstanceQuantity;

namespace pulse
{
  class PBPhysiology;
  class EngineTest;
  /**
   * @brief @copydoc Physiology_BloodChemistrySystemData
   * The Blood Chemistry System holds the system-level blood substance data that is computed on the compartment level by other systems.
   * @details
   * The goal of the blood chemistry system in the Pulse engine is complete analysis on the blood at any given point during the simulation.
   * The blood chemistry system houses all of the blood concentrations and compositions needed to assess a patient's health. This system is under development
   * and will be improved in future releases to include more substances that can provide clinician level details and assessments, such as a CBC and blood panel.
   */
  class PULSE_DECL BloodChemistryModel : public BloodChemistrySystem, public Model
  {
    friend PBPhysiology;//friend the serialization class
    friend EngineTest;
  public:
    BloodChemistryModel(Data& data);
    virtual ~BloodChemistryModel();

    void Clear();

    // Set members to a stable homeostatic state
    void Initialize();
    // Set pointers and other member varialbes common to both homeostatic initialization and loading a state
    void SetUp();

    void AtSteadyState();
    void PreProcess();
    void Process(bool solve_and_transport = true);
    void PostProcess(bool solve_and_transport = true);

    bool CalculateArterialBloodGasTest(SEArterialBloodGasTest& abg) const;
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
    SELiquidCompartment* m_aorta;
    SELiquidSubstanceQuantity* m_aortaO2;
    SELiquidSubstanceQuantity* m_aortaCO2;
    SELiquidSubstanceQuantity* m_aortaCO;
    SELiquidSubstanceQuantity* m_brainO2;
    SELiquidSubstanceQuantity* m_myocardiumO2;
    SELiquidSubstanceQuantity* m_pulmonaryArteriesO2;
    SELiquidSubstanceQuantity* m_pulmonaryArteriesCO2;
    SELiquidSubstanceQuantity* m_pulmonaryVeinsO2;
    SELiquidSubstanceQuantity* m_pulmonaryVeinsCO2;
    SELiquidCompartment* m_venaCava;
    SELiquidSubstanceQuantity* m_venaCavaO2;
    SELiquidSubstanceQuantity* m_venaCavaCO2;
  };
END_NAMESPACE
