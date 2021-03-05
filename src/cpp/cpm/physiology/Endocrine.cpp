/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "physiology/Endocrine.h"
#include "controller/Compartments.h"
#include "controller/Substances.h"
// Dependent Systems
#include "system/physiology/SEEnergySystem.h"
// Actions
#include "engine/SEActionManager.h"
#include "engine/SEPatientActionCollection.h"
#include "patient/actions/SEAcuteStress.h"
// CDM
#include "patient/SEPatient.h"
#include "substance/SESubstance.h"
#include "compartment/fluid/SELiquidCompartment.h"
#include "properties/SEScalarAmountPerTime.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarMassPerAmount.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalar0To1.h"
#include "utils/GeneralMath.h"

Endocrine::Endocrine(PulseData& data) : PulseEndocrineSystem(data)
{
  Clear();
}

Endocrine::~Endocrine()
{
  Clear();
}

void Endocrine::Clear()
{
  SEEndocrineSystem::Clear();
  m_aortaGlucose = nullptr;
  m_aortaEpinephrine = nullptr;
  m_splanchnicInsulin = nullptr;  
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes system properties to valid homeostatic values.
///
/// \details
/// For stabilization only!
/// Called AFTER Setup when stabilizing a new patient
//--------------------------------------------------------------------------------------------------
void Endocrine::Initialize()
{
  PulseSystem::Initialize();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes parameters for Endocrine Class
///
/// \details
/// Called during both State loading and Patient Stabilization
/// Pull and setup up our data (can be from other systems)
/// Initialize will be called after this and can overwrite any of this data (only if stabilizing)
//--------------------------------------------------------------------------------------------------
void Endocrine::SetUp()
{
  SELiquidCompartment* aorta = m_data.GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::Aorta);
  SELiquidCompartment* rkidney = m_data.GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::RightEfferentArteriole);
  SELiquidCompartment* lkidney = m_data.GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::LeftEfferentArteriole);
  m_aortaEpinephrine = aorta->GetSubstanceQuantity(m_data.GetSubstances().GetEpi());
  m_rKidneyEpinephrine = rkidney->GetSubstanceQuantity(m_data.GetSubstances().GetEpi());
  m_lKidneyEpinephrine = lkidney->GetSubstanceQuantity(m_data.GetSubstances().GetEpi());
  m_aortaGlucose = aorta->GetSubstanceQuantity(m_data.GetSubstances().GetGlucose());
  SESubstance* insulin = &m_data.GetSubstances().GetInsulin();
  m_insulinMolarMass_g_Per_mol = insulin->GetMolarMass(MassPerAmountUnit::g_Per_mol);
  m_splanchnicInsulin = m_data.GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::Splanchnic)->GetSubstanceQuantity(*insulin);
}

void Endocrine::AtSteadyState()
{

}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Endocrine process function
///
/// \details
/// Currently, only two hormones exists in the Pulse system: epinephrine and insulin. If the metabolic rate 
/// rises above the basal rate, epinephrine is released. This is meant to simulate a sympathetic 
/// nervous system response. The masses of the hormones are increased in the kidneys' efferent arterioles. 
/// The hormones will then circulate using the transport and substances methodology.
//--------------------------------------------------------------------------------------------------
void Endocrine::Process(bool solve_and_transport)
{
  ReleaseEpinephrine();
  SynthesizeInsulin();
  ComputeExposedModelParameters();
}
void Endocrine::ComputeExposedModelParameters()
{

}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculate the rate of insulin production 
///
/// \details
/// The insulin production rate is calculated based on the relevant range of glucose and instantaneous concentration of glucose in the aorta
/// (representative of the body). The equation for insulin production is from \cite tolic2000insulin
//--------------------------------------------------------------------------------------------------
void Endocrine::SynthesizeInsulin()
{
  double insulinMassDelta_g = 0;
  double bloodGlucoseConcentration_mg_Per_dL = m_data.GetSubstances().GetGlucose().GetBloodConcentration(MassPerVolumeUnit::mg_Per_dL);
  if (bloodGlucoseConcentration_mg_Per_dL >= 80)
  {
    // Linear function for blood glucose between 80-150 mg/dL and insulin synthesis of 100-475 pmol/min [polonsky1987insulin]
    double insulinSynthesisRate_pmol_Per_min = (5.357 * bloodGlucoseConcentration_mg_Per_dL) - 328.56;
    GetInsulinSynthesisRate().SetValue(insulinSynthesisRate_pmol_Per_min, AmountPerTimeUnit::pmol_Per_min);

    insulinMassDelta_g = Convert(insulinSynthesisRate_pmol_Per_min, AmountPerTimeUnit::pmol_Per_min, AmountPerTimeUnit::mol_Per_s);
    insulinMassDelta_g *= m_insulinMolarMass_g_Per_mol * m_data.GetTimeStep_s();
  }
  m_splanchnicInsulin->GetMass().IncrementValue(insulinMassDelta_g, MassUnit::g);
  m_splanchnicInsulin->Balance(BalanceLiquidBy::Mass);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Release epinephrine into the bloodstream and handle sympathetic responses
///
/// \details
/// Epinephrine is released at a basal rate of .18 ug/min \cite best1982release from the kidneys. During
/// certain events, the release rate of epinephrine increases. This is sympathetic response.
//--------------------------------------------------------------------------------------------------
void Endocrine::ReleaseEpinephrine()
{
  SEPatient& Patient = m_data.GetCurrentPatient();
  double patientWeight_kg = Patient.GetWeight(MassUnit::kg);
  double epinephrineBasalReleaseRate_ug_Per_min = .00229393 * patientWeight_kg; //We want it to be ~.18 ug/min for our StandardMale
  double epinephrineRelease_ug = (epinephrineBasalReleaseRate_ug_Per_min / 60) * m_data.GetTimeStep_s();  //amount released per timestep

  double currentMetabolicRate_W = m_data.GetEnergy().GetTotalMetabolicRate(PowerUnit::W);
  double basalMetabolicRate_W = Patient.GetBasalMetabolicRate(PowerUnit::W);
  double releaseMultiplier = 1.0;

  // If we have exercise, release more epi. Release multiplier is a sigmoid based on the total metabolic rate
  // with the maximum multiplier adapted from concentration data presented in @cite tidgren1991renal and @cite stratton1985hemodynamic
  // and the shape adjusted to match data in @cite tidgren1991renal.
  if (currentMetabolicRate_W > basalMetabolicRate_W)
  {
    double exercise_W = (currentMetabolicRate_W - basalMetabolicRate_W);
    double e50_W = 190;
    double eta = 0.035;
    double maxMultiplier = 18.75; 
    releaseMultiplier = 1.0 + GeneralMath::LogisticFunction(maxMultiplier, e50_W, eta, exercise_W);
  }

  // If we have a stress/anxiety response, release more epi
  if (m_data.GetActions().GetPatientActions().HasAcuteStress())
  {
    double severity = m_data.GetActions().GetPatientActions().GetAcuteStress().GetSeverity().GetValue();

    //The highest stress multiplier we currently support is 30
    releaseMultiplier += GeneralMath::LinearInterpolator(0, 1, 0, 30, severity);
  }

  epinephrineRelease_ug *= releaseMultiplier;

  m_rKidneyEpinephrine->GetMass().IncrementValue(0.5*epinephrineRelease_ug, MassUnit::ug);
  m_lKidneyEpinephrine->GetMass().IncrementValue(0.5*epinephrineRelease_ug, MassUnit::ug);
}