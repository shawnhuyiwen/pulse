/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "engine/common/system/Model.h"
#include "engine/common/system/Systems.h"

class SELiquidCompartment;
class SELiquidSubstanceQuantity;
class SEFluidCircuitPath;

namespace pulse
{
  class PBPhysiology;
  class EngineTest;
  /**
  * @brief @copydoc Physiology_GastrointestinalSystemData
  */
  class PULSE_DECL GastrointestinalModel : public GastrointestinalSystem, public Model
  {
    friend PBPhysiology;//friend the serialization class
    friend EngineTest;
  public:
    GastrointestinalModel(Data& data);
    virtual ~GastrointestinalModel();

    void Clear() override;

    // Set members to a stable homeostatic state
    void Initialize() override;
    // Set pointers and other member varialbes common to both homeostatic initialization and loading a state
    void SetUp() override;

    void AtSteadyState() override;
    void PreProcess() override;
    void Process(bool solve_and_transport = true) override;
    void PostProcess(bool solve_and_transport = true) override;

  protected:
    void ComputeExposedModelParameters() override;

    void   DefaultNutritionRates(SENutrition& n);
    void   DigestStomachNutrients(double duration_s);
    double DigestNutrient(SEUnitScalar& totalAmt, SEUnitScalar& rate, bool mass, double duration_s);

    // Serializable member variables (Set in Initialize and in schema)  

    // Stateless member variable (Set in SetUp())
    SEFluidCircuitPath* m_GItoCVPath;
    SEScalarMassPerTime* m_CalciumDigestionRate;
    SEScalarVolumePerTime* m_WaterDigestionRate;

    SELiquidCompartment*       m_SmallIntestine;
    SELiquidSubstanceQuantity* m_SmallIntestineGlucose;
    SELiquidSubstanceQuantity* m_SmallIntestineTristearin;
    SELiquidSubstanceQuantity* m_SmallIntestineCalcium;
    SELiquidSubstanceQuantity* m_SmallIntestineSodium;
    SELiquidSubstanceQuantity* m_SmallIntestineUrea;
    SELiquidCompartment*       m_SmallIntestineChyme;

    bool   m_ConsumeRate; // Some substance digestion rates are specified by the user, some by configuration,
    //if the stomach runs out of a substance with a user provided rate, we invalidate the rate (true)
    //if the rate is a configuration rate, we don't want to invalidate it if we run out of the substance (false)
  };
END_NAMESPACE
