/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "physiology/BloodChemistry.h"
#include "controller/Controller.h"
#include "controller/Substances.h"
#include "controller/Compartments.h"
#include "PulseConfiguration.h"
// Dependent Systems
#include "system/physiology/SECardiovascularSystem.h"
// Assessments
#include "patient/assessments/SECompleteBloodCount.h"
#include "patient/assessments/SEComprehensiveMetabolicPanel.h"
// CDM
#include "patient/SEPatient.h"
#include "substance/SESubstance.h"
#include "engine/SEEventManager.h"
#include "compartment/fluid/SELiquidCompartment.h"
#include "compartment/substances/SELiquidSubstanceQuantity.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarMassPerAmount.h"
#include "properties/SEScalarAmountPerVolume.h"
#include "properties/SEScalarMassPerAmount.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalarMassPerAmount.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarHeatCapacitancePerMass.h"
#include "properties/SERunningAverage.h"
#include "utils/DataTrack.h"

#pragma warning(disable:4786)
#pragma warning(disable:4275)

BloodChemistry::BloodChemistry(PulseData& data) : PulseBloodChemistrySystem(data)
{
  m_ArterialOxygen_mmHg = new SERunningAverage();
  m_ArterialCarbonDioxide_mmHg = new SERunningAverage();
  Clear();
}

BloodChemistry::~BloodChemistry()
{
  Clear();
  delete m_ArterialOxygen_mmHg;
  delete m_ArterialCarbonDioxide_mmHg;
}

void BloodChemistry::Clear()
{
  SEBloodChemistrySystem::Clear();
  m_aorta = nullptr;
  m_aortaO2 = nullptr;
  m_aortaCO2 = nullptr;
  m_aortaCO = nullptr;
  m_brainO2 = nullptr;
  m_myocardiumO2 = nullptr;
  m_pulmonaryArteriesO2 = nullptr;
  m_pulmonaryArteriesCO2 = nullptr;
  m_pulmonaryVeinsO2 = nullptr;
  m_pulmonaryVeinsCO2 = nullptr;
  m_venaCava = nullptr;
  m_venaCavaO2 = nullptr;
  m_venaCavaCO2 = nullptr;
  m_ArterialOxygen_mmHg->Clear();
  m_ArterialCarbonDioxide_mmHg->Clear();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes system properties.
///
/// \details
/// For stabilization only!
/// Called AFTER Setup when stabilizing a new patient
//--------------------------------------------------------------------------------------------------
void BloodChemistry::Initialize()
{
  PulseSystem::Initialize();
  GetBloodDensity().SetValue(1050, MassPerVolumeUnit::kg_Per_m3);
  GetBloodSpecificHeat().SetValue(3617, HeatCapacitancePerMassUnit::J_Per_K_kg);
  GetVolumeFractionNeutralLipidInPlasma().SetValue(0.0023);
  GetVolumeFractionNeutralPhospholipidInPlasma().SetValue(0.0013);
  GetWhiteBloodCellCount().SetValue(7000, AmountPerVolumeUnit::ct_Per_uL);
  GetPhosphate().SetValue(1.1, AmountPerVolumeUnit::mmol_Per_L);
  GetStrongIonDifference().SetValue(40.5, AmountPerVolumeUnit::mmol_Per_L);

  m_ArterialOxygen_mmHg->Sample(m_aortaO2->GetPartialPressure(PressureUnit::mmHg));
  m_ArterialCarbonDioxide_mmHg->Sample(m_aortaCO2->GetPartialPressure(PressureUnit::mmHg));

  GetCarbonMonoxideSaturation().SetValue(0);
  Process();// Calculate the initial system values
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes parameters for BloodChemistry Class
///
/// \details
/// Called during both State loading and Patient Stabilization
/// Pull and setup up our data (can be from other systems)
/// Initialize will be called after this and can overwrite any of this data (only if stabilizing)
//--------------------------------------------------------------------------------------------------
void BloodChemistry::SetUp()
{
  const PulseConfiguration& ConfigData = m_data.GetConfiguration();
  m_redBloodCellVolume_mL = ConfigData.GetMeanCorpuscularVolume(VolumeUnit::mL);
  m_HbPerRedBloodCell_ug_Per_ct = ConfigData.GetMeanCorpuscularHemoglobin(MassPerAmountUnit::ug_Per_ct);

  m_aorta = m_data.GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::Aorta);
  m_aortaO2 = m_aorta->GetSubstanceQuantity(m_data.GetSubstances().GetO2());
  m_aortaCO2 = m_aorta->GetSubstanceQuantity(m_data.GetSubstances().GetCO2());

  m_brainO2 = m_data.GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::Brain)->GetSubstanceQuantity(m_data.GetSubstances().GetO2());
  m_myocardiumO2 = m_data.GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::Myocardium)->GetSubstanceQuantity(m_data.GetSubstances().GetO2());

  m_venaCava = m_data.GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::VenaCava);
  m_venaCavaO2 = m_venaCava->GetSubstanceQuantity(m_data.GetSubstances().GetO2());
  m_venaCavaCO2 = m_venaCava->GetSubstanceQuantity(m_data.GetSubstances().GetCO2());

  SELiquidCompartment* pulmonaryArteries = m_data.GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::PulmonaryArteries);
  m_pulmonaryArteriesO2 = pulmonaryArteries->GetSubstanceQuantity(m_data.GetSubstances().GetO2());
  m_pulmonaryArteriesCO2 = pulmonaryArteries->GetSubstanceQuantity(m_data.GetSubstances().GetCO2());

  SELiquidCompartment* pulmonaryVeins = m_data.GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::PulmonaryVeins);
  m_pulmonaryVeinsO2 = pulmonaryVeins->GetSubstanceQuantity(m_data.GetSubstances().GetO2());
  m_pulmonaryVeinsCO2 = pulmonaryVeins->GetSubstanceQuantity(m_data.GetSubstances().GetCO2());
}

void BloodChemistry::AtSteadyState()
{

}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Blood Chemistry Preprocess method
///
/// \details
/// The current Pulse implementation has no functionality in the preprocess function for blood chemistry.
//--------------------------------------------------------------------------------------------------
void BloodChemistry::PreProcess()
{
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Blood Chemistry process method
///
/// \details
/// The system data (blood concentrations, blood gases, and other blood properties) that are calculated
/// or changed by other systems are set on the blood chemistry system data objects. Events 
/// are triggered at specific blood concentrations of certain substances in CheckBloodGasLevels().
//--------------------------------------------------------------------------------------------------
void BloodChemistry::Process(bool solve_and_transport)
{
  //Push the compartment values of O2 and CO2 partial pressures on the corresponding system data.
  GetOxygenSaturation().Set(m_aortaO2->GetSaturation());
  GetCarbonDioxideSaturation().Set(m_aortaCO2->GetSaturation());
  if (m_aortaCO == nullptr && m_data.GetSubstances().IsActive(m_data.GetSubstances().GetCO()))
    m_aortaCO = m_aorta->GetSubstanceQuantity(m_data.GetSubstances().GetCO());
  if (m_aortaCO != nullptr)
  {
    GetCarbonMonoxideSaturation().Set(m_aortaCO->GetSaturation());
    GetPulseOximetry().SetValue(GetOxygenSaturation().GetValue() + GetCarbonMonoxideSaturation().GetValue());
  }
  else
  {
    GetPulseOximetry().Set(GetOxygenSaturation());
  }

  // This Hemoglobin Content is the mass of the hemoglobin only, not the hemoglobin and bound gas.
  // So we have to take our 4 Hb species masses and remove the mass of the gas.
  // Step 1) Get the mass of the bound species, which includes the mass of the bound gas.
  // Step 2) Convert to moles using the molar weight of the bound species (molar mass of bound species includes the mass of the bound gas and the mass of the unbound species).
  // Step 3) Covert moles of the bound species to moles of the unbound species. i.e.multiply by 1 (this step is implied)
  // Step 4) Convert moles of the unbound species to mass in order to get a total mass of hemoglobin (as opposed to a total mass of hemoglobin plus the bound gases).
  double totalHb_g = m_data.GetSubstances().GetSubstanceMass(m_data.GetSubstances().GetHb(), m_data.GetCompartments().GetVascularLeafCompartments(), MassUnit::g);
  double totalHbO2_g = (m_data.GetSubstances().GetSubstanceMass(m_data.GetSubstances().GetHbO2(), m_data.GetCompartments().GetVascularLeafCompartments(), MassUnit::g) / m_data.GetSubstances().GetHbO2().GetMolarMass(MassPerAmountUnit::g_Per_mol)) * m_data.GetSubstances().GetHb().GetMolarMass(MassPerAmountUnit::g_Per_mol);
  double totalHbCO2_g = (m_data.GetSubstances().GetSubstanceMass(m_data.GetSubstances().GetHbCO2(), m_data.GetCompartments().GetVascularLeafCompartments(), MassUnit::g) / m_data.GetSubstances().GetHbCO2().GetMolarMass(MassPerAmountUnit::g_Per_mol)) * m_data.GetSubstances().GetHb().GetMolarMass(MassPerAmountUnit::g_Per_mol);
  double totalHBO2CO2_g = (m_data.GetSubstances().GetSubstanceMass(m_data.GetSubstances().GetHbO2CO2(), m_data.GetCompartments().GetVascularLeafCompartments(), MassUnit::g) / m_data.GetSubstances().GetHbO2CO2().GetMolarMass(MassPerAmountUnit::g_Per_mol)) * m_data.GetSubstances().GetHb().GetMolarMass(MassPerAmountUnit::g_Per_mol);
  double totalHBCO_g = 0.0;
  if (m_aortaCO != nullptr)
    totalHBCO_g = (m_data.GetSubstances().GetSubstanceMass(m_data.GetSubstances().GetHbCO(), m_data.GetCompartments().GetVascularLeafCompartments(), MassUnit::g) / m_data.GetSubstances().GetHbCO().GetMolarMass(MassPerAmountUnit::g_Per_mol)) * m_data.GetSubstances().GetHb().GetMolarMass(MassPerAmountUnit::g_Per_mol);
  
  double totalHemoglobinO2Hemoglobin_g = totalHb_g + totalHbO2_g + totalHbCO2_g + totalHBO2CO2_g + totalHBCO_g;
  GetHemoglobinContent().SetValue(totalHemoglobinO2Hemoglobin_g, MassUnit::g);

  // Calculate Blood Cell Counts
  double RedBloodCellCount_ct = GetHemoglobinContent(MassUnit::ug) / m_HbPerRedBloodCell_ug_Per_ct;
  double RedBloodCellVolume_mL = RedBloodCellCount_ct*m_redBloodCellVolume_mL;
  double TotalBloodVolume_mL = m_data.GetCardiovascular().GetBloodVolume(VolumeUnit::mL);
  GetHematocrit().SetValue((RedBloodCellVolume_mL / TotalBloodVolume_mL));
  // Yes, we are giving GetRedBloodCellCount a concentration, because that is what it is, but clinically, it is known as RedBloodCellCount
  GetRedBloodCellCount().SetValue(RedBloodCellCount_ct / m_data.GetCardiovascular().GetBloodVolume(VolumeUnit::L), AmountPerVolumeUnit::ct_Per_L);
  GetPlasmaVolume().SetValue(TotalBloodVolume_mL - RedBloodCellVolume_mL, VolumeUnit::mL);

  // Calculate pH 
  /// \todo Change system data so that we have ArterialBloodPH (from aorta) and VenousBloodPH (from vena cava)
  GetBloodPH().Set(m_aorta->GetPH());

  // Pressures
  // arterial gas partial pressures
  GetArterialOxygenPressure().Set(m_aortaO2->GetPartialPressure());
  GetArterialCarbonDioxidePressure().Set(m_aortaCO2->GetPartialPressure());
  // pulmonary arteries
  GetPulmonaryArterialOxygenPressure().Set(m_pulmonaryArteriesO2->GetPartialPressure());
  GetPulmonaryArterialCarbonDioxidePressure().Set(m_pulmonaryArteriesCO2->GetPartialPressure());
  // pulmonary vein gas partial pressures -  the average of right and left pulmonary vein gas pressures
  GetPulmonaryVenousOxygenPressure().Set(m_pulmonaryVeinsO2->GetPartialPressure());
  GetPulmonaryVenousCarbonDioxidePressure().Set(m_pulmonaryVeinsCO2->GetPartialPressure());
  // venous gas partial pressures
  GetVenousOxygenPressure().Set(m_venaCavaO2->GetPartialPressure());
  GetVenousCarbonDioxidePressure().Set(m_venaCavaCO2->GetPartialPressure());

  double totalFlow_mL_Per_min = m_data.GetCardiovascular().GetCardiacOutput(VolumePerTimeUnit::mL_Per_min);
  double shuntFlow_mL_Per_min = m_data.GetCardiovascular().GetPulmonaryMeanShuntFlow(VolumePerTimeUnit::mL_Per_min);
  double shunt = 0.0;
  if (totalFlow_mL_Per_min > 0.0)
  {
    shunt = shuntFlow_mL_Per_min / totalFlow_mL_Per_min;
  }
  shunt = MIN(shunt, 1.0);
  GetShuntFraction().SetValue(shunt);

  CheckBloodGasLevels();

  // Total up all active substances
  double bloodMass_ug;
  double tissueMass_ug;
  for (SESubstance* sub : m_data.GetSubstances().GetActiveSubstances())
  {
    bloodMass_ug = m_data.GetSubstances().GetSubstanceMass(*sub, m_data.GetCompartments().GetVascularLeafCompartments(), MassUnit::ug);
    tissueMass_ug = m_data.GetSubstances().GetSubstanceMass(*sub, m_data.GetCompartments().GetTissueLeafCompartments(), MassUnit::ug);
    sub->GetMassInBody().SetValue(bloodMass_ug + tissueMass_ug, MassUnit::ug);
    sub->GetMassInBlood().SetValue(bloodMass_ug, MassUnit::ug);
    sub->GetMassInTissue().SetValue(tissueMass_ug, MassUnit::ug);
    sub->GetBloodConcentration().SetValue(bloodMass_ug / TotalBloodVolume_mL, MassPerVolumeUnit::ug_Per_mL);
  }
  // Compute Special Concentrations
  GetBloodUreaNitrogenConcentration().SetValue(m_data.GetSubstances().GetUrea().GetBloodConcentration(MassPerVolumeUnit::ug_Per_mL) / 2.14, MassPerVolumeUnit::ug_Per_mL);
  double albuminConcentration_ug_Per_mL = m_data.GetSubstances().GetAlbumin().GetBloodConcentration(MassPerVolumeUnit::ug_Per_mL);
  m_data.GetSubstances().GetGlobulin().GetBloodConcentration().SetValue(albuminConcentration_ug_Per_mL * 1.6 - albuminConcentration_ug_Per_mL, MassPerVolumeUnit::ug_Per_mL);
  double HemoglobinConcentration = totalHemoglobinO2Hemoglobin_g / TotalBloodVolume_mL;
  m_data.GetSubstances().GetHb().GetBloodConcentration().SetValue(HemoglobinConcentration, MassPerVolumeUnit::g_Per_mL);
  GetTotalProteinConcentration().SetValue(albuminConcentration_ug_Per_mL * 1.6, MassPerVolumeUnit::ug_Per_mL);
  // 1.6 comes from reading http://www.drkaslow.com/html/proteins_-_albumin-_globulins-_etc.html
  ComputeExposedModelParameters();
}
void BloodChemistry::ComputeExposedModelParameters()
{

}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Blood Chemistry postprocess method
///
/// \details
/// The current Pulse implementation has no specific postprocess functionality.
//--------------------------------------------------------------------------------------------------
void BloodChemistry::PostProcess(bool solve_and_transport)
{

}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Checks the blood gas (oxygen, carbon dioxide) levels and sets events.
///
/// \details
/// Checks the oxygen and carbon dioxide levels in specific compartments of the body. 
/// Set events for hypercapnia, Hypoxia, and Brain and MyoCardium Oxygen Deficit based on the levels.
//--------------------------------------------------------------------------------------------------
void BloodChemistry::CheckBloodGasLevels()
{
  m_ArterialOxygen_mmHg->Sample(m_aortaO2->GetPartialPressure(PressureUnit::mmHg));
  m_ArterialCarbonDioxide_mmHg->Sample(m_aortaCO2->GetPartialPressure(PressureUnit::mmHg));
  //Only check these at the end of a cardiac cycle and reset at start of cardiac cycle 
  if (m_data.GetEvents().IsEventActive(eEvent::StartOfCardiacCycle))
  {
    if (m_data.GetState() == EngineState::Active)
    {// Don't throw events if we are initializing
      double arterialOxygen_mmHg = m_ArterialOxygen_mmHg->Value();
      double arterialCarbonDioxide_mmHg = m_ArterialCarbonDioxide_mmHg->Value();
      // hypercapnia check
      double hypercapniaFlag = 60.0; // \cite Guyton11thEd p.531 
      double carbonDioxideToxicity = 80.0;  // \cite labertsen1971CarbonDioxideToxicity
      if (arterialCarbonDioxide_mmHg >= hypercapniaFlag)
      {
        /// \event Patient: Hypercapnia. The carbon dioxide partial pressure has risen above 60 mmHg. The patient is now hypercapnic.
        m_data.GetEvents().SetEvent(eEvent::Hypercapnia, true, m_data.GetSimulationTime());

        if (arterialCarbonDioxide_mmHg > carbonDioxideToxicity)
        {
          /// \irreversible The carbon dioxide partial pressure is greater than 80 mmHg. 
          m_data.GetEvents().SetEvent(eEvent::IrreversibleState, true, m_data.GetSimulationTime());
          m_ss << "Arterial Carbon Dioxide partial pressure is " << arterialCarbonDioxide_mmHg << ". This is beyond 80 mmHg triggering extreme Hypercapnia, patient is in an irreversible state.";
          Fatal(m_ss);
        }
      }
      else if (m_data.GetEvents().IsEventActive(eEvent::Hypercapnia) && arterialCarbonDioxide_mmHg < (hypercapniaFlag - 3))
      {
        /// \event Patient: End Hypercapnia. The carbon dioxide partial pressure has fallen below 57 mmHg. The patient is no longer considered to be hypercapnic.
        /// This event is triggered if the patient was hypercapnic and is now considered to be recovered.
        m_data.GetEvents().SetEvent(eEvent::Hypercapnia, false, m_data.GetSimulationTime());
      }

      // hypoxia check
      double hypoxiaFlag = 65.0; //Arterial O2 Partial Pressure in mmHg \cite Pierson2000Pathophysiology
      double hypoxiaIrreversible = 15.0; // \cite hobler1973HypoxemiaCardiacOutput
      if (arterialOxygen_mmHg <= hypoxiaFlag)
      {
        /// \event Patient: Hypoxia Event. The oxygen partial pressure has fallen below 65 mmHg, indicating that the patient is hypoxic.
        m_data.GetEvents().SetEvent(eEvent::Hypoxia, true, m_data.GetSimulationTime());

        if (arterialOxygen_mmHg < hypoxiaIrreversible)
        {
          /// \irreversible Arterial oxygen partial pressure has been critically reduced to below 15 mmHg.
          m_data.GetEvents().SetEvent(eEvent::IrreversibleState, true, m_data.GetSimulationTime());
          m_ss << "Arterial Oxygen partial pressure is " << arterialOxygen_mmHg << ". This is below 15 mmHg triggering extreme Hypoxia, patient is in an irreversible state.";
          Fatal(m_ss);
        }
      }
      else if (arterialOxygen_mmHg > (hypoxiaFlag + 3))
      {
        /// \event Patient: End Hypoxia Event. The oxygen partial pressure has rise above 68 mmHg. If this occurs when the patient is hypoxic, it will reverse the hypoxic event.
        /// The patient is no longer considered to be hypoxic.
        m_data.GetEvents().SetEvent(eEvent::Hypoxia, false, m_data.GetSimulationTime());
      }

      // hyperoxemia check
      double hyperoxemiaModerateFlag = 120.0; //Arterial O2 Partial Pressure in mmHg
      double hyperoxemiaSevereFlag = 200.0; //Arterial O2 Partial Pressure in mmHg
      if (arterialOxygen_mmHg > hyperoxemiaSevereFlag)
      {
        /// \event Patient: Severe Hyperoxemia: Arterial O2 Partial Pressure is above 200 mmHg causing oxygen toxicity
        m_data.GetEvents().SetEvent(eEvent::SevereHyperoxemia, true, m_data.GetSimulationTime());
        m_data.GetEvents().SetEvent(eEvent::ModerateHyperoxemia, false, m_data.GetSimulationTime());
      }
      else if (arterialOxygen_mmHg > hyperoxemiaModerateFlag)
      {
        /// \event Patient: Moderate Hyperoxemia: Arterial O2 Partial Pressure is above 120 mmHg
        m_data.GetEvents().SetEvent(eEvent::SevereHyperoxemia, false, m_data.GetSimulationTime());
        m_data.GetEvents().SetEvent(eEvent::ModerateHyperoxemia, true, m_data.GetSimulationTime());
      }
      else if (arterialOxygen_mmHg < hyperoxemiaModerateFlag - 3)
      {
        m_data.GetEvents().SetEvent(eEvent::SevereHyperoxemia, false, m_data.GetSimulationTime());
        m_data.GetEvents().SetEvent(eEvent::ModerateHyperoxemia, false, m_data.GetSimulationTime());
      }

      // hypocapnia check
      double hypocapniaModerateFlag = 30.0; //Arterial CO2 Partial Pressure in mmHg
      double hypocapniaSevereFlag = 15.0; //Arterial CO2 Partial Pressure in mmHg
      if (arterialOxygen_mmHg < hypocapniaSevereFlag)
      {
        /// \event Patient: Severe Hypocapnia: Arterial CO2 Partial Pressure is below 15 mmHg
        m_data.GetEvents().SetEvent(eEvent::SevereHypocapnia, true, m_data.GetSimulationTime());
        m_data.GetEvents().SetEvent(eEvent::ModerateHypocapnia, false, m_data.GetSimulationTime());
      }
      else if (arterialOxygen_mmHg < hypocapniaModerateFlag)
      {
        /// \event Patient: Moderate Hypocapnia: Arterial CO2 Partial Pressure is below 30 mmHg
        m_data.GetEvents().SetEvent(eEvent::SevereHypocapnia, false, m_data.GetSimulationTime());
        m_data.GetEvents().SetEvent(eEvent::ModerateHypocapnia, true, m_data.GetSimulationTime());
      }
      else if (arterialOxygen_mmHg > hypocapniaModerateFlag + 3)
      {
        m_data.GetEvents().SetEvent(eEvent::SevereHypocapnia, false, m_data.GetSimulationTime());
        m_data.GetEvents().SetEvent(eEvent::ModerateHypocapnia, false, m_data.GetSimulationTime());
      }
    }

    m_ArterialOxygen_mmHg->Clear();
    m_ArterialCarbonDioxide_mmHg->Clear();
  }


  if (m_data.GetState() > EngineState::InitialStabilization)
  {// Don't throw events if we are initializing

    // When the brain oxygen partial pressure is too low, events are triggered and event duration is tracked.
    // If the oxygen tension in the brain remains below the thresholds for the specified amount of time, the body
    // will go into an irreversible state. The threshold values are chosen based on empirical data reviewed in summary in @cite dhawan2011neurointensive 
    // and from data presented in @cite purins2012brain and @cite doppenberg1998determination.
    if (m_brainO2->GetPartialPressure(PressureUnit::mmHg) < 19.0) // We are using the mean from dhawan
    {     
      /// \event Patient: Brain Oxygen Deficit Event. The oxygen partial pressure in the brain has dropped to a dangerously low level.
      m_data.GetEvents().SetEvent(eEvent::BrainOxygenDeficit, true, m_data.GetSimulationTime());

      // If the O2 tension is below a critical threshold, the irreversible damage occurs more quickly
      if (m_brainO2->GetPartialPressure(PressureUnit::mmHg) < 10.0)
      {
        // Irreversible damage occurs if the deficit has gone on too long
        if (m_data.GetEvents().GetEventDuration(eEvent::CriticalBrainOxygenDeficit, TimeUnit::s) > 1800)
        {
          /// \irreversible Brain oxygen pressure has been dangerously low for more than 30 minutes.
          m_data.GetEvents().SetEvent(eEvent::IrreversibleState, true, m_data.GetSimulationTime());
          m_ss << "Brain Oxygen partial pressure is " << m_brainO2->GetPartialPressure(PressureUnit::mmHg) << " and has been below the danger threshold for " <<
            m_data.GetEvents().GetEventDuration(eEvent::CriticalBrainOxygenDeficit, TimeUnit::s) << " seconds. Damage is irreversible.";
          Fatal(m_ss);
        }

        /// \event Patient: Critical Brain Oxygen Deficit Event. The oxygen partial pressure in the brain has dropped to a critically low level.
        m_data.GetEvents().SetEvent(eEvent::CriticalBrainOxygenDeficit, true, m_data.GetSimulationTime());
      }
      else if (m_brainO2->GetPartialPressure(PressureUnit::mmHg) > 12.0)
      {
        /// \event Patient: End Brain Oxygen Deficit Event. The oxygen partial pressure has risen above 12 mmHg in the brain. If this occurs when the patient has a critical brain oxygen deficit event, it will reverse the event.
        /// The brain is not in a critical oxygen deficit.
        m_data.GetEvents().SetEvent(eEvent::CriticalBrainOxygenDeficit, false, m_data.GetSimulationTime());
      }
    }
    else if (m_brainO2->GetPartialPressure(PressureUnit::mmHg) > 25.0)
    {
      /// \event Patient: End Brain Oxygen Deficit Event. The oxygen partial pressure has risen above 25 mmHg in the brain. If this occurs when the patient has a brain oxygen deficit event, it will reverse the event.
      /// The brain is getting oxygen.
      m_data.GetEvents().SetEvent(eEvent::BrainOxygenDeficit, false, m_data.GetSimulationTime());
      // The critical deficit event is also set to false just in case there is an unrealistically rapid transition in oxygen partial pressure.
      m_data.GetEvents().SetEvent(eEvent::CriticalBrainOxygenDeficit, false, m_data.GetSimulationTime());
    }

    //Myocardium Oxygen Check
    if (m_myocardiumO2->GetPartialPressure(PressureUnit::mmHg) < 5)
    {
      /// \event Patient: The heart is not receiving enough oxygen. Coronary arteries should dilate to increase blood flow to the heart.
      m_data.GetEvents().SetEvent(eEvent::MyocardiumOxygenDeficit, true, m_data.GetSimulationTime());

      if (m_data.GetEvents().GetEventDuration(eEvent::MyocardiumOxygenDeficit, TimeUnit::s) > 2400)  // \cite murry1986preconditioning
      {
        /// \irreversible Heart has not been receiving enough oxygen for more than 40 min.
        m_data.GetEvents().SetEvent(eEvent::IrreversibleState, true, m_data.GetSimulationTime());
        m_ss << "Myocardium oxygen partial pressure is  " << m_myocardiumO2->GetPartialPressure(PressureUnit::mmHg) << " and has been sustained for " << m_data.GetEvents().GetEventDuration(eEvent::MyocardiumOxygenDeficit, TimeUnit::s) <<
          "patient heart muscle has experienced necrosis and is in an irreversible state.";
        Fatal(m_ss);
      }
    }
    else if (m_myocardiumO2->GetPartialPressure(PressureUnit::mmHg) > 8)
    {
      /// \event Patient: End Myocardium Oxygen Event. The heart is now receiving enough oxygen. If this occurs when the patient has a heart oxygen deficit event, it will reverse the event.
      /// The brain is getting oxygen.
      m_data.GetEvents().SetEvent(eEvent::MyocardiumOxygenDeficit, false, m_data.GetSimulationTime());
    }
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Sets data on the metabolic panel object.
///
/// \details
/// Sets data on the metabolic panel object to create the metabolic panel.  
/// Uses information from the chem 14 substances that are in %Pulse (see @ref bloodchemistry-assessments)
//--------------------------------------------------------------------------------------------------
bool BloodChemistry::CalculateComprehensiveMetabolicPanel(SEComprehensiveMetabolicPanel& cmp) const
{
  cmp.Clear();
  cmp.GetAlbumin().Set(m_data.GetSubstances().GetAlbumin().GetBloodConcentration());
  //cmp.GetALP().SetValue();
  //cmp.GetALT().SetValue();
  //cmp.GetAST().SetValue();
  cmp.GetBUN().SetValue(GetBloodUreaNitrogenConcentration(MassPerVolumeUnit::ug_Per_mL),MassPerVolumeUnit::ug_Per_mL);
  cmp.GetCalcium().Set(m_data.GetSubstances().GetCalcium().GetBloodConcentration());
  double CL_mmol_Per_L = m_data.GetSubstances().GetChloride().GetBloodConcentration(MassPerVolumeUnit::g_Per_L) /
    m_data.GetSubstances().GetChloride().GetMolarMass(MassPerAmountUnit::g_Per_mmol);
  cmp.GetChloride().SetValue(CL_mmol_Per_L, AmountPerVolumeUnit::mmol_Per_L);
  // CO2 is predominantly Bicarbonate, so going to put that in this slot
  double HCO3_mmol_Per_L = m_data.GetSubstances().GetBicarbonate().GetBloodConcentration(MassPerVolumeUnit::g_Per_L) /
    m_data.GetSubstances().GetHCO3().GetMolarMass(MassPerAmountUnit::g_Per_mmol);
  cmp.GetCO2().SetValue(HCO3_mmol_Per_L, AmountPerVolumeUnit::mmol_Per_L);
  cmp.GetCreatinine().Set(m_data.GetSubstances().GetCreatinine().GetBloodConcentration());
  cmp.GetGlucose().Set(m_data.GetSubstances().GetGlucose().GetBloodConcentration());
  double K_mmol_Per_L = m_data.GetSubstances().GetPotassium().GetBloodConcentration(MassPerVolumeUnit::g_Per_L) /
    m_data.GetSubstances().GetPotassium().GetMolarMass(MassPerAmountUnit::g_Per_mmol);
  cmp.GetPotassium().SetValue(K_mmol_Per_L, AmountPerVolumeUnit::mmol_Per_L);
  double Sodium_mmol_Per_L = m_data.GetSubstances().GetSodium().GetBloodConcentration(MassPerVolumeUnit::g_Per_L) /
    m_data.GetSubstances().GetSodium().GetMolarMass(MassPerAmountUnit::g_Per_mmol);
  cmp.GetSodium().SetValue(Sodium_mmol_Per_L, AmountPerVolumeUnit::mmol_Per_L);
  //cmp.GetTotalBelirubin().SetValue();
  cmp.GetTotalProtein().SetValue(GetTotalProteinConcentration(MassPerVolumeUnit::ug_Per_mL),MassPerVolumeUnit::ug_Per_mL);
  return true;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Sets data on the complete blood count object.
///
/// \details
/// Sets data on the complete blood count object to create the [CBC](@ref bloodchemistry-assessments).
//--------------------------------------------------------------------------------------------------
bool BloodChemistry::CalculateCompleteBloodCount(SECompleteBloodCount& cbc) const
{
  cbc.Clear();
  cbc.GetHematocrit().SetValue(GetHematocrit());
  cbc.GetHemoglobin().Set(m_data.GetSubstances().GetHb().GetBloodConcentration());
  cbc.GetPlateletCount().SetValue(325000, AmountPerVolumeUnit::ct_Per_uL);  // Hardcoded for now, don't support PlateletCount yet
  cbc.GetMeanCorpuscularHemoglobin().SetValue(m_data.GetConfiguration().GetMeanCorpuscularHemoglobin(MassPerAmountUnit::pg_Per_ct), MassPerAmountUnit::pg_Per_ct);
  cbc.GetMeanCorpuscularHemoglobinConcentration().SetValue(m_data.GetSubstances().GetHb().GetBloodConcentration(MassPerVolumeUnit::g_Per_dL) / GetHematocrit(), MassPerVolumeUnit::g_Per_dL);//Average range should be 32-36 g/dL. (https://en.wikipedia.org/wiki/Mean_corpuscular_hemoglobin_concentration)
  cbc.GetMeanCorpuscularVolume().SetValue(m_data.GetConfiguration().GetMeanCorpuscularVolume(VolumeUnit::uL), VolumeUnit::uL);
  cbc.GetRedBloodCellCount().SetValue(GetRedBloodCellCount(AmountPerVolumeUnit::ct_Per_L),AmountPerVolumeUnit::ct_Per_L);
  cbc.GetWhiteBloodCellCount().SetValue(GetWhiteBloodCellCount(AmountPerVolumeUnit::ct_Per_L),AmountPerVolumeUnit::ct_Per_L);
  return true;
}
