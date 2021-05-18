/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "physiology/Respiratory.h"
#include "controller/Circuits.h"
#include "controller/Compartments.h"
#include "controller/Substances.h"
#include "PulseConfiguration.h"
// Conditions 
#include "engine/SEConditionManager.h"
#include "patient/conditions/SEChronicObstructivePulmonaryDisease.h"
#include "patient/conditions/SELobarPneumonia.h"
#include "patient/conditions/SEImpairedAlveolarExchange.h"
#include "patient/conditions/SEPulmonaryFibrosis.h"
#include "patient/conditions/SEPulmonaryShunt.h"
#include "patient/conditions/SEAcuteRespiratoryDistressSyndrome.h"
// Actions
#include "engine/SEActionManager.h"
#include "engine/SEOverrides.h"
#include "engine/SEPatientActionCollection.h"
#include "patient/actions/SEAcuteRespiratoryDistressSyndromeExacerbation.h"
#include "patient/actions/SEAirwayObstruction.h"
#include "patient/actions/SEAsthmaAttack.h"
#include "patient/actions/SEBronchoconstriction.h"
#include "patient/actions/SEChestOcclusiveDressing.h"
#include "patient/actions/SEChronicObstructivePulmonaryDiseaseExacerbation.h"
#include "patient/actions/SEConsciousRespiration.h"
#include "patient/actions/SEForcedExhale.h"
#include "patient/actions/SEForcedInhale.h"
#include "patient/actions/SEForcedPause.h"
#include "patient/actions/SEImpairedAlveolarExchangeExacerbation.h"
#include "patient/actions/SEUseInhaler.h"
#include "patient/actions/SEDyspnea.h"
#include "patient/actions/SEIntubation.h"
#include "patient/actions/SELobarPneumoniaExacerbation.h"
#include "patient/actions/SEMechanicalVentilation.h"
#include "patient/actions/SENeedleDecompression.h"
#include "patient/actions/SEPulmonaryShuntExacerbation.h"
#include "patient/actions/SERespiratoryFatigue.h"
#include "patient/actions/SESupplementalOxygen.h"
#include "patient/actions/SETensionPneumothorax.h"
// Assessments
#include "patient/assessments/SEPulmonaryFunctionTest.h"
// Dependent Systems
#include "system/physiology/SEBloodChemistrySystem.h"
#include "system/physiology/SEDrugSystem.h"
#include "system/physiology/SEEnergySystem.h"
// CDM
#include "patient/SEPatient.h"
#include "engine/SEEventManager.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceAerosolization.h"
#include "substance/SESubstanceConcentration.h"
#include "substance/SESubstanceFraction.h"
#include "substance/SESubstanceTransport.h"
#include "circuit/fluid/SEFluidCircuitCalculator.h"
#include "circuit/fluid/SEFluidCircuit.h"
#include "circuit/fluid/SEFluidCircuitNode.h"
#include "circuit/fluid/SEFluidCircuitPath.h"
#include "compartment/fluid/SEGasCompartmentGraph.h"
#include "compartment/fluid/SELiquidCompartmentGraph.h"
// Properties
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarArea.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarInversePressure.h"
#include "properties/SEScalarInverseVolume.h"
#include "properties/SEScalarLength.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarNegative1To1.h"
#include "properties/SEScalarPower.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarPressurePerVolume.h"
#include "properties/SEScalarPressureTimePerVolume.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalarVolumePerPressure.h"
#include "properties/SEFunctionVolumeVsTime.h"
#include "properties/SERunningAverage.h"
// Utilitiy Classes
#include "utils/DataTrack.h"
#include "utils/GeneralMath.h"

#ifdef _MSC_VER 
#pragma warning( disable : 4305 4244 )  // Disable some warning messages
#endif

//Flag for data tracks
//Should be commented out, unless debugging
//#define DEBUG

//Flag for setting things constant to test
//Should be commented out, unless debugging/tuning
//#define TUNING

Respiratory::Respiratory(PulseData& data) : PulseRespiratorySystem(data)
{
  m_BloodPHRunningAverage = new SERunningAverage();
  m_MeanAirwayPressure_cmH2O = new SERunningAverage();
  m_ArterialO2RunningAverage_mmHg = new SERunningAverage();
  m_ArterialCO2RunningAverage_mmHg = new SERunningAverage();

  m_Calculator = new SEFluidCircuitCalculator(VolumePerPressureUnit::L_Per_cmH2O, VolumePerTimeUnit::L_Per_s, PressureTimeSquaredPerVolumeUnit::cmH2O_s2_Per_L, PressureUnit::cmH2O, VolumeUnit::L, PressureTimePerVolumeUnit::cmH2O_s_Per_L, GetLogger());
  m_GasTransporter = new SEGasTransporter(VolumePerTimeUnit::L_Per_s, VolumeUnit::L, VolumeUnit::L, GetLogger());
  m_AerosolTransporter = new SELiquidTransporter(VolumePerTimeUnit::mL_Per_s, VolumeUnit::mL, MassUnit::ug, MassPerVolumeUnit::ug_Per_mL, GetLogger());
  Clear();
}

Respiratory::~Respiratory()
{
  Clear();
  delete m_BloodPHRunningAverage;
  delete m_MeanAirwayPressure_cmH2O;
  delete m_ArterialO2RunningAverage_mmHg;
  delete m_ArterialCO2RunningAverage_mmHg;

  delete m_Calculator;
  delete m_GasTransporter;
  delete m_AerosolTransporter;
}

void Respiratory::Clear()
{
  SERespiratorySystem::Clear();
  m_PatientActions = nullptr;

  m_Environment = nullptr;
  m_AerosolAirway = nullptr;
  m_AerosolCarina = nullptr;
  m_AerosolLeftAnatomicDeadSpace = nullptr;
  m_AerosolLeftAlveolarDeadSpace = nullptr;
  m_AerosolLeftAlveoli = nullptr;
  m_AerosolRightAnatomicDeadSpace = nullptr;
  m_AerosolRightAlveolarDeadSpace = nullptr;
  m_AerosolRightAlveoli = nullptr;
  m_Lungs = nullptr;
  m_LeftLungExtravascular = nullptr;
  m_RightLungExtravascular = nullptr;
  m_Carina = nullptr;
  m_AortaO2 = nullptr;
  m_AortaCO2 = nullptr;
  m_MechanicalVentilationConnection = nullptr;
  m_MechanicalVentilationAerosolConnection = nullptr;
  m_PleuralCavity = nullptr;

  m_RespiratoryCircuit = nullptr;

  m_Airway = nullptr;
  m_LeftAlveoli = nullptr;
  m_LeftAnatomicDeadSpace = nullptr;
  m_LeftAlveolarDeadSpace = nullptr;
  m_LeftPleural = nullptr;
  m_RespiratoryMuscle = nullptr;
  m_RightAlveoli = nullptr;
  m_RightAnatomicDeadSpace = nullptr;
  m_RightAlveolarDeadSpace = nullptr;
  m_RightPleural = nullptr;

  m_CarinaToLeftAnatomicDeadSpace = nullptr;
  m_CarinaToRightAnatomicDeadSpace = nullptr;
  m_LeftAnatomicDeadSpaceToLeftAlveolarDeadSpace = nullptr;
  m_RightAnatomicDeadSpaceToRightAlveolarDeadSpace = nullptr;
  m_LeftAlveolarDeadSpaceToLeftAlveoli = nullptr;
  m_RightAlveolarDeadSpaceToRightAlveoli = nullptr;
  m_RightPleuralToRespiratoryMuscle = nullptr;
  m_LeftPleuralToRespiratoryMuscle = nullptr;
  m_DriverPressurePath = nullptr;
  m_AirwayToCarina = nullptr;
  m_AirwayToStomach = nullptr;
  m_EnvironmentToLeftChestLeak = nullptr;
  m_EnvironmentToRightChestLeak = nullptr;
  m_LeftAlveoliLeakToLeftPleural = nullptr;
  m_RightAlveoliLeakToRightPleural = nullptr;
  m_LeftPleuralToEnvironment = nullptr;
  m_RightPleuralToEnvironment = nullptr;
  m_RightAlveoliToRightPleuralConnection = nullptr;
  m_LeftAlveoliToLeftPleuralConnection = nullptr;
  m_RightPulmonaryCapillary = nullptr;
  m_LeftPulmonaryCapillary = nullptr;
  m_LeftPulmonaryArteriesToVeins = nullptr;
  m_RightPulmonaryArteriesToVeins = nullptr;
  m_ConnectionToAirway = nullptr;
  m_GroundToConnection = nullptr;

  m_BloodPHRunningAverage->Clear();
  m_MeanAirwayPressure_cmH2O->Clear();
  m_ArterialO2RunningAverage_mmHg->Clear();
  m_ArterialCO2RunningAverage_mmHg->Clear();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes system properties to valid homeostatic values.
///
/// \details
/// For stabilization only!
/// Called AFTER Setup when stabilizing a new patient
//--------------------------------------------------------------------------------------------------
void Respiratory::Initialize()
{
  PulseSystem::Initialize();

  //Vital signs
  m_NotBreathing = false;
  m_TopBreathTotalVolume_L = 0.0;
  m_TopBreathAlveoliVolume_L = 0.0;
  m_TopBreathPleuralVolume_L = 0.0;
  m_TopBreathPleuralPressure_cmH2O = 0.0;
  m_TopBreathAlveoliPressure_cmH2O = 0.0;
  m_TopBreathDriverPressure_cmH2O = 0.0;
  m_LastCardiacCycleBloodPH = 7.4;
  m_TopCarinaO2 = 0.2;
  m_TopBreathElapsedTime_min = 0.0;
  m_BottomBreathElapsedTime_min = 0.0;
  m_BottomBreathTotalVolume_L = 0.0;
  m_BottomBreathAlveoliVolume_L = m_BottomBreathTotalVolume_L;
  m_BottomBreathPleuralVolume_L = 0.0;
  m_BottomBreathPleuralPressure_cmH2O = 0.0;
  m_BottomBreathAlveoliPressure_cmH2O = 0.0;
  m_BottomBreathDriverPressure_cmH2O = 0.0;
  m_PeakAlveolarPressure_cmH2O = 0.0;
  m_MaximalAlveolarPressure_cmH2O = 0.0;

  //Driver
  m_ElapsedBreathingCycleTime_min = 0.0;
  m_TopBreathElapsedTime_min = 0.0;
  m_BreathingCycle = false;  
  m_VentilationFrequency_Per_min = m_data.GetCurrentPatient().GetRespirationRateBaseline(FrequencyUnit::Per_min);
  m_BreathingCycleTime_s = 60.0 / m_VentilationFrequency_Per_min + m_data.GetTimeStep_s(); //Make the engine start at the beginning of a breath
  m_DriverPressure_cmH2O = 0.0;
  m_VentilationToTidalVolumeSlope = 30.0;
  //The peak driver pressure is the pressure above the default pressure
  m_PeakInspiratoryPressure_cmH2O = 0.0;
  m_PeakExpiratoryPressure_cmH2O = 0.0;
  m_PreviousTargetAlveolarVentilation_L_Per_min = m_data.GetCurrentPatient().GetTidalVolumeBaseline(VolumeUnit::L) * m_VentilationFrequency_Per_min;
  m_AverageLocalTissueBronchodilationEffects = 0.0;

  m_IERatioScaleFactor = 1.0;

  m_leftAlveoliDecrease_L = 0;
  m_rightAlveoliDecrease_L = 0;

  //System data
  double TidalVolume_L = m_data.GetCurrentPatient().GetTidalVolumeBaseline(VolumeUnit::L);
  double RespirationRate_Per_min = m_data.GetCurrentPatient().GetRespirationRateBaseline(FrequencyUnit::Per_min);
  GetTidalVolume().SetValue(TidalVolume_L, VolumeUnit::L);
  GetRespirationRate().SetValue(RespirationRate_Per_min, FrequencyUnit::Per_min);
  GetInspiratoryExpiratoryRatio().SetValue(0.5);
  GetCarricoIndex().SetValue(452.0, PressureUnit::mmHg);
  GetFractionOfInsipredOxygen().SetValue(0.21);
  GetSaturationAndFractionOfInspiredOxygenRatio().SetValue(0.0);
  GetOxygenationIndex().SetValue(0.0);
  GetOxygenSaturationIndex().SetValue(0.0, PressureUnit::cmH2O);
  GetMeanAirwayPressure().SetValue(0.0, PressureUnit::cmH2O);

  double AnatomicDeadSpace_L = m_LeftAnatomicDeadSpace->GetVolumeBaseline(VolumeUnit::L) + m_RightAnatomicDeadSpace->GetVolumeBaseline(VolumeUnit::L);
  double AlveolarDeadSpace_L = m_LeftAlveolarDeadSpace->GetVolumeBaseline(VolumeUnit::L) + m_RightAlveolarDeadSpace->GetVolumeBaseline(VolumeUnit::L);
  GetAnatomicDeadSpace().SetValue(AnatomicDeadSpace_L, VolumeUnit::L);
  GetAlveolarDeadSpace().SetValue(AlveolarDeadSpace_L, VolumeUnit::L);
  GetPhysiologicDeadSpace().SetValue(AnatomicDeadSpace_L + AlveolarDeadSpace_L, VolumeUnit::L);

  GetTotalAlveolarVentilation().SetValue(RespirationRate_Per_min * TidalVolume_L, VolumePerTimeUnit::L_Per_min);
  GetTotalPulmonaryVentilation().SetValue(RespirationRate_Per_min * TidalVolume_L, VolumePerTimeUnit::L_Per_min);
  GetTotalDeadSpaceVentilation().SetValue(AnatomicDeadSpace_L * RespirationRate_Per_min, VolumePerTimeUnit::L_Per_min);
  GetSpecificVentilation().SetValue(0.21);

  GetEndTidalCarbonDioxideFraction().SetValue(0.0827);
  GetEndTidalCarbonDioxidePressure().SetValue(0.0, PressureUnit::mmHg);
  GetEndTidalOxygenFraction().SetValue(0.0);
  GetEndTidalOxygenPressure().SetValue(0.0, PressureUnit::mmHg);

  GetInspiratoryFlow().SetValue(0, VolumePerTimeUnit::L_Per_s);
  GetIntrapleuralPressure().SetValue(0.0, PressureUnit::cmH2O);
  GetTransrespiratoryPressure().SetValue(0.0, PressureUnit::cmH2O);
  GetTransairwayPressure().SetValue(0.0, PressureUnit::cmH2O);
  GetTranspulmonaryPressure().SetValue(0.0, PressureUnit::cmH2O);
  GetTransalveolarPressure().SetValue(0.0, PressureUnit::cmH2O);
  GetTransthoracicPressure().SetValue(0.0, PressureUnit::cmH2O);
  GetTransChestWallPressure().SetValue(0.0, PressureUnit::cmH2O);

  GetPulmonaryCompliance().SetValue(0.1, VolumePerPressureUnit::L_Per_cmH2O);
  GetLungCompliance().SetValue(0.1, VolumePerPressureUnit::L_Per_cmH2O);
  GetChestWallCompliance().SetValue(0.2, VolumePerPressureUnit::L_Per_cmH2O);
  GetPulmonaryElastance().SetValue(1.0 / 0.1, PressurePerVolumeUnit::cmH2O_Per_L);

  GetTotalRespiratoryModelCompliance().SetValue(0.1, VolumePerPressureUnit::L_Per_cmH2O);
  GetTotalRespiratoryModelResistance().SetValue(1.5, PressureTimePerVolumeUnit::cmH2O_s_Per_L);

  // Muscle Pressure Waveform
  m_InspiratoryRiseFraction = 0;
  m_InspiratoryHoldFraction = 0;
  m_InspiratoryReleaseFraction = 0;
  m_InspiratoryToExpiratoryPauseFraction = 0;
  m_ExpiratoryRiseFraction = 0;
  m_ExpiratoryHoldFraction = 0;
  m_ExpiratoryReleaseFraction = 0;

  // Disease State
  m_leftAlveoliDecrease_L = 0;
  m_rightAlveoliDecrease_L = 0;

  //Conscious Respiration
  m_ActiveConsciousRespirationCommand = false;

  //Overrides
  m_RespiratoryResistanceOverride_cmH2O_s_Per_L = -1.0;
  m_RespiratoryComplianceOverride_L_Per_cmH2O = -1.0;

  if (m_data.HasCardiovascular())
  {
    m_ArterialO2PartialPressure_mmHg = m_AortaO2->GetPartialPressure(PressureUnit::mmHg);
    m_ArterialCO2PartialPressure_mmHg = m_AortaCO2->GetPartialPressure(PressureUnit::mmHg);
  }

  //Get the substances to a good starting point
  TuneCircuit();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes parameters for Respiratory Class
///
/// \details
/// Called during both State loading and Patient Stabilization
/// Pull and setup up our data (can be from other systems)
/// Initialize will be called after this and can overwrite any of this data (only if stabilizing)
//--------------------------------------------------------------------------------------------------
void Respiratory::SetUp()
{
  //Patient
  m_PatientActions = &m_data.GetActions().GetPatientActions();
  //Driver
  m_MaxDriverPressure_cmH2O = -50.0;
  //Configuration parameters
  m_DefaultOpenResistance_cmH2O_s_Per_L = m_data.GetConfiguration().GetDefaultOpenFlowResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  m_DefaultClosedResistance_cmH2O_s_Per_L = m_data.GetConfiguration().GetDefaultClosedFlowResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  m_RespOpenResistance_cmH2O_s_Per_L = m_data.GetConfiguration().GetRespiratoryOpenResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  m_RespClosedResistance_cmH2O_s_Per_L = m_data.GetConfiguration().GetRespiratoryClosedResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  m_PeripheralControlGainConstant = m_data.GetConfiguration().GetPeripheralVentilatoryControllerGain();
  m_CentralControlGainConstant = m_data.GetConfiguration().GetCentralVentilatoryControllerGain();
  m_VentilationTidalVolumeIntercept = m_data.GetConfiguration().GetVentilationTidalVolumeIntercept(VolumeUnit::L);
  m_VentilatoryOcclusionPressure_cmH2O = m_data.GetConfiguration().GetVentilatoryOcclusionPressure(PressureUnit::cmH2O); //This increases the absolute max driver pressure
  m_MinimumAllowableTidalVolume_L = m_data.GetConfiguration().GetMinimumAllowableTidalVolume(VolumeUnit::L);
  m_MinimumAllowableInpiratoryAndExpiratoryPeriod_s = m_data.GetConfiguration().GetMinimumAllowableInpiratoryAndExpiratoryPeriod(TimeUnit::s);
  //Compartments
  m_Environment = m_data.GetCompartments().GetGasCompartment(pulse::EnvironmentCompartment::Ambient);
  m_AerosolAirway = m_data.GetCompartments().GetLiquidCompartment(pulse::PulmonaryCompartment::Airway);
  m_AerosolCarina = m_data.GetCompartments().GetLiquidCompartment(pulse::PulmonaryCompartment::Carina);
  m_AerosolLeftAnatomicDeadSpace = m_data.GetCompartments().GetLiquidCompartment(pulse::PulmonaryCompartment::LeftAnatomicDeadSpace);
  m_AerosolLeftAlveolarDeadSpace = m_data.GetCompartments().GetLiquidCompartment(pulse::PulmonaryCompartment::LeftAlveolarDeadSpace);
  m_AerosolLeftAlveoli = m_data.GetCompartments().GetLiquidCompartment(pulse::PulmonaryCompartment::LeftAlveoli);
  m_AerosolRightAnatomicDeadSpace = m_data.GetCompartments().GetLiquidCompartment(pulse::PulmonaryCompartment::RightAnatomicDeadSpace);
  m_AerosolRightAlveolarDeadSpace = m_data.GetCompartments().GetLiquidCompartment(pulse::PulmonaryCompartment::RightAlveolarDeadSpace);
  m_AerosolRightAlveoli = m_data.GetCompartments().GetLiquidCompartment(pulse::PulmonaryCompartment::RightAlveoli);
  m_Lungs = m_data.GetCompartments().GetGasCompartment(pulse::PulmonaryCompartment::Lungs);
  m_LeftLung = m_data.GetCompartments().GetGasCompartment(pulse::PulmonaryCompartment::LeftLung);
  m_RightLung = m_data.GetCompartments().GetGasCompartment(pulse::PulmonaryCompartment::RightLung);
  m_PleuralCavity = m_data.GetCompartments().GetGasCompartment(pulse::PulmonaryCompartment::PleuralCavity);
  m_LeftLungExtravascular = m_data.GetCompartments().GetLiquidCompartment(pulse::ExtravascularCompartment::LeftLungIntracellular);
  m_RightLungExtravascular = m_data.GetCompartments().GetLiquidCompartment(pulse::ExtravascularCompartment::RightLungIntracellular);
  m_Carina = m_data.GetCompartments().GetGasCompartment(pulse::PulmonaryCompartment::Carina);
  m_CarinaO2 = m_Carina->GetSubstanceQuantity(m_data.GetSubstances().GetO2());
  m_LeftAlveoliO2 = m_data.GetCompartments().GetGasCompartment(pulse::PulmonaryCompartment::LeftAlveoli)->GetSubstanceQuantity(m_data.GetSubstances().GetO2());
  m_RightAlveoliO2 = m_data.GetCompartments().GetGasCompartment(pulse::PulmonaryCompartment::RightAlveoli)->GetSubstanceQuantity(m_data.GetSubstances().GetO2());
  // Compartments we will process aerosol effects on
  m_AerosolEffects.clear();
  m_AerosolEffects.push_back(m_data.GetCompartments().GetLiquidCompartment(pulse::PulmonaryCompartment::Carina));
  m_AerosolEffects.push_back(m_data.GetCompartments().GetLiquidCompartment(pulse::PulmonaryCompartment::LeftAlveoli));
  m_AerosolEffects.push_back(m_data.GetCompartments().GetLiquidCompartment(pulse::PulmonaryCompartment::LeftAnatomicDeadSpace));
  m_AerosolEffects.push_back(m_data.GetCompartments().GetLiquidCompartment(pulse::PulmonaryCompartment::LeftAlveolarDeadSpace));
  m_AerosolEffects.push_back(m_data.GetCompartments().GetLiquidCompartment(pulse::PulmonaryCompartment::RightAlveoli));
  m_AerosolEffects.push_back(m_data.GetCompartments().GetLiquidCompartment(pulse::PulmonaryCompartment::RightAnatomicDeadSpace));
  m_AerosolEffects.push_back(m_data.GetCompartments().GetLiquidCompartment(pulse::PulmonaryCompartment::RightAlveolarDeadSpace));
  //Circuits
  m_RespiratoryCircuit = &m_data.GetCircuits().GetRespiratoryCircuit();
  //Nodes
  m_Airway = m_RespiratoryCircuit->GetNode(pulse::RespiratoryNode::Airway);
  m_LeftAlveoli = m_RespiratoryCircuit->GetNode(pulse::RespiratoryNode::LeftAlveoli);
  m_LeftAnatomicDeadSpace = m_RespiratoryCircuit->GetNode(pulse::RespiratoryNode::LeftAnatomicDeadSpace);
  m_LeftAlveolarDeadSpace = m_RespiratoryCircuit->GetNode(pulse::RespiratoryNode::LeftAlveolarDeadSpace);
  m_LeftPleural = m_RespiratoryCircuit->GetNode(pulse::RespiratoryNode::LeftPleural);
  m_RespiratoryMuscle = m_RespiratoryCircuit->GetNode(pulse::RespiratoryNode::RespiratoryMuscle);
  m_RightAlveoli = m_RespiratoryCircuit->GetNode(pulse::RespiratoryNode::RightAlveoli);
  m_RightAnatomicDeadSpace = m_RespiratoryCircuit->GetNode(pulse::RespiratoryNode::RightAnatomicDeadSpace);
  m_RightAlveolarDeadSpace = m_RespiratoryCircuit->GetNode(pulse::RespiratoryNode::RightAlveolarDeadSpace);
  m_RightPleural = m_RespiratoryCircuit->GetNode(pulse::RespiratoryNode::RightPleural);
  m_Ambient = m_RespiratoryCircuit->GetNode(pulse::EnvironmentNode::Ambient);
  m_Stomach = m_RespiratoryCircuit->GetNode(pulse::RespiratoryNode::Stomach);
  //Paths
  m_CarinaToLeftAnatomicDeadSpace = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::CarinaToLeftAnatomicDeadSpace);
  m_CarinaToRightAnatomicDeadSpace = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::CarinaToRightAnatomicDeadSpace);
  m_LeftAnatomicDeadSpaceToLeftAlveolarDeadSpace = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::LeftAnatomicDeadSpaceToLeftAlveolarDeadSpace);
  m_RightAnatomicDeadSpaceToRightAlveolarDeadSpace = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::RightAnatomicDeadSpaceToRightAlveolarDeadSpace);
  m_LeftAlveolarDeadSpaceToLeftAlveoli = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::LeftAlveolarDeadSpaceToLeftAlveoli);
  m_RightAlveolarDeadSpaceToRightAlveoli = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::RightAlveolarDeadSpaceToRightAlveoli);
  m_RightPleuralToRespiratoryMuscle = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::RightPleuralToRespiratoryMuscle);
  m_LeftPleuralToRespiratoryMuscle = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::LeftPleuralToRespiratoryMuscle);
  m_DriverPressurePath = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::EnvironmentToRespiratoryMuscle);
  m_AirwayToCarina = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::AirwayToCarina);
  m_AirwayToStomach = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::AirwayToStomach);
  m_EnvironmentToLeftChestLeak = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::EnvironmentToLeftChestLeak);
  m_EnvironmentToRightChestLeak = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::EnvironmentToRightChestLeak);
  m_LeftAlveoliLeakToLeftPleural = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::LeftAlveoliLeakToLeftPleural);
  m_RightAlveoliLeakToRightPleural = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::RightAlveoliLeakToRightPleural);
  m_LeftPleuralToEnvironment = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::LeftPleuralToEnvironment);
  m_RightPleuralToEnvironment = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::RightPleuralToEnvironment);
  m_RightAlveoliToRightPleuralConnection = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::RightAlveoliToRightPleuralConnection);
  m_LeftAlveoliToLeftPleuralConnection = m_RespiratoryCircuit->GetPath(pulse::RespiratoryPath::LeftAlveoliToLeftPleuralConnection);
  m_ConnectionToAirway = m_data.GetCircuits().GetRespiratoryAndMechanicalVentilationCircuit().GetPath(pulse::MechanicalVentilationPath::ConnectionToAirway);
  m_GroundToConnection = m_data.GetCircuits().GetRespiratoryAndMechanicalVentilationCircuit().GetPath(pulse::MechanicalVentilationPath::GroundToConnection);

  /// \todo figure out how to modify these resistances without getting the cv circuit - maybe add a parameter, like baroreceptors does
  //Venous Return
  m_RightPulmonaryCapillary = m_data.GetCircuits().GetCardiovascularCircuit().GetPath(pulse::CardiovascularPath::RightPulmonaryCapillariesToRightPulmonaryVeins);
  m_LeftPulmonaryCapillary = m_data.GetCircuits().GetCardiovascularCircuit().GetPath(pulse::CardiovascularPath::LeftPulmonaryCapillariesToLeftPulmonaryVeins);
  //Pulmonary Shunt
  m_LeftPulmonaryArteriesToVeins = m_data.GetCircuits().GetCardiovascularCircuit().GetPath(pulse::CardiovascularPath::LeftPulmonaryArteriesToLeftPulmonaryVeins);
  m_RightPulmonaryArteriesToVeins = m_data.GetCircuits().GetCardiovascularCircuit().GetPath(pulse::CardiovascularPath::RightPulmonaryArteriesToRightPulmonaryVeins);

  //Mechanical Ventilation Compartments
  m_MechanicalVentilationConnection = m_data.GetCompartments().GetGasCompartment(pulse::MechanicalVentilationCompartment::Connection);
  m_MechanicalVentilationAerosolConnection = m_data.GetCompartments().GetLiquidCompartment(pulse::MechanicalVentilationCompartment::Connection);

  if (m_data.HasCardiovascular())
  {
    SELiquidCompartment* Aorta = m_data.GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::Aorta);
    m_AortaO2 = Aorta->GetSubstanceQuantity(m_data.GetSubstances().GetO2());
    m_AortaCO2 = Aorta->GetSubstanceQuantity(m_data.GetSubstances().GetCO2());
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Respiratory system at steady state
///
/// \details
/// Initializes respiratory conditions if any are present.
///  <UL>
///   <LI>COPD</LI>
///   <LI>Lobar Pneumonia</LI>
///   <LI>ImpairedAlveolarExchange</LI>
///   <LI>Pulmonary Fibrosis< / LI>
///  </UL>
///
//--------------------------------------------------------------------------------------------------
void Respiratory::AtSteadyState()
{
  /// \todo figure out how to save an initial healthy/baseline patient definition and another current patient definition
  //Experimentally determined  
  double tidalVolumeBaseline_L = GetTidalVolume(VolumeUnit::L);
  //Calculated
  double totalLungCapacity_L = m_data.GetCurrentPatient().GetTotalLungCapacity(VolumeUnit::L);
  double functionalResidualCapacity_L = m_data.GetCurrentPatient().GetFunctionalResidualCapacity(VolumeUnit::L);
  double inspiratoryReserveVolume_L = totalLungCapacity_L - functionalResidualCapacity_L - tidalVolumeBaseline_L;

  std::string typeString;
  if (m_data.GetState() == EngineState::AtInitialStableState)
  {
    typeString = "Initial Stabilization Homeostasis: ";
    //At Resting State
    //Note: Respiratory conditions are applied each timestep to handle combined effects properly

    //Update healthy patient volumes
    //Tidal Volume is experimentatlly determined and is dependent on other patient settings
    //Inspiratory Reserve Volume is calcualted from Tidal Volume
    m_data.GetCurrentPatient().GetTidalVolumeBaseline().SetValue(tidalVolumeBaseline_L, VolumeUnit::L);
    m_data.GetCurrentPatient().GetInspiratoryReserveVolume().SetValue(inspiratoryReserveVolume_L, VolumeUnit::L);
  }
  else if (m_data.GetState() == EngineState::AtSecondaryStableState)
    typeString = "Secondary Stabilization Homeostasis: ";
  else if (m_data.GetState() == EngineState::Active)
    typeString = "Final Stabilization Homeostasis: ";

  std::stringstream ss;
  ss << typeString << "Patient tidal volume = " << tidalVolumeBaseline_L << " L.";
  Info(ss);
  ss << typeString << "Patient inspiratory reserve volume = " << inspiratoryReserveVolume_L << " L.";
  Info(ss);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Respiratory system preprocess function
///
/// \details
/// Calculates the muscle pressure source that drives the respiratory system.
/// Updates airway resistance to account for changes arising from factors 
/// like drugs and respiratory insults and interventions. 
//// Updates the chest wall variable compliance. Handles all respiratory
/// insults and actions.
//--------------------------------------------------------------------------------------------------
void Respiratory::PreProcess()
{
  if (m_data.HasOverride())
  {
    // Look for any known overrides
    for (auto& o : m_data.GetOverrides())
    {
      if (o.name == "RespiratoryResistance")
      {
        if (std::isnan(o.value))
          m_RespiratoryResistanceOverride_cmH2O_s_Per_L = -1;
        else
          m_RespiratoryResistanceOverride_cmH2O_s_Per_L = Convert(o.value, PressureTimePerVolumeUnit::GetCompoundUnit(o.unit), PressureTimePerVolumeUnit::cmH2O_s_Per_L);
      }
      else if (o.name == "RespiratoryCompliance")
      {
        if (std::isnan(o.value))
          m_RespiratoryComplianceOverride_L_Per_cmH2O = -1;
        else
          m_RespiratoryComplianceOverride_L_Per_cmH2O = Convert(o.value, VolumePerPressureUnit::GetCompoundUnit(o.unit), VolumePerPressureUnit::L_Per_cmH2O);
      }
    }
  }

  CalculateWork();
  CalculateFatigue();

  UpdateChestWallCompliances();
  UpdateVolumes();
  UpdateResistances();
  UpdateAlveolarCompliances();
  UpdateInspiratoryExpiratoryRatio();
  UpdateDiffusion();
  if (m_data.HasCardiovascular())
  {
    UpdatePulmonaryCapillary();
    UpdatePulmonaryShunt();
  }

  ProcessAerosolSubstances();
  Pneumothorax();

  MechanicalVentilation();
  SupplementalOxygen();

  if (m_data.HasCardiovascular())
  {
    RespiratoryDriver();
  }

  //Do the overrides
  SetRespiratoryResistance();
  SetRespiratoryCompliance();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Respiratory system process function
///
/// \details
/// Ensures the selection of the respiratory system with or without the anesthesia machine. 
///  Handles the integration of the anesthesia machine to the respiratory system when the anesthesia machine is turned on.
/// The integration of the anesthesia machine to the respiratory system is handled at run time by constructing a combined circuit of 
/// the respiratory and anesthesia machine.  
/// Handles lung volume changes during alveolar gas transfer. 
/// Calculates physiological parameters such as respiration rate, tidal volume and others that belonging to the respiratory system.
//--------------------------------------------------------------------------------------------------
void Respiratory::Process(bool solve_and_transport)
{
  if (solve_and_transport)
  {
    // Respiration circuit changes based on if Anesthesia Machine is on or off
    // When dynamic intercircuit connections work, we can stash off the respiration circuit in a member variable
    SEFluidCircuit& RespirationCircuit = m_data.GetCircuits().GetActiveRespiratoryCircuit();

    // Calc the circuits
    m_Calculator->Process(RespirationCircuit, m_data.GetTimeStep_s());

    //ModifyPleuralVolume();
    SEGasCompartmentGraph& RespirationGraph = m_data.GetCompartments().GetActiveRespiratoryGraph();
    SELiquidCompartmentGraph& AerosolGraph = m_data.GetCompartments().GetActiveAerosolGraph();

    // Transport substances
    m_GasTransporter->Transport(RespirationGraph, m_data.GetTimeStep_s());
    if (m_AerosolAirway->HasSubstanceQuantities())
      m_AerosolTransporter->Transport(AerosolGraph, m_data.GetTimeStep_s());

#ifdef DEBUG
    Debugging(RespirationCircuit);
#endif
  }
  //Update system data
  CalculateVitalSigns();

  ComputeExposedModelParameters();
}
void Respiratory::ComputeExposedModelParameters()
{
  double leftChestWallCompliance_L_Per_cmH2O = m_LeftPleuralToRespiratoryMuscle->GetNextCompliance(VolumePerPressureUnit::L_Per_cmH2O);
  double rightChestWallCompliance_L_Per_cmH2O = m_RightPleuralToRespiratoryMuscle->GetNextCompliance(VolumePerPressureUnit::L_Per_cmH2O);
  double leftLungCompliance_L_Per_cmH2O = m_LeftAlveoliToLeftPleuralConnection->GetNextCompliance(VolumePerPressureUnit::L_Per_cmH2O);
  double rightLungCompliance_L_Per_cmH2O = m_RightAlveoliToRightPleuralConnection->GetNextCompliance(VolumePerPressureUnit::L_Per_cmH2O);
  double leftSideCompliance_L_Per_cmH2O = 1.0 / (1.0 / leftChestWallCompliance_L_Per_cmH2O + 1.0 / leftLungCompliance_L_Per_cmH2O);
  double rightSideCompliance_L_Per_cmH2O = 1.0 / (1.0 / rightChestWallCompliance_L_Per_cmH2O + 1.0 / rightLungCompliance_L_Per_cmH2O);
  double totalCompliance_L_Per_cmH2O = leftSideCompliance_L_Per_cmH2O + rightSideCompliance_L_Per_cmH2O;
  GetTotalRespiratoryModelCompliance().SetValue(totalCompliance_L_Per_cmH2O, VolumePerPressureUnit::L_Per_cmH2O);

  double airwayResistance_cmH2O_s_Per_L = m_AirwayToCarina->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  double leftBronchiResistance_cmH2O_s_Per_L = m_CarinaToLeftAnatomicDeadSpace->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  double rightBronchiResistance_cmH2O_s_Per_L = m_CarinaToRightAnatomicDeadSpace->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  double leftAlveoliDuctResistance_cmH2O_s_Per_L = m_LeftAnatomicDeadSpaceToLeftAlveolarDeadSpace->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  double rightAlveoliDuctResistance_cmH2O_s_Per_L = m_RightAnatomicDeadSpaceToRightAlveolarDeadSpace->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  double leftSideResistance_cmH2O_s_Per_L = leftBronchiResistance_cmH2O_s_Per_L + leftAlveoliDuctResistance_cmH2O_s_Per_L;
  double rightSideResistance_cmH2O_s_Per_L = rightBronchiResistance_cmH2O_s_Per_L + rightAlveoliDuctResistance_cmH2O_s_Per_L;
  double totalDownstreamResistance_cmH2O_s_Per_L = 1.0 / (1.0 / leftSideResistance_cmH2O_s_Per_L + 1.0 / rightSideResistance_cmH2O_s_Per_L);
  double totalResistance_cmH2O_s_Per_L = airwayResistance_cmH2O_s_Per_L + totalDownstreamResistance_cmH2O_s_Per_L;

  GetTotalRespiratoryModelResistance().SetValue(totalResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Respiratory system postprocess function
///
/// \details
/// Updates the current values of the gas volume fraction and gas volumes for the nodes in the respiratory circuit 
/// or the nodes in the combined (respiratory + anesthesia machine) circuit when the anesthesia machine is turned on.
//--------------------------------------------------------------------------------------------------
void Respiratory::PostProcess(bool solve_and_transport)
{
  if (solve_and_transport)
  {
    // Respiration circuit changes based on if Anesthesia Machine is on or off
    // When dynamic intercircuit connections work, we can stash off the respiration circuit in a member variable
    SEFluidCircuit& RespirationCircuit = m_data.GetCircuits().GetActiveRespiratoryCircuit();
    m_Calculator->PostProcess(RespirationCircuit);
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Compute deposited mass, update localized PD effects 
///
/// \details
/// For each aerosol get the SIDE coefficient to determine deposited mass in each respiratory compartment.
/// Adjust the resistances between compartments as a function of deposited mass to reach validated data.
/// Liquid and solid aerosols are handeled here. 
//--------------------------------------------------------------------------------------------------
void Respiratory::ProcessAerosolSubstances()
{
  m_AverageLocalTissueBronchodilationEffects = 0.0; //No effect

  size_t numAerosols = m_AerosolAirway->GetSubstanceQuantities().size();
  if (numAerosols == 0)
    return;

  double inflammationCoefficient;

  // For this time step
  double airwayDepositied_ug = 0;
  double carinaDepositied_ug = 0;
  double leftDeadSpaceDepositied_ug = 0;
  double leftAlveoliDepositied_ug = 0;
  double rightDeadSpaceDepositied_ug = 0;
  double rightAlveoliDepositied_ug = 0;

  // Total amount deposited (including this time step)
  double airwayTotalDepositied_ug = 0;
  double carinaTotalDepositied_ug = 0;
  double leftDeadSpaceTotalDepositied_ug = 0;
  double leftAlveoliTotalDepositied_ug = 0;
  double rightDeadSpaceTotalDepositied_ug = 0;
  double rightAlveoliTotalDepositied_ug = 0;

  // Resistance Modifier Sum
  double airwayResistanceModifier=1;
  double carinaResistanceModifier=1;
  double leftDeadSpaceResistanceModifier=1;
  double leftAlveoliResistanceModifier=1;
  double rightDeadSpaceResistanceModifier=1;
  double rightAlveoliResistanceModifier=1;

  // Currently, There is no way to clear out depositied particulate out of the respiratory systems.
  // Maybe we could have it to cough or some other excretion related method... 
  
  SELiquidSubstanceQuantity* subQ;
  SELiquidSubstanceQuantity* tSubQ;
  const SizeIndependentDepositionEfficencyCoefficient* SIDECoeff;
  double combinedRightBronchodilationEffects = 0.0;
  double combinedLeftBronchodilationEffects = 0.0;
  for (size_t i=0; i<numAerosols; i++)
  {
    //initialize substance
    subQ = m_AerosolAirway->GetSubstanceQuantities()[i];
    //Adjust inflammation coefficient (scaled down):
    inflammationCoefficient = subQ->GetSubstance().GetAerosolization().GetInflammationCoefficient().GetValue();// Once for each subQ
    inflammationCoefficient *= 0.01;
    //Airway
    SIDECoeff = &m_data.GetSubstances().GetSizeIndependentDepositionEfficencyCoefficient(subQ->GetSubstance());// Once for each subQ
    airwayDepositied_ug = subQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL)*m_AerosolAirway->GetInFlow(VolumePerTimeUnit::mL_Per_s)*m_data.GetTimeStep_s()*SIDECoeff->GetAirway();
    if (airwayDepositied_ug > subQ->GetMass(MassUnit::ug))
    {
      airwayDepositied_ug = subQ->GetMass(MassUnit::ug);
      subQ->GetMass().SetValue(0, MassUnit::ug);
    }
    else
      subQ->GetMass().IncrementValue(-airwayDepositied_ug, MassUnit::ug);
    subQ->Balance(BalanceLiquidBy::Mass);
    airwayTotalDepositied_ug = subQ->GetMassDeposited().IncrementValue(airwayDepositied_ug, MassUnit::ug);
    airwayResistanceModifier += airwayTotalDepositied_ug*inflammationCoefficient;
    //Carina
    subQ = m_AerosolCarina->GetSubstanceQuantities()[i];
    carinaDepositied_ug = subQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL)*m_AerosolCarina->GetInFlow(VolumePerTimeUnit::mL_Per_s)*m_data.GetTimeStep_s()*SIDECoeff->GetCarina();
    if (carinaDepositied_ug > subQ->GetMass(MassUnit::ug))
    {
      carinaDepositied_ug = subQ->GetMass(MassUnit::ug);
      subQ->GetMass().SetValue(0, MassUnit::ug);
    }
    else
      subQ->GetMass().IncrementValue(-carinaDepositied_ug, MassUnit::ug);
    subQ->Balance(BalanceLiquidBy::Mass);
    carinaTotalDepositied_ug = subQ->GetMassDeposited().IncrementValue(carinaDepositied_ug, MassUnit::ug);
    carinaResistanceModifier += carinaTotalDepositied_ug*inflammationCoefficient;
    //Left DeadSpace
    subQ = m_AerosolLeftAnatomicDeadSpace->GetSubstanceQuantities()[i];
    leftDeadSpaceDepositied_ug = subQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL)*m_AerosolLeftAnatomicDeadSpace->GetInFlow(VolumePerTimeUnit::mL_Per_s)*m_data.GetTimeStep_s()*SIDECoeff->GetDeadSpace();
    if (leftDeadSpaceDepositied_ug > subQ->GetMass(MassUnit::ug))
    {
      leftDeadSpaceDepositied_ug = subQ->GetMass(MassUnit::ug);
      subQ->GetMass().SetValue(0, MassUnit::ug);
    }
    else
      subQ->GetMass().IncrementValue(-leftDeadSpaceDepositied_ug, MassUnit::ug);
    subQ->Balance(BalanceLiquidBy::Mass);
    leftDeadSpaceTotalDepositied_ug = subQ->GetMassDeposited().IncrementValue(leftDeadSpaceDepositied_ug, MassUnit::ug);
    leftDeadSpaceResistanceModifier += leftDeadSpaceTotalDepositied_ug*inflammationCoefficient;
    //Left Alveoli
    subQ = m_AerosolLeftAlveoli->GetSubstanceQuantities()[i];
    leftAlveoliDepositied_ug = subQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL)*m_AerosolLeftAlveoli->GetInFlow(VolumePerTimeUnit::mL_Per_s)*m_data.GetTimeStep_s()*SIDECoeff->GetAlveoli();
    if (leftAlveoliDepositied_ug > subQ->GetMass(MassUnit::ug))
    {
      leftAlveoliDepositied_ug = subQ->GetMass(MassUnit::ug);
      subQ->GetMass().SetValue(0, MassUnit::ug);
    }
    else
      subQ->GetMass().IncrementValue(-leftAlveoliDepositied_ug, MassUnit::ug);
    subQ->Balance(BalanceLiquidBy::Mass);
    leftAlveoliTotalDepositied_ug = subQ->GetMassDeposited().IncrementValue(leftAlveoliDepositied_ug, MassUnit::ug);
    leftAlveoliResistanceModifier += leftAlveoliTotalDepositied_ug*inflammationCoefficient;
    //Right DeadSpace
    subQ = m_AerosolRightAnatomicDeadSpace->GetSubstanceQuantities()[i];
    rightDeadSpaceDepositied_ug = subQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL)*m_AerosolRightAnatomicDeadSpace->GetInFlow(VolumePerTimeUnit::mL_Per_s)*m_data.GetTimeStep_s()*SIDECoeff->GetDeadSpace();
    if (rightDeadSpaceDepositied_ug > subQ->GetMass(MassUnit::ug))
    {
      rightDeadSpaceDepositied_ug = subQ->GetMass(MassUnit::ug);
      subQ->GetMass().SetValue(0, MassUnit::ug);
    }
    else
      subQ->GetMass().IncrementValue(-rightDeadSpaceDepositied_ug, MassUnit::ug);
    subQ->Balance(BalanceLiquidBy::Mass);
    rightDeadSpaceTotalDepositied_ug = subQ->GetMassDeposited().IncrementValue(rightDeadSpaceDepositied_ug, MassUnit::ug);
    rightDeadSpaceResistanceModifier += rightDeadSpaceTotalDepositied_ug*inflammationCoefficient;
    //Right Alveoli
    subQ = m_AerosolRightAlveoli->GetSubstanceQuantities()[i];
    rightAlveoliDepositied_ug = subQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL)*m_AerosolRightAlveoli->GetInFlow(VolumePerTimeUnit::mL_Per_s)*m_data.GetTimeStep_s()*SIDECoeff->GetAlveoli();
    if (rightAlveoliDepositied_ug > subQ->GetMass(MassUnit::ug))
    {
      rightAlveoliDepositied_ug = subQ->GetMass(MassUnit::ug);
      subQ->GetMass().SetValue(0, MassUnit::ug);
    }
    else
      subQ->GetMass().IncrementValue(-rightAlveoliDepositied_ug, MassUnit::ug);
    subQ->Balance(BalanceLiquidBy::Mass);
    rightAlveoliTotalDepositied_ug = subQ->GetMassDeposited().IncrementValue(rightAlveoliDepositied_ug, MassUnit::ug);
    rightAlveoliResistanceModifier += rightAlveoliTotalDepositied_ug*inflammationCoefficient;
    
    // Apply the BronchileModifier dilation effect
    // This is all just tuned to Albuterol - it'll work for other substances, and can be tuned using the other parameters (especially BronchioleModifier)
    if (subQ->GetSubstance().GetState() == eSubstance_State::Liquid)
    {
      // Sum the Bronchiole Effects
      // Must be positive
      double bronchioleModifier = subQ->GetSubstance().GetAerosolization().GetBronchioleModifier().GetValue();

      // Diffuse into Tissues
      // We only process mass deposited on the lungs (dead space and alveoli)
      // We do not currently do anything with the mass in the airway and carina
      // Could possibly let it go into the stomach somehow... 
      tSubQ = m_LeftLungExtravascular->GetSubstanceQuantity(subQ->GetSubstance());
      tSubQ->GetMass().IncrementValue(leftDeadSpaceDepositied_ug + leftAlveoliDepositied_ug, MassUnit::ug); tSubQ->Balance(BalanceLiquidBy::Mass);    
      combinedLeftBronchodilationEffects += bronchioleModifier * tSubQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL);

      tSubQ = m_RightLungExtravascular->GetSubstanceQuantity(subQ->GetSubstance());
      tSubQ->GetMass().IncrementValue(rightDeadSpaceDepositied_ug + rightAlveoliDepositied_ug, MassUnit::ug); tSubQ->Balance(BalanceLiquidBy::Mass);
      combinedRightBronchodilationEffects += bronchioleModifier * tSubQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL);
    }
  }

  //We're going to make the bronchodilation effects be based off of Albuterol.
  //Experimentally, 1mg of Albuterol given via a spacer device on an endotracheal tube caused a pulmonary resistance change of ~20% @cite mancebo1991effects.
  //The bronchi are ~30% of the total pulmonary resistance, so we'll make a dilation effect really strong with a normal dose.
  //This was tuned using a standard inhaler dose of 90ug
  m_AverageLocalTissueBronchodilationEffects = (combinedLeftBronchodilationEffects + combinedRightBronchodilationEffects) / 2.0;
  combinedLeftBronchodilationEffects = exp(-32894.0 * combinedLeftBronchodilationEffects);
  combinedRightBronchodilationEffects = exp(-32894.0 * combinedRightBronchodilationEffects);  

  // Change resistances due to deposition
  double dTracheaResistance = m_AirwayToCarina->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L)*carinaResistanceModifier;
  double dLeftAlveoliResistance = m_LeftAnatomicDeadSpaceToLeftAlveolarDeadSpace->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L)*leftAlveoliResistanceModifier;
  double dRightAlveoliResistance = m_RightAnatomicDeadSpaceToRightAlveolarDeadSpace->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L)*rightAlveoliResistanceModifier;

  double dLeftBronchiResistance = m_CarinaToLeftAnatomicDeadSpace->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  double dRightBronchiResistance = m_CarinaToRightAnatomicDeadSpace->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  dLeftBronchiResistance *= leftDeadSpaceResistanceModifier * combinedLeftBronchodilationEffects;
  dRightBronchiResistance *= rightDeadSpaceResistanceModifier * combinedRightBronchodilationEffects;
  dLeftBronchiResistance = LIMIT(dLeftBronchiResistance, m_RespClosedResistance_cmH2O_s_Per_L, m_RespOpenResistance_cmH2O_s_Per_L);
  dRightBronchiResistance = LIMIT(dRightBronchiResistance, m_RespClosedResistance_cmH2O_s_Per_L, m_RespOpenResistance_cmH2O_s_Per_L);

  m_AirwayToCarina->GetNextResistance().SetValue(dTracheaResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  m_CarinaToLeftAnatomicDeadSpace->GetNextResistance().SetValue(dLeftBronchiResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  m_CarinaToRightAnatomicDeadSpace->GetNextResistance().SetValue(dRightBronchiResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  m_LeftAnatomicDeadSpaceToLeftAlveolarDeadSpace->GetNextResistance().SetValue(dLeftAlveoliResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  m_RightAnatomicDeadSpaceToRightAlveolarDeadSpace->GetNextResistance().SetValue(dRightAlveoliResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Modifies the pressure and/or flow at the airway
///
/// \details
/// Handles the mechanical ventilation action that adds a flow and pressure source to instantaneously
/// set the respiratory connection (airway) to user specified values.  
//--------------------------------------------------------------------------------------------------
void Respiratory::MechanicalVentilation()
{
  if (m_data.GetActions().GetPatientActions().HasMechanicalVentilation())
  {
    SEMechanicalVentilation& mv = m_data.GetActions().GetPatientActions().GetMechanicalVentilation();
    // You only get here if action is On
    m_data.SetAirwayMode(eAirwayMode::MechanicalVentilation);

    //Set the substance volume fractions ********************************************
    std::vector<SESubstanceFraction*> gasFractions = mv.GetGasFractions();

    //Reset the substance quantities at the connection
    for (SEGasSubstanceQuantity* subQ : m_MechanicalVentilationConnection->GetSubstanceQuantities())
      subQ->SetToZero();

    //If no gas fractions specified, assume ambient
    if (gasFractions.empty())
    {
      for (auto s : m_Environment->GetSubstanceQuantities())
      {
        m_MechanicalVentilationConnection->GetSubstanceQuantity(s->GetSubstance())->GetVolumeFraction().Set(s->GetVolumeFraction());
      }
    }
    else
    {
      //Has fractions defined
      for (auto f : gasFractions)
      {
        const SESubstance& sub = f->GetSubstance();
        double fraction = f->GetFractionAmount().GetValue();

        //Do this, just in case it's something new
        m_data.GetSubstances().AddActiveSubstance(sub);

        //Now set it on the connection compartment
        //It has a NaN volume, so this will keep the same volume fraction no matter what's going on around it
        m_MechanicalVentilationConnection->GetSubstanceQuantity(sub)->GetVolumeFraction().SetValue(fraction);
      }
    }

    //Set the aerosol concentrations ********************************************
    std::vector<SESubstanceConcentration*> liquidConcentrations = mv.GetAerosols();

    //Reset the substance quantities at the connection
    for (SELiquidSubstanceQuantity* subQ : m_MechanicalVentilationAerosolConnection->GetSubstanceQuantities())
      subQ->SetToZero();

    if (!liquidConcentrations.empty())
    {
      //Has fractions defined
      for (auto f : liquidConcentrations)
      {
        const SESubstance& sub = f->GetSubstance();
        SEScalarMassPerVolume concentration = f->GetConcentration();

        //Do this, just in case it's something new
        m_data.GetSubstances().AddActiveSubstance(sub);

        //Now set it on the connection compartment
        //It has a NaN volume, so this will keep the same volume fraction no matter what's going on around it
        m_MechanicalVentilationAerosolConnection->GetSubstanceQuantity(sub)->GetConcentration().Set(concentration);
      }
    }

    //Apply the instantaneous flow ********************************************
    if (mv.HasFlow())
    {
      m_ConnectionToAirway->GetNextFlowSource().Set(mv.GetFlow());
      //It may or may not be there
      if (!m_ConnectionToAirway->HasFlowSource())
      {
        m_data.GetCircuits().GetRespiratoryAndMechanicalVentilationCircuit().StateChange();
      }
    }
    else
    {
      //If there's no flow specified, we need to remove the flow source    
      if (m_ConnectionToAirway->HasNextFlowSource())
      {
        m_ConnectionToAirway->GetNextFlowSource().Invalidate();
        m_data.GetCircuits().GetRespiratoryAndMechanicalVentilationCircuit().StateChange();
      }
    }

    //Apply the instantaneous pressure ********************************************  
    if (mv.HasPressure())
    {
      //This is the pressure above ambient
      m_GroundToConnection->GetNextPressureSource().Set(mv.GetPressure());
    }
    else
    {
      //Pressure is same as ambient
      m_GroundToConnection->GetNextPressureSource().SetValue(0.0, PressureUnit::cmH2O);
    }
  }
  else if (m_data.GetAirwayMode() == eAirwayMode::MechanicalVentilation)
  {
    // Was just turned off
    m_data.SetAirwayMode(eAirwayMode::Free);
    if (m_ConnectionToAirway->HasNextFlowSource())
    {
      m_ConnectionToAirway->GetNextFlowSource().Invalidate();
      m_data.GetCircuits().GetRespiratoryAndMechanicalVentilationCircuit().StateChange();
    }
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Applys supplemental oxygen equipment
///
/// \details
/// This will provide supplemental oxygen by connecting and updating the circuit and graph for nasal
/// cannula, simple mask, or nonrebreather mask
//--------------------------------------------------------------------------------------------------
void Respiratory::SupplementalOxygen()
{
  ///\todo - Maybe this and mechanical ventilation should be broken out to their own class, like anesthesia machine?

  if (!m_data.GetActions().GetPatientActions().HasSupplementalOxygen())
    return;
  SESupplementalOxygen& so = m_data.GetActions().GetPatientActions().GetSupplementalOxygen();

  // Get flow
  double flow_L_Per_min = 0;
  if(so.HasFlow())
    flow_L_Per_min = so.GetFlow(VolumePerTimeUnit::L_Per_min);
  //Get tank pressure node and flow control resistor path
  SEFluidCircuitPath* Tank = nullptr;
  SEFluidCircuitPath* OxygenInlet = nullptr;
  SEFluidCircuit* RespirationCircuit = nullptr;
  switch (so.GetDevice())
  {
    case eSupplementalOxygen_Device::None:
    {
      m_data.SetAirwayMode(eAirwayMode::Free);
      m_data.GetActions().GetPatientActions().RemoveSupplementalOxygen();
      return;
    }
    case eSupplementalOxygen_Device::NasalCannula:
    {
      // This implementation can replace other Supplemental O2 modes
      if (m_data.GetAirwayMode() == eAirwayMode::SimpleMask ||
          m_data.GetAirwayMode() == eAirwayMode::NonRebreatherMask)
        m_data.SetAirwayMode(eAirwayMode::Free);

      m_data.SetAirwayMode(eAirwayMode::NasalCannula);
      if (!so.HasFlow())
      {
        flow_L_Per_min = 3.5;
        so.GetFlow().SetValue(flow_L_Per_min, VolumePerTimeUnit::L_Per_min);
        Info("Supplemental oxygen flow not set. Using default value of " + cdm::to_string(flow_L_Per_min) + " L/min.");
      }
      RespirationCircuit = &m_data.GetCircuits().GetActiveRespiratoryCircuit();
      OxygenInlet = RespirationCircuit->GetPath(pulse::NasalCannulaPath::NasalCannulaOxygenInlet);
      Tank = RespirationCircuit->GetPath(pulse::NasalCannulaPath::NasalCannulaPressure);
      break;
    }
    case eSupplementalOxygen_Device::NonRebreatherMask:
    {
      // This implementation can replace other Supplemental O2 modes
      if (m_data.GetAirwayMode() == eAirwayMode::SimpleMask ||
          m_data.GetAirwayMode() == eAirwayMode::NasalCannula)
        m_data.SetAirwayMode(eAirwayMode::Free);
      m_data.SetAirwayMode(eAirwayMode::NonRebreatherMask);

      if (!so.HasFlow())
      {
        flow_L_Per_min = 10.0;
        so.GetFlow().SetValue(flow_L_Per_min, VolumePerTimeUnit::L_Per_min);
        Info("Supplemental oxygen flow not set. Using default value of " + cdm::to_string(flow_L_Per_min) + " L/min.");
      }
      RespirationCircuit = &m_data.GetCircuits().GetActiveRespiratoryCircuit();
      OxygenInlet = RespirationCircuit->GetPath(pulse::NonRebreatherMaskPath::NonRebreatherMaskOxygenInlet);
      Tank = RespirationCircuit->GetPath(pulse::NonRebreatherMaskPath::NonRebreatherMaskPressure);
      break;
    }
    case eSupplementalOxygen_Device::SimpleMask:
    {
      // This implementation can replace other Supplemental O2 modes
      if (m_data.GetAirwayMode() == eAirwayMode::NasalCannula ||
          m_data.GetAirwayMode() == eAirwayMode::NonRebreatherMask)
        m_data.SetAirwayMode(eAirwayMode::Free);
      m_data.SetAirwayMode(eAirwayMode::SimpleMask);

      if (!so.HasFlow())
      {
        flow_L_Per_min = 7.5;
        so.GetFlow().SetValue(flow_L_Per_min, VolumePerTimeUnit::L_Per_min);
        Info("Supplemental oxygen flow not set. Using default value of " + cdm::to_string(flow_L_Per_min) + " L/min.");
      }
      RespirationCircuit = &m_data.GetCircuits().GetActiveRespiratoryCircuit();
      OxygenInlet = RespirationCircuit->GetPath(pulse::SimpleMaskPath::SimpleMaskOxygenInlet);
      Tank = RespirationCircuit->GetPath(pulse::SimpleMaskPath::SimpleMaskPressure);
      break;
    }
    default:
    {
      Error("Ignoring unsupported supplemental oxygen type : " + eSupplementalOxygen_Device_Name(so.GetDevice()));
      return;
    }
  }

  //Use a default tank volume if it wasn't explicity set
  if (!so.HasVolume())
  {
    so.GetVolume().SetValue(425.0, VolumeUnit::L);
    Info("Supplemental oxygen initial tank volume not set. Using default value of 425 L.");
  }

  //Decrement volume from the tank
  //Inf volume is assumed to be a wall connection that will never run out
  if (so.GetVolume(VolumeUnit::L) != std::numeric_limits<double>::infinity())
  {
    so.GetVolume().IncrementValue(-flow_L_Per_min * m_data.GetTimeStep_s()/60, VolumeUnit::L);
    //Check if the tank is depleated
    if (so.GetVolume(VolumeUnit::L) <= 0.0)
    {
      so.GetVolume().SetValue(0.0, VolumeUnit::L);
      flow_L_Per_min = 0.0;
      /// \event Supplemental Oxygen: Oxygen bottle is exhausted. There is no longer any oxygen to provide.
      m_data.GetEvents().SetEvent(eEvent::SupplementalOxygenBottleExhausted, true, m_data.GetSimulationTime());
    }
    else
      m_data.GetEvents().SetEvent(eEvent::SupplementalOxygenBottleExhausted, false, m_data.GetSimulationTime());
  }
  else
    m_data.GetEvents().SetEvent(eEvent::SupplementalOxygenBottleExhausted, false, m_data.GetSimulationTime());

  //Nonrebreather mask works differently with the bag and doesn't have a pressure source for the tank
  if (so.GetDevice() != eSupplementalOxygen_Device::NonRebreatherMask)
  {
    //Determine flow control resistance
    //Assume pressure outside tank is comparatively approximately ambient
    double tankPressure_cmH2O = Tank->GetNextPressureSource(PressureUnit::cmH2O);
    double resistance_cmH2O_s_Per_L = tankPressure_cmH2O / (flow_L_Per_min / 60.0); //convert from min to s
    resistance_cmH2O_s_Per_L = LIMIT(resistance_cmH2O_s_Per_L, m_DefaultClosedResistance_cmH2O_s_Per_L, m_DefaultOpenResistance_cmH2O_s_Per_L);
    
    //Apply flow
    OxygenInlet->GetNextResistance().SetValue(resistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  }
  else
  {
    //Handle nonrebreather mask bag
    SEFluidCircuitNode* NonRebreatherMaskBag = RespirationCircuit->GetNode(pulse::NonRebreatherMaskNode::NonRebreatherMaskBag);
    SEFluidCircuitPath* NonRebreatherMaskReservoirValve = RespirationCircuit->GetPath(pulse::NonRebreatherMaskPath::NonRebreatherMaskReservoirValve);

    double flowOut_L_Per_min = 0.0;
    if (NonRebreatherMaskReservoirValve->HasNextFlow())
    {
      flowOut_L_Per_min = NonRebreatherMaskReservoirValve->GetNextFlow(VolumePerTimeUnit::L_Per_min);
    }

    double bagVolume_L = NonRebreatherMaskBag->GetNextVolume(VolumeUnit::L);
    bagVolume_L = bagVolume_L + (flow_L_Per_min - flowOut_L_Per_min) * m_data.GetTimeStep_s()/60;
    if (bagVolume_L < 0.0)
    {
      bagVolume_L = 0.0;

      //No air can come from the bag
      OxygenInlet->GetNextResistance().SetValue(m_RespOpenResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);

      /// \event Supplemental Oxygen: The nonrebreather mask is empty. Oxygen may need to be provided at a faster rate.
      m_data.GetEvents().SetEvent(eEvent::NonRebreatherMaskOxygenBagEmpty, true, m_data.GetSimulationTime());
    }
    else
    {
      if (bagVolume_L > 1.0)
      {
        bagVolume_L = 1.0;
      }
      m_data.GetEvents().SetEvent(eEvent::NonRebreatherMaskOxygenBagEmpty, false, m_data.GetSimulationTime());
    }
    
    NonRebreatherMaskBag->GetNextVolume().SetValue(bagVolume_L, VolumeUnit::L);
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Respiratory driver pressure source
///
/// \details
/// Calculates the muscle pressure source pressure by using the chemical stimuli as feedback control mechanism.
/// The method reads the arterial O2 and CO2 partial pressures. Using these partial pressures, the method calculates the
/// alveolar ventilation from which the method calculates the target breathing frequency. The target breathing frequency is
/// used in the equation for muscle pressure source. The muscle pressure source is used as a driver for ventilation.
/// This method also calculates the drug modifiers that adjusts the depth and frequency of respiration.
//--------------------------------------------------------------------------------------------------
void Respiratory::RespiratoryDriver()
{
  /// \event Patient: Start of exhale/inhale
  if (m_data.GetEvents().IsEventActive(eEvent::StartOfExhale))
    m_data.GetEvents().SetEvent(eEvent::StartOfExhale, false, m_data.GetSimulationTime());
  if (m_data.GetEvents().IsEventActive(eEvent::StartOfInhale))
    m_data.GetEvents().SetEvent(eEvent::StartOfInhale, false, m_data.GetSimulationTime());

  //Keep a running average of the Arterial Partial Pressures  
  m_ArterialO2RunningAverage_mmHg->Sample(m_AortaO2->GetPartialPressure(PressureUnit::mmHg));
  m_ArterialCO2RunningAverage_mmHg->Sample(m_AortaCO2->GetPartialPressure(PressureUnit::mmHg));
  //Reset at start of cardiac cycle 
  if (m_data.GetEvents().IsEventActive(eEvent::StartOfCardiacCycle))
  {
    m_ArterialO2PartialPressure_mmHg = m_ArterialO2RunningAverage_mmHg->Value();
    m_ArterialCO2PartialPressure_mmHg = m_ArterialCO2RunningAverage_mmHg->Value();

    m_ArterialO2RunningAverage_mmHg->Clear();
    m_ArterialCO2RunningAverage_mmHg->Clear();
  }

#ifdef TUNING
  m_ArterialO2PartialPressure_mmHg = 95.0;
  m_ArterialCO2PartialPressure_mmHg = 40.0;
#endif  

  double TotalBreathingCycleTime_s = 0.0;
  if (m_VentilationFrequency_Per_min > 1.0 || m_ActiveConsciousRespirationCommand)
  {
    TotalBreathingCycleTime_s = 60.0 / m_VentilationFrequency_Per_min; //Total time of one breathing cycle  
  }

  //Prepare for the next cycle -------------------------------------------------------------------------------
  if ((m_BreathingCycleTime_s > TotalBreathingCycleTime_s) ||                              //End of the cycle or currently not breathing
    (m_PatientActions->HasConsciousRespiration() && !m_ActiveConsciousRespirationCommand)) //Or new consious respiration command to start immediately
  {
    m_BreathingCycleTime_s = 0.0;

    if (m_PatientActions->HasConsciousRespiration())
    {
      m_ActiveConsciousRespirationCommand = true;
      ConsciousRespiration();
    }
    else //Spontaneous breathing
    {
      m_ActiveConsciousRespirationCommand = false;

      // Make a cardicArrestEffect that is 1.0 unless cardiac arrest is true
      double cardiacArrestEffect = 1.0;
      // If the cv system parameter is true, then make the cardicArrestEffect = 0
      if (m_data.GetEvents().IsEventActive(eEvent::CardiacArrest))
      {
        cardiacArrestEffect = 0.0;
      }

      //Ventilatory Negative Feedback Control *************************************************************************
      double PeripheralCO2PartialPressureSetPoint = m_data.GetConfiguration().GetPeripheralControllerCO2PressureSetPoint(PressureUnit::mmHg);
      double CentralCO2PartialPressureSetPoint = m_data.GetConfiguration().GetCentralControllerCO2PressureSetPoint(PressureUnit::mmHg);

      double metabolicModifier = 1.0;
      double TMR_W = m_data.GetEnergy().GetTotalMetabolicRate(PowerUnit::W);
      double BMR_W = m_data.GetCurrentPatient().GetBasalMetabolicRate(PowerUnit::W);
      double metabolicFraction = TMR_W / BMR_W;

      //Get Drug Effects
      SEDrugSystem& Drugs = m_data.GetDrugs();
      double DrugRRChange_Per_min = Drugs.GetRespirationRateChange(FrequencyUnit::Per_min);
      double DrugsTVChange_L = Drugs.GetTidalVolumeChange(VolumeUnit::L);
      double NMBModifier = 1.0;
      double SedationModifier = 1.0;
      //Make changes to Respiration Rate change based on the neuromuscular block level
      if (Drugs.GetNeuromuscularBlockLevel().GetValue() > 0.135)
      {
        NMBModifier = 0.0;
        DrugRRChange_Per_min = 0.0;
        DrugsTVChange_L = 0.0;
      }
      else if (Drugs.GetNeuromuscularBlockLevel().GetValue() > 0.11)
      {
        NMBModifier = 0.5;
        DrugRRChange_Per_min = 0.0;
        DrugsTVChange_L = 0.0;
      }
      else if (Drugs.GetSedationLevel().GetValue() > 0.15 && std::abs(m_data.GetCurrentPatient().GetRespirationRateBaseline(FrequencyUnit::Per_min) + DrugRRChange_Per_min) < 1.0)
      {
        SedationModifier = 0.0;
        DrugRRChange_Per_min = 0.0;
        DrugsTVChange_L = 0.0;
      }

      //Calculate the target Alveolar Ventilation based on the Arterial O2 and CO2 concentrations
      double dTargetAlveolarVentilation_L_Per_min = m_PeripheralControlGainConstant * exp(-0.05 * m_ArterialO2PartialPressure_mmHg) * MAX(0., m_ArterialCO2PartialPressure_mmHg - PeripheralCO2PartialPressureSetPoint); //Peripheral portion
      dTargetAlveolarVentilation_L_Per_min += m_CentralControlGainConstant * MAX(0., m_ArterialCO2PartialPressure_mmHg - CentralCO2PartialPressureSetPoint); //Central portion

      //Metabolic modifier is used to drive the system to reasonable levels achievable during increased metabolic exertion
      //The modifier is tuned to achieve the correct respiratory response for near maximal exercise. A linear relationship is assumed
      // for the respiratory effects due to increased metabolic exertion    
      double tunedVolumeMetabolicSlope = 0.2; //Tuned fractional increase of the tidal volume due to increased metabolic rate
      metabolicModifier = 1.0 + tunedVolumeMetabolicSlope * (metabolicFraction - 1.0);
      dTargetAlveolarVentilation_L_Per_min *= metabolicModifier;

      //Only move it part way to where it wants to be.
      //If it stays there, it will get there, just more controlled/slowly.
      //This is needed so we don't overshoot and introduce low frequency oscillations into the system (i.e. overdamped).
      double targetCO2PP_mmHg = 40.0;
      double changeFraction = 1.0;
      if (m_data.GetState() <= EngineState::InitialStabilization)
      {
        //This gets it nice and stable
        changeFraction = 0.1;
      }
      else
      {
        //This keeps it from going crazy with modifiers applied
        changeFraction = std::abs(m_ArterialCO2PartialPressure_mmHg - targetCO2PP_mmHg) / targetCO2PP_mmHg * 0.5;
      }
      changeFraction = MIN(changeFraction, 1.0);

#ifdef TUNING
      changeFraction = 1.0;
#endif

      dTargetAlveolarVentilation_L_Per_min = m_PreviousTargetAlveolarVentilation_L_Per_min + (dTargetAlveolarVentilation_L_Per_min - m_PreviousTargetAlveolarVentilation_L_Per_min) * changeFraction;
      m_PreviousTargetAlveolarVentilation_L_Per_min = dTargetAlveolarVentilation_L_Per_min;

      //Target Tidal Volume (i.e. Driver amplitude) *************************************************************************
      //Calculate the target Tidal Volume based on the Alveolar Ventilation
      double dTargetPulmonaryVentilation_L_Per_min = dTargetAlveolarVentilation_L_Per_min;

      double dMaximumPulmonaryVentilationRate = m_data.GetConfiguration().GetPulmonaryVentilationRateMaximum(VolumePerTimeUnit::L_Per_min);
      /// \event Patient: Maximum Pulmonary Ventilation Rate : Pulmonary ventilation exceeds maximum value
      if (dTargetPulmonaryVentilation_L_Per_min > dMaximumPulmonaryVentilationRate)
      {
        dTargetPulmonaryVentilation_L_Per_min = dMaximumPulmonaryVentilationRate;
        m_data.GetEvents().SetEvent(eEvent::MaximumPulmonaryVentilationRate, true, m_data.GetSimulationTime());
      }

      if (dTargetPulmonaryVentilation_L_Per_min < dMaximumPulmonaryVentilationRate && m_data.GetEvents().IsEventActive(eEvent::MaximumPulmonaryVentilationRate))
      {
        m_data.GetEvents().SetEvent(eEvent::MaximumPulmonaryVentilationRate, false, m_data.GetSimulationTime());
      }

      //Calculate the target Tidal Volume based on the calculated target pulmonary ventilation, plot slope (determined during initialization), and x-intercept
      double dTargetTidalVolume_L = dTargetPulmonaryVentilation_L_Per_min / m_VentilationToTidalVolumeSlope + m_VentilationTidalVolumeIntercept;

      //Modify the target tidal volume due to other external effects - probably eventually replaced by the Nervous system
      dTargetTidalVolume_L *= cardiacArrestEffect * NMBModifier;

      //Apply Drug Effects to the target tidal volume
      dTargetTidalVolume_L += DrugsTVChange_L;

      //This is a piecewise function that plateaus at the Tidal Volume equal to 1/2 * Vital Capacity
      //The Respiration Rate will make up for the Alveoli Ventilation difference
      double dHalfVitalCapacity_L = m_data.GetInitialPatient().GetVitalCapacity(VolumeUnit::L) / 2;
      dTargetTidalVolume_L = MIN(dTargetTidalVolume_L, dHalfVitalCapacity_L);

      //Map the Target Tidal Volume to the Driver
      double TargetVolume_L = m_data.GetInitialPatient().GetFunctionalResidualCapacity(VolumeUnit::L) + dTargetTidalVolume_L;
      m_PeakInspiratoryPressure_cmH2O = VolumeToDriverPressure(TargetVolume_L);
      //There's a maximum force the driver can try to achieve
      m_PeakInspiratoryPressure_cmH2O = MAX(m_PeakInspiratoryPressure_cmH2O, m_MaxDriverPressure_cmH2O);

      //This is always zero, unless there's conscious respiration
      m_PeakExpiratoryPressure_cmH2O = 0.0;

      //Respiration Rate (i.e. Driver frequency) *************************************************************************
      //Calculate the Respiration Rate given the Alveolar Ventilation and the Target Tidal Volume
      if (SEScalar::IsZero(dTargetTidalVolume_L, ZERO_APPROX)) //Can't divide by zero
      {
        m_VentilationFrequency_Per_min = 0.0;
        m_NotBreathing = true;
      }
      else
      {
        m_VentilationFrequency_Per_min = dTargetPulmonaryVentilation_L_Per_min / dTargetTidalVolume_L; //breaths/min
        m_VentilationFrequency_Per_min *= NMBModifier * SedationModifier;
        m_VentilationFrequency_Per_min += DrugRRChange_Per_min;
        m_NotBreathing = false;
      }

      m_VentilationFrequency_Per_min = LIMIT(m_VentilationFrequency_Per_min, 0.0, dMaximumPulmonaryVentilationRate / dHalfVitalCapacity_L);

      //Patient Definition *************************************************************************
      //We need to hit the patient's defined Respiration Rate Baseline, no matter what,
      //so we'll keep adjusting the slope of the function to achieve this
      if (m_data.GetState() <= EngineState::InitialStabilization)
      {
        double dRespirationRateBaseline_Per_min = m_data.GetCurrentPatient().GetRespirationRateBaseline(FrequencyUnit::Per_min);
        double dPercentError = (m_VentilationFrequency_Per_min - dRespirationRateBaseline_Per_min) / dRespirationRateBaseline_Per_min; //negative if too low

        //Amplitude set-point - this will set the Tidal Volume baseline when O2 and CO2 are at the correct/balanced level
        m_VentilationToTidalVolumeSlope = m_VentilationToTidalVolumeSlope - m_VentilationToTidalVolumeSlope * dPercentError;

        //Put bounds on this
        m_VentilationToTidalVolumeSlope = LIMIT(m_VentilationToTidalVolumeSlope, 1.0, 100.0);
      }

#ifdef TUNING
      m_VentilationToTidalVolumeSlope = 40.0;
      m_VentilationFrequency_Per_min = 16.0;
#endif

      SetBreathCycleFractions();
    }
  }

  //Run the driver based on the waveform -------------------------------------------------------------------------------
  TotalBreathingCycleTime_s = 60.0 / m_VentilationFrequency_Per_min; //Recalculate, in case we just calculated a new breath

  double InspiratoryRiseTimeStart_s = 0.0;
  double InspiratoryHoldTimeStart_s = InspiratoryRiseTimeStart_s + m_InspiratoryRiseFraction * TotalBreathingCycleTime_s;
  double InspiratoryReleaseTimeStart_s = InspiratoryHoldTimeStart_s + m_InspiratoryHoldFraction * TotalBreathingCycleTime_s;
  double InspiratoryToExpiratoryPauseTimeStart_s = InspiratoryReleaseTimeStart_s + m_InspiratoryReleaseFraction * TotalBreathingCycleTime_s;
  double ExpiratoryRiseTimeStart_s = InspiratoryToExpiratoryPauseTimeStart_s + m_InspiratoryToExpiratoryPauseFraction * TotalBreathingCycleTime_s;
  double ExpiratoryHoldTimeStart_s = ExpiratoryRiseTimeStart_s + m_ExpiratoryRiseFraction * TotalBreathingCycleTime_s;
  double ExpiratoryReleaseTimeStart_s = ExpiratoryHoldTimeStart_s + m_ExpiratoryHoldFraction * TotalBreathingCycleTime_s;
  double ResidueFractionTimeStart_s = ExpiratoryReleaseTimeStart_s + m_ExpiratoryReleaseFraction * TotalBreathingCycleTime_s;

  if (SEScalar::IsZero(m_BreathingCycleTime_s, ZERO_APPROX) &&
    m_InspiratoryRiseFraction != 0.0) //Only call this once per cycle - needed here for conscious respiration
  {
    m_data.GetEvents().SetEvent(eEvent::StartOfInhale, true, m_data.GetSimulationTime());
  }  

  if (m_BreathingCycleTime_s >= InspiratoryReleaseTimeStart_s &&
    m_BreathingCycleTime_s < InspiratoryReleaseTimeStart_s + m_data.GetTimeStep_s()) //Only call this once per cycle
  {
    m_data.GetEvents().SetEvent(eEvent::StartOfExhale, true, m_data.GetSimulationTime());
  }

  double pi = 3.14159265359;
  if (m_BreathingCycleTime_s >= ResidueFractionTimeStart_s)
  {
    m_DriverPressure_cmH2O = 0.0;
  }
  else if (m_BreathingCycleTime_s >= ExpiratoryReleaseTimeStart_s)
  {
    double segmentTime_s = ResidueFractionTimeStart_s - ExpiratoryReleaseTimeStart_s;
    m_DriverPressure_cmH2O = m_PeakExpiratoryPressure_cmH2O * sin(pi / 2.0 * (m_BreathingCycleTime_s + segmentTime_s - ExpiratoryReleaseTimeStart_s) / segmentTime_s);
  }
  else if (m_BreathingCycleTime_s >= ExpiratoryHoldTimeStart_s)
  {
    m_DriverPressure_cmH2O = m_PeakExpiratoryPressure_cmH2O;
  }
  else if (m_BreathingCycleTime_s >= ExpiratoryRiseTimeStart_s)
  {
    double segmentTime_s = ExpiratoryHoldTimeStart_s - ExpiratoryRiseTimeStart_s;
    m_DriverPressure_cmH2O = m_PeakExpiratoryPressure_cmH2O * sin(pi / 2.0 * (m_BreathingCycleTime_s - ExpiratoryRiseTimeStart_s) / segmentTime_s);
  }
  else if (m_BreathingCycleTime_s >= InspiratoryToExpiratoryPauseTimeStart_s)
  {
    m_DriverPressure_cmH2O = 0.0;
  }
  else if (m_BreathingCycleTime_s >= InspiratoryReleaseTimeStart_s)
  {
    double segmentTime_s = InspiratoryToExpiratoryPauseTimeStart_s - InspiratoryReleaseTimeStart_s;
    m_DriverPressure_cmH2O = m_PeakInspiratoryPressure_cmH2O * sin(pi / 2.0 * (m_BreathingCycleTime_s + segmentTime_s - InspiratoryReleaseTimeStart_s) / segmentTime_s);
  }
  else if (m_BreathingCycleTime_s >= InspiratoryHoldTimeStart_s)
  {
    m_DriverPressure_cmH2O = m_PeakInspiratoryPressure_cmH2O;
  }
  else //(m_BreathingCycleTime_s >= InspiratoryRiseTimeStart_s)
  {
    m_DriverPressure_cmH2O = m_PeakInspiratoryPressure_cmH2O * sin(pi / 2.0 * m_BreathingCycleTime_s / InspiratoryHoldTimeStart_s);
  }

  if (!m_PatientActions->HasConsciousRespiration())
  {
    ModifyDriverPressure();

    if (m_NotBreathing)
    {
      m_DriverPressure_cmH2O = 0.0;
    }
  }

  //Push Driving Data to the Circuit -------------------------------------------------------------------------------
  m_DriverPressurePath->GetNextPressureSource().SetValue(m_DriverPressure_cmH2O, PressureUnit::cmH2O);

  //We need to do this here to allow for the inhaler to get called before the next go-around
  m_BreathingCycleTime_s += m_data.GetTimeStep_s();
  if (m_BreathingCycleTime_s > TotalBreathingCycleTime_s) //End of the cycle or currently not breathing
  {
    if (m_ActiveConsciousRespirationCommand)
    {
      SEConsciousRespiration& cr = m_PatientActions->GetConsciousRespiration();
      SEConsciousRespirationCommand* cmd = cr.GetActiveCommand();
      m_ss << "Completed Conscious Respiration Command : " << *cmd;
      Info(m_ss);
      cr.RemoveActiveCommand();
    }
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Set inspiratory and expiratory timing
///
/// \details
/// This sets the fraction/percentage of each segment of a breath cycle. The values are used by the
/// driver to produce the pressure waveform for a single breath. The fractions must sum to 1.0, and 
/// anything remaining will be added at the end as a hold at zero.
//--------------------------------------------------------------------------------------------------
void Respiratory::SetBreathCycleFractions()
{
  m_IERatioScaleFactor *= 0.94; //Tuning factor determined from healthy validation

  //Healthy = ~1:2 IE Ratio = 0.33 inpiration and 0.67 expiration
  ///\cite Fresnel2014musclePressure
  //Adjust for standard 12 bpm giving ~0.33 instead of 16 bpm by adding 4  
  double inspiratoryFraction = (0.0125 * (m_VentilationFrequency_Per_min + 4.0) + 0.125) * m_IERatioScaleFactor;
  inspiratoryFraction = LIMIT(inspiratoryFraction, 0.1, 0.9);
  double expiratoryFraction = 1.0 - inspiratoryFraction;

  m_InspiratoryRiseFraction = inspiratoryFraction;
  m_InspiratoryHoldFraction = 0.0;
  m_InspiratoryReleaseFraction = MIN(inspiratoryFraction, expiratoryFraction * 0.5);
  m_InspiratoryToExpiratoryPauseFraction = 1.0 - m_InspiratoryRiseFraction - m_InspiratoryReleaseFraction;
  m_ExpiratoryRiseFraction = 0.0;
  m_ExpiratoryHoldFraction = 0.0;
  m_ExpiratoryReleaseFraction = 0.0;
 }

//--------------------------------------------------------------------------------------------------
/// \brief
/// Tension pneumothorax
///
/// \details
/// Pneumothorax is an abnormal accumulation of air in the pleural cavity. Tension pneumothorax occurs when 
/// air escapes into the pleural space on inspiration, but cannot return on expiration. Different types of pneumothorces are known.
/// The engine models  open (sucking chest wound) and closed (intact thoracic cage) pnumothoraces whereas occlusive dressing
/// and needle decompression are modeled as interventions. 
/// The function models pneumothorax and needle decompression by opening respiratory circuits that allow air flows into 
/// and out of the pleural cavity. Under normal condition, the path between the lung node or the outside environment and the intrapleural cavity 
/// is blocked via significantly high resistances that serve as open switches. When open or closed pneumothorax is triggered, these resistances 
/// are reduced in proportion to the severity of the pneumothorax. This reduction permits air flow into the pleural cavity a
/// and cause respiratory and cardiovascular distress.
/// The function allows the selection of tension pneumothorax and/or needle decompression events on the left or right lung side. The effects on 
/// the cardiovascular system parameters is accounted through modifiers that scale in proportion to the accumulated gas. 
//--------------------------------------------------------------------------------------------------
void Respiratory::Pneumothorax()
{
  if (m_PatientActions->HasTensionPneumothorax())
  {
    // Minimum flow resistance for the chest cavity or alveoli leak 
    double PneumoMinResistance_cmH2O_s_Per_L = 50.0;
    // Maximum flow resistance for the chest cavity or alveoli leak
    double PneumoMaxResistance_cmH2O_s_Per_L = m_DefaultOpenResistance_cmH2O_s_Per_L;
    // Flow resistance for the decompression needle, if used
    double NeedleResistance_cmH2O_s_Per_L = 100.0;

    if (m_PatientActions->HasLeftOpenTensionPneumothorax())
    {
      // Scale the flow resistance through the chest opening based on severity
      double severity = m_PatientActions->GetLeftOpenTensionPneumothorax().GetSeverity().GetValue();
      double resistance_cmH2O_s_Per_L = PneumoMaxResistance_cmH2O_s_Per_L;
      if (severity > 0.0 && !m_PatientActions->HasLeftChestOcclusiveDressing())
      {
        resistance_cmH2O_s_Per_L = PneumoMinResistance_cmH2O_s_Per_L / pow(severity, 2.0);
      }
      resistance_cmH2O_s_Per_L = MIN(resistance_cmH2O_s_Per_L, PneumoMaxResistance_cmH2O_s_Per_L);
      m_EnvironmentToLeftChestLeak->GetNextResistance().SetValue(resistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);

      if (m_PatientActions->HasLeftNeedleDecompression())
      {
        DoLeftNeedleDecompression(NeedleResistance_cmH2O_s_Per_L);
      }
    }

    if (m_PatientActions->HasRightOpenTensionPneumothorax())
    {
      // Scale the flow resistance through the chest opening based on severity
      double severity = m_PatientActions->GetRightOpenTensionPneumothorax().GetSeverity().GetValue();
      double resistance_cmH2O_s_Per_L = PneumoMaxResistance_cmH2O_s_Per_L;
      if (severity > 0.0 && !m_PatientActions->HasRightChestOcclusiveDressing())
      {
        resistance_cmH2O_s_Per_L = PneumoMinResistance_cmH2O_s_Per_L / pow(severity, 2.0);
      }
      resistance_cmH2O_s_Per_L = MIN(resistance_cmH2O_s_Per_L, PneumoMaxResistance_cmH2O_s_Per_L);
      m_EnvironmentToRightChestLeak->GetNextResistance().SetValue(resistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);

      if (m_PatientActions->HasRightNeedleDecompression())
      {
        DoRightNeedleDecompression(NeedleResistance_cmH2O_s_Per_L);
      }
      m_EnvironmentToRightChestLeak->GetNextResistance().SetValue(resistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
    }

    if (m_PatientActions->HasLeftClosedTensionPneumothorax())
    {
      // Scale the flow resistance through the chest opening based on severity
      double severity = m_PatientActions->GetLeftClosedTensionPneumothorax().GetSeverity().GetValue();
      double resistance_cmH2O_s_Per_L = PneumoMaxResistance_cmH2O_s_Per_L;
      if (severity > 0.0)
      {
        resistance_cmH2O_s_Per_L = PneumoMinResistance_cmH2O_s_Per_L / pow(severity, 2.0);
      }
      resistance_cmH2O_s_Per_L = MIN(resistance_cmH2O_s_Per_L, PneumoMaxResistance_cmH2O_s_Per_L);
      m_LeftAlveoliLeakToLeftPleural->GetNextResistance().SetValue(resistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);

      if (m_PatientActions->HasLeftNeedleDecompression())
      {
        DoLeftNeedleDecompression(NeedleResistance_cmH2O_s_Per_L);
      }
    }

    if (m_PatientActions->HasRightClosedTensionPneumothorax())
    {
      // Scale the flow resistance through the chest opening based on severity
      double severity = m_PatientActions->GetRightClosedTensionPneumothorax().GetSeverity().GetValue();
      double resistance_cmH2O_s_Per_L = PneumoMaxResistance_cmH2O_s_Per_L;
      if (severity > 0.0)
      {
        resistance_cmH2O_s_Per_L = PneumoMinResistance_cmH2O_s_Per_L / pow(severity, 2.0);
      }
      resistance_cmH2O_s_Per_L = MIN(resistance_cmH2O_s_Per_L, PneumoMaxResistance_cmH2O_s_Per_L);
      m_RightAlveoliLeakToRightPleural->GetNextResistance().SetValue(resistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);

      if (m_PatientActions->HasRightNeedleDecompression())
      {
        DoRightNeedleDecompression(NeedleResistance_cmH2O_s_Per_L);
      }
    }

    //Check for interventions without insult
    if (!m_PatientActions->HasLeftClosedTensionPneumothorax() && !m_PatientActions->HasLeftOpenTensionPneumothorax())
    {
      if (m_PatientActions->HasLeftChestOcclusiveDressing() || m_PatientActions->HasLeftNeedleDecompression())
      {
        /// \error Patient: Cannot perform an intervention if Tension Pneumothorax is not present on that side.
        Error("Cannot perform an intervention if Tension Pneumothorax is not present on that side.");
        m_PatientActions->RemoveLeftChestOcclusiveDressing();
        m_PatientActions->RemoveLeftNeedleDecompression();
        return;
      }
    }
    if (!m_PatientActions->HasRightClosedTensionPneumothorax() && !m_PatientActions->HasRightOpenTensionPneumothorax())
    {
      if (m_PatientActions->HasRightChestOcclusiveDressing() || m_PatientActions->HasRightNeedleDecompression())
      {
        /// \error Patient: Cannot perform an intervention if Tension Pneumothorax is not present on that side.
        Error("Cannot perform an intervention if Tension Pneumothorax is not present on that side.");
        m_PatientActions->RemoveRightChestOcclusiveDressing();
        m_PatientActions->RemoveRightNeedleDecompression();
        return;
      }
    }
  }
  else
  {
    // Check for cases where the needle decompression or occlusive dressing actions are called without having
    // initiated a pneumothorax action
    if (m_PatientActions->HasNeedleDecompression())
    {
      /// \error Patient: can't process needle decompression if no pneumothorax is present
      Error("Cannot perform a Needle Decompression intervention if Tension Pneumothorax is not present");
      m_PatientActions->RemoveLeftNeedleDecompression();
      m_PatientActions->RemoveRightNeedleDecompression();
      return;
    }
    if (m_PatientActions->HasChestOcclusiveDressing())
    {
      /// \error Patient: can't process a chest occlusive dressing if no pneumothorax is present      
      Error("Cannot perform a Chest Occlusive Dressing intervention if Tension Pneumothorax is not present");
      m_PatientActions->RemoveLeftChestOcclusiveDressing();
      m_PatientActions->RemoveRightChestOcclusiveDressing();
      return;
    }
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Left Side Needle Decompression
///
/// \param  dPressureTimePerVolume - Resistance value for air flow through the needle
///
/// \details
/// Used for left side needle decompression. this is an intervention (action) used to treat left 
/// side tension pneumothorax
//--------------------------------------------------------------------------------------------------
void Respiratory::DoLeftNeedleDecompression(double dPressureTimePerVolume)
{
  //Leak flow resistance that is scaled in proportion to Lung resistance, depending on severity
  double dScalingFactor = 0.5; //Tuning parameter to allow gas flow due to needle decompression using lung resistance as reference
  double dPressureTimePerVolumeLeftNeedle = dScalingFactor * dPressureTimePerVolume;
  m_LeftPleuralToEnvironment->GetNextResistance().SetValue(dPressureTimePerVolumeLeftNeedle, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Right Side Needle Decompression
///
/// \param  dPressureTimePerVolume - Resistance value for air flow through the needle
///
/// \details
/// Used for right side needle decompression. this is an intervention (action) used to treat right
/// side tension pneumothorax
//--------------------------------------------------------------------------------------------------
void Respiratory::DoRightNeedleDecompression(double dPressureTimePerVolume)
{
  //Leak flow resistance that is scaled in proportion to Lung resistance, depending on severity
  double dScalingFactor = 0.5; //Tuning parameter to allow gas flow due to needle decompression using lung resistance as reference
  double dPressureTimePerVolumeRightNeedle = dScalingFactor * dPressureTimePerVolume;
  m_RightPleuralToEnvironment->GetNextResistance().SetValue(dPressureTimePerVolumeRightNeedle, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Conscious respiration
///
/// \param  None
///
/// \return void
///
/// \details
/// This method processes a conscious respiration command and removes by setting the effort waveform
/// parameters. New commands will overwrite incomplete commands.
//--------------------------------------------------------------------------------------------------
void Respiratory::ConsciousRespiration()
{
  SEConsciousRespiration& cr = m_PatientActions->GetConsciousRespiration();

  m_InspiratoryRiseFraction = 0.0;
  m_InspiratoryHoldFraction = 0.0;
  m_InspiratoryReleaseFraction = 0.0;
  m_InspiratoryToExpiratoryPauseFraction = 0.0;
  m_ExpiratoryRiseFraction = 0.0;
  m_ExpiratoryHoldFraction = 0.0;
  m_ExpiratoryReleaseFraction = 0.0;

  SEConsciousRespirationCommand* cmd = cr.GetActiveCommand();

  SEForcedInhale* fi = dynamic_cast<SEForcedInhale*>(cmd);
  if (fi != nullptr)
  {
    double pressureFraction = 0.0;
    double risePeriod_s = 0.0;
    double holdPeriod_s = 0.0;
    double releasePeriod_s = 0.0;

    if (fi->HasInspiratoryCapacityFraction())
    {
      pressureFraction = fi->GetInspiratoryCapacityFraction().GetValue();
    }

    if (fi->HasInhalePeriod())
    {
      risePeriod_s = fi->GetInhalePeriod().GetValue(TimeUnit::s);
    }
    if (fi->HasHoldPeriod())
    {
      holdPeriod_s = fi->GetHoldPeriod().GetValue(TimeUnit::s);
    }
    if (fi->HasReleasePeriod())
    {
      releasePeriod_s = fi->GetReleasePeriod().GetValue(TimeUnit::s);
    }

    double TargetVolume_L = m_data.GetInitialPatient().GetFunctionalResidualCapacity(VolumeUnit::L) + m_data.GetInitialPatient().GetInspiratoryCapacity(VolumeUnit::L) * pressureFraction;
    m_PeakInspiratoryPressure_cmH2O = VolumeToDriverPressure(TargetVolume_L);

    double totalPeriod = risePeriod_s + holdPeriod_s + releasePeriod_s;

    if (SEScalar::IsZero(totalPeriod, ZERO_APPROX))
    {
      m_VentilationFrequency_Per_min = 0.0;
      m_InspiratoryRiseFraction = 0.0;
      m_InspiratoryHoldFraction = 0.0;
      m_InspiratoryReleaseFraction = 0.0;
    }
    else
    {
      m_VentilationFrequency_Per_min = 60.0 / totalPeriod;
      m_InspiratoryRiseFraction = risePeriod_s / totalPeriod;
      m_InspiratoryHoldFraction = holdPeriod_s / totalPeriod;
      m_InspiratoryReleaseFraction = releasePeriod_s / totalPeriod;
    }    

    return;
  }

  SEForcedExhale* fe = dynamic_cast<SEForcedExhale*>(cmd);
  if (fe != nullptr)
  {
    double pressureFraction = 0.0;
    double risePeriod_s = 0.0;
    double holdPeriod_s = 0.0;
    double releasePeriod_s = 0.0;

    if (fe->HasExpiratoryReserveVolumeFraction())
    {
      pressureFraction = fe->GetExpiratoryReserveVolumeFraction().GetValue();
    }

    if (fe->HasExhalePeriod())
    {
      risePeriod_s = fe->GetExhalePeriod().GetValue(TimeUnit::s);
    }
    if (fe->HasHoldPeriod())
    {
      holdPeriod_s = fe->GetHoldPeriod().GetValue(TimeUnit::s);
    }
    if (fe->HasReleasePeriod())
    {
      releasePeriod_s = fe->GetReleasePeriod().GetValue(TimeUnit::s);
    }

    double TargetVolume_L = m_data.GetInitialPatient().GetResidualVolume(VolumeUnit::L) + m_data.GetInitialPatient().GetExpiratoryReserveVolume(VolumeUnit::L) * (1.0 - pressureFraction);

    m_PeakExpiratoryPressure_cmH2O = VolumeToDriverPressure(TargetVolume_L);

    double totalPeriod = risePeriod_s + holdPeriod_s + releasePeriod_s;
    if (SEScalar::IsZero(totalPeriod, ZERO_APPROX))
    {
      m_VentilationFrequency_Per_min = 0.0;
      m_ExpiratoryRiseFraction = 0.0;
      m_ExpiratoryHoldFraction = 0.0;
      m_ExpiratoryReleaseFraction = 0.0;
    }
    else
    {
      m_VentilationFrequency_Per_min = 60.0 / totalPeriod;
      m_ExpiratoryRiseFraction = risePeriod_s / totalPeriod;
      m_ExpiratoryHoldFraction = holdPeriod_s / totalPeriod;
      m_ExpiratoryReleaseFraction = releasePeriod_s / totalPeriod;
    }

    return;
  }

  SEForcedPause* fp = dynamic_cast<SEForcedPause*>(cmd);
  if (fp != nullptr)
  {
    double period_s = 0.0;
    if (fp->HasPeriod())
    {
      period_s = fp->GetPeriod().GetValue(TimeUnit::s);
    }

    m_VentilationFrequency_Per_min = (SEScalar::IsZero(period_s, ZERO_APPROX)) ? 0.0 : 60.0 / period_s;
    m_InspiratoryToExpiratoryPauseFraction = 1.0;

    return;
  } 
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculates key respiratory physiological parameters
///
/// \details
/// Calculates the respiration rate and tidal volume. For each breathing cycle, the tidal volume is calculated by identifying 
/// the peaks in the total lung volume.
/// The tidal volume of a particular breathing cycle is then calculated by taking the difference between the 
/// maximum and minimum values of the total lung volume. 
/// The respiration rate is calculated by measuring the duration for one complete breathing cycle and then 
/// converting the duration to the number of breaths per minute.
// The method also checks for bradypnea and tachypnea events and reports to the use.
//--------------------------------------------------------------------------------------------------
void Respiratory::CalculateVitalSigns()
{
  //Record values each time-step
  std::stringstream ss;

  //Total Respiratory Volume - this should not include the Pleural Space
  double totalLungVolume_L = m_Lungs->GetVolume(VolumeUnit::L);
  GetTotalLungVolume().SetValue(totalLungVolume_L, VolumeUnit::L);

  double AnatomicDeadSpace_L = m_LeftAnatomicDeadSpace->GetNextVolume(VolumeUnit::L) + m_RightAnatomicDeadSpace->GetNextVolume(VolumeUnit::L);
  double AlveolarDeadSpace_L = m_LeftAlveolarDeadSpace->GetNextVolume(VolumeUnit::L) + m_RightAlveolarDeadSpace->GetNextVolume(VolumeUnit::L);
  GetAnatomicDeadSpace().SetValue(AnatomicDeadSpace_L, VolumeUnit::L);  
  GetAlveolarDeadSpace().SetValue(AlveolarDeadSpace_L, VolumeUnit::L);
  GetPhysiologicDeadSpace().SetValue(AnatomicDeadSpace_L + AlveolarDeadSpace_L, VolumeUnit::L);

  double tracheaFlow_L_Per_s = m_AirwayToCarina->GetNextFlow().GetValue(VolumePerTimeUnit::L_Per_s);
  double previousInspiratoryFlow_L_Per_s = GetInspiratoryFlow(VolumePerTimeUnit::L_Per_s);
  GetInspiratoryFlow().SetValue(tracheaFlow_L_Per_s, VolumePerTimeUnit::L_Per_s);
  GetExpiratoryFlow().SetValue(-tracheaFlow_L_Per_s, VolumePerTimeUnit::L_Per_s);

  /// \cite kacmarek2016egan page 227-228 ---------------------------------------------------
  double airwayOpeningPressure_cmH2O = m_Airway->GetPressure(PressureUnit::cmH2O);
  double alveolarPressure_cmH2O = (m_LeftAlveoli->GetNextPressure().GetValue(PressureUnit::cmH2O) + m_RightAlveoli->GetNextPressure().GetValue(PressureUnit::cmH2O)) / 2.0; //Average of L and R
  double pleuralPressure_cmH2O = (m_LeftPleural->GetNextPressure().GetValue(PressureUnit::cmH2O) + m_RightPleural->GetNextPressure().GetValue(PressureUnit::cmH2O)) / 2.0; //Average of L and R
  double musclePressure_cmH2O = m_RespiratoryMuscle->GetNextPressure(PressureUnit::cmH2O);
  double bodySurfacePressure_cmH2O = m_Ambient->GetPressure(PressureUnit::cmH2O);

  double transrespiratoryPressure_cmH2O = airwayOpeningPressure_cmH2O - bodySurfacePressure_cmH2O;
  double transairwayPressure_cmH2O = airwayOpeningPressure_cmH2O - alveolarPressure_cmH2O;
  double transpulmonaryPressure_cmH2O = airwayOpeningPressure_cmH2O - pleuralPressure_cmH2O;
  double transalveolarPressure_cmH2O = alveolarPressure_cmH2O - pleuralPressure_cmH2O;
  double transthoracicPressure_cmH2O = alveolarPressure_cmH2O - bodySurfacePressure_cmH2O;
  double transChestWallPressure_cmH2O = pleuralPressure_cmH2O - bodySurfacePressure_cmH2O;
  double transMusclePressure_cmH2O = musclePressure_cmH2O - bodySurfacePressure_cmH2O;

  GetRespiratoryMusclePressure().SetValue(musclePressure_cmH2O, PressureUnit::cmH2O);
  GetIntrapleuralPressure().SetValue(transChestWallPressure_cmH2O, PressureUnit::cmH2O);
  GetIntrapulmonaryPressure().SetValue(-transMusclePressure_cmH2O, PressureUnit::cmH2O);
  GetTransrespiratoryPressure().SetValue(transrespiratoryPressure_cmH2O, PressureUnit::cmH2O);
  GetTransairwayPressure().SetValue(transairwayPressure_cmH2O, PressureUnit::cmH2O);
  GetTranspulmonaryPressure().SetValue(transpulmonaryPressure_cmH2O, PressureUnit::cmH2O);
  GetTransalveolarPressure().SetValue(transalveolarPressure_cmH2O, PressureUnit::cmH2O);
  GetTransthoracicPressure().SetValue(transthoracicPressure_cmH2O, PressureUnit::cmH2O);
  GetTransChestWallPressure().SetValue(transChestWallPressure_cmH2O, PressureUnit::cmH2O);
  GetTransMusclePressure().SetValue(transMusclePressure_cmH2O, PressureUnit::cmH2O);

  if (m_data.HasCardiovascular())
  {
    double averageAlveoliO2PartialPressure_mmHg = (m_LeftAlveoliO2->GetPartialPressure(PressureUnit::mmHg) + m_RightAlveoliO2->GetPartialPressure(PressureUnit::mmHg)) / 2.0;
    GetAlveolarArterialGradient().SetValue(averageAlveoliO2PartialPressure_mmHg - m_AortaO2->GetPartialPressure(PressureUnit::mmHg), PressureUnit::mmHg);
  }
  else
  {
    GetAlveolarArterialGradient().Clear();
  }

  m_MeanAirwayPressure_cmH2O->Sample(airwayOpeningPressure_cmH2O - bodySurfacePressure_cmH2O);

  if (tracheaFlow_L_Per_s > ZERO_APPROX)
  {
    GetInspiratoryPulmonaryResistance().SetValue((airwayOpeningPressure_cmH2O - alveolarPressure_cmH2O) / tracheaFlow_L_Per_s, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  }
  else if (tracheaFlow_L_Per_s < ZERO_APPROX)
  {
    GetExpiratoryPulmonaryResistance().SetValue((airwayOpeningPressure_cmH2O - alveolarPressure_cmH2O) / tracheaFlow_L_Per_s, PressureTimePerVolumeUnit::cmH2O_s_Per_L);    
  }

  //It's a pain to figure out how to hold onto this data, so let's just set it at a sensitive transition point
  if (GetInspiratoryFlow(VolumePerTimeUnit::L_Per_s) > 0.0 //We're inhaling
    && previousInspiratoryFlow_L_Per_s <= 0.0) //We were exhaling
  {
    //Transition from exhale to inhale
    // Set the End Tidal Concentration for our gases at the end of a respiration cycle
    for (SEGasSubstanceQuantity* subQ : m_Carina->GetSubstanceQuantities())
    {
      subQ->GetSubstance().GetEndTidalFraction().Set(subQ->GetVolumeFraction());
      subQ->GetSubstance().GetEndTidalPressure().Set(subQ->GetPartialPressure());
    }
    GetEndTidalCarbonDioxideFraction().Set(m_data.GetSubstances().GetCO2().GetEndTidalFraction());
    GetEndTidalCarbonDioxidePressure().Set(m_data.GetSubstances().GetCO2().GetEndTidalPressure());
    GetEndTidalOxygenFraction().Set(m_data.GetSubstances().GetO2().GetEndTidalFraction());
    GetEndTidalOxygenPressure().Set(m_data.GetSubstances().GetO2().GetEndTidalPressure());
  }

  //Record values at the breathing inflection points (i.e. switch between inhale and exhale)  
  // Temporal tolerance to avoid accidental entry in the the inhalation and exhalation code blocks 
  m_ElapsedBreathingCycleTime_min += m_data.GetTimeStep_s()/60;

  if (m_BreathingCycle) //Exhaling
  {    
    if (totalLungVolume_L <= m_BottomBreathTotalVolume_L)
    {
      m_BottomBreathTotalVolume_L = totalLungVolume_L;
      m_BottomBreathElapsedTime_min = m_ElapsedBreathingCycleTime_min - m_TopBreathElapsedTime_min;
      m_BottomBreathAlveoliVolume_L = m_RightAlveoli->GetNextVolume().GetValue(VolumeUnit::L) + m_LeftAlveoli->GetNextVolume().GetValue(VolumeUnit::L);
      m_BottomBreathPleuralVolume_L = m_RightPleural->GetNextVolume().GetValue(VolumeUnit::L) + m_LeftPleural->GetNextVolume().GetValue(VolumeUnit::L);
      m_BottomBreathPleuralPressure_cmH2O = pleuralPressure_cmH2O;
      m_BottomBreathAlveoliPressure_cmH2O = alveolarPressure_cmH2O;
      m_BottomBreathDriverPressure_cmH2O = m_RespiratoryMuscle->GetNextPressure(PressureUnit::cmH2O);
    }

    if (totalLungVolume_L - m_BottomBreathTotalVolume_L > m_MinimumAllowableTidalVolume_L //Volume has transitioned sufficiently
      && m_BottomBreathElapsedTime_min > m_MinimumAllowableInpiratoryAndExpiratoryPeriod_s / 60.0 / 10.0) //We've waited a sufficient amount of time to transition
    {
      //Transition to inhale

      // Calculate Respiration Rate and track time and update cycle flag
      double expirationTime_min = m_BottomBreathElapsedTime_min;
      double inspirationTime_min = m_TopBreathElapsedTime_min;
      double RespirationRate_Per_min = 1.0 / (expirationTime_min + inspirationTime_min);
      GetRespirationRate().SetValue(RespirationRate_Per_min, FrequencyUnit::Per_min);
      GetInspiratoryExpiratoryRatio().SetValue(inspirationTime_min / expirationTime_min);

      m_ElapsedBreathingCycleTime_min -= (expirationTime_min + inspirationTime_min);
      m_BreathingCycle = false;

      // Calculate the Tidal Volume from the last peak
      double TidalVolume_L = std::abs(m_TopBreathTotalVolume_L - m_BottomBreathTotalVolume_L);
      double AlveoliDeltaVolume_L = std::abs(m_TopBreathAlveoliVolume_L - m_BottomBreathAlveoliVolume_L);
      double PleuralDeltaVolume_L = std::abs(m_TopBreathPleuralVolume_L - m_BottomBreathPleuralVolume_L);
      double PleuralDeltaPressure_cmH2O = std::abs((m_BottomBreathPleuralPressure_cmH2O - m_BottomBreathDriverPressure_cmH2O) - (m_TopBreathPleuralPressure_cmH2O - m_TopBreathDriverPressure_cmH2O));
      double AlveoliDeltaPressure_cmH2O = std::abs((m_BottomBreathAlveoliPressure_cmH2O - m_BottomBreathPleuralPressure_cmH2O) - (m_TopBreathAlveoliPressure_cmH2O - m_TopBreathPleuralPressure_cmH2O));
      double LungDeltaPressure_cmH2O = std::abs((m_BottomBreathAlveoliPressure_cmH2O - m_BottomBreathDriverPressure_cmH2O) - (m_TopBreathAlveoliPressure_cmH2O - m_TopBreathDriverPressure_cmH2O));

      GetTidalVolume().SetValue(TidalVolume_L, VolumeUnit::L);

      double pulmonaryCompiance_L_Per_cmH2O = TidalVolume_L / LungDeltaPressure_cmH2O;
      double lungCompliance_L_Per_cmH2O = AlveoliDeltaVolume_L / AlveoliDeltaPressure_cmH2O;
      double chestWallCompliance_L_Per_cmH2O = PleuralDeltaVolume_L / PleuralDeltaPressure_cmH2O;
      GetPulmonaryCompliance().SetValue(pulmonaryCompiance_L_Per_cmH2O, VolumePerPressureUnit::L_Per_cmH2O);
      GetLungCompliance().SetValue(lungCompliance_L_Per_cmH2O, VolumePerPressureUnit::L_Per_cmH2O);
      GetChestWallCompliance().SetValue(chestWallCompliance_L_Per_cmH2O, VolumePerPressureUnit::L_Per_cmH2O);
      GetPulmonaryElastance().SetValue(1.0 / pulmonaryCompiance_L_Per_cmH2O, PressurePerVolumeUnit::cmH2O_Per_L);

      GetPeakInspiratoryPressure().SetValue(m_PeakAlveolarPressure_cmH2O - bodySurfacePressure_cmH2O, PressureUnit::cmH2O);
      GetPositiveEndExpiratoryPressure().SetValue(m_BottomBreathAlveoliPressure_cmH2O - bodySurfacePressure_cmH2O, PressureUnit::cmH2O);
      GetMaximalInspiratoryPressure().SetValue(m_MaximalAlveolarPressure_cmH2O - bodySurfacePressure_cmH2O, PressureUnit::cmH2O);

      // Calculate Ventilations
      GetTotalPulmonaryVentilation().SetValue(TidalVolume_L * RespirationRate_Per_min, VolumePerTimeUnit::L_Per_min);
      GetSpecificVentilation().SetValue(TidalVolume_L / m_BottomBreathTotalVolume_L);
      GetTotalAlveolarVentilation().SetValue(TidalVolume_L * RespirationRate_Per_min, VolumePerTimeUnit::L_Per_min);
      GetTotalDeadSpaceVentilation().SetValue((AnatomicDeadSpace_L + AlveolarDeadSpace_L) * RespirationRate_Per_min, VolumePerTimeUnit::L_Per_min);

      m_TopBreathTotalVolume_L = totalLungVolume_L;
      m_TopBreathElapsedTime_min = m_ElapsedBreathingCycleTime_min;
      m_PeakAlveolarPressure_cmH2O = m_BottomBreathAlveoliPressure_cmH2O;
      m_MaximalAlveolarPressure_cmH2O = m_BottomBreathAlveoliPressure_cmH2O;

      //We can approximate the mean here, since we got a full waveform
      //It will be off a little when each breath isn't the same
      //It's too hard to keep a runnning average otherwise
      GetMeanAirwayPressure().SetValue(m_MeanAirwayPressure_cmH2O->Value(), PressureUnit::cmH2O);
      m_MeanAirwayPressure_cmH2O->Clear();
    }
  }
  else //Inhaling
  {
    if (totalLungVolume_L >= m_TopBreathTotalVolume_L)
    {
      m_TopBreathTotalVolume_L = totalLungVolume_L;
      m_TopBreathElapsedTime_min = m_ElapsedBreathingCycleTime_min;
      m_TopBreathTotalVolume_L = totalLungVolume_L;
      m_TopBreathAlveoliVolume_L = m_RightAlveoli->GetNextVolume().GetValue(VolumeUnit::L) + m_LeftAlveoli->GetNextVolume().GetValue(VolumeUnit::L);
      m_TopBreathPleuralVolume_L = m_RightPleural->GetNextVolume().GetValue(VolumeUnit::L) + m_LeftPleural->GetNextVolume().GetValue(VolumeUnit::L);
      m_TopBreathPleuralPressure_cmH2O = pleuralPressure_cmH2O;
      m_TopBreathAlveoliPressure_cmH2O = alveolarPressure_cmH2O;
      m_TopBreathDriverPressure_cmH2O = m_RespiratoryMuscle->GetNextPressure(PressureUnit::cmH2O);
      m_TopCarinaO2 = m_CarinaO2->GetVolumeFraction().GetValue();
    }

    m_PeakAlveolarPressure_cmH2O = MAX(m_PeakAlveolarPressure_cmH2O, alveolarPressure_cmH2O);
    m_MaximalAlveolarPressure_cmH2O = MIN(m_MaximalAlveolarPressure_cmH2O, alveolarPressure_cmH2O);

    if (m_TopBreathTotalVolume_L - totalLungVolume_L > m_MinimumAllowableTidalVolume_L //Volume has transitioned sufficiently
      && m_TopBreathElapsedTime_min > m_MinimumAllowableInpiratoryAndExpiratoryPeriod_s / 60.0 / 2.0) //We've waited a sufficient amount of time to transition
    {
      //Transition to exhaling
      m_BreathingCycle = true;

      //We want the peak Carina O2 value - this should be the inspired value
      double FiO2 = m_TopCarinaO2;
      GetFractionOfInsipredOxygen().SetValue(FiO2);
      double meanAirwayPressure_mmHg = Convert(m_MeanAirwayPressure_cmH2O->Value(), PressureUnit::cmH2O, PressureUnit::mmHg);

      if (m_data.HasCardiovascular())
      {
        GetCarricoIndex().SetValue(m_ArterialO2PartialPressure_mmHg / FiO2, PressureUnit::mmHg);
        GetOxygenationIndex().SetValue(FiO2* meanAirwayPressure_mmHg * 100.0 / m_ArterialO2PartialPressure_mmHg);
      }
      else
      {
        GetCarricoIndex().Clear();
        GetOxygenationIndex().Clear();
      }

      if (m_data.HasBloodChemistry())
      {
        GetSaturationAndFractionOfInspiredOxygenRatio().SetValue(m_data.GetBloodChemistry().GetOxygenSaturation().GetValue() / FiO2);
        GetOxygenSaturationIndex().SetValue(FiO2* meanAirwayPressure_mmHg / m_data.GetBloodChemistry().GetOxygenSaturation().GetValue(), PressureUnit::cmH2O);
      }
      else
      {
        GetSaturationAndFractionOfInspiredOxygenRatio().Clear();
        GetOxygenSaturationIndex().Clear();
      }

      m_BottomBreathTotalVolume_L = totalLungVolume_L;
      m_BottomBreathElapsedTime_min = m_ElapsedBreathingCycleTime_min - m_TopBreathElapsedTime_min;
    }
  }

  //Zero out if waiting longer than 15 sec
  if (m_ElapsedBreathingCycleTime_min > 0.25)
  {
    GetRespirationRate().SetValue(0.0, FrequencyUnit::Per_min);
    GetTidalVolume().SetValue(0.0, VolumeUnit::L);
    GetTotalAlveolarVentilation().SetValue(0.0, VolumePerTimeUnit::L_Per_min);
    GetTotalPulmonaryVentilation().SetValue(0.0, VolumePerTimeUnit::L_Per_min);
    GetMeanAirwayPressure().SetValue(m_MeanAirwayPressure_cmH2O->Value(), PressureUnit::cmH2O);
    m_MeanAirwayPressure_cmH2O->Clear();
  }

  if (m_data.GetState() > EngineState::InitialStabilization)
  {// Don't throw events if we are initializing

    //Bradypnea
    if (GetRespirationRate().GetValue(FrequencyUnit::Per_min) < 10)
    {
      /// \event Patient: Bradypnea: Respiration rate is below 10 breaths per minute
      /// The patient has bradypnea.
      m_data.GetEvents().SetEvent(eEvent::Bradypnea, true, m_data.GetSimulationTime());  /// \cite overdyk2007continuous
    }
    else if (GetRespirationRate().GetValue(FrequencyUnit::Per_min) >= 10.5)  // offset by .5 
    {
      /// \event Patient: End Bradypnea Event. The respiration rate has risen above 10. 
      /// The patient is no longer considered to have bradypnea.
      m_data.GetEvents().SetEvent(eEvent::Bradypnea, false, m_data.GetSimulationTime());
    }

    //Tachypnea
    if (GetRespirationRate().GetValue(FrequencyUnit::Per_min) > 20)
    {
      /// \event Patient: Tachypnea: Respiration rate is above 20 breaths per minute.
      /// The patient has tachypnea.
      m_data.GetEvents().SetEvent(eEvent::Tachypnea, true, m_data.GetSimulationTime());  /// \cite 
    }
    else if (GetRespirationRate().GetValue(FrequencyUnit::Per_min) <= 19.5) // offset by .5 
    {
      /// \event Patient: End Tachypnea Event. The respiration rate has fallen below 19.5. 
      /// The patient is no longer considered to have tachypnea.
      m_data.GetEvents().SetEvent(eEvent::Tachypnea, false, m_data.GetSimulationTime());
    }
  }

  if (m_data.HasBloodChemistry() && m_data.HasCardiovascular())
  {
    /// \todo Move to blood chemistry
     // Although it is called respiratory acidosis/alkalosis, the detection and event setting is actually a part of the @ref BloodChemistrySystem
     // The terms "metabolic" and "respiratory" refer to the origin of the acid-base disturbance
     // The hydrogen ion concentration is a property of the blood
     // The events related to blood concentrations should be detected and set in blood chemistry.
     //Keep a running average of the pH
    m_BloodPHRunningAverage->Sample(m_data.GetBloodChemistry().GetBloodPH().GetValue());
    //Reset at start of cardiac cycle 
    if (m_data.GetEvents().IsEventActive(eEvent::StartOfCardiacCycle))
    {
      m_LastCardiacCycleBloodPH = m_BloodPHRunningAverage->Value();
      m_BloodPHRunningAverage->Clear();
    }

    if (m_data.GetState() > EngineState::InitialStabilization)
    {// Don't throw events if we are initializing

      double highPh = 8.5;
      double lowPh = 6.5;   // \cite Edge2006AcidosisConscious
      //// Respiratory Acidosis
      if (m_LastCardiacCycleBloodPH < 7.35 && m_ArterialCO2PartialPressure_mmHg > 47.0)
      {
        /// \event Patient: Respiratory Acidosis: event is triggered when blood pH is below 7.36
        /// The patient has respiratory acidosis.
        m_data.GetEvents().SetEvent(eEvent::RespiratoryAcidosis, true, m_data.GetSimulationTime());

        /// \event Patient: arterial blood ph has dropped below 6.5.
        if (m_LastCardiacCycleBloodPH < lowPh)
        {
          /// \irreversible Extreme respiratory Acidosis: blood pH below 6.5.
          m_data.GetEvents().SetEvent(eEvent::IrreversibleState, true, m_data.GetSimulationTime());
          ss << "Arterial blood pH is  " << m_LastCardiacCycleBloodPH << ". This is below 6.5, Patient is experiencing extreme respiratory Acidosis and is in an irreversible state.";
          Fatal(ss);
        }
      }
      else if (m_LastCardiacCycleBloodPH >= 7.38 && m_ArterialCO2PartialPressure_mmHg < 44.0)
      {
        /// \event Patient: End Respiratory Acidosis Event. The pH value has risen above 7.38. 
        /// The patient is no longer considered to have respiratory acidosis.
        m_data.GetEvents().SetEvent(eEvent::RespiratoryAcidosis, false, m_data.GetSimulationTime());
      }

      ////Respiratory Alkalosis
      if (m_LastCardiacCycleBloodPH > 7.45 && m_ArterialCO2PartialPressure_mmHg < 37.0)
      {
        /// \event Patient: Respiratory Alkalosis: event is triggered when blood pH is above 7.45
        /// The patient has respiratory alkalosis.
        m_data.GetEvents().SetEvent(eEvent::RespiratoryAlkalosis, true, m_data.GetSimulationTime());

        /// \event Patient: arterial blood ph has gotten above 8.5.
        if (m_LastCardiacCycleBloodPH > highPh)
        {
          /// \irreversible Extreme respiratory Alkalosis: blood pH above 8.5.
          m_data.GetEvents().SetEvent(eEvent::IrreversibleState, true, m_data.GetSimulationTime());
          ss << "Arterial blood pH is  " << m_LastCardiacCycleBloodPH << ". This is above 8.5, Patient is experiencing extreme respiratory Alkalosis and is in an irreversible state.";
          Fatal(ss);
        }
      }
      else if (m_LastCardiacCycleBloodPH <= 7.43 && m_ArterialCO2PartialPressure_mmHg > 39.0)
      {
        /// \event Patient: End Respiratory Alkalosis Event. The pH value has has fallen below 7.45. 
        /// The patient is no longer considered to have respiratory alkalosis.
        m_data.GetEvents().SetEvent(eEvent::RespiratoryAlkalosis, false, m_data.GetSimulationTime());
      }
    }
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Peak Driver Pressure Based on Target Volume 
///
/// \param  TargetVolume_L        Target volume (L)
///
/// \return Peak Driver Pressure    The driver pressure required to achieve target volume (cmH2O)
///
/// \details
/// This method returns an estimated driver pressure as a function of target volume. The equation
/// uses a constant total compliance from the basline values. This is an approximation and will
/// not acheive the desired volume exactly.
//--------------------------------------------------------------------------------------------------
double Respiratory::VolumeToDriverPressure(double TargetVolume_L)
{
  //Calculate as if constant compliances
  double leftHealthyLungCompliance_L_Per_cmH2O = m_LeftAlveoliToLeftPleuralConnection->GetComplianceBaseline(VolumePerPressureUnit::L_Per_cmH2O);
  double rightHealthyLungCompliance_L_Per_cmH2O = m_RightAlveoliToRightPleuralConnection->GetComplianceBaseline(VolumePerPressureUnit::L_Per_cmH2O);

  double leftHealthyChestWallCompliance_L_Per_cmH2O = m_LeftPleuralToRespiratoryMuscle->GetComplianceBaseline(VolumePerPressureUnit::L_Per_cmH2O);
  double rightHealthyChestWallCompliance_L_Per_cmH2O = m_RightPleuralToRespiratoryMuscle->GetComplianceBaseline(VolumePerPressureUnit::L_Per_cmH2O);
  
  double totalCompliance_L_Per_cmH2O = 1.0 / (1.0 / leftHealthyChestWallCompliance_L_Per_cmH2O + 1.0 / leftHealthyLungCompliance_L_Per_cmH2O) +
    1.0 / (1.0 / rightHealthyChestWallCompliance_L_Per_cmH2O + 1.0 / rightHealthyLungCompliance_L_Per_cmH2O);

  double driverPressure_cmH2O = -(TargetVolume_L - m_data.GetInitialPatient().GetFunctionalResidualCapacity(VolumeUnit::L)) / totalCompliance_L_Per_cmH2O;
  return driverPressure_cmH2O;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Populate the Pulmonary Function Test Assessment
///
/// \param  pft        pft data
///
/// \return success        true, if everything worked out
//--------------------------------------------------------------------------------------------------
bool Respiratory::CalculatePulmonaryFunctionTest(SEPulmonaryFunctionTest& pft) const
{
  pft.Clear();
  pft.GetExpiratoryReserveVolume().Set(m_data.GetCurrentPatient().GetExpiratoryReserveVolume());
  pft.GetFunctionalResidualCapacity().Set(m_data.GetCurrentPatient().GetFunctionalResidualCapacity());
  pft.GetInspiratoryCapacity().Set(m_data.GetCurrentPatient().GetInspiratoryCapacity());
  pft.GetInspiratoryReserveVolume().Set(m_data.GetCurrentPatient().GetInspiratoryReserveVolume());
  pft.GetResidualVolume().Set(m_data.GetCurrentPatient().GetResidualVolume());
  pft.GetTotalLungCapacity().Set(m_data.GetCurrentPatient().GetTotalLungCapacity());
  pft.GetVitalCapacity().Set(m_data.GetCurrentPatient().GetVitalCapacity());

  double rr_Hz = GetRespirationRate(FrequencyUnit::Hz);
  double tv_L = GetTidalVolume(VolumeUnit::L);
  double waveRespirationRate = rr_Hz;
  double pi = 3.14159265359;
  double magnitude = 0.5*tv_L;
  double superPosition = m_data.GetCurrentPatient().GetFunctionalResidualCapacity(VolumeUnit::L) + magnitude;
  double waveTime = 0.0;
  double currentTime = 0.0;

  double numPlotPoint = pft.GetNumberOfPlotPoints();
  SEFunctionVolumeVsTime& plot = pft.GetLungVolumePlot();
  plot.Invalidate();
  std::vector<double>& time = plot.GetTime();
  std::vector<double>& volume = plot.GetVolume();
  plot.SetTimeUnit(TimeUnit::s);
  plot.SetVolumeUnit(VolumeUnit::L);
  if (numPlotPoint > 0)
  {
    double dt = 60.0 / numPlotPoint;
    for (unsigned int i = 0; i < numPlotPoint; i++)
    {
      time.push_back(currentTime);
      if (waveTime >= 12.0 && waveTime < 13.0)
      {//Period dilation due to forced inspiration
        magnitude = 0.5*tv_L + pft.GetInspiratoryReserveVolume().GetValue(VolumeUnit::L);
        waveRespirationRate = rr_Hz
          / (1 + pft.GetExpiratoryReserveVolume().GetValue(VolumeUnit::L) / tv_L);
      }
      else if (waveTime >= 13.0 && waveTime < 14.0)
      {//Period dilation due to forced expiration
        magnitude = 0.5*tv_L + pft.GetExpiratoryReserveVolume().GetValue(VolumeUnit::L);
        waveRespirationRate = rr_Hz
          / (1 + pft.GetExpiratoryReserveVolume().GetValue(VolumeUnit::L) / tv_L);
      }
      else
      {
        magnitude = 0.5*tv_L;
        waveRespirationRate = rr_Hz;
      }

      volume.push_back(magnitude*sin(pi*waveTime) + superPosition);
      waveTime += 2.0*waveRespirationRate*dt;
      currentTime += dt;
    }
  }

  /// \todo Insert more PFT calculations here
  /*
  All Known:
  VitalCapacity=TotaLungCapacity-ResidualVolume
  InspiratoryCapacity=TotaLungCapacity-FunctionalResidualCapacity
  InspiratoryReserveVolume=InspiratoryCapacity-TidalVolume
  ExpiratoryReserveVolume=FunctionalResidualCapacity-ResidualVolume

  Need to define:
  ForcedVitalCapacity        |  Increase the pressure driver in the lungs
  ForcedExpiratoryVolume      |  for forced PFT constituents?
  ForcedExpiratoryFlow      |
  PeakExpiratoryFlow        |
  MaximumVoluntaryVentilation    |  The same pressure driver increase should be used here. Calculate as a time integration of lung volume over 1 min.

  SlowVitalCapacity        |

  */
  return true;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Run the circuit standalone once to get it into a good starting state 
/// - keep volume fractions constant
//--------------------------------------------------------------------------------------------------
void Respiratory::TuneCircuit()
{
  SEFluidCircuit& RespiratoryCircuit = m_data.GetCircuits().GetRespiratoryCircuit();
  //Set the starting/default driver pressure
  m_DriverPressurePath->GetNextPressureSource().Set(m_DriverPressurePath->GetPressureSourceBaseline());
  m_Calculator->Process(RespiratoryCircuit, m_data.GetTimeStep_s());
  m_Calculator->PostProcess(RespiratoryCircuit);

  //Make sure the new volumes are accounted for with all the substance stuff
  //Keep the same volume fraction originally initialized
  SEGasCompartmentGraph* RespiratoryGraph = &m_data.GetCompartments().GetRespiratoryGraph();
  for (SEGasCompartment* compartment : RespiratoryGraph->GetCompartments())
  {
    if (compartment->HasVolume())
      compartment->Balance(BalanceGasBy::VolumeFraction);
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Modify the chest wall compliance based on lung volume.
///
/// \details
/// The chest wall compliance is modified using a sigmoidal function. Extreme values are defined by 
/// a volume vs. pressure curve between reserve volume (minimum lung volume) and total lung capacity
/// (maximum lung volume). The instantaneous compliance is calculated each time step to follow this 
/// defined volume vs. pressure piecewise function.
//--------------------------------------------------------------------------------------------------
void Respiratory::UpdateChestWallCompliances()
{
  /// \cite venegas1998comprehensive

  double rightLungRatio = m_data.GetCurrentPatient().GetRightLungRatio().GetValue();
  double leftLungRatio = 1.0 - rightLungRatio;

  double rightVolume_L = m_RightLung->GetVolume(VolumeUnit::L) - m_RightAlveolarDeadSpace->GetNextVolume(VolumeUnit::L) + m_rightAlveoliDecrease_L;
  double leftVolume_L = m_LeftLung->GetVolume(VolumeUnit::L) - m_LeftAlveolarDeadSpace->GetNextVolume(VolumeUnit::L) + m_leftAlveoliDecrease_L;

  for (unsigned int iterLung = 0; iterLung < 2; iterLung++)
  {
    //0 = right lung, 1 = left lung

    double lungRatio = 0.0;
    double lungVolume_L = 0.0;
    SEFluidCircuitPath* chestWallPath;
    SEFluidCircuitPath* lungPath;
    
    if (iterLung == 0)
    {
      lungRatio = rightLungRatio;
      lungVolume_L = rightVolume_L;
      chestWallPath = m_RightPleuralToRespiratoryMuscle;
      lungPath = m_RightAlveoliToRightPleuralConnection;
    }
    else
    {
      lungRatio = leftLungRatio;
      lungVolume_L = leftVolume_L;
      chestWallPath = m_LeftPleuralToRespiratoryMuscle;
      lungPath = m_LeftAlveoliToLeftPleuralConnection;
    }

    double healthyChestWallCompliance_L_Per_cmH2O = chestWallPath->GetComplianceBaseline(VolumePerPressureUnit::L_Per_cmH2O);
    double healthyLungCompliance_L_Per_cmH2O = lungPath->GetComplianceBaseline(VolumePerPressureUnit::L_Per_cmH2O);

    double residualVolume_L = m_data.GetInitialPatient().GetResidualVolume(VolumeUnit::L) * lungRatio;
    double vitalCapacity_L = m_data.GetInitialPatient().GetVitalCapacity(VolumeUnit::L) * lungRatio;
    double functionalResidualCapacity_L = m_data.GetInitialPatient().GetFunctionalResidualCapacity(VolumeUnit::L) * lungRatio;
        
    double healthySideCompliance_L_Per_cmH2O = 1.0 / (1.0 / healthyChestWallCompliance_L_Per_cmH2O + 1.0 / healthyLungCompliance_L_Per_cmH2O);

    double minCompliance_L_Per_cmH2O = 0.01; //Minimum possible compliance
    double sideCompliance_L_Per_cmH2O = minCompliance_L_Per_cmH2O;
    if (lungVolume_L > residualVolume_L && lungVolume_L < vitalCapacity_L + residualVolume_L)
    {
      double pressureCornerUpper_cmH2O = (vitalCapacity_L - functionalResidualCapacity_L) / healthySideCompliance_L_Per_cmH2O;
      double naturalLog = log((functionalResidualCapacity_L - residualVolume_L) / (residualVolume_L + vitalCapacity_L - functionalResidualCapacity_L));
      double c = -(pressureCornerUpper_cmH2O / 2.0 * naturalLog) / (1.0 / 2.0 - naturalLog);
      double d = (pressureCornerUpper_cmH2O - c) / 2.0;
      double expectedPressure_cmH2O = d * log((lungVolume_L - residualVolume_L) / (residualVolume_L + vitalCapacity_L - lungVolume_L)) + c;
      double volumeAtZeroPressure = residualVolume_L + (vitalCapacity_L / (1.0 + exp(c / d)));

      sideCompliance_L_Per_cmH2O = healthySideCompliance_L_Per_cmH2O;

      if (!SEScalar::IsZero(expectedPressure_cmH2O, ZERO_APPROX))
      {
        //Not dividing by ~0
        sideCompliance_L_Per_cmH2O = (lungVolume_L - volumeAtZeroPressure) / expectedPressure_cmH2O;
      }

      if (SEScalar::IsZero(sideCompliance_L_Per_cmH2O,ZERO_APPROX))
      {
        sideCompliance_L_Per_cmH2O = healthySideCompliance_L_Per_cmH2O;
      }
      sideCompliance_L_Per_cmH2O = MAX(sideCompliance_L_Per_cmH2O, minCompliance_L_Per_cmH2O);
    }
    
    double chestWallCompliance_L_Per_cmH2O = healthyChestWallCompliance_L_Per_cmH2O;
    //Can't divide by zero
    if (sideCompliance_L_Per_cmH2O != 0.0 && sideCompliance_L_Per_cmH2O != healthyLungCompliance_L_Per_cmH2O)
    {
      chestWallCompliance_L_Per_cmH2O = 1.0 / (1.0 / sideCompliance_L_Per_cmH2O - 1.0 / healthyLungCompliance_L_Per_cmH2O);
    }

    //Dampen the change to prevent potential craziness
    //It will only change half as much as it wants to each time step to ensure it's critically damped and doesn't overshoot
    double dampenFraction_perSec = 0.5 * 50.0;

    double previousChestWallCompliance_L_Per_cmH2O = chestWallPath->GetCompliance(VolumePerPressureUnit::L_Per_cmH2O);
    double complianceChange_L_Per_cmH2O = (chestWallCompliance_L_Per_cmH2O - previousChestWallCompliance_L_Per_cmH2O) * dampenFraction_perSec * m_data.GetTimeStep_s();
 
    chestWallCompliance_L_Per_cmH2O = previousChestWallCompliance_L_Per_cmH2O + complianceChange_L_Per_cmH2O;
    chestWallPath->GetNextCompliance().SetValue(chestWallCompliance_L_Per_cmH2O, VolumePerPressureUnit::L_Per_cmH2O);
  }  
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculate the instantaneous work of breathing.
///
/// \details
/// ToDo
//--------------------------------------------------------------------------------------------------
void Respiratory::CalculateWork()
{
  ///\ToDo Add work calculation
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculate and apply repiratory fatigue effects.
///
/// \details
/// ToDo
//--------------------------------------------------------------------------------------------------
void Respiratory::CalculateFatigue()
{
  ///\ToDo Add fatigue calculation
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Modify lung volumes based on actions and conditions.
///
/// \details
/// Increase the alveolar dead space volume due to COPD. Also, make sure the stomach volume does
/// not cause a problem with ambient changes.
//--------------------------------------------------------------------------------------------------
void Respiratory::UpdateVolumes()
{
  //Standard male patient ideal weight = 75.3 kg
  double standardFunctionalResidualCapacity_L = 30.0 * 75.3 / 1000.0;
  double pateintMultiplier = m_data.GetInitialPatient().GetFunctionalResidualCapacity(VolumeUnit::L) / standardFunctionalResidualCapacity_L;
  
  //Don't modify the stomach on environment changes
  if (m_Ambient->GetNextPressure(PressureUnit::cmH2O) != m_Ambient->GetPressure(PressureUnit::cmH2O))
  {
    //The environment just changed
    //Keep volume the same by changing the pressure equally on both sides
    double pressureChange_cmH2O = m_Ambient->GetNextPressure(PressureUnit::cmH2O) - m_Ambient->GetPressure(PressureUnit::cmH2O);
    m_Stomach->GetNextPressure().IncrementValue(pressureChange_cmH2O, PressureUnit::cmH2O);
  }

  double leftAlveolarDeadSpace_L = m_LeftAlveolarDeadSpace->GetVolumeBaseline(VolumeUnit::L);
  double rightAlveolarDeadSpace_L = m_RightAlveolarDeadSpace->GetVolumeBaseline(VolumeUnit::L);

  double leftAlveolarDeadSpaceIncrease_L = 0.0;
  double rightAlveolarDeadSpaceIncrease_L = 0.0;

  double leftAlveoliDecrease_L = 0.0;
  double rightAlveoliDecrease_L = 0.0;

  //------------------------------------------------------------------------------------------------------
  //ARDS
  //Exacerbation will overwrite the condition, even if it means improvement

  //The dead space cannot be greater than the FRC in our model.

  if (m_data.GetConditions().HasAcuteRespiratoryDistressSyndrome() || m_PatientActions->HasAcuteRespiratoryDistressSyndromeExacerbation())
  {
    double severity = 0.0;
    double leftLungFraction = 0.0;
    double rightLungFraction = 0.0;
    if (m_PatientActions->HasAcuteRespiratoryDistressSyndromeExacerbation())
    {
      severity = m_PatientActions->GetAcuteRespiratoryDistressSyndromeExacerbation().GetSeverity().GetValue();
      leftLungFraction = m_PatientActions->GetAcuteRespiratoryDistressSyndromeExacerbation().GetLeftLungAffected().GetValue();
      rightLungFraction = m_PatientActions->GetAcuteRespiratoryDistressSyndromeExacerbation().GetRightLungAffected().GetValue();
    }
    else
    {
      severity = m_data.GetConditions().GetAcuteRespiratoryDistressSyndrome().GetSeverity().GetValue();
      leftLungFraction = m_data.GetConditions().GetAcuteRespiratoryDistressSyndrome().GetLeftLungAffected().GetValue();
      rightLungFraction = m_data.GetConditions().GetAcuteRespiratoryDistressSyndrome().GetRightLungAffected().GetValue();
    }

    double alveolarDeadSpace_L = 0.0;
    if (severity > 0.3)
    {
      // best fit for (severity, volume): (0, 0), (0.3, 0), (0.6, 0.003), (0.9, 0.15)
      alveolarDeadSpace_L = 0.3704 * std::pow(severity, 3.0) - 0.1667 * std::pow(severity, 2.0) + 0.0167 * severity;
    }

    double rightLungRatio = m_data.GetCurrentPatient().GetRightLungRatio().GetValue();
    double leftLungRatio = 1.0 - rightLungRatio;

    leftAlveolarDeadSpaceIncrease_L = MAX(leftAlveolarDeadSpaceIncrease_L, alveolarDeadSpace_L * leftLungRatio * leftLungFraction);
    rightAlveolarDeadSpaceIncrease_L = MAX(rightAlveolarDeadSpaceIncrease_L, alveolarDeadSpace_L * rightLungRatio * rightLungFraction);

    leftAlveoliDecrease_L = GeneralMath::LinearInterpolator(0.0, 1.0, 0.0, 0.6, severity);
    rightAlveoliDecrease_L = GeneralMath::LinearInterpolator(0.0, 1.0, 0.0, 0.6, severity);
  }

  //------------------------------------------------------------------------------------------------------
  //COPD
  //Exacerbation will overwrite the condition, even if it means improvement

  ///\cite guyton2006medical p504: In a person with partially functional or nonfunctional alveoli in some parts of the lungs, 
  //the physiological dead space may be as much as 10 times the volume of the anatomical dead space, or 1 to 2 liters.

  if (m_data.GetConditions().HasChronicObstructivePulmonaryDisease() || m_PatientActions->HasChronicObstructivePulmonaryDiseaseExacerbation())
  {
    double bronchitisSeverity = 0.0;
    double emphysemaSeverity = 0.0;
    if (m_PatientActions->HasChronicObstructivePulmonaryDiseaseExacerbation())
    {
      bronchitisSeverity = m_PatientActions->GetChronicObstructivePulmonaryDiseaseExacerbation().GetBronchitisSeverity().GetValue();
      emphysemaSeverity = m_PatientActions->GetChronicObstructivePulmonaryDiseaseExacerbation().GetEmphysemaSeverity().GetValue();
    }
    else
    {
      bronchitisSeverity = m_data.GetConditions().GetChronicObstructivePulmonaryDisease().GetBronchitisSeverity().GetValue();
      emphysemaSeverity = m_data.GetConditions().GetChronicObstructivePulmonaryDisease().GetEmphysemaSeverity().GetValue();
    }

    double combinedSeverity = 0.0;
    combinedSeverity = MAX(combinedSeverity, emphysemaSeverity);
    combinedSeverity = MAX(combinedSeverity, bronchitisSeverity);

    double alveolarDeadSpace_L = GeneralMath::LinearInterpolator(0.0, 1.0, 0.0, 1.0, combinedSeverity);

    double rightLungRatio = m_data.GetCurrentPatient().GetRightLungRatio().GetValue();
    double leftLungRatio = 1.0 - rightLungRatio;
    
    leftAlveolarDeadSpaceIncrease_L = MAX(leftAlveolarDeadSpaceIncrease_L, alveolarDeadSpace_L * leftLungRatio);
    rightAlveolarDeadSpaceIncrease_L = MAX(rightAlveolarDeadSpaceIncrease_L, alveolarDeadSpace_L * rightLungRatio);
  }
 
  //---------------------------------------------------------------------------------------------------------------------------------------------
  //Pulmonary Fibrosis

  if (m_data.GetConditions().HasPulmonaryFibrosis())
  {
    double Severity = m_data.GetConditions().GetPulmonaryFibrosis().GetSeverity().GetValue();

    double alveolarDeadSpace_L = GeneralMath::LinearInterpolator(0.0, 1.0, 0.0, 1.0, Severity);

    double rightLungRatio = m_data.GetCurrentPatient().GetRightLungRatio().GetValue();
    double leftLungRatio = 1.0 - rightLungRatio;

    leftAlveolarDeadSpaceIncrease_L = MAX(leftAlveolarDeadSpaceIncrease_L, alveolarDeadSpace_L * leftLungRatio);
    rightAlveolarDeadSpaceIncrease_L = MAX(rightAlveolarDeadSpaceIncrease_L, alveolarDeadSpace_L * rightLungRatio);
  }

  //---------------------------------------------------------------------------------------------------------------------------------------------
  leftAlveolarDeadSpace_L += leftAlveolarDeadSpaceIncrease_L;
  rightAlveolarDeadSpace_L += rightAlveolarDeadSpaceIncrease_L;

  leftAlveolarDeadSpace_L *= pateintMultiplier;
  rightAlveolarDeadSpace_L *= pateintMultiplier;

  //---------------------------------------------------------------------------------------------------------------------------------------------
  //Update alveoli volume that participates in gas exchange

  //Only do this once on the timestep that it changes
  //Change both next and current volume to handle compliance properly
  if (leftAlveoliDecrease_L != m_leftAlveoliDecrease_L)
  {
    m_LeftAlveoli->GetNextVolume().SetReadOnly(false);
    m_LeftAlveoli->GetVolume().SetReadOnly(false);
    m_LeftAlveoli->GetNextVolume().IncrementValue(m_leftAlveoliDecrease_L - leftAlveoliDecrease_L, VolumeUnit::L);
    m_LeftAlveoli->GetVolume().IncrementValue(m_leftAlveoliDecrease_L - leftAlveoliDecrease_L, VolumeUnit::L);
    m_LeftAlveoli->GetNextVolume().SetReadOnly(true);
    m_LeftAlveoli->GetVolume().SetReadOnly(true);
    m_leftAlveoliDecrease_L = leftAlveoliDecrease_L;

    m_LeftPleural->GetNextVolume().SetReadOnly(false);
    m_LeftPleural->GetVolume().SetReadOnly(false);
    m_LeftPleural->GetNextVolume().IncrementValue(m_leftAlveoliDecrease_L - leftAlveoliDecrease_L, VolumeUnit::L);
    m_LeftPleural->GetVolume().IncrementValue(m_leftAlveoliDecrease_L - leftAlveoliDecrease_L, VolumeUnit::L);
    m_LeftPleural->GetNextVolume().SetReadOnly(true);
    m_LeftPleural->GetVolume().SetReadOnly(true);
  }

  if (rightAlveoliDecrease_L != m_rightAlveoliDecrease_L)
  {
    m_RightAlveoli->GetNextVolume().SetReadOnly(false);
    m_RightAlveoli->GetVolume().SetReadOnly(false);
    m_RightAlveoli->GetNextVolume().IncrementValue(m_rightAlveoliDecrease_L - rightAlveoliDecrease_L, VolumeUnit::L);
    m_RightAlveoli->GetVolume().IncrementValue(m_rightAlveoliDecrease_L - rightAlveoliDecrease_L, VolumeUnit::L);
    m_RightAlveoli->GetNextVolume().SetReadOnly(true);
    m_RightAlveoli->GetVolume().SetReadOnly(true);
    m_rightAlveoliDecrease_L = rightAlveoliDecrease_L;

    m_RightPleural->GetNextVolume().SetReadOnly(false);
    m_RightPleural->GetVolume().SetReadOnly(false);
    m_RightPleural->GetNextVolume().IncrementValue(m_leftAlveoliDecrease_L - leftAlveoliDecrease_L, VolumeUnit::L);
    m_RightPleural->GetVolume().IncrementValue(m_leftAlveoliDecrease_L - leftAlveoliDecrease_L, VolumeUnit::L);
    m_RightPleural->GetNextVolume().SetReadOnly(true);
    m_RightPleural->GetVolume().SetReadOnly(true);
  }

  m_LeftAlveolarDeadSpace->GetNextVolume().SetValue(leftAlveolarDeadSpace_L, VolumeUnit::L);
  m_RightAlveolarDeadSpace->GetNextVolume().SetValue(rightAlveolarDeadSpace_L, VolumeUnit::L);

  //Update lung volumes
  //Don't change residual volume or total lung capacity
  double functionalResidualCapacity_L = m_data.GetInitialPatient().GetFunctionalResidualCapacity(VolumeUnit::L);
  double residualVolume_L = m_data.GetInitialPatient().GetResidualVolume(VolumeUnit::L);
  double totalLungCapacity_L = m_data.GetInitialPatient().GetTotalLungCapacity(VolumeUnit::L);
  functionalResidualCapacity_L += leftAlveolarDeadSpaceIncrease_L - m_leftAlveoliDecrease_L + rightAlveolarDeadSpaceIncrease_L - m_rightAlveoliDecrease_L;
  residualVolume_L += leftAlveolarDeadSpaceIncrease_L + rightAlveolarDeadSpaceIncrease_L;
  totalLungCapacity_L += leftAlveolarDeadSpaceIncrease_L + rightAlveolarDeadSpaceIncrease_L;

  double tidalVolumeBaseline_L = m_data.GetCurrentPatient().GetTidalVolumeBaseline(VolumeUnit::L);

  double expiratoryReserveVolume_L = functionalResidualCapacity_L - residualVolume_L;
  double inspiratoryReserveVolume_L = totalLungCapacity_L - functionalResidualCapacity_L - tidalVolumeBaseline_L;
  double inspiratoryCapacity_L = totalLungCapacity_L - functionalResidualCapacity_L;
  double vitalCapacity_L = totalLungCapacity_L - residualVolume_L;

  m_data.GetCurrentPatient().GetFunctionalResidualCapacity().SetValue(functionalResidualCapacity_L, VolumeUnit::L);
  m_data.GetCurrentPatient().GetResidualVolume().SetValue(residualVolume_L, VolumeUnit::L);
  m_data.GetCurrentPatient().GetTotalLungCapacity().SetValue(totalLungCapacity_L, VolumeUnit::L);
  m_data.GetCurrentPatient().GetExpiratoryReserveVolume().SetValue(expiratoryReserveVolume_L, VolumeUnit::L);
  m_data.GetCurrentPatient().GetInspiratoryReserveVolume().SetValue(inspiratoryReserveVolume_L, VolumeUnit::L);
  m_data.GetCurrentPatient().GetInspiratoryCapacity().SetValue(inspiratoryCapacity_L, VolumeUnit::L);
  m_data.GetCurrentPatient().GetVitalCapacity().SetValue(vitalCapacity_L, VolumeUnit::L);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Update respiratory resistances due to actions and conditions.
///
/// \details
/// Update the trachea, bronchial, and alveoli resistances as appropriate. Effects are combined by
/// taking the action/conditions that has the greatest change from baseline.
//--------------------------------------------------------------------------------------------------
void Respiratory::UpdateResistances()
{
  double tracheaResistance_cmH2O_s_Per_L = m_AirwayToCarina->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  double rightBronchiResistance_cmH2O_s_Per_L = m_CarinaToRightAnatomicDeadSpace->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  double leftBronchiResistance_cmH2O_s_Per_L = m_CarinaToLeftAnatomicDeadSpace->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  double rightAlveoliResistance_cmH2O_s_Per_L = m_RightAnatomicDeadSpaceToRightAlveolarDeadSpace->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  double leftAlveoliResistance_cmH2O_s_Per_L = m_LeftAnatomicDeadSpaceToLeftAlveolarDeadSpace->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  double esophagusResistance_cmH2O_s_Per_L = m_AirwayToStomach->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  
  //------------------------------------------------------------------------------------------------------
  //Positive Pressure Ventilation
  if (m_data.GetAirwayMode() == eAirwayMode::AnesthesiaMachine ||
    m_data.GetAirwayMode() == eAirwayMode::MechanicalVentilation || 
    m_data.GetAirwayMode() == eAirwayMode::MechanicalVentilator)
  {
    tracheaResistance_cmH2O_s_Per_L *= 11.0;
  }

  //------------------------------------------------------------------------------------------------------
  //Intubation
  if (m_PatientActions->HasIntubation())
  {
    m_data.SetIntubation(eSwitch::On);
    SEIntubation& intubation = m_PatientActions->GetIntubation();
    switch (intubation.GetType())
    {
    case eIntubation_Type::Tracheal:
    {
      //The proper way to intubate
      //Airway mode handles this case by default
      break;
    }
    case eIntubation_Type::Esophageal:
    {
      //During mechanical ventilation, one of the clinical complications of endotracheal intubation is esophageal intubation. This involves the
      //misplacement of the tube down the esophagus. Such event prohibits air flow into or out of the lungs. The circuit handles
      //this respiratory distress by manipulating the tracheal resistance. When esophageal intubation incidence is triggered, significantly large
      //resistance is assigned to the trachea compartment. Otherwise, the esophageal compartment resistance is set to be significantly
      //large value under normal condition.

      // Allow air flow between Airway and Stomach
      esophagusResistance_cmH2O_s_Per_L = 1.2;
      // Stop air flow between the Airway and Carina
      //This is basically an open switch.  We don't need to worry about anyone else modifying it if this action is on.
      tracheaResistance_cmH2O_s_Per_L = m_DefaultOpenResistance_cmH2O_s_Per_L;
      break;
    }
    case eIntubation_Type::RightMainstem:
    {
      leftBronchiResistance_cmH2O_s_Per_L = m_RespOpenResistance_cmH2O_s_Per_L;
      break;
    }
    case eIntubation_Type::LeftMainstem:
    {
      rightBronchiResistance_cmH2O_s_Per_L = m_RespOpenResistance_cmH2O_s_Per_L;
      break;
    }
    }
  }
  else
  {
    m_data.SetIntubation(eSwitch::Off);
  }

  //------------------------------------------------------------------------------------------------------
  //Airway obstruction
  if (m_PatientActions->HasAirwayObstruction())
  {
    double Severity = m_PatientActions->GetAirwayObstruction().GetSeverity().GetValue();
    tracheaResistance_cmH2O_s_Per_L = GeneralMath::ExponentialGrowthFunction(20.0, tracheaResistance_cmH2O_s_Per_L, m_RespOpenResistance_cmH2O_s_Per_L, Severity);
    tracheaResistance_cmH2O_s_Per_L = MIN(tracheaResistance_cmH2O_s_Per_L, m_RespOpenResistance_cmH2O_s_Per_L);
  }

  //------------------------------------------------------------------------------------------------------
  //Broncho constriction
  if (m_PatientActions->HasBronchoconstriction())
  {
    double dSeverity = m_PatientActions->GetBronchoconstriction().GetSeverity().GetValue();
    leftBronchiResistance_cmH2O_s_Per_L = GeneralMath::ExponentialGrowthFunction(70.0, leftBronchiResistance_cmH2O_s_Per_L, m_RespOpenResistance_cmH2O_s_Per_L, dSeverity);
    rightBronchiResistance_cmH2O_s_Per_L = GeneralMath::ExponentialGrowthFunction(70.0, rightBronchiResistance_cmH2O_s_Per_L, m_RespOpenResistance_cmH2O_s_Per_L, dSeverity);
    leftBronchiResistance_cmH2O_s_Per_L = MIN(leftBronchiResistance_cmH2O_s_Per_L, m_RespOpenResistance_cmH2O_s_Per_L);
    rightBronchiResistance_cmH2O_s_Per_L = MIN(rightBronchiResistance_cmH2O_s_Per_L, m_RespOpenResistance_cmH2O_s_Per_L);
  }

  if (m_data.HasDrugs())
  {
    //------------------------------------------------------------------------------------------------------
    //Bronchodilation
    //Note: this could constrict with a negative number from PD
    //We're going to make the bronchodilation effects be based off of Albuterol.
    //A value of 1.0 will be the effective Albuterol EMax (with the current, non-overdose implementation), and -1.0 will be a bronchconstriction the same percentage in the other direction.
    //Experimentally, 1mg of Albuterol given via a spacer device on an endotracheal tube caused a pulmonary resistance change of ~20% @cite mancebo1991effects.
    //The bronchi are ~30% of the total pulmonary resistance, so we'll make a dilation effect of 1.0 be at the respiratory open resistance.
    //Dilation effect values have max effect at 1 and below -1, so anything outside of that will maintain that effect.
    double bronchoDilationEffect = m_data.GetDrugs().GetBronchodilationLevel().GetValue();
    if (bronchoDilationEffect != 0.0)
    {
      //It'll pretty much always get in here because there's epinephrine present
      //Get the path resistances 
      if (bronchoDilationEffect >= 0.0)// positive, therefore dilation
      {
        bronchoDilationEffect = MIN(bronchoDilationEffect, 1.0);
        leftBronchiResistance_cmH2O_s_Per_L = GeneralMath::ExponentialDecayFunction(10.0, m_RespClosedResistance_cmH2O_s_Per_L, leftBronchiResistance_cmH2O_s_Per_L, bronchoDilationEffect);
        rightBronchiResistance_cmH2O_s_Per_L = GeneralMath::ExponentialDecayFunction(10.0, m_RespClosedResistance_cmH2O_s_Per_L, rightBronchiResistance_cmH2O_s_Per_L, bronchoDilationEffect);
      }
      else //negative, therefore constriction
      {
        bronchoDilationEffect = MIN(-bronchoDilationEffect, 1.0);
        leftBronchiResistance_cmH2O_s_Per_L = GeneralMath::ExponentialGrowthFunction(10.0, leftBronchiResistance_cmH2O_s_Per_L, m_RespOpenResistance_cmH2O_s_Per_L, bronchoDilationEffect);
        rightBronchiResistance_cmH2O_s_Per_L = GeneralMath::ExponentialGrowthFunction(10.0, rightBronchiResistance_cmH2O_s_Per_L, m_RespOpenResistance_cmH2O_s_Per_L, bronchoDilationEffect);
      }
      leftBronchiResistance_cmH2O_s_Per_L = BLIM(leftBronchiResistance_cmH2O_s_Per_L, m_RespClosedResistance_cmH2O_s_Per_L, m_RespOpenResistance_cmH2O_s_Per_L);
      rightBronchiResistance_cmH2O_s_Per_L = BLIM(rightBronchiResistance_cmH2O_s_Per_L, m_RespClosedResistance_cmH2O_s_Per_L, m_RespOpenResistance_cmH2O_s_Per_L);
    }
  }

  //------------------------------------------------------------------------------------------------------
  //Obstructive
  double obstructiveResistanceScalingFactor = 1.0;

  //------------------------------------------------------------------------------------------------------
  //Asthma
  if (m_PatientActions->HasAsthmaAttack())
  {
    double severity = m_PatientActions->GetAsthmaAttack().GetSeverity().GetValue();
    obstructiveResistanceScalingFactor = GeneralMath::LinearInterpolator(0.0, 1.0, 1.0, 60.0, severity);
  }

  //------------------------------------------------------------------------------------------------------
  //COPD
  //Exacerbation will overwrite the condition, even if it means improvement
  if (m_data.GetConditions().HasChronicObstructivePulmonaryDisease() || m_PatientActions->HasChronicObstructivePulmonaryDiseaseExacerbation())
  {
    double bronchitisSeverity = 0.0;
    if (m_PatientActions->HasChronicObstructivePulmonaryDiseaseExacerbation())
    {
      bronchitisSeverity = m_PatientActions->GetChronicObstructivePulmonaryDiseaseExacerbation().GetBronchitisSeverity().GetValue();
    }
    else
    {
      bronchitisSeverity = m_data.GetConditions().GetChronicObstructivePulmonaryDisease().GetBronchitisSeverity().GetValue();
    }

    double dResistanceScalingFactor = GeneralMath::LinearInterpolator(0.0, 1.0, 1.0, 60.0, bronchitisSeverity);
    obstructiveResistanceScalingFactor = MAX(obstructiveResistanceScalingFactor, dResistanceScalingFactor);
  }

  leftBronchiResistance_cmH2O_s_Per_L *= obstructiveResistanceScalingFactor;
  rightBronchiResistance_cmH2O_s_Per_L *= obstructiveResistanceScalingFactor;
  leftBronchiResistance_cmH2O_s_Per_L = BLIM(leftBronchiResistance_cmH2O_s_Per_L, m_RespClosedResistance_cmH2O_s_Per_L, m_RespOpenResistance_cmH2O_s_Per_L);
  rightBronchiResistance_cmH2O_s_Per_L = BLIM(rightBronchiResistance_cmH2O_s_Per_L, m_RespClosedResistance_cmH2O_s_Per_L, m_RespOpenResistance_cmH2O_s_Per_L);

  //------------------------------------------------------------------------------------------------------
  //Restrictive - No change
  //ARDS
  //LobarPneumonia

  //------------------------------------------------------------------------------------------------------
  //Set new values
  m_AirwayToCarina->GetNextResistance().SetValue(tracheaResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  m_CarinaToRightAnatomicDeadSpace->GetNextResistance().SetValue(rightBronchiResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  m_CarinaToLeftAnatomicDeadSpace->GetNextResistance().SetValue(leftBronchiResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  m_RightAnatomicDeadSpaceToRightAlveolarDeadSpace->GetNextResistance().SetValue(rightAlveoliResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  m_LeftAnatomicDeadSpaceToLeftAlveolarDeadSpace->GetNextResistance().SetValue(leftAlveoliResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  m_AirwayToStomach->GetNextResistance().SetValue(esophagusResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Update lung compliance due to actions and conditions.
///
/// \details
/// The alveoli/lung compliance is a constant value (linear volume vs. presure function) that is
/// modified and tuned to provide the desired total pulmonary compliance. The maximal obstructive and 
/// maximal restrictive effects are combined through individual multipliers.
//--------------------------------------------------------------------------------------------------
void Respiratory::UpdateAlveolarCompliances()
{
  double rightAlveoliCompliance_L_Per_cmH2O = m_RightAlveoliToRightPleuralConnection->GetNextCompliance(VolumePerPressureUnit::L_Per_cmH2O);
  double leftAlveoliCompliance_L_Per_cmH2O = m_LeftAlveoliToLeftPleuralConnection->GetNextCompliance(VolumePerPressureUnit::L_Per_cmH2O);

  //------------------------------------------------------------------------------------------------------
  //Positive Pressure Ventilation
  if (m_data.GetAirwayMode() == eAirwayMode::AnesthesiaMachine ||
    m_data.GetAirwayMode() == eAirwayMode::MechanicalVentilation ||
    m_data.GetAirwayMode() == eAirwayMode::MechanicalVentilator)
  {
    rightAlveoliCompliance_L_Per_cmH2O *= 0.38;
    leftAlveoliCompliance_L_Per_cmH2O *= 0.38;
  }

  //------------------------------------------------------------------------------------------------------
  //Obstructive = Increase
  double leftObstructiveComplianceScalingFactor = 1.0;
  double rightObstructiveComplianceScalingFactor = 1.0;

  //------------------------------------------------------------------------------------------------------
  //Asthma
  //ToDo

  //------------------------------------------------------------------------------------------------------
  //COPD
  //Exacerbation will overwrite the condition, even if it means improvement
  if (m_data.GetConditions().HasChronicObstructivePulmonaryDisease() || m_PatientActions->HasChronicObstructivePulmonaryDiseaseExacerbation())
  {
    double emphysemaSeverity = 0.0;
    if (m_PatientActions->HasChronicObstructivePulmonaryDiseaseExacerbation())
    {
      emphysemaSeverity = m_PatientActions->GetChronicObstructivePulmonaryDiseaseExacerbation().GetEmphysemaSeverity().GetValue();
    }
    else
    {
      emphysemaSeverity = m_data.GetConditions().GetChronicObstructivePulmonaryDisease().GetEmphysemaSeverity().GetValue();
    }

    double complianceScalingFactor = GeneralMath::ExponentialGrowthFunction(10, 1.0, 1.15, emphysemaSeverity);

    leftObstructiveComplianceScalingFactor = MAX(leftObstructiveComplianceScalingFactor, complianceScalingFactor);
    rightObstructiveComplianceScalingFactor = MAX(rightObstructiveComplianceScalingFactor, complianceScalingFactor);
  }

  //------------------------------------------------------------------------------------------------------
  //Restrictive = Decrease
  double leftRestrictiveComplianceScalingFactor = 1.0;
  double rightRestrictiveComplianceScalingFactor = 1.0;

  //------------------------------------------------------------------------------------------------------
  //Lobar Pneumonia
  //Exacerbation will overwrite the condition, even if it means improvement
  if (m_data.GetConditions().HasLobarPneumonia() || m_PatientActions->HasLobarPneumoniaExacerbation())
  {
    double severity = 0.0;
    double leftLungFraction = 0.0;
    double rightLungFraction = 0.0;
    if (m_PatientActions->HasLobarPneumoniaExacerbation())
    {
      SELobarPneumoniaExacerbation& lpe = m_PatientActions->GetLobarPneumoniaExacerbation();
      severity = lpe.GetSeverity().GetValue();
      leftLungFraction = lpe.GetLeftLungAffected().GetValue();
      rightLungFraction = lpe.GetRightLungAffected().GetValue();
    }
    else
    {
      severity = m_data.GetConditions().GetLobarPneumonia().GetSeverity().GetValue();
      leftLungFraction = m_data.GetConditions().GetLobarPneumonia().GetLeftLungAffected().GetValue();
      rightLungFraction = m_data.GetConditions().GetLobarPneumonia().GetRightLungAffected().GetValue();
    }
    
    double complianceScalingFactor = GeneralMath::ExponentialDecayFunction(10, 0.32, 1.0, severity);

    leftRestrictiveComplianceScalingFactor = MIN(leftRestrictiveComplianceScalingFactor, 1.0 - (1.0 - complianceScalingFactor) * leftLungFraction);
    rightRestrictiveComplianceScalingFactor = MIN(rightRestrictiveComplianceScalingFactor, 1.0 - (1.0 - complianceScalingFactor) * rightLungFraction);
  }

  //------------------------------------------------------------------------------------------------------
  //Pulmonary Fibrosis
  if (m_data.GetConditions().HasPulmonaryFibrosis())
  {
    double severity = m_data.GetConditions().GetPulmonaryFibrosis().GetSeverity().GetValue();

    double complianceScalingFactor = GeneralMath::ExponentialDecayFunction(10, 0.32, 1.0, severity);

    leftRestrictiveComplianceScalingFactor = MIN(leftRestrictiveComplianceScalingFactor, complianceScalingFactor);
    rightRestrictiveComplianceScalingFactor = MIN(rightRestrictiveComplianceScalingFactor, complianceScalingFactor);
  }

  //------------------------------------------------------------------------------------------------------
  //ARDS
  //Exacerbation will overwrite the condition, even if it means improvement
  //Same as lobar pneumonia for now
  if (m_data.GetConditions().HasAcuteRespiratoryDistressSyndrome() || m_PatientActions->HasAcuteRespiratoryDistressSyndromeExacerbation())
  {
    double severity = 0.0;
    double leftLungFraction = 0.0;
    double rightLungFraction = 0.0;
    if (m_PatientActions->HasAcuteRespiratoryDistressSyndromeExacerbation())
    {
      SEAcuteRespiratoryDistressSyndromeExacerbation& ardse = m_PatientActions->GetAcuteRespiratoryDistressSyndromeExacerbation();
      severity = ardse.GetSeverity().GetValue();
      leftLungFraction = ardse.GetLeftLungAffected().GetValue();
      rightLungFraction = ardse.GetRightLungAffected().GetValue();
    }
    else
    {
      severity = m_data.GetConditions().GetAcuteRespiratoryDistressSyndrome().GetSeverity().GetValue();
      leftLungFraction = m_data.GetConditions().GetAcuteRespiratoryDistressSyndrome().GetLeftLungAffected().GetValue();
      rightLungFraction = m_data.GetConditions().GetAcuteRespiratoryDistressSyndrome().GetRightLungAffected().GetValue();
    }

    double complianceScalingFactor = GeneralMath::ExponentialDecayFunction(10, 0.32, 1.0, severity);

    leftRestrictiveComplianceScalingFactor = MIN(leftRestrictiveComplianceScalingFactor, 1.0 - (1.0 - complianceScalingFactor) * leftLungFraction);
    rightRestrictiveComplianceScalingFactor = MIN(rightRestrictiveComplianceScalingFactor, 1.0 - (1.0 - complianceScalingFactor) * rightLungFraction);
  }

  //------------------------------------------------------------------------------------------------------
  //Set new values
  leftAlveoliCompliance_L_Per_cmH2O = leftAlveoliCompliance_L_Per_cmH2O * leftObstructiveComplianceScalingFactor * leftRestrictiveComplianceScalingFactor;
  rightAlveoliCompliance_L_Per_cmH2O = rightAlveoliCompliance_L_Per_cmH2O * rightObstructiveComplianceScalingFactor * rightRestrictiveComplianceScalingFactor;
  
  m_RightAlveoliToRightPleuralConnection->GetNextCompliance().SetValue(rightAlveoliCompliance_L_Per_cmH2O, VolumePerPressureUnit::L_Per_cmH2O);
  m_LeftAlveoliToLeftPleuralConnection->GetNextCompliance().SetValue(leftAlveoliCompliance_L_Per_cmH2O, VolumePerPressureUnit::L_Per_cmH2O);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Update the inspiratory:expiratory ratio due to actions and conditions.
///
/// \details
/// The inspiratory and expiratory ratio scaling factors are determine thorugh the tuned combined 
/// effects of respiratory conditions and actions, as well as drug PD effects. The resulting modifier
/// is then applied to the breath cycle fraction to ultimately modify the relaive inspiratory and 
/// expiratory time.
//--------------------------------------------------------------------------------------------------
void Respiratory::UpdateInspiratoryExpiratoryRatio()
{
  m_IERatioScaleFactor = 1.0; //Reset

  //Adjust the inspiration/expiration ratio based on severity
  double combinedObstructiveSeverity = 0.0;
  double combinedRestrictiveSeverity = 0.0;

  //------------------------------------------------------------------------------------------------------
  //Obsructive = Decrease (prolonged expiration)

  //------------------------------------------------------------------------------------------------------
  //Asthma
  if (m_PatientActions->HasAsthmaAttack())
  {
    combinedObstructiveSeverity = m_PatientActions->GetAsthmaAttack().GetSeverity().GetValue();
  }

  //------------------------------------------------------------------------------------------------------
  //COPD
  //Exacerbation will overwrite the condition, even if it means improvement
  if (m_data.GetConditions().HasChronicObstructivePulmonaryDisease() || m_PatientActions->HasChronicObstructivePulmonaryDiseaseExacerbation())
  {
    double bronchitisSeverity = 0.0;
    double emphysemaSeverity = 0.0;
    if (m_PatientActions->HasChronicObstructivePulmonaryDiseaseExacerbation())
    {
      bronchitisSeverity = m_PatientActions->GetChronicObstructivePulmonaryDiseaseExacerbation().GetBronchitisSeverity().GetValue();
      emphysemaSeverity = m_PatientActions->GetChronicObstructivePulmonaryDiseaseExacerbation().GetEmphysemaSeverity().GetValue();
    }
    else
    {
      bronchitisSeverity = m_data.GetConditions().GetChronicObstructivePulmonaryDisease().GetBronchitisSeverity().GetValue();
      emphysemaSeverity = m_data.GetConditions().GetChronicObstructivePulmonaryDisease().GetEmphysemaSeverity().GetValue();
    }

    combinedObstructiveSeverity = MAX(combinedObstructiveSeverity, emphysemaSeverity);
    combinedObstructiveSeverity = MAX(combinedObstructiveSeverity, bronchitisSeverity);
  }

  //------------------------------------------------------------------------------------------------------
  //Restrictive = Increase

  //------------------------------------------------------------------------------------------------------
  //LobarPneumonia
  //Exacerbation will overwrite the condition, even if it means improvement
  if (m_data.GetConditions().HasLobarPneumonia() || m_PatientActions->HasLobarPneumoniaExacerbation())
  {
    double severity = 0.0;
    double leftLungFraction = 0.0;
    double rightLungFraction = 0.0;
    if (m_PatientActions->HasLobarPneumoniaExacerbation())
    {
      severity = m_PatientActions->GetLobarPneumoniaExacerbation().GetSeverity().GetValue();
      leftLungFraction = m_PatientActions->GetLobarPneumoniaExacerbation().GetLeftLungAffected().GetValue();
      rightLungFraction = m_PatientActions->GetLobarPneumoniaExacerbation().GetRightLungAffected().GetValue();
    }
    else
    {
      severity = m_data.GetConditions().GetLobarPneumonia().GetSeverity().GetValue();
      leftLungFraction = m_data.GetConditions().GetLobarPneumonia().GetLeftLungAffected().GetValue();
      rightLungFraction = m_data.GetConditions().GetLobarPneumonia().GetRightLungAffected().GetValue();
    }

    // Get the right and left lung ratios
    double dRightLungRatio = m_data.GetCurrentPatient().GetRightLungRatio().GetValue();
    double dLeftLungRatio = 1.0 - dRightLungRatio;

    double scaledSeverity = severity * leftLungFraction * dLeftLungRatio + severity * rightLungFraction * dRightLungRatio;
    combinedRestrictiveSeverity = MAX(combinedRestrictiveSeverity, scaledSeverity);
  }

  //------------------------------------------------------------------------------------------------------
  // Pulmonary Fibrosis
  if (m_data.GetConditions().HasPulmonaryFibrosis())
  {
    double Severity = m_data.GetConditions().GetPulmonaryFibrosis().GetSeverity().GetValue();

    combinedRestrictiveSeverity = MAX(combinedRestrictiveSeverity, Severity);
  }

  //------------------------------------------------------------------------------------------------------
  //ARDS
  //Exacerbation will overwrite the condition, even if it means improvement
  //Same as lobar pneumonia for now
  if (m_data.GetConditions().HasAcuteRespiratoryDistressSyndrome() || m_PatientActions->HasAcuteRespiratoryDistressSyndromeExacerbation())
  {
    double severity = 0.0;
    double leftLungFraction = 0.0;
    double rightLungFraction = 0.0;
    if (m_PatientActions->HasAcuteRespiratoryDistressSyndromeExacerbation())
    {
      severity = m_PatientActions->GetAcuteRespiratoryDistressSyndromeExacerbation().GetSeverity().GetValue();
      leftLungFraction = m_PatientActions->GetAcuteRespiratoryDistressSyndromeExacerbation().GetLeftLungAffected().GetValue();
      rightLungFraction = m_PatientActions->GetAcuteRespiratoryDistressSyndromeExacerbation().GetRightLungAffected().GetValue();
    }
    else
    {
      severity = m_data.GetConditions().GetAcuteRespiratoryDistressSyndrome().GetSeverity().GetValue();
      leftLungFraction = m_data.GetConditions().GetAcuteRespiratoryDistressSyndrome().GetLeftLungAffected().GetValue();
      rightLungFraction = m_data.GetConditions().GetAcuteRespiratoryDistressSyndrome().GetRightLungAffected().GetValue();
    }

    // Get the right and left lung ratios
    double dRightLungRatio = m_data.GetCurrentPatient().GetRightLungRatio().GetValue();
    double dLeftLungRatio = 1.0 - dRightLungRatio;

    double scaledSeverity = severity * leftLungFraction * dLeftLungRatio + severity * rightLungFraction * dRightLungRatio;
    combinedRestrictiveSeverity = MAX(combinedRestrictiveSeverity, scaledSeverity);
  }

  //------------------------------------------------------------------------------------------------------
  //Set new value & Drugs/PD

  // Approximate mapping without frequency effects (standard respiration rate)
  //
  // Factor | Inspiratory Fraction | IE Ratio
  // -------|----------------------|-----------
  // 0.5    | 0.17                 | 1:5 (0.2)
  // 0.75   | 0.25                 | 1:3 (0.33)
  // 1.0    | 0.33                 | 1:2 (0.5)
  // 1.5    | 0.5                  | 1:1 (1.0)
  // 2.0    | 0.67                 | 2:1 (2.0)

  // Obstructive effects
  //Multiplier included to counterbalance effects of RC time constant
  m_IERatioScaleFactor *= GeneralMath::LinearInterpolator(0.0, 1.0, 1.0, 0.5 * 0.2, combinedObstructiveSeverity);

  // Bronchodilators
  //When albuterol is administered, the bronchodilation also causes the IE ratio to correct itself
  m_IERatioScaleFactor *= exp(7728.4 * m_AverageLocalTissueBronchodilationEffects);
  //Lower than 0.1 causes simulation instability
  m_IERatioScaleFactor = LIMIT(m_IERatioScaleFactor, 0.1, 1.0);

  // Restrictive effects
  m_IERatioScaleFactor *= GeneralMath::LinearInterpolator(0.0, 1.0, 1.0, 1.5, combinedRestrictiveSeverity);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Update the diffusion surface area due to actions and conditions.
///
/// \details
/// The surface area is an important factor for gas exchange between the respiratory and 
/// cardiovascular systems. Several conditions effectively reduce the area of alveoli that
/// participate in gas exchange and cause increased CO2 and decreased O2 in the blood. This models 
/// the destruction of alveoli membranes and/or lung consolidation.
//--------------------------------------------------------------------------------------------------
void Respiratory::UpdateDiffusion()
{
  //These fractions all stack
  /// \todo Update this with new healthy vs. current patient methodology
  double alveoliDiffusionArea_cm2 = m_data.GetInitialPatient().GetAlveoliSurfaceArea(AreaUnit::cm2);

  //------------------------------------------------------------------------------------------------------
  //Impaired Alveolar Exchange
  if (m_data.GetConditions().HasImpairedAlveolarExchange() || m_PatientActions->HasImpairedAlveolarExchangeExacerbation())
  {
    if (m_PatientActions->HasImpairedAlveolarExchangeExacerbation())
    {
      SEImpairedAlveolarExchangeExacerbation& iaee = m_PatientActions->GetImpairedAlveolarExchangeExacerbation();
      unsigned int total = int(iaee.HasImpairedSurfaceArea()) +
        int(iaee.HasImpairedFraction()) +
        int(iaee.HasSeverity());

      if (total == 0)
      {
        /// \error Fatal: The Impaired Alveolar Exchange action must include a surface area, fraction, or severity.
        Fatal("The Impaired Alveolar Exchange action must include a surface area, fraction, or severity.");
      }

      if (total > 1)
      {
        /// \error Warning: The Impaired Alveolar Exchange action is defined with mulitple values. Defaulting to the surface area value first, impaired fraction next, and severity last.
        Warning("The Impaired Alveolar Exchange action is defined with mulitple values. Defaulting to the surface area value first, impaired fraction next, and severity last.");
      }

      if (iaee.HasImpairedSurfaceArea())
      {
        
        alveoliDiffusionArea_cm2 = alveoliDiffusionArea_cm2 - iaee.GetImpairedSurfaceArea(AreaUnit::cm2);
      }
      else if (iaee.HasImpairedFraction())
      {
        alveoliDiffusionArea_cm2 = (1.0 - iaee.GetImpairedFraction().GetValue()) * alveoliDiffusionArea_cm2;
      }
      else if (iaee.HasSeverity())
      {
        double severity = iaee.GetSeverity().GetValue();
        double gasDiffusionScalingFactor = GeneralMath::ExponentialDecayFunction(10, 0.1, 1.0, severity);
        alveoliDiffusionArea_cm2 *= gasDiffusionScalingFactor;
      }
    }
    else
    {
      unsigned int total = int(m_data.GetConditions().GetImpairedAlveolarExchange().HasImpairedSurfaceArea()) +
        int(m_data.GetConditions().GetImpairedAlveolarExchange().HasImpairedFraction()) +
        int(m_data.GetConditions().GetImpairedAlveolarExchange().HasSeverity());

      if (total == 0)
      {
        /// \error Fatal: The Impaired Alveolar Exchange action must include a surface area, fraction, or severity.
        Fatal("The Impaired Alveolar Exchange action must include a surface area, fraction, or severity.");
      }

      if (total > 1)
      {
        /// \error Warning: The Impaired Alveolar Exchange condition is defined with mulitple values. Defaulting to the surface area value first, impaired fraction next, and severity last.
        Warning("The Impaired Alveolar Exchange condition is defined with mulitple values. Defaulting to the surface area value first, impaired fraction next, and severity last.");
      }

      if (m_data.GetConditions().GetImpairedAlveolarExchange().HasImpairedSurfaceArea())
      {
        alveoliDiffusionArea_cm2 = alveoliDiffusionArea_cm2 - m_data.GetConditions().GetImpairedAlveolarExchange().GetImpairedSurfaceArea(AreaUnit::cm2);
      }
      else if (m_data.GetConditions().GetImpairedAlveolarExchange().HasImpairedFraction())
      {
        alveoliDiffusionArea_cm2 = (1.0 - m_data.GetConditions().GetImpairedAlveolarExchange().GetImpairedFraction().GetValue()) * alveoliDiffusionArea_cm2;
      }
      else if (m_data.GetConditions().GetImpairedAlveolarExchange().HasSeverity())
      {
        double severity = m_data.GetConditions().GetImpairedAlveolarExchange().GetSeverity().GetValue();
        double gasDiffusionScalingFactor = GeneralMath::ExponentialDecayFunction(10, 0.1, 1.0, severity);
        alveoliDiffusionArea_cm2 *= gasDiffusionScalingFactor;
      }
    }
  }

  //------------------------------------------------------------------------------------------------------
  //COPD
  //Exacerbation will overwrite the condition, even if it means improvement
  if (m_data.GetConditions().HasChronicObstructivePulmonaryDisease() || m_PatientActions->HasChronicObstructivePulmonaryDiseaseExacerbation())
  {
    double emphysemaSeverity = 0.0;
    if (m_PatientActions->HasChronicObstructivePulmonaryDiseaseExacerbation())
    {
      emphysemaSeverity = m_PatientActions->GetChronicObstructivePulmonaryDiseaseExacerbation().GetEmphysemaSeverity().GetValue();
    }
    else
    {
      emphysemaSeverity = m_data.GetConditions().GetChronicObstructivePulmonaryDisease().GetEmphysemaSeverity().GetValue();
    }

    double gasDiffusionScalingFactor = GeneralMath::ExponentialDecayFunction(10, 0.15, 1.0, emphysemaSeverity);

    // Calculate the total surface area
    alveoliDiffusionArea_cm2 *= gasDiffusionScalingFactor;
  }

  //------------------------------------------------------------------------------------------------------
  //LobarPneumonia
  //Exacerbation will overwrite the condition, even if it means improvement
  if (m_data.GetConditions().HasLobarPneumonia() || m_PatientActions->HasLobarPneumoniaExacerbation())
  {
    double severity = 0.0;
    double leftLungFraction = 0.0;
    double rightLungFraction = 0.0;
    if (m_PatientActions->HasLobarPneumoniaExacerbation())
    {
      severity = m_PatientActions->GetLobarPneumoniaExacerbation().GetSeverity().GetValue();
      leftLungFraction = m_PatientActions->GetLobarPneumoniaExacerbation().GetLeftLungAffected().GetValue();
      rightLungFraction = m_PatientActions->GetLobarPneumoniaExacerbation().GetRightLungAffected().GetValue();
    }
    else
    {
      severity = m_data.GetConditions().GetLobarPneumonia().GetSeverity().GetValue();
      leftLungFraction = m_data.GetConditions().GetLobarPneumonia().GetLeftLungAffected().GetValue();
      rightLungFraction = m_data.GetConditions().GetLobarPneumonia().GetRightLungAffected().GetValue();
    }

    // Get the right and left lung ratios
    double rightLungRatio = m_data.GetCurrentPatient().GetRightLungRatio().GetValue();
    double leftLungRatio = 1.0 - rightLungRatio;

    double gasDiffusionScalingFactor = GeneralMath::ExponentialDecayFunction(10, 0.1, 1.0, severity);

    // Calculate the surface area contributions for each lung
    double dRightContribution = rightLungRatio * ((alveoliDiffusionArea_cm2 * (1.0 - rightLungFraction)) + (alveoliDiffusionArea_cm2 * gasDiffusionScalingFactor * rightLungFraction));
    double dLeftContribution = leftLungRatio * ((alveoliDiffusionArea_cm2 * (1.0 - leftLungFraction)) + (alveoliDiffusionArea_cm2 * gasDiffusionScalingFactor * leftLungFraction));

    // Calculate the total surface area
    alveoliDiffusionArea_cm2 = dLeftContribution + dRightContribution;
  }

  //------------------------------------------------------------------------------------------------------
 //PulmonaryFibrosis
  if (m_data.GetConditions().HasPulmonaryFibrosis())
  {
    double Severity = m_data.GetConditions().GetPulmonaryFibrosis().GetSeverity().GetValue();

    double gasDiffusionScalingFactor = GeneralMath::ExponentialDecayFunction(10, 0.1, 1.0, Severity);

    // Calculate the total surface area
    alveoliDiffusionArea_cm2 *= gasDiffusionScalingFactor;
  }

  //------------------------------------------------------------------------------------------------------
  //ARDS
  //Exacerbation will overwrite the condition, even if it means improvement
  //Same as lobar pneumonia for now
  if (m_data.GetConditions().HasAcuteRespiratoryDistressSyndrome() || m_PatientActions->HasAcuteRespiratoryDistressSyndromeExacerbation())
  {
    double severity = 0.0;
    double leftLungFraction = 0.0;
    double rightLungFraction = 0.0;
    if (m_PatientActions->HasAcuteRespiratoryDistressSyndromeExacerbation())
    {
      severity = m_PatientActions->GetAcuteRespiratoryDistressSyndromeExacerbation().GetSeverity().GetValue();
      leftLungFraction = m_PatientActions->GetAcuteRespiratoryDistressSyndromeExacerbation().GetLeftLungAffected().GetValue();
      rightLungFraction = m_PatientActions->GetAcuteRespiratoryDistressSyndromeExacerbation().GetRightLungAffected().GetValue();
    }
    else
    {
      severity = m_data.GetConditions().GetAcuteRespiratoryDistressSyndrome().GetSeverity().GetValue();
      leftLungFraction = m_data.GetConditions().GetAcuteRespiratoryDistressSyndrome().GetLeftLungAffected().GetValue();
      rightLungFraction = m_data.GetConditions().GetAcuteRespiratoryDistressSyndrome().GetRightLungAffected().GetValue();
    }

    // Get the right and left lung ratios
    double rightLungRatio = m_data.GetCurrentPatient().GetRightLungRatio().GetValue();
    double leftLungRatio = 1.0 - rightLungRatio;

    double gasDiffusionScalingFactor = GeneralMath::ExponentialDecayFunction(10, 0.1, 1.0, severity);

    // Calculate the surface area contributions for each lung
    double dRightContribution = rightLungRatio * ((alveoliDiffusionArea_cm2 * (1.0 - rightLungFraction)) + (alveoliDiffusionArea_cm2 * gasDiffusionScalingFactor * rightLungFraction));
    double dLeftContribution = leftLungRatio * ((alveoliDiffusionArea_cm2 * (1.0 - leftLungFraction)) + (alveoliDiffusionArea_cm2 * gasDiffusionScalingFactor * leftLungFraction));

    // Calculate the total surface area
    alveoliDiffusionArea_cm2 = dLeftContribution + dRightContribution;
  }

  //------------------------------------------------------------------------------------------------------
  //Set new value
  m_data.GetCurrentPatient().GetAlveoliSurfaceArea().SetValue(alveoliDiffusionArea_cm2, AreaUnit::cm2);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Update Pulmonary Capillary Resistance 
///
/// \details
/// This method determines a resistance scaling factor from COPD severity to updates the pulmonary 
/// capillary resistance in order to model the destruction of capillaries in the alveolus membrane.
//--------------------------------------------------------------------------------------------------
void Respiratory::UpdatePulmonaryCapillary()
{
  //COPD
  //Exacerbation will overwrite the condition, even if it means improvement
  if (m_data.GetConditions().HasChronicObstructivePulmonaryDisease() || m_PatientActions->HasChronicObstructivePulmonaryDiseaseExacerbation())
  {
    double bronchitisSeverity = 0.0;
    double emphysemaSeverity = 0.0;
    if (m_PatientActions->HasChronicObstructivePulmonaryDiseaseExacerbation())
    {
      bronchitisSeverity = m_PatientActions->GetChronicObstructivePulmonaryDiseaseExacerbation().GetBronchitisSeverity().GetValue();
      emphysemaSeverity = m_PatientActions->GetChronicObstructivePulmonaryDiseaseExacerbation().GetEmphysemaSeverity().GetValue();
    }
    else
    {
      bronchitisSeverity = m_data.GetConditions().GetChronicObstructivePulmonaryDisease().GetBronchitisSeverity().GetValue();
      emphysemaSeverity = m_data.GetConditions().GetChronicObstructivePulmonaryDisease().GetEmphysemaSeverity().GetValue();
    }

    // Calculate Pulmonary Capillary Resistance Multiplier based on severities
    double maxSeverity = MAX(bronchitisSeverity, emphysemaSeverity);
    double pulmonaryResistanceMultiplier = GeneralMath::LinearInterpolator(0.0, 1.0, 1.0, 6.0, maxSeverity);

    double rightPulmonaryCapillaryResistance = m_RightPulmonaryCapillary->GetNextResistance().GetValue(PressureTimePerVolumeUnit::mmHg_s_Per_mL);
    double leftPulmonaryCapillaryResistance = m_LeftPulmonaryCapillary->GetNextResistance().GetValue(PressureTimePerVolumeUnit::mmHg_s_Per_mL);

    rightPulmonaryCapillaryResistance *= pulmonaryResistanceMultiplier;
    leftPulmonaryCapillaryResistance *= pulmonaryResistanceMultiplier;
    
    m_RightPulmonaryCapillary->GetNextResistance().SetValue(rightPulmonaryCapillaryResistance, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
    m_LeftPulmonaryCapillary->GetNextResistance().SetValue(leftPulmonaryCapillaryResistance, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  }

  //-------------------------------------------------------------------------------------------------------------------
  //PulmonaryFibrosis
  if (m_data.GetConditions().HasPulmonaryFibrosis())
  {
    double severity = m_data.GetConditions().GetPulmonaryFibrosis().GetSeverity().GetValue();

    // Calculate Pulmonary Capillary Resistance Multiplier based on severity

    double pulmonaryResistanceMultiplier = GeneralMath::LinearInterpolator(0.0, 1.0, 1.0, 6.0, severity);

    double rightPulmonaryCapillaryResistance = m_RightPulmonaryCapillary->GetNextResistance().GetValue(PressureTimePerVolumeUnit::mmHg_s_Per_mL);
    double leftPulmonaryCapillaryResistance = m_LeftPulmonaryCapillary->GetNextResistance().GetValue(PressureTimePerVolumeUnit::mmHg_s_Per_mL);

    rightPulmonaryCapillaryResistance *= pulmonaryResistanceMultiplier;
    leftPulmonaryCapillaryResistance *= pulmonaryResistanceMultiplier;

    m_RightPulmonaryCapillary->GetNextResistance().SetValue(rightPulmonaryCapillaryResistance, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
    m_LeftPulmonaryCapillary->GetNextResistance().SetValue(leftPulmonaryCapillaryResistance, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Update Pulmonary Shunt Resistance 
///
/// \details
/// This decreases the pulmonary shunt resistance in the cardiovascular system.  The resistance is 
/// inversely proportional to the severity.  The shunt allows deoxgenated blood to pass without
/// participating in gas exchange.  This often occurs with alveoli filled with fluid.
//--------------------------------------------------------------------------------------------------
void Respiratory::UpdatePulmonaryShunt()
{
  double rightSeverity = 0.0;
  double leftSeverity = 0.0;

  double rightPulmonaryShuntScalingFactor = 1.0;
  double leftPulmonaryShuntScalingFactor = 1.0;

  //------------------------------------------------------------------------------------------------------
  //Positive Pressure Ventilation
  if (m_data.GetAirwayMode() == eAirwayMode::AnesthesiaMachine ||
    m_data.GetAirwayMode() == eAirwayMode::MechanicalVentilation ||
    m_data.GetAirwayMode() == eAirwayMode::MechanicalVentilator)
  {
    double shuntResistanceFactor = 0.45;
    rightPulmonaryShuntScalingFactor = shuntResistanceFactor;
    leftPulmonaryShuntScalingFactor = shuntResistanceFactor;
  }

  //------------------------------------------------------------------------------------------------------
  //PulmonaryShunt
  //Exacerbation will overwrite the condition, even if it means improvement
  if (m_data.GetConditions().HasPulmonaryShunt() || m_PatientActions->HasPulmonaryShuntExacerbation())
  {
    double severity = 0.0;
    if (m_PatientActions->HasPulmonaryShuntExacerbation())
    {
      severity = m_PatientActions->GetPulmonaryShuntExacerbation().GetSeverity().GetValue();
    }
    else
    {
      severity = m_data.GetConditions().GetPulmonaryShunt().GetSeverity().GetValue();
    }

    rightSeverity = severity;
    leftSeverity = severity;
  }

  //------------------------------------------------------------------------------------------------------
  //LobarPneumonia
  //Exacerbation will overwrite the condition, even if it means improvement
  if (m_data.GetConditions().HasLobarPneumonia() || m_PatientActions->HasLobarPneumoniaExacerbation())
  {
    double severity = 0.0;
    double leftLungFraction = 0.0;
    double rightLungFraction = 0.0;
    if (m_PatientActions->HasLobarPneumoniaExacerbation())
    {
      severity = m_PatientActions->GetLobarPneumoniaExacerbation().GetSeverity().GetValue();
      leftLungFraction = m_PatientActions->GetLobarPneumoniaExacerbation().GetLeftLungAffected().GetValue();
      rightLungFraction = m_PatientActions->GetLobarPneumoniaExacerbation().GetRightLungAffected().GetValue();
    }
    else
    {
      severity = m_data.GetConditions().GetLobarPneumonia().GetSeverity().GetValue();
      leftLungFraction = m_data.GetConditions().GetLobarPneumonia().GetLeftLungAffected().GetValue();
      rightLungFraction = m_data.GetConditions().GetLobarPneumonia().GetRightLungAffected().GetValue();
    }

    double rightScaledSeverity = severity * rightLungFraction;
    double leftScaledSeverity = severity * leftLungFraction;
    rightSeverity = MAX(rightSeverity, rightScaledSeverity);
    leftSeverity = MAX(leftSeverity, leftScaledSeverity);
  }

  //------------------------------------------------------------------------------------------------------
  //ARDS
  //Exacerbation will overwrite the condition, even if it means improvement
  //Same as lobar pneumonia for now
  if (m_data.GetConditions().HasAcuteRespiratoryDistressSyndrome() || m_PatientActions->HasAcuteRespiratoryDistressSyndromeExacerbation())
  {
    double severity = 0.0;
    double leftLungFraction = 0.0;
    double rightLungFraction = 0.0;
    if (m_PatientActions->HasAcuteRespiratoryDistressSyndromeExacerbation())
    {
      severity = m_PatientActions->GetAcuteRespiratoryDistressSyndromeExacerbation().GetSeverity().GetValue();
      leftLungFraction = m_PatientActions->GetAcuteRespiratoryDistressSyndromeExacerbation().GetLeftLungAffected().GetValue();
      rightLungFraction = m_PatientActions->GetAcuteRespiratoryDistressSyndromeExacerbation().GetRightLungAffected().GetValue();
    }
    else
    {
      severity = m_data.GetConditions().GetAcuteRespiratoryDistressSyndrome().GetSeverity().GetValue();
      leftLungFraction = m_data.GetConditions().GetAcuteRespiratoryDistressSyndrome().GetLeftLungAffected().GetValue();
      rightLungFraction = m_data.GetConditions().GetAcuteRespiratoryDistressSyndrome().GetRightLungAffected().GetValue();
    }

    double rightScaledSeverity = severity * rightLungFraction;
    double leftScaledSeverity = severity * leftLungFraction;
    rightSeverity = MAX(rightSeverity, rightScaledSeverity);
    leftSeverity = MAX(leftSeverity, leftScaledSeverity);
  }

  rightPulmonaryShuntScalingFactor = MIN(rightPulmonaryShuntScalingFactor, GeneralMath::ExponentialDecayFunction(10, 0.02, 1.0, rightSeverity));
  leftPulmonaryShuntScalingFactor = MIN(leftPulmonaryShuntScalingFactor, GeneralMath::ExponentialDecayFunction(10, 0.02, 1.0, leftSeverity));

  //------------------------------------------------------------------------------------------------------
  //COPD - shunting occurs in UpdatePulmonaryCapillary

  //------------------------------------------------------------------------------------------------------

  double rightPulmonaryShuntResistance = m_RightPulmonaryArteriesToVeins->GetNextResistance().GetValue(PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  double leftPulmonaryShuntResistance = m_LeftPulmonaryArteriesToVeins->GetNextResistance().GetValue(PressureTimePerVolumeUnit::mmHg_s_Per_mL);

  rightPulmonaryShuntResistance *= rightPulmonaryShuntScalingFactor;
  leftPulmonaryShuntResistance *= leftPulmonaryShuntScalingFactor;

  m_RightPulmonaryArteriesToVeins->GetNextResistance().SetValue(rightPulmonaryShuntResistance, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  m_LeftPulmonaryArteriesToVeins->GetNextResistance().SetValue(leftPulmonaryShuntResistance, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Reduce the driver pressure based on action and conditions.
///
/// \details
/// This method scales the driver pressure lower than "requested" by the chemoreceptors and other
/// nervous system / PD effects. This ultamitely caused lower O2 in the blood and higher respiration
/// rates compared to lower tidal volumes.
//--------------------------------------------------------------------------------------------------
void Respiratory::ModifyDriverPressure()
{
  double dyspneaSeverity = 0.0;

  //------------------------------------------------------------------------------------------------------
  //Dyspnea
  if (m_PatientActions->HasDyspnea())
  {
    dyspneaSeverity = m_PatientActions->GetDyspnea().GetSeverity().GetValue();
  }

  //------------------------------------------------------------------------------------------------------
  //Fatigue
  if (m_PatientActions->HasRespiratoryFatigue())
  {
    double thisDyspneaSeverity = m_PatientActions->GetRespiratoryFatigue().GetSeverity().GetValue();
    dyspneaSeverity = MAX(dyspneaSeverity, thisDyspneaSeverity);
  }

  //------------------------------------------------------------------------------------------------------
  //COPD
  //Exacerbation will overwrite the condition, even if it means improvement
  if (m_data.GetConditions().HasChronicObstructivePulmonaryDisease() || m_PatientActions->HasChronicObstructivePulmonaryDiseaseExacerbation())
  {
    double bronchitisSeverity = 0.0;
    double emphysemaSeverity = 0.0;
    if (m_PatientActions->HasChronicObstructivePulmonaryDiseaseExacerbation())
    {
      bronchitisSeverity = m_PatientActions->GetChronicObstructivePulmonaryDiseaseExacerbation().GetBronchitisSeverity().GetValue();
      emphysemaSeverity = m_PatientActions->GetChronicObstructivePulmonaryDiseaseExacerbation().GetEmphysemaSeverity().GetValue();
    }
    else
    {
      bronchitisSeverity = m_data.GetConditions().GetChronicObstructivePulmonaryDisease().GetBronchitisSeverity().GetValue();
      emphysemaSeverity = m_data.GetConditions().GetChronicObstructivePulmonaryDisease().GetEmphysemaSeverity().GetValue();
    }

    double combinedSeverity = 0.0;
    combinedSeverity = MAX(combinedSeverity, emphysemaSeverity);
    combinedSeverity = MAX(combinedSeverity, bronchitisSeverity);

    //Linear function: Min = 0.0, Max = 0.8 (increasing with severity)
    double thisDyspneaSeverity = GeneralMath::LinearInterpolator(0.0, 1.0, 0.0, 0.4, combinedSeverity);
    dyspneaSeverity = MAX(dyspneaSeverity, thisDyspneaSeverity);
  }

  //------------------------------------------------------------------------------------------------------
  //LobarPneumonia
  //Exacerbation will overwrite the condition, even if it means improvement
  if (m_data.GetConditions().HasLobarPneumonia() || m_PatientActions->HasLobarPneumoniaExacerbation())
  {
    double severity = 0.0;
    double leftLungFraction = 0.0;
    double rightLungFraction = 0.0;
    if (m_PatientActions->HasLobarPneumoniaExacerbation())
    {
      severity = m_PatientActions->GetLobarPneumoniaExacerbation().GetSeverity().GetValue();
      leftLungFraction = m_PatientActions->GetLobarPneumoniaExacerbation().GetLeftLungAffected().GetValue();
      rightLungFraction = m_PatientActions->GetLobarPneumoniaExacerbation().GetRightLungAffected().GetValue();
    }
    else
    {
      severity = m_data.GetConditions().GetLobarPneumonia().GetSeverity().GetValue();
      leftLungFraction = m_data.GetConditions().GetLobarPneumonia().GetLeftLungAffected().GetValue();
      rightLungFraction = m_data.GetConditions().GetLobarPneumonia().GetRightLungAffected().GetValue();
    }

    // Get the right and left lung ratios
    double dRightLungRatio = m_data.GetCurrentPatient().GetRightLungRatio().GetValue();
    double dLeftLungRatio = 1.0 - dRightLungRatio;

    double combinedSeverity = severity * (dLeftLungRatio * leftLungFraction + dRightLungRatio * rightLungFraction);

    //Linear function: Min = 0.0, Max = 0.8 (increasing with severity)
    double thisDyspneaSeverity = GeneralMath::LinearInterpolator(0.0, 1.0, 0.0, 0.8, combinedSeverity);
    dyspneaSeverity = MAX(dyspneaSeverity, thisDyspneaSeverity);
  }

  //------------------------------------------------------------------------------------------------------
  //Pulmonary Fibrosis

  if (m_data.GetConditions().HasPulmonaryFibrosis())
  {
    double Severity = m_data.GetConditions().GetPulmonaryFibrosis().GetSeverity().GetValue();

    //Linear function: Min = 0.0, Max = 0.8 (increasing with severity)
    double thisDyspneaSeverity = GeneralMath::LinearInterpolator(0.0, 1.0, 0.0, 0.8, Severity);
    dyspneaSeverity = MAX(dyspneaSeverity, thisDyspneaSeverity);
  }

  //------------------------------------------------------------------------------------------------------
  //ARDS
  //Exacerbation will overwrite the condition, even if it means improvement
  //Same as lobar pneumonia for now
  if (m_data.GetConditions().HasAcuteRespiratoryDistressSyndrome() || m_PatientActions->HasAcuteRespiratoryDistressSyndromeExacerbation())
  {
    double severity = 0.0;
    double leftLungFraction = 0.0;
    double rightLungFraction = 0.0;
    if (m_PatientActions->HasAcuteRespiratoryDistressSyndromeExacerbation())
    {
      severity = m_PatientActions->GetAcuteRespiratoryDistressSyndromeExacerbation().GetSeverity().GetValue();
      leftLungFraction = m_PatientActions->GetAcuteRespiratoryDistressSyndromeExacerbation().GetLeftLungAffected().GetValue();
      rightLungFraction = m_PatientActions->GetAcuteRespiratoryDistressSyndromeExacerbation().GetRightLungAffected().GetValue();
    }
    else
    {
      severity = m_data.GetConditions().GetAcuteRespiratoryDistressSyndrome().GetSeverity().GetValue();
      leftLungFraction = m_data.GetConditions().GetAcuteRespiratoryDistressSyndrome().GetLeftLungAffected().GetValue();
      rightLungFraction = m_data.GetConditions().GetAcuteRespiratoryDistressSyndrome().GetRightLungAffected().GetValue();
    }

    // Get the right and left lung ratios
    double dRightLungRatio = m_data.GetCurrentPatient().GetRightLungRatio().GetValue();
    double dLeftLungRatio = 1.0 - dRightLungRatio;

    double combinedSeverity = severity * (dLeftLungRatio * leftLungFraction + dRightLungRatio * rightLungFraction);

    //Linear function: Min = 0.0, Max = 0.8 (increasing with severity)
    double thisDyspneaSeverity = GeneralMath::LinearInterpolator(0.0, 1.0, 0.0, 0.8, combinedSeverity);
    dyspneaSeverity = MAX(dyspneaSeverity, thisDyspneaSeverity);
  }

  if (dyspneaSeverity == 1.0)
  {
    m_NotBreathing = true;
  }

  //Just reduce the tidal volume by the percentage given
  m_DriverPressure_cmH2O = m_DriverPressure_cmH2O * (1 - dyspneaSeverity);

#ifdef DEBUG
  m_data.GetDataTrack().Probe("fatigueFactor", 1 - dyspneaSeverity);
#endif  
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Override the total system resistance.
///
//--------------------------------------------------------------------------------------------------
void Respiratory::SetRespiratoryResistance()
{
  if (m_RespiratoryResistanceOverride_cmH2O_s_Per_L < 0.0)
  {
    return;
  }

  //Set the Airway resistance to get achieve this given value
  double leftBronchiResistance_cmH2O_s_Per_L = m_CarinaToLeftAnatomicDeadSpace->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  double rightBronchiResistance_cmH2O_s_Per_L = m_CarinaToRightAnatomicDeadSpace->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  double leftAlveoliDuctResistance_cmH2O_s_Per_L = m_LeftAnatomicDeadSpaceToLeftAlveolarDeadSpace->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  double rightAlveoliDuctResistance_cmH2O_s_Per_L = m_RightAnatomicDeadSpaceToRightAlveolarDeadSpace->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);

  //Do the circuit math
  double leftSideResistance_cmH2O_s_Per_L = leftBronchiResistance_cmH2O_s_Per_L + leftAlveoliDuctResistance_cmH2O_s_Per_L;
  double rightSideResistance_cmH2O_s_Per_L = rightBronchiResistance_cmH2O_s_Per_L + rightAlveoliDuctResistance_cmH2O_s_Per_L;
  double totalDownstreamResistance_cmH2O_s_Per_L = 1.0 / (1.0 / leftSideResistance_cmH2O_s_Per_L + 1.0 / rightSideResistance_cmH2O_s_Per_L);
  double airwayResistance_cmH2O_s_Per_L = m_RespiratoryResistanceOverride_cmH2O_s_Per_L - totalDownstreamResistance_cmH2O_s_Per_L;

  if (airwayResistance_cmH2O_s_Per_L <= 0)
  {
    /// \error Error: Ignoring the resistance override.  The airway resistance cannot be lowered enough to meet the criteria.
    Error("Ignoring the resistance override.  The airway resistance cannot be lowered enough to meet the criteria.");
  }

  m_AirwayToCarina->GetNextResistance().SetValue(airwayResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Override the total system compliance.
///
//--------------------------------------------------------------------------------------------------
void Respiratory::SetRespiratoryCompliance()
{
  if (m_RespiratoryComplianceOverride_L_Per_cmH2O < 0.0)
  {
    return;
  }

  //Set all compliances the same
  double RespiratorySideCompliance_L_Per_cmH2O = m_RespiratoryComplianceOverride_L_Per_cmH2O / 2.0;
  double LungCompliance_L_Per_cmH2O = 2.0 * RespiratorySideCompliance_L_Per_cmH2O;
  double ChestWallCompliance_L_Per_cmH2O = LungCompliance_L_Per_cmH2O;

  m_LeftAlveoliToLeftPleuralConnection->GetNextCompliance().SetValue(LungCompliance_L_Per_cmH2O, VolumePerPressureUnit::L_Per_cmH2O);
  m_RightAlveoliToRightPleuralConnection->GetNextCompliance().SetValue(LungCompliance_L_Per_cmH2O, VolumePerPressureUnit::L_Per_cmH2O);
  m_LeftPleuralToRespiratoryMuscle->GetNextCompliance().SetValue(ChestWallCompliance_L_Per_cmH2O, VolumePerPressureUnit::L_Per_cmH2O);
  m_RightPleuralToRespiratoryMuscle->GetNextCompliance().SetValue(ChestWallCompliance_L_Per_cmH2O, VolumePerPressureUnit::L_Per_cmH2O);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Output data tracks for debugging purposes
//--------------------------------------------------------------------------------------------------
void Respiratory::Debugging(SEFluidCircuit& RespirationCircuit)
{
  //m_data.GetDataTrack().Probe(RespirationCircuit);

  double leftAlveoliPressure = m_LeftAlveoli->GetNextPressure(PressureUnit::cmH2O);
  double leftAlveoliVolume = m_LeftAlveoli->GetNextVolume(VolumeUnit::L);
  double leftPleuralPressure = m_LeftPleural->GetNextPressure(PressureUnit::cmH2O);
  double leftPleuralVolume = m_LeftPleural->GetNextVolume(VolumeUnit::L);
  double leftFlow = m_LeftAlveolarDeadSpaceToLeftAlveoli->GetNextFlow(VolumePerTimeUnit::L_Per_s);
  double leftChestWallCompliance_L_Per_cmH2O = m_LeftPleuralToRespiratoryMuscle->GetNextCompliance(VolumePerPressureUnit::L_Per_cmH2O);
  double leftLungCompliance_L_Per_cmH2O = m_LeftAlveoliToLeftPleuralConnection->GetNextCompliance(VolumePerPressureUnit::L_Per_cmH2O);

  double rightAlveoliPressure = m_RightAlveoli->GetNextPressure(PressureUnit::cmH2O);
  double rightAlveoliVolume = m_RightAlveoli->GetNextVolume(VolumeUnit::L);
  double rightPleuralPressure = m_RightPleural->GetNextPressure(PressureUnit::cmH2O);
  double rightPleuralVolume = m_RightPleural->GetNextVolume(VolumeUnit::L);
  double rightFlow = m_RightAlveolarDeadSpaceToRightAlveoli->GetNextFlow(VolumePerTimeUnit::L_Per_s);
  double rightChestWallCompliance_L_Per_cmH2O = m_RightPleuralToRespiratoryMuscle->GetNextCompliance(VolumePerPressureUnit::L_Per_cmH2O);
  double rightLungCompliance_L_Per_cmH2O = m_RightAlveoliToRightPleuralConnection->GetNextCompliance(VolumePerPressureUnit::L_Per_cmH2O);

  m_data.GetDataTrack().Probe("leftAlveoliPressure", leftAlveoliPressure);
  m_data.GetDataTrack().Probe("leftAlveoliVolume", leftAlveoliVolume);
  m_data.GetDataTrack().Probe("leftPleuralPressure", leftPleuralPressure);
  m_data.GetDataTrack().Probe("leftPleuralVolume", leftPleuralVolume);
  m_data.GetDataTrack().Probe("leftFlow", leftFlow);
  m_data.GetDataTrack().Probe("leftChestWallCompliance_L_Per_cmH2O", leftChestWallCompliance_L_Per_cmH2O);
  m_data.GetDataTrack().Probe("leftLungCompliance_L_Per_cmH2O", leftLungCompliance_L_Per_cmH2O);

  m_data.GetDataTrack().Probe("rightAlveoliPressure", rightAlveoliPressure);
  m_data.GetDataTrack().Probe("rightAlveoliVolume", rightAlveoliVolume);
  m_data.GetDataTrack().Probe("rightPleuralPressure", rightPleuralPressure);
  m_data.GetDataTrack().Probe("rightPleuralVolume", rightPleuralVolume);
  m_data.GetDataTrack().Probe("rightFlow", rightFlow);
  m_data.GetDataTrack().Probe("rightChestWallCompliance_L_Per_cmH2O", rightChestWallCompliance_L_Per_cmH2O);
  m_data.GetDataTrack().Probe("rightLungCompliance_L_Per_cmH2O", rightLungCompliance_L_Per_cmH2O);

  double AmbientPresure = 1033.23; // = 1 atm
  m_data.GetDataTrack().Probe("LeftRelativePleuralPressure_cmH2O", leftPleuralPressure - AmbientPresure);
  m_data.GetDataTrack().Probe("RightRelativePleuralPressure_cmH2O", rightPleuralPressure - AmbientPresure);

  m_data.GetDataTrack().Probe("LeftRelativeAlveoliPressure_cmH2O", leftAlveoliPressure - AmbientPresure);
  m_data.GetDataTrack().Probe("RightRelativeAlveoliPressure_cmH2O", rightAlveoliPressure - AmbientPresure);

  double leftSideCompliance_L_Per_cmH2O = 1.0 / (1.0 / leftChestWallCompliance_L_Per_cmH2O + 1.0 / leftLungCompliance_L_Per_cmH2O);
  double rightSideCompliance_L_Per_cmH2O = 1.0 / (1.0 / rightChestWallCompliance_L_Per_cmH2O + 1.0 / rightLungCompliance_L_Per_cmH2O);

  double totalCompliance_L_Per_cmH2O = leftSideCompliance_L_Per_cmH2O + rightSideCompliance_L_Per_cmH2O;
  m_data.GetDataTrack().Probe("totalCompliance_L_Per_cmH2O", totalCompliance_L_Per_cmH2O);

  //Parameter table outputs for methodology report
  double leftAlveolarDeadSpace_L = 0.0;
  double rightAlveolarDeadSpace_L = 0.0;
  if (m_LeftAlveolarDeadSpace->HasNextVolume())
    leftAlveolarDeadSpace_L = m_LeftAlveolarDeadSpace->GetNextVolume(VolumeUnit::L);
  if (m_RightAlveolarDeadSpace->HasNextVolume())
    rightAlveolarDeadSpace_L = m_RightAlveolarDeadSpace->GetNextVolume(VolumeUnit::L);
  double totalAlveolarDeadSpace_L = leftAlveolarDeadSpace_L + rightAlveolarDeadSpace_L;
  m_data.GetDataTrack().Probe("totalAlveolarDeadSpace_L", totalAlveolarDeadSpace_L);

  double airwayResistance_cmH2O_s_Per_L = m_AirwayToCarina->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  m_data.GetDataTrack().Probe("airwayResistance_cmH2O_s_Per_L", airwayResistance_cmH2O_s_Per_L);

  double rightBronchiResistance_cmH2O_s_Per_L = m_CarinaToRightAnatomicDeadSpace->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  double leftBronchiResistance_cmH2O_s_Per_L = m_CarinaToLeftAnatomicDeadSpace->GetNextResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  double averageBronchiResistance_cmH2O_s_Per_L = (rightBronchiResistance_cmH2O_s_Per_L + leftBronchiResistance_cmH2O_s_Per_L) / 2.0;
  m_data.GetDataTrack().Probe("averageBronchiResistance_cmH2O_s_Per_L", averageBronchiResistance_cmH2O_s_Per_L);

  double averageLungCompliance_L_Per_cmH2O = (rightLungCompliance_L_Per_cmH2O + leftLungCompliance_L_Per_cmH2O) / 2.0;
  m_data.GetDataTrack().Probe("averageLungCompliance_L_Per_cmH2O", averageLungCompliance_L_Per_cmH2O);

  double inspiratoryExpiratoryRatio = m_InspiratoryRiseFraction / (1.0 - m_InspiratoryRiseFraction);
  m_data.GetDataTrack().Probe("inspiratoryExpiratoryRatio", inspiratoryExpiratoryRatio);

  double diffusionSurfaceArea_m2 = m_data.GetCurrentPatient().GetAlveoliSurfaceArea(AreaUnit::m2);
  m_data.GetDataTrack().Probe("diffusionSurfaceArea_m2", diffusionSurfaceArea_m2);

  double rightPulmonaryCapillaryResistance_mmHg_s_Per_mL = m_RightPulmonaryCapillary->GetNextResistance(PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  double leftPulmonaryCapillaryResistance_mmHg_s_Per_mL = m_LeftPulmonaryCapillary->GetNextResistance(PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  double averagePulmonaryCapillaryResistance_mmHg_s_Per_mL = (rightPulmonaryCapillaryResistance_mmHg_s_Per_mL + leftPulmonaryCapillaryResistance_mmHg_s_Per_mL) / 2.0;
  m_data.GetDataTrack().Probe("averagePulmonaryCapillaryResistance_mmHg_s_Per_mL", averagePulmonaryCapillaryResistance_mmHg_s_Per_mL);

  double rightShuntResistance_mmHg_s_Per_mL = m_RightPulmonaryArteriesToVeins->GetNextResistance(PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  double leftShuntResistance_mmHg_s_Per_mL = m_LeftPulmonaryArteriesToVeins->GetNextResistance(PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  double averageShuntResistance_mmHg_s_Per_mL = (rightShuntResistance_mmHg_s_Per_mL + leftShuntResistance_mmHg_s_Per_mL) / 2.0;
  m_data.GetDataTrack().Probe("averageShuntResistance_mmHg_s_Per_mL", averageShuntResistance_mmHg_s_Per_mL);
}
