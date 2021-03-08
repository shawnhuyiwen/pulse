/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "physiology/Gastrointestinal.h"
#include "controller/Circuits.h"
#include "controller/Compartments.h"
#include "controller/Substances.h"
#include "PulseConfiguration.h"
   // Conditions
#include "engine/SEConditionManager.h"
#include "patient/conditions/SEConsumeMeal.h"
// Actions
#include "engine/SEActionManager.h"
#include "engine/SEPatientActionCollection.h"
#include "patient/actions/SEConsumeNutrients.h"
// Dependent Systems
#include "system/physiology/SEBloodChemistrySystem.h"
#include "system/physiology/SECardiovascularSystem.h"
#include "system/physiology/SETissueSystem.h"
// CDM
#include "patient/SEPatient.h"
#include "patient/SEMeal.h"
#include "patient/SENutrition.h"
#include "substance/SESubstance.h"
#include "circuit/fluid/SEFluidCircuit.h"
#include "compartment/fluid/SELiquidCompartment.h"
#include "compartment/substances/SELiquidSubstanceQuantity.h"
#include "properties/SEScalar.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarAmountPerVolume.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarMassPerTime.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarMassPerAmount.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalarTime.h"
#include "utils/DataTrack.h"

#pragma warning(disable:4786)
#pragma warning(disable:4275)

//#define logMeal

Gastrointestinal::Gastrointestinal(PulseData& data) : PulseGastrointestinalSystem(data)
{
  Clear();
  m_CalciumDigestionRate = new SEScalarMassPerTime();
  m_WaterDigestionRate = new SEScalarVolumePerTime();
  /* Move to a unit test
  SENutrition one(m_Logger);
  SENutrition two(m_Logger);
  one.GetCarbohydrate()->SetValue(1, MassUnit::g);
  one.GetCarbohydrateDigestionRate()->SetValue(1, MassPerTimeUnit::g_Per_s);
  one.GetFat()->SetValue(2, MassUnit::g);
  one.GetFatDigestionRate()->SetValue(2, MassPerTimeUnit::g_Per_s);
  one.GetProtein()->SetValue(3, MassUnit::g);
  one.GetProteinDigestionRate()->SetValue(3, MassPerTimeUnit::g_Per_s);

  two.GetCarbohydrate()->SetValue(1, MassUnit::g);
  two.GetCarbohydrateDigestionRate()->SetValue(2, MassPerTimeUnit::g_Per_s);
  two.GetFat()->SetValue(2, MassUnit::g);
  two.GetFatDigestionRate()->SetValue(4, MassPerTimeUnit::g_Per_s);
  two.GetProtein()->SetValue(3, MassUnit::g);
  two.GetProteinDigestionRate()->SetValue(6, MassPerTimeUnit::g_Per_s);
  one.Increment(two);
  std::cout << "Carbo Rate: " << one.GetCarbohydrateDigestionRate() << std::endl;
  std::cout << "Fat Rate: " << one.GetFatDigestionRate() << std::endl;
  std::cout << "Protein Rate: " << one.GetProteinDigestionRate() << std::endl;
  */
}

Gastrointestinal::~Gastrointestinal()
{
  Clear();
  delete m_CalciumDigestionRate;
  delete m_WaterDigestionRate;
}

void Gastrointestinal::Clear()
{
  SEGastrointestinalSystem::Clear();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes system properties to valid homeostatic values.
///
/// \details
/// For stabilization only!
/// Called AFTER Setup when stabilizing a new patient
//--------------------------------------------------------------------------------------------------
void Gastrointestinal::Initialize()
{
  PulseSystem::Initialize();

  if (m_data.GetConfiguration().HasDefaultStomachContents()) // Fill Stomach with default meal
    GetStomachContents().Copy(*m_data.GetConfiguration().GetDefaultStomachContents());
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes parameters for Gastrointestinal Class
///
/// \details
/// Called during both State loading and Patient Stabilization
/// Pull and setup up our data (can be from other systems)
/// Initialize will be called after this and can overwrite any of this data (only if stabilizing)
//--------------------------------------------------------------------------------------------------
void Gastrointestinal::SetUp()
{
  m_ConsumeRate = false;
  m_WaterDigestionRate->SetValue(m_data.GetConfiguration().GetWaterDigestionRate(VolumePerTimeUnit::mL_Per_s), VolumePerTimeUnit::mL_Per_s);
  m_CalciumDigestionRate->SetValue(m_data.GetConfiguration().GetCalciumDigestionRate(MassPerTimeUnit::g_Per_s), MassPerTimeUnit::g_Per_s);

  m_GItoCVPath = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(pulse::ChymePath::SmallIntestineC1ToSmallIntestine1);

  m_SmallIntestineChyme = m_data.GetCompartments().GetLiquidCompartment(pulse::ChymeCompartment::SmallIntestine);
  m_SmallIntestine = m_data.GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::SmallIntestine);
  m_SmallIntestineGlucose = m_SmallIntestine->GetSubstanceQuantity(m_data.GetSubstances().GetGlucose());
  m_SmallIntestineTristearin = m_SmallIntestine->GetSubstanceQuantity(m_data.GetSubstances().GetTristearin());
  m_SmallIntestineCalcium = m_SmallIntestine->GetSubstanceQuantity(m_data.GetSubstances().GetCalcium());
  m_SmallIntestineSodium = m_SmallIntestine->GetSubstanceQuantity(m_data.GetSubstances().GetSodium());
  m_SmallIntestineUrea = m_SmallIntestine->GetSubstanceQuantity(m_data.GetSubstances().GetUrea());
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Do anything needed between stabilization
///
/// \details
/// At the end of resting stabilization we apply a ConsumeMeal condition to modify the body state
/// if no condition is present we reset the body state substance levels to their original level
//--------------------------------------------------------------------------------------------------

void Gastrointestinal::AtSteadyState()
{

}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Gastrointestinal Preprocess function
///
/// \details
/// Apply any meal consumtion actions
/// Absorb fluids from the tissues back into the stomach so it always has fluid in it
/// Digest substances based on our time step and transfer them to the Gut
/// Absorb substances from the gut into the vascular system via the small intestine 
//--------------------------------------------------------------------------------------------------

void Gastrointestinal::PreProcess()
{
  double absorptionRate_mL_Per_min;
  if (m_data.GetState() == EngineState::Active)
  {
    if (m_data.GetActions().GetPatientActions().HasConsumeNutrients())
    {
      // Use Default Rates if none provided
      SEConsumeNutrients& c = m_data.GetActions().GetPatientActions().GetConsumeNutrients();
      if (c.HasNutritionFile())
      {// Grab file, then load it (note GetNutrition will remove the file name, so get it first)
        std::string file = c.GetNutritionFile();
        if (!c.GetNutrition().SerializeFromFile(file))
        {
          /// \error Unable to read consume meal action file
          Error("Could not read provided nutrition file", "Gastrointestinal::PreProcess");
        }
      }
      DefaultNutritionRates(c.GetNutrition());
      m_StomachContents->Increment(c.GetNutrition());
      m_data.GetCurrentPatient().GetWeight().IncrementValue(c.GetNutrition().GetWeight(MassUnit::kg), MassUnit::kg);
      m_data.GetActions().GetPatientActions().RemoveConsumeNutrients();
    }
    DigestStomachNutrients(m_data.GetTimeStep_s());
    absorptionRate_mL_Per_min = 3.3;
  }
  else
  {
    // Try to keep all concentration values level
    double smiGlucose_mg = m_SmallIntestineGlucose->GetMass(MassUnit::mg);
    m_SmallIntestineGlucose->GetMass().SetValue(smiGlucose_mg + 0.12, MassUnit::mg);
    m_SmallIntestineGlucose->Balance(BalanceLiquidBy::Mass);
    double smiTristearin_mg = m_SmallIntestineTristearin->GetMass(MassUnit::mg);
    m_SmallIntestineTristearin->GetMass().SetValue(smiTristearin_mg + 0.001, MassUnit::mg);
    m_SmallIntestineTristearin->Balance(BalanceLiquidBy::Mass);
    double smiCalcium_mg = m_SmallIntestineCalcium->GetMass(MassUnit::mg);
    m_SmallIntestineCalcium->GetMass().SetValue(smiCalcium_mg + 0, MassUnit::mg);
    m_SmallIntestineCalcium->Balance(BalanceLiquidBy::Mass);
    double smiSodium_g = m_SmallIntestineSodium->GetMass(MassUnit::g);
    m_SmallIntestineSodium->GetMass().SetValue(smiSodium_g + 0, MassUnit::g);
    m_SmallIntestineSodium->Balance(BalanceLiquidBy::Mass);
    double smiUrea_mg = m_SmallIntestineUrea->GetMass(MassUnit::mg);
    m_SmallIntestineUrea->GetMass().SetValue(smiUrea_mg + 0.007, MassUnit::mg);
    m_SmallIntestineUrea->Balance(BalanceLiquidBy::Mass);
    absorptionRate_mL_Per_min = 1.1;
  }
  double absorbedVolume_mL = absorptionRate_mL_Per_min * m_data.GetTimeStep_s() / 60;
  double siVolume_mL = m_SmallIntestineChyme->GetVolume().GetValue(VolumeUnit::mL);
  if (siVolume_mL <= 0)
  {
    absorptionRate_mL_Per_min = 0;// No volume, no flow
  }
  else if (absorbedVolume_mL > siVolume_mL)
  {//Only take what we have
    absorbedVolume_mL = siVolume_mL;
    absorptionRate_mL_Per_min = 60 * (absorbedVolume_mL / m_data.GetTimeStep_s());
  }
  m_GItoCVPath->GetNextFlowSource().SetValue(absorptionRate_mL_Per_min, VolumePerTimeUnit::mL_Per_min);
  //Need to manually remove volume, since there isn't a compliance
  if (m_data.GetState() == EngineState::Active) // Only if we are active
    m_GItoCVPath->GetSourceNode().GetNextVolume().IncrementValue(-absorptionRate_mL_Per_min / 60.0 * m_data.GetTimeStep_s(), VolumeUnit::mL);
  GetWaterAbsorptionRate().SetValue(absorptionRate_mL_Per_min, VolumePerTimeUnit::mL_Per_min);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Digest substances based on our time step and transfer them to the Gut
///
/// \details
/// Each substance can provide a digestion rate, if none is provided a default will be used
/// A portion of each substance is removed from the stomach based on time step and digestion rate
/// The substance is then incremented into the Gut. Each substance can have it's own movement logic
/// Water is also transfered to the Gut based on a constant rate
//--------------------------------------------------------------------------------------------------
void Gastrointestinal::DigestStomachNutrients(double duration_s)
{
  if (duration_s <= 0)
    return; // Nothing to do then

  double digestedAmount;// either in g or mL

  if (m_StomachContents->GetWater().IsPositive())
  {
    if (m_StomachContents->GetSodium().IsPositive())
    {// Sodium rate is a function of the concentration of sodium in the stomach
      double totalNa_g = m_StomachContents->GetSodium(MassUnit::g);
      double digestedNa_g = (totalNa_g / m_StomachContents->GetWater(VolumeUnit::mL))
        * m_WaterDigestionRate->GetValue(VolumePerTimeUnit::mL_Per_s) * duration_s;
      if (totalNa_g <= digestedNa_g)
      {
        digestedNa_g = totalNa_g;
        m_StomachContents->GetSodium().Invalidate();
        Info("Stomach is out of Sodium");
      }
      else
      {
        m_StomachContents->GetSodium().IncrementValue(-digestedNa_g, MassUnit::g);
      }
#ifdef logDigest
      m_ss << "Digested " << digestedNa_g << "(g) of Sodium";
      Info(m_ss);
#endif
      m_SmallIntestineSodium->GetMass().IncrementValue(digestedNa_g, MassUnit::g);
      m_SmallIntestineSodium->Balance(BalanceLiquidBy::Mass);
    }

    digestedAmount = DigestNutrient(m_StomachContents->GetWater(), *m_WaterDigestionRate, false, duration_s);
    if (digestedAmount > 0)
    {
#ifdef logDigest
      m_ss << "Digested " << digestedAmount << "(mL) of Water";
      Info(m_ss);
#endif
      m_SmallIntestineChyme->GetVolume().IncrementValue(digestedAmount, VolumeUnit::mL);
    }
    else
      Info("Stomach is out of Water");
  }

  m_ConsumeRate = true;// We are digesting nutrient rates provided by our consume action, if we run out of a substance, we invalidate this rate
  if (m_StomachContents->HasCarbohydrate())
  {
    digestedAmount = DigestNutrient(m_StomachContents->GetCarbohydrate(), m_StomachContents->GetCarbohydrateDigestionRate(), true, duration_s);
    digestedAmount *= m_data.GetConfiguration().GetCarbohydrateAbsorptionFraction(); // Take off percent that usually passes through the body
    if (digestedAmount > 0)
    {
#ifdef logDigest
      m_ss << "Digested " << digestedAmount << "(g) of Carbs";
      Info(m_ss);
#endif
      m_SmallIntestineGlucose->GetMass().IncrementValue(digestedAmount, MassUnit::g);
      m_SmallIntestineGlucose->Balance(BalanceLiquidBy::Mass);
    }
    else
      Info("Stomach is out of Carbs");
  }
  if (m_StomachContents->HasFat())
  {
    digestedAmount = DigestNutrient(m_StomachContents->GetFat(), m_StomachContents->GetFatDigestionRate(), true, duration_s);
    digestedAmount *= m_data.GetConfiguration().GetFatAbsorptionFraction(); // Take off percent that usually passes through the body
    if (digestedAmount > 0)
    {
#ifdef logDigest
      m_ss << "Digested " << digestedAmount << "(g) of Fat";
      Info(m_ss);
#endif
      m_SmallIntestineTristearin->GetMass().IncrementValue(digestedAmount, MassUnit::g);
      m_SmallIntestineTristearin->Balance(BalanceLiquidBy::Mass);
    }
    else
      Info("Stomach is out of Fat");
  }
  if (m_StomachContents->HasProtein())
  {
    digestedAmount = DigestNutrient(m_StomachContents->GetProtein(), m_StomachContents->GetProteinDigestionRate(), true, duration_s);
    digestedAmount *= m_data.GetConfiguration().GetProteinToUreaFraction(); // How much gets turned into Urea
    if (digestedAmount > 0)
    {
#ifdef logDigest
      m_ss << "Digested " << digestedAmount << "(g) of Urea";
      Info(m_ss);
#endif
      double tuningFactor = 0.75; /// \todo Remove tuning factor and adjust protein to urea fraction following investigation
      m_SmallIntestineUrea->GetMass().IncrementValue(digestedAmount * tuningFactor, MassUnit::g);
      m_SmallIntestineUrea->Balance(BalanceLiquidBy::Mass);
    }
    else
      Info("Stomach is out of Protien");
  }
  m_ConsumeRate = false; // Other rates are specified by configuration and should never be invalidated

  if (m_StomachContents->HasCalcium())
  {
    digestedAmount = DigestNutrient(m_StomachContents->GetCalcium(), *m_CalciumDigestionRate, true, duration_s);
    digestedAmount *= m_data.GetConfiguration().GetCalciumAbsorptionFraction(); // Take off percent that usually passes through the body
    if (digestedAmount > 0)
    {
#ifdef logDigest
      m_ss << "Digested " << digestedAmount << "(g) of Calcium";
      Info(m_ss);
#endif
      m_SmallIntestineCalcium->GetMass().IncrementValue(digestedAmount, MassUnit::g);
      m_SmallIntestineCalcium->Balance(BalanceLiquidBy::Mass);
    }
    else
      Info("Stomach is out of Calcium");
  }
}

// --------------------------------------------------------------------------------------------------
/// \brief
/// Generic code for removal of an amout of a substance from the stomach based on rate and duration
///
/// \details
//--------------------------------------------------------------------------------------------------
double Gastrointestinal::DigestNutrient(SEUnitScalar& totalAmt, SEUnitScalar& rate, bool mass, double duration_s)
{
  double digestedAmt = 0;
  if (totalAmt.IsValid())
  {
    double t = totalAmt.GetValue(mass ? (const CCompoundUnit&)MassUnit::g : (const CCompoundUnit&)VolumeUnit::mL);
    digestedAmt = rate.GetValue(mass ? (const CCompoundUnit&)MassPerTimeUnit::g_Per_s : (const CCompoundUnit&)VolumePerTimeUnit::mL_Per_s) * duration_s;
    if (t <= digestedAmt)
    {
      digestedAmt = t;
      totalAmt.Invalidate();
      if (m_ConsumeRate)// We keep this rate, it's a system parameter not a per nutrition rate as the masses are
        rate.Invalidate();
    }
    else
    {
      totalAmt.IncrementValue(-digestedAmt, mass ? (const CCompoundUnit&)MassUnit::g : (const CCompoundUnit&)VolumeUnit::mL);
    }
  }
  return digestedAmt;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// If no substance rates are provided on meals (Action or Condition) use a default from configuration
///
/// \details
//--------------------------------------------------------------------------------------------------
void Gastrointestinal::DefaultNutritionRates(SENutrition& n)
{
  if (n.HasCarbohydrate() && !n.HasCarbohydrateDigestionRate())
    n.GetCarbohydrateDigestionRate().SetValue(m_data.GetConfiguration().GetDefaultCarbohydrateDigestionRate(MassPerTimeUnit::g_Per_min), MassPerTimeUnit::g_Per_min);
  if (n.HasFat() && !n.HasFatDigestionRate())
    n.GetFatDigestionRate().SetValue(m_data.GetConfiguration().GetDefaultFatDigestionRate(MassPerTimeUnit::g_Per_min), MassPerTimeUnit::g_Per_min);
  if (n.HasProtein() && !n.HasProteinDigestionRate())
    n.GetProteinDigestionRate().SetValue(m_data.GetConfiguration().GetDefaultProteinDigestionRate(MassPerTimeUnit::g_Per_min), MassPerTimeUnit::g_Per_min);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Gastrointestinal Preprocess function
///
/// \details
/// The current Pulse implementation has no functionality in the process function for Gastrointestinal.
//--------------------------------------------------------------------------------------------------
void Gastrointestinal::Process(bool solve_and_transport)
{
  ComputeExposedModelParameters();
}
void Gastrointestinal::ComputeExposedModelParameters()
{

}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Gastrointestinal postprocess function
///
/// \details
/// The current Pulse implementation has no specific postprocess functionality.
//--------------------------------------------------------------------------------------------------
void Gastrointestinal::PostProcess(bool solve_and_transport)
{

}