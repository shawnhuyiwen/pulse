/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "controller/Controller.h"

#include "controller/Circuits.h"
#include "controller/Compartments.h"
#include "controller/Substances.h"
#include "environment/Environment.h"
#include "PulseConfiguration.h"

#include "patient/SEPatient.h"

#include "system/environment/SEEnvironmentalConditions.h"

#include "circuit/SECircuitManager.h"
#include "circuit/fluid/SEFluidCircuit.h"
#include "circuit/fluid/SEFluidCircuitNode.h"
#include "circuit/fluid/SEFluidCircuitPath.h"
#include "circuit/thermal/SEThermalCircuit.h"
#include "circuit/thermal/SEThermalCircuitNode.h"
#include "circuit/thermal/SEThermalCircuitPath.h"

#include "compartment/fluid/SEGasCompartment.h"
#include "compartment/fluid/SEGasCompartmentLink.h"
#include "compartment/fluid/SEGasCompartmentGraph.h"
#include "compartment/fluid/SELiquidCompartment.h"
#include "compartment/fluid/SELiquidCompartmentLink.h"
#include "compartment/fluid/SELiquidCompartmentGraph.h"
#include "compartment/thermal/SEThermalCompartment.h"
#include "compartment/thermal/SEThermalCompartmentLink.h"
#include "compartment/tissue/SETissueCompartment.h"

#include "properties/SEScalar0To1.h"
#include "properties/SEScalarArea.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarHeatCapacitancePerMass.h"
#include "properties/SEScalarLength.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarMassPerMass.h"

bool PulseController::CreateCircuitsAndCompartments()
{
  m_Circuits->Clear();
  m_Compartments->Clear();
  m_Compartments->Setup();
  SetupCardiovascular();
  if (m_Config->IsRenalEnabled())
    SetupRenal();
  if (m_Config->IsTissueEnabled())
    SetupTissue();
  if (m_Config->IsCerebrospinalFluidEnabled())
    SetupCerebrospinalFluid();
  SetupGastrointestinal();

  ///////////////////////////////////////////////////////////////////
  // Create abd Combine External and Internal Temperature Circuits //
  ///////////////////////////////////////////////////////////////////
  SetupExternalTemperature();
  SetupInternalTemperature();
  SEThermalCircuit& cThermal = m_Circuits->GetTemperatureCircuit();
  SEThermalCircuit& CInthermal = m_Circuits->GetInternalTemperatureCircuit();
  SEThermalCircuit& cExthermal = m_Circuits->GetExternalTemperatureCircuit();
  cThermal.AddCircuit(CInthermal);
  cThermal.AddCircuit(cExthermal);
  SEThermalCircuitNode* exCore = cExthermal.GetNode(pulse::ExternalTemperatureNode::ExternalCore);
  SEThermalCircuitNode* exSkin = cExthermal.GetNode(pulse::ExternalTemperatureNode::ExternalSkin);
  SEThermalCircuitNode* inCore = CInthermal.GetNode(pulse::InternalTemperatureNode::InternalCore);
  SEThermalCircuitNode* inSkin = CInthermal.GetNode(pulse::InternalTemperatureNode::InternalSkin);
  SEThermalCircuitPath& CoreTemperatureConnection = cThermal.CreatePath(*inCore, *exCore, pulse::CombinedTemperaturePath::InternalCoreToExternalCore);
  SEThermalCircuitPath& SkinTemperatureConnection = cThermal.CreatePath(*inSkin, *exSkin, pulse::CombinedTemperaturePath::InternalSkinToExternalSkin);
  cThermal.SetNextAndCurrentFromBaselines();
  cThermal.StateChange();

  SEThermalCompartment* cExCore = m_Compartments->GetThermalCompartment(pulse::TemperatureCompartment::ExternalCore);
  SEThermalCompartment* cExSkin = m_Compartments->GetThermalCompartment(pulse::TemperatureCompartment::ExternalSkin);
  SEThermalCompartment* cInCore = m_Compartments->GetThermalCompartment(pulse::TemperatureCompartment::InternalCore);
  SEThermalCompartment* cInSkin = m_Compartments->GetThermalCompartment(pulse::TemperatureCompartment::InternalSkin);
  SEThermalCompartmentLink& InternalCoreToExternalCore = m_Compartments->CreateThermalLink(*cInCore, *cExCore, pulse::TemperatureLink::InternalCoreToExternalCore);
  InternalCoreToExternalCore.MapPath(CoreTemperatureConnection);
  SEThermalCompartmentLink& InternalSkinToExternalSkin = m_Compartments->CreateThermalLink(*cInSkin, *cExSkin, pulse::TemperatureLink::InternalSkinToExternalSkin);
  InternalSkinToExternalSkin.MapPath(SkinTemperatureConnection);

  // This node is shared between the respiratory, anesthesia, and inhaler circuits
  SEFluidCircuitNode& Ambient = m_Circuits->CreateFluidNode(pulse::EnvironmentNode::Ambient);
  Ambient.GetNextVolume().SetValue(std::numeric_limits<double>::infinity(), VolumeUnit::L);
  Ambient.GetVolumeBaseline().SetValue(std::numeric_limits<double>::infinity(), VolumeUnit::L);
  SEGasCompartment& gEnvironment = m_Compartments->CreateGasCompartment(pulse::EnvironmentCompartment::Ambient);
  gEnvironment.MapNode(Ambient);
  SELiquidCompartment& lEnvironment = m_Compartments->CreateLiquidCompartment(pulse::EnvironmentCompartment::Ambient);
  lEnvironment.MapNode(Ambient);
  Ambient.SetAsReferenceNode();

  m_Environment->Initialize();
  m_Environment->GetEnvironmentalConditions().Copy(m_Config->GetInitialEnvironmentalConditions(), *m_Substances);
  m_Environment->StateChange();
  // Update the environment pressures on all the 'air' circuits to match what the environment was set to
  gEnvironment.GetPressure().Set(m_Environment->GetEnvironmentalConditions().GetAtmosphericPressure());

  SetupRespiratory();
  SetupAnesthesiaMachine();
  SetupInhaler();
  SetupMechanicalVentilation();
  SetupMechanicalVentilator();
  SetupNasalCannula();
  SetupNonRebreatherMask();
  SetupSimpleMask();

  m_Compartments->StateChange();
  return true;
}

// assumes circuit overrides and doesn't check if override is not applied
bool PulseController::OverrideCircuits()
{
  bool bReturn = true;
  if (m_ScalarOverrides.empty())
    return bReturn;

  SEFluidCircuit& cv = GetCircuits().GetActiveCardiovascularCircuit();
  SEFluidCircuit& resp = GetCircuits().GetRespiratoryCircuit();
  for (auto& sp : m_ScalarOverrides)
  {
    SEFluidCircuitNode* node = nullptr;
    SEFluidCircuitPath* path = nullptr;
    if (resp.HasPath(sp.name))
      path = resp.GetPath(sp.name);
    else if (cv.HasPath(sp.name))
      path = cv.GetPath(sp.name);
    else if (resp.HasNode(sp.name))
      node = resp.GetNode(sp.name);
    else if (cv.HasNode(sp.name))
      node = cv.GetNode(sp.name);

    if (path == nullptr && node == nullptr)
      continue; // Must be an override for a system variable

    // Apply Overrides (Note using Force, as these values are locked (for good reason)
    // But we know what we are doing, right?
    if (PressureTimePerVolumeUnit::IsValidUnit(sp.unit))
    {// Assume its a resistor
      const PressureTimePerVolumeUnit& u = PressureTimePerVolumeUnit::GetCompoundUnit(sp.unit);
      path->GetResistance().ForceValue(sp.value, u);
      path->GetNextResistance().ForceValue(sp.value, u);
      path->GetResistanceBaseline().ForceValue(sp.value, u);
    }
    else if (VolumePerPressureUnit::IsValidUnit(sp.unit))
    {
      const VolumePerPressureUnit& u = VolumePerPressureUnit::GetCompoundUnit(sp.unit);
      path->GetCompliance().ForceValue(sp.value, u);
      path->GetNextCompliance().ForceValue(sp.value, u);
      path->GetComplianceBaseline().ForceValue(sp.value, u);
    }
    else if (VolumeUnit::IsValidUnit(sp.unit))
    {
      const VolumeUnit& u = VolumeUnit::GetCompoundUnit(sp.unit);
      node->GetVolume().ForceValue(sp.value, u);
      node->GetNextVolume().ForceValue(sp.value, u);
      node->GetVolumeBaseline().ForceValue(sp.value, u);
    }
    else
    {
      Error("Could not process circuit override " + sp.name);
      bReturn = false;
    }
  }

  return bReturn;
}

void PulseController::SetupCardiovascular()
{
  Info("Setting Up Cardiovascular");
  bool male = m_InitialPatient->GetSex() == ePatient_Sex::Male ? true : false;
  double RightLungRatio = m_InitialPatient->GetRightLungRatio().GetValue();
  double LeftLungRatio = 1 - RightLungRatio;
  double bloodVolume_mL = m_InitialPatient->GetBloodVolumeBaseline(VolumeUnit::mL);

  double systolicPressureTarget_mmHg = m_InitialPatient->GetSystolicArterialPressureBaseline(PressureUnit::mmHg);
  double heartRate_bpm = m_InitialPatient->GetHeartRateBaseline(FrequencyUnit::Per_min);
  const double strokeVolumeTarget_mL = 81.0;
  double cardiacOutputTarget_mL_Per_s = heartRate_bpm / 60.0 * strokeVolumeTarget_mL;
  const double diastolicPressureTarget_mmHg = 80.0;
  const double centralVenousPressureTarget_mmHg = 4.0;
  const double pulmonaryShuntFractionFactor = 0.009; // Used to set the pulmonary shunt fraction. Actual shunt will be roughly double this value (two lungs).
  // The way this works is we compute resistances and compliances based on the hemodynamic variables above that are either in the patient
  // file or we use the defaults if nothing is there. Because the actual impedance depends on the frequency, the computations assume a resting heart rate.
  // So if a user needs to put pressures in the patient file assuming that the pts baseline hr is in the normal range (around 72).
  // If someone wants a patient with a high hr because s/he is exercising or something, then they need to use the action.
  // If a user wants a patient with a ridiculously high resting hr, then they will need to estimate what the pressures and CO would be if the pt had a normal resting hr.

  // We compute a tuning modifier to adjust some baseline resistances and compliances to get closer to the target systolic and diastolic pressures from the patient file
  // The tuning method in cardiovascular will do the fine tuning. This just speeds up the process.
  /// \todo Make these a function of the systolic and diastolic pressure by fitting a curve to the data from the variations test
  const double systemicResistanceModifier = 0.849;
  const double largeArteriesComplianceModifier = 0.4333;

  // Volume fractions and flow rates from \cite valtin1995renal
  // Pressure targets derived from information available in \cite guyton2006medical and \cite van2013davis
  double VolumeFractionAorta = 0.05, VascularPressureTargetAorta = 1.0 * systolicPressureTarget_mmHg, VascularFlowTargetAorta = 1.0 * cardiacOutputTarget_mL_Per_s;
  double VolumeFractionArmLeft = 0.01, VascularPressureTargetArmLeft = 0.33 * systolicPressureTarget_mmHg, VascularFlowTargetArmLeft = male ? 0.01448 * cardiacOutputTarget_mL_Per_s : 0.01664 * cardiacOutputTarget_mL_Per_s;
  double VolumeFractionArmRight = VolumeFractionArmLeft, VascularPressureTargetArmRight = 0.33 * systolicPressureTarget_mmHg;
  double VolumeFractionBone = 0.07, VascularPressureTargetBone = 0.33 * systolicPressureTarget_mmHg, VascularFlowTargetBone = 0.05 * cardiacOutputTarget_mL_Per_s;
  double VolumeFractionBrain = 0.012, VascularPressureTargetBrain = 0.08 * systolicPressureTarget_mmHg, VascularFlowTargetBrain = 0.12 * cardiacOutputTarget_mL_Per_s;
  double VolumeFractionFat = male ? 0.05 : 0.085, VascularPressureTargetFat = 0.33 * systolicPressureTarget_mmHg, VascularFlowTargetFat = male ? 0.05 * cardiacOutputTarget_mL_Per_s : 0.0085 * cardiacOutputTarget_mL_Per_s;
  double VolumeFractionHeartLeft = 0.0025, VascularPressureTargetHeartLeft = 1.06667 * systolicPressureTarget_mmHg;      /*No flow targets heart right*/
  double VolumeFractionHeartRight = 0.0025, VascularPressureTargetHeartRight = 0.16667 * systolicPressureTarget_mmHg;     /*No flow targets heart left*/
  double VolumeFractionKidney = 0.0202, VascularPressureTargetKidney = 0.33 * systolicPressureTarget_mmHg, VascularFlowTargetKidney = male ? 0.098 * cardiacOutputTarget_mL_Per_s : 0.088 * cardiacOutputTarget_mL_Per_s;
  double VolumeFractionLargeIntestine = 0.019, VascularPressureTargetLargeIntestine = 0.33 * systolicPressureTarget_mmHg, VascularFlowTargetLargeIntestine = male ? 0.04 * cardiacOutputTarget_mL_Per_s : 0.05 * cardiacOutputTarget_mL_Per_s;
  double VolumeFractionLegLeft = 0.0151, VascularPressureTargetLegLeft = 0.33 * systolicPressureTarget_mmHg, VascularFlowTargetLegLeft = male ? 0.02872 * cardiacOutputTarget_mL_Per_s : 0.0330 * cardiacOutputTarget_mL_Per_s;
  double VolumeFractionLegRight = VolumeFractionLegLeft, VascularPressureTargetLegRight = 0.33 * systolicPressureTarget_mmHg;
  double VolumeFractionLiver = 0.106, VascularPressureTargetLiver = 0.25 * systolicPressureTarget_mmHg, VascularFlowTargetLiver = 0.075 * cardiacOutputTarget_mL_Per_s;
  double VolumeFractionMuscle = male ? 0.14 : 0.105, VascularPressureTargetMuscle = 0.33 * systolicPressureTarget_mmHg, VascularFlowTargetMuscle = male ? 0.17 * cardiacOutputTarget_mL_Per_s : 0.12 * cardiacOutputTarget_mL_Per_s;
  double VolumeFractionMyocardium = 0.007, VascularPressureTargetMyocardium = 0.33 * systolicPressureTarget_mmHg, VascularFlowTargetMyocardium = male ? 0.04 * cardiacOutputTarget_mL_Per_s : 0.05 * cardiacOutputTarget_mL_Per_s;
  double VolumeFractionPulmArtRight = 0.034 * RightLungRatio, VascularPressureTargetPulmArtRight = 0.13333 * systolicPressureTarget_mmHg;
  double VolumeFractionPulmCapRight = 0.023 * RightLungRatio, VascularPressureTargetPulmCapRight = 0.0650 * systolicPressureTarget_mmHg, VascularFlowTargetPulmCapRight = RightLungRatio * cardiacOutputTarget_mL_Per_s * (1 - pulmonaryShuntFractionFactor);
  double VolumeFractionPulmVeinsRight = 0.068 * RightLungRatio, VascularPressureTargetPulmVeinsRight = 0.03846 * systolicPressureTarget_mmHg, VascularFlowTargetPulmVeinsRight = RightLungRatio * cardiacOutputTarget_mL_Per_s * (1 - pulmonaryShuntFractionFactor);
  double VolumeFractionPulmArtLeft = 0.034 * LeftLungRatio, VascularPressureTargetPulmArtLeft = 0.13333 * systolicPressureTarget_mmHg, VascularFlowTargetPulmArtLeft = LeftLungRatio * cardiacOutputTarget_mL_Per_s * (1 - pulmonaryShuntFractionFactor);
  double VolumeFractionPulmCapLeft = 0.023 * LeftLungRatio, VascularPressureTargetPulmCapLeft = 0.0650 * systolicPressureTarget_mmHg, VascularFlowTargetPulmCapLeft = LeftLungRatio * cardiacOutputTarget_mL_Per_s * (1 - pulmonaryShuntFractionFactor);
  double VolumeFractionPulmVeinsLeft = 0.068 * LeftLungRatio, VascularPressureTargetPulmVeinsLeft = 0.03846 * systolicPressureTarget_mmHg, VascularFlowTargetPulmVeinsLeft = LeftLungRatio * cardiacOutputTarget_mL_Per_s * (1 - pulmonaryShuntFractionFactor);
  double VolumeFractionSkin = 0.032, VascularPressureTargetSkin = 0.0833 * systolicPressureTarget_mmHg, VascularFlowTargetSkin = 0.067 * cardiacOutputTarget_mL_Per_s;
  double VolumeFractionSmallIntestine = 0.038, VascularPressureTargetSmallIntestine = 0.33 * systolicPressureTarget_mmHg, VascularFlowTargetSmallIntestine = male ? 0.1 * cardiacOutputTarget_mL_Per_s : 0.11 * cardiacOutputTarget_mL_Per_s;
  double VolumeFractionSplanchnic = 0.0116, VascularPressureTargetSplanchnic = 0.33 * systolicPressureTarget_mmHg, VascularFlowTargetSplanchnic = male ? 0.0258 * cardiacOutputTarget_mL_Per_s : 0.0255 * cardiacOutputTarget_mL_Per_s;
  double VolumeFractionSpleen = 0.014, VascularPressureTargetSpleen = 0.33 * systolicPressureTarget_mmHg, VascularFlowTargetSpleen = 0.03 * cardiacOutputTarget_mL_Per_s;
  double VolumeFractionVenaCava = 0.247, VascularPressureTargetVenaCava = 0.0333 * systolicPressureTarget_mmHg;
  /*Portal Vein is path only*/                    
  double VascularFlowTargetPortalVein = VascularFlowTargetLargeIntestine + VascularFlowTargetSmallIntestine + VascularFlowTargetSplanchnic + VascularFlowTargetSpleen;

  // Compute resistances from mean flow rates and pressure targets
  double ResistanceAorta = 1.12 * (VascularPressureTargetHeartLeft - systolicPressureTarget_mmHg) / VascularFlowTargetAorta;                        /*No Downstream Resistance Aorta*/
  double ResistanceArmLeft = 1.15 * (systolicPressureTarget_mmHg - VascularPressureTargetArmLeft) / VascularFlowTargetArmLeft, ResistanceArmLeftVenous = (VascularPressureTargetArmLeft - VascularPressureTargetVenaCava) / VascularFlowTargetArmLeft;
  double ResistanceArmRight = ResistanceArmLeft, ResistanceArmRightVenous = ResistanceArmLeftVenous;
  double ResistanceBone = 1.02 * (systolicPressureTarget_mmHg - VascularPressureTargetBone) / VascularFlowTargetBone, ResistanceBoneVenous = (VascularPressureTargetBone - VascularPressureTargetVenaCava) / VascularFlowTargetBone;
  double ResistanceBrain = 1.0 * (systolicPressureTarget_mmHg - VascularPressureTargetBrain) / VascularFlowTargetBrain, ResistanceBrainVenous = (VascularPressureTargetBrain - VascularPressureTargetVenaCava) / VascularFlowTargetBrain;
  double ResistanceFat = 1.02 * (systolicPressureTarget_mmHg - VascularPressureTargetFat) / VascularFlowTargetFat, ResistanceFatVenous = (VascularPressureTargetFat - VascularPressureTargetVenaCava) / VascularFlowTargetFat;
  double ResistanceHeartLeft = 0.0008;                                                                                                          /*No Downstream Resistance HeartLeft*/
  double ResistanceHeartRight = (0.04225 * systolicPressureTarget_mmHg - VascularPressureTargetVenaCava) / cardiacOutputTarget_mL_Per_s; // Describes the flow resistance between the systemic vasculature and the right atrium    /*No Downstream Resistance Heart Right*/
  double ResistanceKidney = 1.5 * (systolicPressureTarget_mmHg - VascularPressureTargetKidney) / VascularFlowTargetKidney, ResistanceKidneyVenous = (VascularPressureTargetKidney - VascularPressureTargetVenaCava) / VascularFlowTargetKidney;
  double ResistanceLargeIntestine = 1.05 * (systolicPressureTarget_mmHg - VascularPressureTargetLargeIntestine) / VascularFlowTargetLargeIntestine, ResistanceLargeIntestineVenous = (VascularPressureTargetLargeIntestine - VascularPressureTargetLiver) / VascularFlowTargetLargeIntestine;
  double ResistanceLegLeft = 1.1 * (systolicPressureTarget_mmHg - VascularPressureTargetLegLeft) / VascularFlowTargetLegLeft, ResistanceLegLeftVenous = (VascularPressureTargetLegLeft - VascularPressureTargetVenaCava) / VascularFlowTargetLegLeft;
  double ResistanceLegRight = 1.00 * ResistanceLegLeft, ResistanceLegRightVenous = ResistanceLegLeftVenous;
  double ResistanceLiver = 1.1 * (systolicPressureTarget_mmHg - VascularPressureTargetLiver) / VascularFlowTargetLiver, ResistanceLiverVenous = (VascularPressureTargetLiver - VascularPressureTargetVenaCava) / (VascularFlowTargetLiver + VascularFlowTargetPortalVein);
  double ResistanceMuscle = 1.15 * (systolicPressureTarget_mmHg - VascularPressureTargetMuscle) / VascularFlowTargetMuscle, ResistanceMuscleVenous = (VascularPressureTargetMuscle - VascularPressureTargetVenaCava) / VascularFlowTargetMuscle;
  double ResistanceMyocardium = 0.95 * (systolicPressureTarget_mmHg - VascularPressureTargetMyocardium) / VascularFlowTargetMyocardium, ResistanceMyocardiumVenous = (VascularPressureTargetMyocardium - VascularPressureTargetVenaCava) / VascularFlowTargetMyocardium;
  double ResistancePulmCapRight = 1.0 * (VascularPressureTargetPulmArtRight - VascularPressureTargetPulmCapRight) / VascularFlowTargetPulmCapRight;      /*No Downstream Resistance PulmCap*/
  double ResistancePulmVeinsRight = 1.0 * (VascularPressureTargetPulmCapRight - VascularPressureTargetPulmVeinsRight) / VascularFlowTargetPulmVeinsRight;/*No Downstream Resistance PulmVeins*/
  double ResistancePulmArtLeft = 1.0 * (VascularPressureTargetHeartRight - VascularPressureTargetPulmArtLeft) / VascularFlowTargetPulmArtLeft;           /*No Downstream Resistance PulmArt*/
  double ResistancePulmCapLeft = 1.0 * (VascularPressureTargetPulmArtLeft - VascularPressureTargetPulmCapLeft) / VascularFlowTargetPulmCapLeft;          /*No Downstream Resistance PulmCap*/
  double ResistancePulmVeinsLeft = 1.0 * (VascularPressureTargetPulmCapLeft - VascularPressureTargetPulmVeinsLeft) / VascularFlowTargetPulmVeinsLeft;    /*No Downstream Resistance PulmVeins*/
  double ResistanceSkin = 1.0 * (systolicPressureTarget_mmHg - VascularPressureTargetSkin) / VascularFlowTargetSkin, ResistanceSkinVenous = (VascularPressureTargetSkin - VascularPressureTargetVenaCava) / VascularFlowTargetSkin;
  double ResistanceSmallIntestine = 1.14 * (systolicPressureTarget_mmHg - VascularPressureTargetSmallIntestine) / VascularFlowTargetSmallIntestine, ResistanceSmallIntestineVenous = (VascularPressureTargetArmLeft - VascularPressureTargetLiver) / VascularFlowTargetSmallIntestine;
  double ResistanceSplanchnic = 0.95 * (systolicPressureTarget_mmHg - VascularPressureTargetSplanchnic) / VascularFlowTargetSplanchnic, ResistanceSplanchnicVenous = (VascularPressureTargetArmLeft - VascularPressureTargetLiver) / VascularFlowTargetSplanchnic;
  double ResistanceSpleen = 0.95 * (systolicPressureTarget_mmHg - VascularPressureTargetSpleen) / VascularFlowTargetSpleen, ResistanceSpleenVenous = (VascularPressureTargetArmLeft - VascularPressureTargetLiver) / VascularFlowTargetSpleen;

  // Portal vein and shunt are just paths - only have resistance
  double ResistancePortalVein = 0.001; // The portal vein is just a pathway in Pulse. The pressure across this path does not represent portal vein pressure (if it did our patient would always be portal hypertensive)
  double ResistanceShuntRight = (VascularPressureTargetPulmArtRight - VascularPressureTargetPulmCapRight) / (cardiacOutputTarget_mL_Per_s * pulmonaryShuntFractionFactor);
  double ResistanceShuntLeft = (VascularPressureTargetPulmArtLeft - VascularPressureTargetPulmCapLeft) / (cardiacOutputTarget_mL_Per_s * pulmonaryShuntFractionFactor);

  // Make a circuit
  SEFluidCircuit& cCardiovascular = m_Circuits->GetCardiovascularCircuit();

  // Create Nodes, set volume baselines and pressures where appropriate
  SEFluidCircuitNode& RightHeart1 = cCardiovascular.CreateNode(pulse::CardiovascularNode::RightHeart1);
  RightHeart1.GetPressure().SetValue(0.0, PressureUnit::mmHg);
  SEFluidCircuitNode& RightHeart2 = cCardiovascular.CreateNode(pulse::CardiovascularNode::RightHeart2);
  SEFluidCircuitNode& RightHeart3 = cCardiovascular.CreateNode(pulse::CardiovascularNode::RightHeart3);
  RightHeart3.GetPressure().SetValue(0.0, PressureUnit::mmHg);
  RightHeart1.GetVolumeBaseline().SetValue(VolumeFractionHeartRight * bloodVolume_mL, VolumeUnit::mL);

  SEFluidCircuitNode& MainPulmonaryArteries = cCardiovascular.CreateNode(pulse::CardiovascularNode::MainPulmonaryArteries);

  SEFluidCircuitNode& RightIntermediatePulmonaryArteries = cCardiovascular.CreateNode(pulse::CardiovascularNode::RightIntermediatePulmonaryArteries);
  SEFluidCircuitNode& RightPulmonaryArteries = cCardiovascular.CreateNode(pulse::CardiovascularNode::RightPulmonaryArteries);
  RightPulmonaryArteries.GetVolumeBaseline().SetValue(VolumeFractionPulmArtRight * bloodVolume_mL, VolumeUnit::mL);
  RightPulmonaryArteries.GetPressure().SetValue(VascularPressureTargetPulmArtRight, PressureUnit::mmHg);

  SEFluidCircuitNode& LeftIntermediatePulmonaryArteries = cCardiovascular.CreateNode(pulse::CardiovascularNode::LeftIntermediatePulmonaryArteries);
  SEFluidCircuitNode& LeftPulmonaryArteries = cCardiovascular.CreateNode(pulse::CardiovascularNode::LeftPulmonaryArteries);
  LeftPulmonaryArteries.GetVolumeBaseline().SetValue(VolumeFractionPulmArtLeft * bloodVolume_mL, VolumeUnit::mL);
  LeftPulmonaryArteries.GetPressure().SetValue(VascularPressureTargetPulmArtLeft, PressureUnit::mmHg);

  SEFluidCircuitNode& RightPulmonaryCapillaries = cCardiovascular.CreateNode(pulse::CardiovascularNode::RightPulmonaryCapillaries);
  RightPulmonaryCapillaries.GetVolumeBaseline().SetValue(VolumeFractionPulmCapRight * bloodVolume_mL, VolumeUnit::mL);
  RightPulmonaryCapillaries.GetPressure().SetValue(VascularPressureTargetPulmCapRight, PressureUnit::mmHg);

  SEFluidCircuitNode& LeftPulmonaryCapillaries = cCardiovascular.CreateNode(pulse::CardiovascularNode::LeftPulmonaryCapillaries);
  LeftPulmonaryCapillaries.GetVolumeBaseline().SetValue(VolumeFractionPulmCapLeft * bloodVolume_mL, VolumeUnit::mL);
  LeftPulmonaryCapillaries.GetPressure().SetValue(VascularPressureTargetPulmCapLeft, PressureUnit::mmHg);

  SEFluidCircuitNode& RightIntermediatePulmonaryVeins = cCardiovascular.CreateNode(pulse::CardiovascularNode::RightIntermediatePulmonaryVeins);
  SEFluidCircuitNode& RightPulmonaryVeins = cCardiovascular.CreateNode(pulse::CardiovascularNode::RightPulmonaryVeins);
  RightPulmonaryVeins.GetVolumeBaseline().SetValue(VolumeFractionPulmVeinsRight * bloodVolume_mL, VolumeUnit::mL);
  RightPulmonaryVeins.GetPressure().SetValue(VascularPressureTargetPulmVeinsRight, PressureUnit::mmHg);

  SEFluidCircuitNode& LeftIntermediatePulmonaryVeins = cCardiovascular.CreateNode(pulse::CardiovascularNode::LeftIntermediatePulmonaryVeins);
  SEFluidCircuitNode& LeftPulmonaryVeins = cCardiovascular.CreateNode(pulse::CardiovascularNode::LeftPulmonaryVeins);
  LeftPulmonaryVeins.GetVolumeBaseline().SetValue(VolumeFractionPulmVeinsLeft * bloodVolume_mL, VolumeUnit::mL);
  LeftPulmonaryVeins.GetPressure().SetValue(VascularPressureTargetPulmVeinsLeft, PressureUnit::mmHg);

  SEFluidCircuitNode& LeftHeart1 = cCardiovascular.CreateNode(pulse::CardiovascularNode::LeftHeart1);
  LeftHeart1.GetPressure().SetValue(0.0, PressureUnit::mmHg);
  SEFluidCircuitNode& LeftHeart2 = cCardiovascular.CreateNode(pulse::CardiovascularNode::LeftHeart2);
  SEFluidCircuitNode& LeftHeart3 = cCardiovascular.CreateNode(pulse::CardiovascularNode::LeftHeart3);
  LeftHeart3.GetPressure().SetValue(0.0, PressureUnit::mmHg);
  LeftHeart1.GetVolumeBaseline().SetValue(VolumeFractionHeartLeft * bloodVolume_mL, VolumeUnit::mL);

  SEFluidCircuitNode& Aorta1 = cCardiovascular.CreateNode(pulse::CardiovascularNode::Aorta1);
  SEFluidCircuitNode& Aorta2 = cCardiovascular.CreateNode(pulse::CardiovascularNode::Aorta2);
  SEFluidCircuitNode& Aorta3 = cCardiovascular.CreateNode(pulse::CardiovascularNode::Aorta3);
  SEFluidCircuitNode& Aorta4 = cCardiovascular.CreateNode(pulse::CardiovascularNode::Aorta4);
  Aorta4.GetPressure().SetValue(0.0, PressureUnit::mmHg);
  Aorta1.GetVolumeBaseline().SetValue(VolumeFractionAorta * bloodVolume_mL, VolumeUnit::mL);
  Aorta1.GetPressure().SetValue(VascularPressureTargetAorta, PressureUnit::mmHg);

  SEFluidCircuitNode& Brain1 = cCardiovascular.CreateNode(pulse::CardiovascularNode::Brain1);
  SEFluidCircuitNode& Brain2 = cCardiovascular.CreateNode(pulse::CardiovascularNode::Brain2);
  Brain1.GetVolumeBaseline().SetValue(VolumeFractionBrain * bloodVolume_mL, VolumeUnit::mL);
  Brain1.GetPressure().SetValue(0.0, PressureUnit::mmHg);
  Brain1.GetPressure().SetValue(VascularPressureTargetBrain, PressureUnit::mmHg);

  SEFluidCircuitNode& Bone1 = cCardiovascular.CreateNode(pulse::CardiovascularNode::Bone1);
  SEFluidCircuitNode& Bone2 = cCardiovascular.CreateNode(pulse::CardiovascularNode::Bone2);
  Bone1.GetVolumeBaseline().SetValue(VolumeFractionBone * bloodVolume_mL, VolumeUnit::mL);
  Bone1.GetPressure().SetValue(VascularPressureTargetBone, PressureUnit::mmHg);

  SEFluidCircuitNode& Fat1 = cCardiovascular.CreateNode(pulse::CardiovascularNode::Fat1);
  SEFluidCircuitNode& Fat2 = cCardiovascular.CreateNode(pulse::CardiovascularNode::Fat2);
  Fat1.GetVolumeBaseline().SetValue(VolumeFractionFat * bloodVolume_mL, VolumeUnit::mL);
  Fat1.GetPressure().SetValue(VascularPressureTargetFat, PressureUnit::mmHg);

  SEFluidCircuitNode& LargeIntestine = cCardiovascular.CreateNode(pulse::CardiovascularNode::LargeIntestine1);
  LargeIntestine.GetVolumeBaseline().SetValue(VolumeFractionLargeIntestine * bloodVolume_mL, VolumeUnit::mL);
  LargeIntestine.GetPressure().SetValue(VascularPressureTargetLargeIntestine, PressureUnit::mmHg);

  SEFluidCircuitNode& Liver1 = cCardiovascular.CreateNode(pulse::CardiovascularNode::Liver1);
  SEFluidCircuitNode& Liver2 = cCardiovascular.CreateNode(pulse::CardiovascularNode::Liver2);
  Liver1.GetVolumeBaseline().SetValue(VolumeFractionLiver * bloodVolume_mL, VolumeUnit::mL);
  Liver1.GetPressure().SetValue(VascularPressureTargetLiver, PressureUnit::mmHg);

  SEFluidCircuitNode& LeftArm1 = cCardiovascular.CreateNode(pulse::CardiovascularNode::LeftArm1);
  SEFluidCircuitNode& LeftArm2 = cCardiovascular.CreateNode(pulse::CardiovascularNode::LeftArm2);
  LeftArm1.GetVolumeBaseline().SetValue(VolumeFractionArmLeft * bloodVolume_mL, VolumeUnit::mL);
  LeftArm1.GetPressure().SetValue(VascularPressureTargetArmRight, PressureUnit::mmHg);

  SEFluidCircuitNode& LeftKidney1 = cCardiovascular.CreateNode(pulse::CardiovascularNode::LeftKidney1);
  SEFluidCircuitNode& LeftKidney2 = cCardiovascular.CreateNode(pulse::CardiovascularNode::LeftKidney2);
  LeftKidney1.GetVolumeBaseline().SetValue(0.5 * VolumeFractionKidney * bloodVolume_mL, VolumeUnit::mL);
  LeftKidney1.GetPressure().SetValue(VascularPressureTargetKidney, PressureUnit::mmHg);

  SEFluidCircuitNode& LeftLeg1 = cCardiovascular.CreateNode(pulse::CardiovascularNode::LeftLeg1);
  SEFluidCircuitNode& LeftLeg2 = cCardiovascular.CreateNode(pulse::CardiovascularNode::LeftLeg2);
  LeftLeg1.GetVolumeBaseline().SetValue(VolumeFractionLegLeft * bloodVolume_mL, VolumeUnit::mL);
  LeftLeg1.GetPressure().SetValue(VascularPressureTargetLegLeft, PressureUnit::mmHg);

  SEFluidCircuitNode& Muscle1 = cCardiovascular.CreateNode(pulse::CardiovascularNode::Muscle1);
  SEFluidCircuitNode& Muscle2 = cCardiovascular.CreateNode(pulse::CardiovascularNode::Muscle2);
  Muscle1.GetVolumeBaseline().SetValue(VolumeFractionMuscle * bloodVolume_mL, VolumeUnit::mL);
  Muscle1.GetPressure().SetValue(VascularPressureTargetMuscle, PressureUnit::mmHg);

  SEFluidCircuitNode& Myocardium1 = cCardiovascular.CreateNode(pulse::CardiovascularNode::Myocardium1);
  SEFluidCircuitNode& Myocardium2 = cCardiovascular.CreateNode(pulse::CardiovascularNode::Myocardium2);
  Myocardium1.GetVolumeBaseline().SetValue(VolumeFractionMyocardium * bloodVolume_mL, VolumeUnit::mL);
  Myocardium1.GetPressure().SetValue(VascularPressureTargetMyocardium, PressureUnit::mmHg);

  SEFluidCircuitNode& PortalVein = cCardiovascular.CreateNode(pulse::CardiovascularNode::PortalVein1);

  SEFluidCircuitNode& RightArm1 = cCardiovascular.CreateNode(pulse::CardiovascularNode::RightArm1);
  SEFluidCircuitNode& RightArm2 = cCardiovascular.CreateNode(pulse::CardiovascularNode::RightArm2);
  RightArm1.GetVolumeBaseline().SetValue(VolumeFractionArmRight * bloodVolume_mL, VolumeUnit::mL);
  RightArm1.GetPressure().SetValue(VascularPressureTargetArmRight, PressureUnit::mmHg);

  SEFluidCircuitNode& RightKidney1 = cCardiovascular.CreateNode(pulse::CardiovascularNode::RightKidney1);
  SEFluidCircuitNode& RightKidney2 = cCardiovascular.CreateNode(pulse::CardiovascularNode::RightKidney2);
  RightKidney1.GetVolumeBaseline().SetValue(0.5 * VolumeFractionKidney * bloodVolume_mL, VolumeUnit::mL);
  RightKidney1.GetPressure().SetValue(VascularPressureTargetKidney, PressureUnit::mmHg);

  SEFluidCircuitNode& RightLeg1 = cCardiovascular.CreateNode(pulse::CardiovascularNode::RightLeg1);
  SEFluidCircuitNode& RightLeg2 = cCardiovascular.CreateNode(pulse::CardiovascularNode::RightLeg2);
  RightLeg1.GetVolumeBaseline().SetValue(VolumeFractionLegRight * bloodVolume_mL, VolumeUnit::mL);
  RightLeg1.GetPressure().SetValue(VascularPressureTargetLegRight, PressureUnit::mmHg);

  SEFluidCircuitNode& Skin1 = cCardiovascular.CreateNode(pulse::CardiovascularNode::Skin1);
  SEFluidCircuitNode& Skin2 = cCardiovascular.CreateNode(pulse::CardiovascularNode::Skin2);
  Skin1.GetVolumeBaseline().SetValue(VolumeFractionSkin * bloodVolume_mL, VolumeUnit::mL);
  Skin1.GetPressure().SetValue(VascularPressureTargetSkin, PressureUnit::mmHg);

  SEFluidCircuitNode& SmallIntestine = cCardiovascular.CreateNode(pulse::CardiovascularNode::SmallIntestine1);
  SmallIntestine.GetVolumeBaseline().SetValue(VolumeFractionSmallIntestine * bloodVolume_mL, VolumeUnit::mL);
  SmallIntestine.GetPressure().SetValue(VascularPressureTargetSmallIntestine, PressureUnit::mmHg);

  SEFluidCircuitNode& Splanchnic = cCardiovascular.CreateNode(pulse::CardiovascularNode::Splanchnic1);
  Splanchnic.GetVolumeBaseline().SetValue(VolumeFractionSplanchnic * bloodVolume_mL, VolumeUnit::mL);
  Splanchnic.GetPressure().SetValue(VascularPressureTargetSplanchnic, PressureUnit::mmHg);

  SEFluidCircuitNode& Spleen = cCardiovascular.CreateNode(pulse::CardiovascularNode::Spleen1);
  Spleen.GetVolumeBaseline().SetValue(VolumeFractionSpleen * bloodVolume_mL, VolumeUnit::mL);
  Spleen.GetPressure().SetValue(VascularPressureTargetSpleen, PressureUnit::mmHg);

  SEFluidCircuitNode& VenaCava = cCardiovascular.CreateNode(pulse::CardiovascularNode::VenaCava);
  VenaCava.GetVolumeBaseline().SetValue(VolumeFractionVenaCava * bloodVolume_mL, VolumeUnit::mL);
  VenaCava.GetPressure().SetValue(VascularPressureTargetVenaCava, PressureUnit::mmHg);

  SEFluidCircuitNode& Ground = cCardiovascular.CreateNode(pulse::CardiovascularNode::Ground);
  Ground.SetAsReferenceNode();
  Ground.GetPressure().SetValue(0.0, PressureUnit::mmHg);

  double blood_mL = 0;
  for (SEFluidCircuitNode* n : cCardiovascular.GetNodes())
  {
    if (n->HasVolumeBaseline())
      blood_mL += n->GetVolumeBaseline(VolumeUnit::mL);
  }
  if (blood_mL > bloodVolume_mL)
    Error("Blood volume greater than total blood volume");

  SEFluidCircuitNode& Pericardium = cCardiovascular.CreateNode(pulse::CardiovascularNode::Pericardium1);
  Pericardium.GetVolumeBaseline().SetValue(15.0, VolumeUnit::mL);
  Pericardium.GetPressure().SetValue(1.0, PressureUnit::mmHg);

  SEFluidCircuitNode& AbdominalCavity = cCardiovascular.CreateNode(pulse::CardiovascularNode::AbdominalCavity1);
  AbdominalCavity.GetVolumeBaseline().SetValue(10.0, VolumeUnit::mL);
  AbdominalCavity.GetPressure().SetValue(0.0, PressureUnit::mmHg);

  // Create Paths, set switch (diodes), compliances, and resistances where appropriate
  SEFluidCircuitPath& VenaCavaToRightHeart2 = cCardiovascular.CreatePath(VenaCava, RightHeart2, pulse::CardiovascularPath::VenaCavaToRightHeart2);
  VenaCavaToRightHeart2.GetResistanceBaseline().SetValue(ResistanceHeartRight, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& RightHeart2ToRightHeart1 = cCardiovascular.CreatePath(RightHeart2, RightHeart1, pulse::CardiovascularPath::RightHeart2ToRightHeart1);
  RightHeart2ToRightHeart1.SetNextValve(eGate::Closed);
  SEFluidCircuitPath& RightHeart1ToRightHeart3 = cCardiovascular.CreatePath(RightHeart1, RightHeart3, pulse::CardiovascularPath::RightHeart1ToRightHeart3);
  SEFluidCircuitPath& RightHeart3ToGround = cCardiovascular.CreatePath(Ground, RightHeart3, pulse::CardiovascularPath::RightHeart3ToGround);
  RightHeart3ToGround.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::mmHg);

  SEFluidCircuitPath& RightHeart1ToMainPulmonaryArteries = cCardiovascular.CreatePath(RightHeart1, MainPulmonaryArteries, pulse::CardiovascularPath::RightHeart1ToMainPulmonaryArteries);
  RightHeart1ToMainPulmonaryArteries.SetNextValve(eGate::Closed);

  SEFluidCircuitPath& MainPulmonaryArteriesToRightIntermediatePulmonaryArteries = cCardiovascular.CreatePath(MainPulmonaryArteries, RightIntermediatePulmonaryArteries, pulse::CardiovascularPath::MainPulmonaryArteriesToRightIntermediatePulmonaryArteries);
  //MainPulmonaryArteriesToRightIntermediatePulmonaryArteries.SetNextValve(eGate::Closed);
  SEFluidCircuitPath& RightIntermediatePulmonaryArteriesToRightPulmonaryArteries = cCardiovascular.CreatePath(RightIntermediatePulmonaryArteries, RightPulmonaryArteries, pulse::CardiovascularPath::RightIntermediatePulmonaryArteriesToRightPulmonaryArteries);
  RightIntermediatePulmonaryArteriesToRightPulmonaryArteries.GetResistanceBaseline().SetValue(ResistancePulmArtLeft, PressureTimePerVolumeUnit::mmHg_s_Per_mL);

  SEFluidCircuitPath& RightPulmonaryArteriesToRightPulmonaryVeins = cCardiovascular.CreatePath(RightPulmonaryArteries, RightPulmonaryVeins, pulse::CardiovascularPath::RightPulmonaryArteriesToRightPulmonaryVeins);
  RightPulmonaryArteriesToRightPulmonaryVeins.GetResistanceBaseline().SetValue(ResistanceShuntRight, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& RightPulmonaryArteriesToRightPulmonaryCapillaries = cCardiovascular.CreatePath(RightPulmonaryArteries, RightPulmonaryCapillaries, pulse::CardiovascularPath::RightPulmonaryArteriesToRightPulmonaryCapillaries);
  RightPulmonaryArteriesToRightPulmonaryCapillaries.GetResistanceBaseline().SetValue(ResistancePulmCapRight, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& RightPulmonaryArteriesToGround = cCardiovascular.CreatePath(RightPulmonaryArteries, Ground, pulse::CardiovascularPath::RightPulmonaryArteriesToGround);
  RightPulmonaryArteriesToGround.GetComplianceBaseline().SetValue(0.0, VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& RightPulmonaryCapillariesToRightPulmonaryVeins = cCardiovascular.CreatePath(RightPulmonaryCapillaries, RightPulmonaryVeins, pulse::CardiovascularPath::RightPulmonaryCapillariesToRightPulmonaryVeins);
  RightPulmonaryCapillariesToRightPulmonaryVeins.GetResistanceBaseline().SetValue(ResistancePulmVeinsRight, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& RightPulmonaryCapillariesToGround = cCardiovascular.CreatePath(RightPulmonaryCapillaries, Ground, pulse::CardiovascularPath::RightPulmonaryCapillariesToGround);
  RightPulmonaryCapillariesToGround.GetComplianceBaseline().SetValue(0.0, VolumePerPressureUnit::mL_Per_mmHg);

  SEFluidCircuitPath& RightPulmonaryVeinsToRightIntermediatePulmonaryVeins = cCardiovascular.CreatePath(RightPulmonaryVeins, RightIntermediatePulmonaryVeins, pulse::CardiovascularPath::RightPulmonaryVeinsToRightIntermediatePulmonaryVeins);
  RightPulmonaryVeinsToRightIntermediatePulmonaryVeins.GetResistanceBaseline().SetValue(ResistanceHeartLeft, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& RightPulmonaryVeinsToGround = cCardiovascular.CreatePath(RightPulmonaryVeins, Ground, pulse::CardiovascularPath::RightPulmonaryVeinsToGround);
  RightPulmonaryVeinsToGround.GetComplianceBaseline().SetValue(0.0, VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& RightIntermediatePulmonaryVeinsToLeftHeart2 = cCardiovascular.CreatePath(RightIntermediatePulmonaryVeins, LeftHeart2, pulse::CardiovascularPath::RightIntermediatePulmonaryVeinsToLeftHeart2);
  //RightIntermediatePulmonaryVeinsToLeftHeart2.SetNextValve(eGate::Closed);

  SEFluidCircuitPath& MainPulmonaryArteriesToLeftIntermediatePulmonaryArteries = cCardiovascular.CreatePath(MainPulmonaryArteries, LeftIntermediatePulmonaryArteries, pulse::CardiovascularPath::MainPulmonaryArteriesToLeftIntermediatePulmonaryArteries);
  //MainPulmonaryArteriesToLeftIntermediatePulmonaryArteries.SetNextValve(eGate::Closed);
  SEFluidCircuitPath& LeftIntermediatePulmonaryArteriesToLeftPulmonaryArteries = cCardiovascular.CreatePath(LeftIntermediatePulmonaryArteries, LeftPulmonaryArteries, pulse::CardiovascularPath::LeftIntermediatePulmonaryArteriesToLeftPulmonaryArteries);
  LeftIntermediatePulmonaryArteriesToLeftPulmonaryArteries.GetResistanceBaseline().SetValue(ResistancePulmArtLeft, PressureTimePerVolumeUnit::mmHg_s_Per_mL);

  SEFluidCircuitPath& LeftPulmonaryArteriesToLeftPulmonaryVeins = cCardiovascular.CreatePath(LeftPulmonaryArteries, LeftPulmonaryVeins, pulse::CardiovascularPath::LeftPulmonaryArteriesToLeftPulmonaryVeins);
  LeftPulmonaryArteriesToLeftPulmonaryVeins.GetResistanceBaseline().SetValue(ResistanceShuntLeft, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& LeftPulmonaryArteriesToLeftPulmonaryCapillaries = cCardiovascular.CreatePath(LeftPulmonaryArteries, LeftPulmonaryCapillaries, pulse::CardiovascularPath::LeftPulmonaryArteriesToLeftPulmonaryCapillaries);
  LeftPulmonaryArteriesToLeftPulmonaryCapillaries.GetResistanceBaseline().SetValue(ResistancePulmCapLeft, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& LeftPulmonaryArteriesToGround = cCardiovascular.CreatePath(LeftPulmonaryArteries, Ground, pulse::CardiovascularPath::LeftPulmonaryArteriesToGround);
  LeftPulmonaryArteriesToGround.GetComplianceBaseline().SetValue(0.0, VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& LeftPulmonaryCapillariesToGround = cCardiovascular.CreatePath(LeftPulmonaryCapillaries, Ground, pulse::CardiovascularPath::LeftPulmonaryCapillariesToGround);
  LeftPulmonaryCapillariesToGround.GetComplianceBaseline().SetValue(0.0, VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& LeftPulmonaryCapillariesToLeftPulmonaryVeins = cCardiovascular.CreatePath(LeftPulmonaryCapillaries, LeftPulmonaryVeins, pulse::CardiovascularPath::LeftPulmonaryCapillariesToLeftPulmonaryVeins);
  LeftPulmonaryCapillariesToLeftPulmonaryVeins.GetResistanceBaseline().SetValue(ResistancePulmVeinsLeft, PressureTimePerVolumeUnit::mmHg_s_Per_mL);

  SEFluidCircuitPath& LeftPulmonaryVeinsToLeftIntermediatePulmonaryVeins = cCardiovascular.CreatePath(LeftPulmonaryVeins, LeftIntermediatePulmonaryVeins, pulse::CardiovascularPath::LeftPulmonaryVeinsToLeftIntermediatePulmonaryVeins);
  LeftPulmonaryVeinsToLeftIntermediatePulmonaryVeins.GetResistanceBaseline().SetValue(ResistanceHeartLeft, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& LeftPulmonaryVeinsToGround = cCardiovascular.CreatePath(LeftPulmonaryVeins, Ground, pulse::CardiovascularPath::LeftPulmonaryVeinsToGround);
  LeftPulmonaryVeinsToGround.GetComplianceBaseline().SetValue(0.0, VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& LeftIntermediatePulmonaryVeinsToLeftHeart2 = cCardiovascular.CreatePath(LeftIntermediatePulmonaryVeins, LeftHeart2, pulse::CardiovascularPath::LeftIntermediatePulmonaryVeinsToLeftHeart2);
  //LeftIntermediatePulmonaryVeinsToLeftHeart2.SetNextValve(eGate::Closed);
  SEFluidCircuitPath& LeftHeart2ToLeftHeart1 = cCardiovascular.CreatePath(LeftHeart2, LeftHeart1, pulse::CardiovascularPath::LeftHeart2ToLeftHeart1);
  LeftHeart2ToLeftHeart1.SetNextValve(eGate::Closed);
  SEFluidCircuitPath& LeftHeart1ToLeftHeart3 = cCardiovascular.CreatePath(LeftHeart1, LeftHeart3, pulse::CardiovascularPath::LeftHeart1ToLeftHeart3);

  SEFluidCircuitPath& LeftHeart3ToGround = cCardiovascular.CreatePath(Ground, LeftHeart3, pulse::CardiovascularPath::LeftHeart3ToGround);
  LeftHeart3ToGround.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::mmHg);
  SEFluidCircuitPath& LeftHeart1ToAorta2 = cCardiovascular.CreatePath(LeftHeart1, Aorta2, pulse::CardiovascularPath::LeftHeart1ToAorta2);
  LeftHeart1ToAorta2.SetNextValve(eGate::Closed);
  cCardiovascular.CreatePath(Aorta2, Aorta3, pulse::CardiovascularPath::Aorta2ToAorta3);
  SEFluidCircuitPath& Aorta3ToAorta1 = cCardiovascular.CreatePath(Aorta3, Aorta1, pulse::CardiovascularPath::Aorta3ToAorta1);
  Aorta3ToAorta1.GetResistanceBaseline().SetValue(ResistanceAorta, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& Aorta1ToAorta4 = cCardiovascular.CreatePath(Aorta1, Aorta4, pulse::CardiovascularPath::Aorta1ToAorta4);
  Aorta1ToAorta4.GetComplianceBaseline().SetValue(0.0, VolumePerPressureUnit::mL_Per_mmHg);

  SEFluidCircuitPath& Aorta1ToBrain1 = cCardiovascular.CreatePath(Aorta1, Brain1, pulse::CardiovascularPath::Aorta1ToBrain1);
  Aorta1ToBrain1.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceBrain, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& Brain1ToGround = cCardiovascular.CreatePath(Brain1, Ground, pulse::CardiovascularPath::Brain1ToGround);
  Brain1ToGround.GetComplianceBaseline().SetValue(0.0, VolumePerPressureUnit::mL_Per_mmHg);
  /*SEFluidCircuitPath& GroundToBrain1 = cCardiovascular.CreatePath(Ground, Brain1, pulse::CardiovascularPath::GroundToBrain1);
  GroundToBrain1.GetPressureSourceBaseline().SetValue(1.0, PressureUnit::mmHg);*/
  SEFluidCircuitPath& Brain1ToBrain2 = cCardiovascular.CreatePath(Brain1, Brain2, pulse::CardiovascularPath::Brain1ToBrain2);
  Brain1ToBrain2.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceBrainVenous, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& Brain2ToVenaCava = cCardiovascular.CreatePath(Brain2, VenaCava, pulse::CardiovascularPath::Brain2ToVenaCava);

  SEFluidCircuitPath& Aorta1ToBone1 = cCardiovascular.CreatePath(Aorta1, Bone1, pulse::CardiovascularPath::Aorta1ToBone1);
  Aorta1ToBone1.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceBone, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& Bone1ToGround = cCardiovascular.CreatePath(Bone1, Ground, pulse::CardiovascularPath::Bone1ToGround);
  Bone1ToGround.GetComplianceBaseline().SetValue(0.0, VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& Bone1ToBone2 = cCardiovascular.CreatePath(Bone1, Bone2, pulse::CardiovascularPath::Bone1ToBone2);
  Bone1ToBone2.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceBoneVenous, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& Bone2ToVenaCava = cCardiovascular.CreatePath(Bone2, VenaCava, pulse::CardiovascularPath::Bone2ToVenaCava);

  SEFluidCircuitPath& Aorta1ToFat1 = cCardiovascular.CreatePath(Aorta1, Fat1, pulse::CardiovascularPath::Aorta1ToFat1);
  Aorta1ToFat1.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceFat, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& Fat1ToGround = cCardiovascular.CreatePath(Fat1, Ground, pulse::CardiovascularPath::Fat1ToGround);
  Fat1ToGround.GetComplianceBaseline().SetValue(0.0, VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& Fat1ToFat2 = cCardiovascular.CreatePath(Fat1, Fat2, pulse::CardiovascularPath::Fat1ToFat2);
  Fat1ToFat2.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceFatVenous, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& Fat2ToVenaCava = cCardiovascular.CreatePath(Fat2, VenaCava, pulse::CardiovascularPath::Fat2ToVenaCava);

  SEFluidCircuitPath& Aorta1ToLargeIntestine = cCardiovascular.CreatePath(Aorta1, LargeIntestine, pulse::CardiovascularPath::Aorta1ToLargeIntestine);
  Aorta1ToLargeIntestine.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceLargeIntestine, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& LargeIntestineToGround = cCardiovascular.CreatePath(LargeIntestine, Ground, pulse::CardiovascularPath::LargeIntestineToGround);
  LargeIntestineToGround.GetComplianceBaseline().SetValue(0.0, VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& LargeIntestineToPortalVein = cCardiovascular.CreatePath(LargeIntestine, PortalVein, pulse::CardiovascularPath::LargeIntestineToPortalVein);
  LargeIntestineToPortalVein.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceLargeIntestineVenous, PressureTimePerVolumeUnit::mmHg_s_Per_mL);

  SEFluidCircuitPath& Aorta1ToLeftArm1 = cCardiovascular.CreatePath(Aorta1, LeftArm1, pulse::CardiovascularPath::Aorta1ToLeftArm1);
  Aorta1ToLeftArm1.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceArmLeft, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& LeftArm1ToGround = cCardiovascular.CreatePath(LeftArm1, Ground, pulse::CardiovascularPath::LeftArm1ToGround);
  LeftArm1ToGround.GetComplianceBaseline().SetValue(0.0, VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& LeftArm1ToLeftArm2 = cCardiovascular.CreatePath(LeftArm1, LeftArm2, pulse::CardiovascularPath::LeftArm1ToLeftArm2);
  LeftArm1ToLeftArm2.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceArmLeftVenous, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& LeftArm2ToVenaCava = cCardiovascular.CreatePath(LeftArm2, VenaCava, pulse::CardiovascularPath::LeftArm2ToVenaCava);

  SEFluidCircuitPath& Aorta1ToLeftKidney1 = cCardiovascular.CreatePath(Aorta1, LeftKidney1, pulse::CardiovascularPath::Aorta1ToLeftKidney1);
  Aorta1ToLeftKidney1.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceKidney, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& LeftKidney1ToGround = cCardiovascular.CreatePath(LeftKidney1, Ground, pulse::CardiovascularPath::LeftKidney1ToGround);
  LeftKidney1ToGround.GetComplianceBaseline().SetValue(0.0, VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& LeftKidney1ToLeftKidney2 = cCardiovascular.CreatePath(LeftKidney1, LeftKidney2, pulse::CardiovascularPath::LeftKidney1ToLeftKidney2);
  LeftKidney1ToLeftKidney2.GetResistanceBaseline().SetValue(ResistanceKidneyVenous, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& LeftKidney2ToVenaCava = cCardiovascular.CreatePath(LeftKidney2, VenaCava, pulse::CardiovascularPath::LeftKidney2ToVenaCava);

  SEFluidCircuitPath& Aorta1ToLeftLeg1 = cCardiovascular.CreatePath(Aorta1, LeftLeg1, pulse::CardiovascularPath::Aorta1ToLeftLeg1);
  Aorta1ToLeftLeg1.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceLegLeft, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& LeftLeg1ToGround = cCardiovascular.CreatePath(LeftLeg1, Ground, pulse::CardiovascularPath::LeftLeg1ToGround);
  LeftLeg1ToGround.GetComplianceBaseline().SetValue(0.0, VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& LeftLeg1ToLeftLeg2 = cCardiovascular.CreatePath(LeftLeg1, LeftLeg2, pulse::CardiovascularPath::LeftLeg1ToLeftLeg2);
  LeftLeg1ToLeftLeg2.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceLegLeftVenous, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& LeftLeg2ToVenaCava = cCardiovascular.CreatePath(LeftLeg2, VenaCava, pulse::CardiovascularPath::LeftLeg2ToVenaCava);

  SEFluidCircuitPath& Aorta1ToLiver1 = cCardiovascular.CreatePath(Aorta1, Liver1, pulse::CardiovascularPath::Aorta1ToLiver1);
  Aorta1ToLiver1.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceLiver, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& Liver1ToGround = cCardiovascular.CreatePath(Liver1, Ground, pulse::CardiovascularPath::Liver1ToGround);
  Liver1ToGround.GetComplianceBaseline().SetValue(0.0, VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& PortalVeinToLiver1 = cCardiovascular.CreatePath(PortalVein, Liver1, pulse::CardiovascularPath::PortalVeinToLiver1);
  PortalVeinToLiver1.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistancePortalVein, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& Liver1ToLiver2 = cCardiovascular.CreatePath(Liver1, Liver2, pulse::CardiovascularPath::Liver1ToLiver2);
  Liver1ToLiver2.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceLiverVenous, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& Liver2ToVenaCava = cCardiovascular.CreatePath(Liver2, VenaCava, pulse::CardiovascularPath::Liver2ToVenaCava);

  SEFluidCircuitPath& Aorta1ToMuscle1 = cCardiovascular.CreatePath(Aorta1, Muscle1, pulse::CardiovascularPath::Aorta1ToMuscle1);
  Aorta1ToMuscle1.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceMuscle, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& Muscle1ToGround = cCardiovascular.CreatePath(Muscle1, Ground, pulse::CardiovascularPath::Muscle1ToGround);
  Muscle1ToGround.GetComplianceBaseline().SetValue(0.0, VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& Muscle1ToMuscle2 = cCardiovascular.CreatePath(Muscle1, Muscle2, pulse::CardiovascularPath::Muscle1ToMuscle2);
  Muscle1ToMuscle2.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceMuscleVenous, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& Muscle2ToVenaCava = cCardiovascular.CreatePath(Muscle2, VenaCava, pulse::CardiovascularPath::Muscle2ToVenaCava);

  SEFluidCircuitPath& Aorta1ToMyocardium1 = cCardiovascular.CreatePath(Aorta1, Myocardium1, pulse::CardiovascularPath::Aorta1ToMyocardium1);
  Aorta1ToMyocardium1.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceMyocardium, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& Myocardium1ToGround = cCardiovascular.CreatePath(Myocardium1, Ground, pulse::CardiovascularPath::Myocardium1ToGround);
  Myocardium1ToGround.GetComplianceBaseline().SetValue(0.0, VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& Myocardium1ToMyocardium2 = cCardiovascular.CreatePath(Myocardium1, Myocardium2, pulse::CardiovascularPath::Myocardium1ToMyocardium2);
  Myocardium1ToMyocardium2.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceMyocardiumVenous, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& Myocardium2ToVenaCava = cCardiovascular.CreatePath(Myocardium2, VenaCava, pulse::CardiovascularPath::Myocardium2ToVenaCava);

  SEFluidCircuitPath& PericardiumToGround = cCardiovascular.CreatePath(Pericardium, Ground, pulse::CardiovascularPath::Pericardium1ToGround);
  SEFluidCircuitPath& GroundToPericardium = cCardiovascular.CreatePath(Ground, Pericardium, pulse::CardiovascularPath::GroundToPericardium1);
  GroundToPericardium.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);

  SEFluidCircuitPath& Aorta1ToRightArm1 = cCardiovascular.CreatePath(Aorta1, RightArm1, pulse::CardiovascularPath::Aorta1ToRightArm1);
  Aorta1ToRightArm1.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceArmRight, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& RightArm1ToGround = cCardiovascular.CreatePath(RightArm1, Ground, pulse::CardiovascularPath::RightArm1ToGround);
  RightArm1ToGround.GetComplianceBaseline().SetValue(0.0, VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& RightArm1ToRightArm2 = cCardiovascular.CreatePath(RightArm1, RightArm2, pulse::CardiovascularPath::RightArm1ToRightArm2);
  RightArm1ToRightArm2.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceArmRightVenous, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& RightArm2ToVenaCava = cCardiovascular.CreatePath(RightArm2, VenaCava, pulse::CardiovascularPath::RightArm2ToVenaCava);

  SEFluidCircuitPath& Aorta1ToRightKidney1 = cCardiovascular.CreatePath(Aorta1, RightKidney1, pulse::CardiovascularPath::Aorta1ToRightKidney1);
  Aorta1ToRightKidney1.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceKidney, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& RightKidney1ToGround = cCardiovascular.CreatePath(RightKidney1, Ground, pulse::CardiovascularPath::RightKidney1ToGround);
  RightKidney1ToGround.GetComplianceBaseline().SetValue(0.0, VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& RightKidney1ToRightKidney2 = cCardiovascular.CreatePath(RightKidney1, RightKidney2, pulse::CardiovascularPath::RightKidney1ToRightKidney2);
  RightKidney1ToRightKidney2.GetResistanceBaseline().SetValue(ResistanceKidneyVenous, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& RightKidney2ToVenaCava = cCardiovascular.CreatePath(RightKidney2, VenaCava, pulse::CardiovascularPath::RightKidney2ToVenaCava);

  SEFluidCircuitPath& Aorta1ToRightLeg1 = cCardiovascular.CreatePath(Aorta1, RightLeg1, pulse::CardiovascularPath::Aorta1ToRightLeg1);
  Aorta1ToRightLeg1.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceLegRight, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& RightLeg1ToGround = cCardiovascular.CreatePath(RightLeg1, Ground, pulse::CardiovascularPath::RightLeg1ToGround);
  RightLeg1ToGround.GetComplianceBaseline().SetValue(0.0, VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& RightLeg1ToRightLeg2 = cCardiovascular.CreatePath(RightLeg1, RightLeg2, pulse::CardiovascularPath::RightLeg1ToRightLeg2);
  RightLeg1ToRightLeg2.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceLegRightVenous, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& RightLeg2ToVenaCava = cCardiovascular.CreatePath(RightLeg2, VenaCava, pulse::CardiovascularPath::RightLeg2ToVenaCava);

  SEFluidCircuitPath& Aorta1ToSkin1 = cCardiovascular.CreatePath(Aorta1, Skin1, pulse::CardiovascularPath::Aorta1ToSkin1);
  Aorta1ToSkin1.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceSkin, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& Skin1ToGround = cCardiovascular.CreatePath(Skin1, Ground, pulse::CardiovascularPath::Skin1ToGround);
  Skin1ToGround.GetComplianceBaseline().SetValue(0.0, VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& Skin1ToSkin2 = cCardiovascular.CreatePath(Skin1, Skin2, pulse::CardiovascularPath::Skin1ToSkin2);
  Skin1ToSkin2.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceSkinVenous, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& Skin2ToVenaCava = cCardiovascular.CreatePath(Skin2, VenaCava, pulse::CardiovascularPath::Skin2ToVenaCava);

  SEFluidCircuitPath& Aorta1ToSmallIntestine = cCardiovascular.CreatePath(Aorta1, SmallIntestine, pulse::CardiovascularPath::Aorta1ToSmallIntestine);
  Aorta1ToSmallIntestine.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceSmallIntestine, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& SmallIntestineToGround = cCardiovascular.CreatePath(SmallIntestine, Ground, pulse::CardiovascularPath::SmallIntestineToGround);
  SmallIntestineToGround.GetComplianceBaseline().SetValue(0.0, VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& SmallIntestineToPortalVein = cCardiovascular.CreatePath(SmallIntestine, PortalVein, pulse::CardiovascularPath::SmallIntestineToPortalVein);
  SmallIntestineToPortalVein.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceSmallIntestineVenous, PressureTimePerVolumeUnit::mmHg_s_Per_mL);

  SEFluidCircuitPath& Aorta1ToSplanchnic = cCardiovascular.CreatePath(Aorta1, Splanchnic, pulse::CardiovascularPath::Aorta1ToSplanchnic);
  Aorta1ToSplanchnic.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceSplanchnic, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& SplanchnicToGround = cCardiovascular.CreatePath(Splanchnic, Ground, pulse::CardiovascularPath::SplanchnicToGround);
  SplanchnicToGround.GetComplianceBaseline().SetValue(0.0, VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& SplanchnicToPortalVein = cCardiovascular.CreatePath(Splanchnic, PortalVein, pulse::CardiovascularPath::SplanchnicToPortalVein);
  SplanchnicToPortalVein.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceSplanchnicVenous, PressureTimePerVolumeUnit::mmHg_s_Per_mL);

  SEFluidCircuitPath& Aorta1ToSpleen = cCardiovascular.CreatePath(Aorta1, Spleen, pulse::CardiovascularPath::Aorta1ToSpleen);
  Aorta1ToSpleen.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceSpleen, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& SpleenToGround = cCardiovascular.CreatePath(Spleen, Ground, pulse::CardiovascularPath::SpleenToGround);
  SpleenToGround.GetComplianceBaseline().SetValue(0.0, VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& SpleenToPortalVein = cCardiovascular.CreatePath(Spleen, PortalVein, pulse::CardiovascularPath::SpleenToPortalVein);
  SpleenToPortalVein.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceSpleenVenous, PressureTimePerVolumeUnit::mmHg_s_Per_mL);

  SEFluidCircuitPath& AbdominalCavityToGround = cCardiovascular.CreatePath(AbdominalCavity, Ground, pulse::CardiovascularPath::AbdominalCavity1ToGround);
  SEFluidCircuitPath& GroundToAbdominalCavity = cCardiovascular.CreatePath(Ground, AbdominalCavity, pulse::CardiovascularPath::GroundToAbdominalCavity1);
  GroundToAbdominalCavity.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);

  SEFluidCircuitPath& GroundToAorta4 = cCardiovascular.CreatePath(Ground, Aorta4, pulse::CardiovascularPath::GroundToAorta4);
  GroundToAorta4.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::mmHg);

  SEFluidCircuitPath& VenaCavaToGround = cCardiovascular.CreatePath(VenaCava, Ground, pulse::CardiovascularPath::VenaCavaToGround);
  VenaCavaToGround.GetComplianceBaseline().SetValue(0.0, VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& IVToVenaCava = cCardiovascular.CreatePath(Ground, VenaCava, pulse::CardiovascularPath::IVToVenaCava);
  IVToVenaCava.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);

  // Compute compliances from target pressures and baseline volumes
  for (SEFluidCircuitPath* p : cCardiovascular.GetPaths())
  {
    if (p->HasCapacitanceBaseline())
    {
      SEFluidCircuitNode& src = p->GetSourceNode();
      if (!src.HasVolumeBaseline())
        Fatal("Compliance paths must have a volume baseline.");
      double pressure = src.GetPressure(PressureUnit::mmHg);
      double volume = src.GetVolumeBaseline(VolumeUnit::mL);
      p->GetComplianceBaseline().SetValue(volume / pressure, VolumePerPressureUnit::mL_Per_mmHg);
    }
  }
  // The vena cava compliance needs to be decreased to ensure proper return
  double venaCavaComplianceTuning = 0.9;
  VenaCavaToGround.GetCapacitanceBaseline().SetValue(venaCavaComplianceTuning * VenaCavaToGround.GetComplianceBaseline().GetValue(VolumePerPressureUnit::mL_Per_mmHg), VolumePerPressureUnit::mL_Per_mmHg);

  // Hearts and pericardium have special compliance computations
  double InitialComplianceHeartRight = 1.0 / 0.0243;
  double InitialComplianceHeartLeft = 1.0 / 0.049;
  // Volumes are initialized from the volume baselines. The heart volume initialization is a little tricky. To much prime and the 
  // initial pressure wave will be devastating to the rest of the CV system during the first contraction phase. Too little prime 
  // and there will be issues with available flow as the elastance decreases during the first relaxation phase.
  // The 1/4 full initialization gives decent results.
  RightHeart1ToRightHeart3.GetComplianceBaseline().SetValue(InitialComplianceHeartRight, VolumePerPressureUnit::mL_Per_mmHg);
  LeftHeart1ToLeftHeart3.GetComplianceBaseline().SetValue(InitialComplianceHeartLeft, VolumePerPressureUnit::mL_Per_mmHg);
  PericardiumToGround.GetComplianceBaseline().SetValue(100.0, VolumePerPressureUnit::mL_Per_mmHg);

  //Make these polarized, so chest compression can't give a negative volume
  RightHeart1ToRightHeart3.SetNextPolarizedState(eGate::Closed);
  LeftHeart1ToLeftHeart3.SetNextPolarizedState(eGate::Closed);

  double VolumeModifierAorta = 1.16722 * 1.018749, VolumeModifierBrain = 0.998011 * 1.038409, VolumeModifierBone = 1.175574 * 0.985629, VolumeModifierFat = 1.175573 * 0.986527;
  double VolumeModifierLargeIntestine = 1.17528 * 0.985609, VolumeModifierArmL = 1.175573 * 0.986529, VolumeModifierKidneyL = 0.737649 * 0.954339, VolumeModifierLegL = 1.175573 * 0.986529;
  double VolumeModifierPulmArtL = 0.855566 * 1.095697, VolumeModifierPulmCapL = 0.724704 * 1.079139, VolumeModifierPulmVeinL = 0.548452 * 1.056844 * 1.062, VolumeModifierLiver = 1.157475 * 0.991848;
  double VolumeModifierMuscle = 1.175573 * 0.986529, VolumeModifierMyocard = 1.175564 * 0.986531, VolumeModifierArmR = 1.175573 * 0.986529, VolumeModifierKidneyR = 0.737649 * 0.954339;
  double VolumeModifierLegR = 1.175573 * 0.986529, VolumeModifierPulmArtR = 0.756158 * 1.121167, VolumeModifierPulmCapR = 0.602545 * 1.118213, VolumeModifierPulmVeinR = 0.395656 * 1.11424 * 1.11;
  double VolumeModifierSkin = 1.007306 * 1.035695, VolumeModifierSmallIntestine = 1.17528 * 0.986509, VolumeModifierSplanchnic = 1.17528 * 0.986509, VolumeModifierSpleen = 1.17528 * 0.986509;
  double VolumeModifierVenaCava = 0.66932 * 1.134447;

  //And also modify the compliances
  Aorta1ToAorta4.GetComplianceBaseline().SetValue(largeArteriesComplianceModifier * Aorta1ToAorta4.GetComplianceBaseline(VolumePerPressureUnit::mL_Per_mmHg), VolumePerPressureUnit::mL_Per_mmHg);

  //For Internal Hemorrhage
  AbdominalCavityToGround.GetComplianceBaseline().SetValue(100.0, VolumePerPressureUnit::mL_Per_mmHg);

  RightPulmonaryArteries.GetVolumeBaseline().SetValue(VolumeModifierPulmArtR * RightPulmonaryArteries.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  LeftPulmonaryArteries.GetVolumeBaseline().SetValue(VolumeModifierPulmArtL * LeftPulmonaryArteries.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  RightPulmonaryCapillaries.GetVolumeBaseline().SetValue(VolumeModifierPulmCapR * RightPulmonaryCapillaries.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  LeftPulmonaryCapillaries.GetVolumeBaseline().SetValue(VolumeModifierPulmCapL * LeftPulmonaryCapillaries.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  RightPulmonaryVeins.GetVolumeBaseline().SetValue(VolumeModifierPulmVeinR * RightPulmonaryVeins.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  LeftPulmonaryVeins.GetVolumeBaseline().SetValue(VolumeModifierPulmVeinL * LeftPulmonaryVeins.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  Aorta1.GetVolumeBaseline().SetValue(VolumeModifierAorta * Aorta1.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  Brain1.GetVolumeBaseline().SetValue(VolumeModifierBrain * Brain1.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  Bone1.GetVolumeBaseline().SetValue(VolumeModifierBone * Bone1.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  Fat1.GetVolumeBaseline().SetValue(VolumeModifierFat * Fat1.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  LargeIntestine.GetVolumeBaseline().SetValue(VolumeModifierLargeIntestine * LargeIntestine.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  Liver1.GetVolumeBaseline().SetValue(VolumeModifierLiver * Liver1.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  LeftArm1.GetVolumeBaseline().SetValue(VolumeModifierArmL * LeftArm1.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  LeftKidney1.GetVolumeBaseline().SetValue(VolumeModifierKidneyL * LeftKidney1.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  LeftLeg1.GetVolumeBaseline().SetValue(VolumeModifierLegL * LeftLeg1.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  Muscle1.GetVolumeBaseline().SetValue(VolumeModifierMuscle * Muscle1.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  Myocardium1.GetVolumeBaseline().SetValue(VolumeModifierMyocard * Myocardium1.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  RightArm1.GetVolumeBaseline().SetValue(VolumeModifierArmR * RightArm1.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  RightKidney1.GetVolumeBaseline().SetValue(VolumeModifierKidneyR * RightKidney1.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  RightLeg1.GetVolumeBaseline().SetValue(VolumeModifierLegR * RightLeg1.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  Skin1.GetVolumeBaseline().SetValue(VolumeModifierSkin * Skin1.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  SmallIntestine.GetVolumeBaseline().SetValue(VolumeModifierSmallIntestine * SmallIntestine.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  Splanchnic.GetVolumeBaseline().SetValue(VolumeModifierSplanchnic * Splanchnic.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  Spleen.GetVolumeBaseline().SetValue(VolumeModifierSpleen * Spleen.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  VenaCava.GetVolumeBaseline().SetValue(VolumeModifierVenaCava * VenaCava.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);

  // Prepare circuit for compartment creation
  cCardiovascular.SetNextAndCurrentFromBaselines();
  cCardiovascular.StateChange();

  SEFluidCircuit& cCombinedCardiovascular = m_Circuits->GetActiveCardiovascularCircuit();
  cCombinedCardiovascular.AddCircuit(cCardiovascular);
  cCombinedCardiovascular.SetNextAndCurrentFromBaselines();
  cCombinedCardiovascular.StateChange();

  /////////////////////////
  // Create Compartments //
  /////////////////////////

  /////////////////
  // Right Heart //
  SELiquidCompartment& vRightHeart = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::RightHeart);
  vRightHeart.MapNode(RightHeart1);
  vRightHeart.MapNode(RightHeart2);
  vRightHeart.MapNode(RightHeart3);
  vRightHeart.MapNode(MainPulmonaryArteries);
  //////////////////////////////
  // Right Pulmonary Arteries //
  SELiquidCompartment& vRightPulmonaryArteries = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::RightPulmonaryArteries);
  vRightPulmonaryArteries.MapNode(RightPulmonaryArteries);
  vRightPulmonaryArteries.MapNode(RightIntermediatePulmonaryArteries);
  /////////////////////////////
  // Left Pulmonary Arteries //
  SELiquidCompartment& vLeftPulmonaryArteries = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::LeftPulmonaryArteries);
  vLeftPulmonaryArteries.MapNode(LeftPulmonaryArteries);
  vLeftPulmonaryArteries.MapNode(LeftIntermediatePulmonaryArteries);
  ////////////////////////
  // Pulmonary Arteries //
  SELiquidCompartment& vPulmonaryArteries = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::PulmonaryArteries);
  vPulmonaryArteries.AddChild(vRightPulmonaryArteries);
  vPulmonaryArteries.AddChild(vLeftPulmonaryArteries);
  /////////////////////////////////
  // Right Pulmonary Capillaries //
  SELiquidCompartment& vRightPulmonaryCapillaries = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::RightPulmonaryCapillaries);
  vRightPulmonaryCapillaries.MapNode(RightPulmonaryCapillaries);
  ////////////////////////////////
  // Left Pulmonary Capillaries //
  SELiquidCompartment& vLeftPulmonaryCapillaries = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::LeftPulmonaryCapillaries);
  vLeftPulmonaryCapillaries.MapNode(LeftPulmonaryCapillaries);
  ///////////////////////////
  // Pulmonary Capillaries //
  SELiquidCompartment& vPulmonaryCapillaries = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::PulmonaryCapillaries);
  vPulmonaryCapillaries.AddChild(vRightPulmonaryCapillaries);
  vPulmonaryCapillaries.AddChild(vLeftPulmonaryCapillaries);
  ///////////////////////////
  // Right Pulmonary Veins //
  SELiquidCompartment& vRightPulmonaryVeins = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::RightPulmonaryVeins);
  vRightPulmonaryVeins.MapNode(RightPulmonaryVeins);
  //////////////////////////
  // Left Pulmonary Veins //
  SELiquidCompartment& vLeftPulmonaryVeins = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::LeftPulmonaryVeins);
  vLeftPulmonaryVeins.MapNode(LeftPulmonaryVeins);
  /////////////////////
  // Pulmonary Veins //
  SELiquidCompartment& vPulmonaryVeins = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::PulmonaryVeins);
  vPulmonaryVeins.AddChild(vRightPulmonaryVeins);
  vPulmonaryVeins.AddChild(vLeftPulmonaryVeins);
  ////////////////
  // Left Heart //
  SELiquidCompartment& vLeftHeart = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::LeftHeart);
  vLeftHeart.MapNode(LeftHeart1);
  vLeftHeart.MapNode(LeftHeart2);
  vLeftHeart.MapNode(LeftHeart3);
  ///////////
  // Aorta //
  SELiquidCompartment& vAorta = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::Aorta);
  vAorta.MapNode(Aorta1);
  vAorta.MapNode(Aorta2);
  vAorta.MapNode(Aorta3);
  vAorta.MapNode(Aorta4);
  ///////////
  // Abdominal Cavity //
  SELiquidCompartment& vAbdominalCavity = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::AbdominalCavity);
  vAbdominalCavity.MapNode(AbdominalCavity);
  // Brain //
  SELiquidCompartment& vBrain = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::Brain);
  vBrain.MapNode(Brain1);
  vBrain.MapNode(Brain2);
  //////////
  // Bone //
  SELiquidCompartment& vBone = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::Bone);
  vBone.MapNode(Bone1);
  vBone.MapNode(Bone2);
  /////////
  // Fat //
  SELiquidCompartment& vFat = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::Fat);
  vFat.MapNode(Fat1);
  vFat.MapNode(Fat2);
  /////////////////////
  // Large Intestine //
  SELiquidCompartment& vLargeIntestine = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::LargeIntestine);
  vLargeIntestine.MapNode(LargeIntestine);
  ///////////
  // Liver //
  SELiquidCompartment& vLiver = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::Liver);
  vLiver.MapNode(Liver1);
  vLiver.MapNode(Liver2);
  vLiver.MapNode(PortalVein);
  //////////////
  // Left Arm //
  SELiquidCompartment& vLeftArm = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::LeftArm);
  vLeftArm.MapNode(LeftArm1);
  vLeftArm.MapNode(LeftArm2);
  /////////////////
  // Left Kidney //
  SELiquidCompartment& vLeftKidney = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::LeftKidney);
  vLeftKidney.MapNode(LeftKidney1);
  vLeftKidney.MapNode(LeftKidney2);
  //////////////
  // Left Leg //
  SELiquidCompartment& vLeftLeg = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::LeftLeg);
  vLeftLeg.MapNode(LeftLeg1);
  vLeftLeg.MapNode(LeftLeg2);
  ////////////
  // Muscle //
  SELiquidCompartment& vMuscle = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::Muscle);
  vMuscle.MapNode(Muscle1);
  vMuscle.MapNode(Muscle2);
  ////////////////
  // Myocardium //
  SELiquidCompartment& vMyocardium = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::Myocardium);
  vMyocardium.MapNode(Myocardium1);
  vMyocardium.MapNode(Myocardium2);
  /////////////////
  // Pericardium //
  SELiquidCompartment& vPericardium = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::Pericardium);
  vPericardium.MapNode(Pericardium);
  ///////////////
  // Right Arm //
  SELiquidCompartment& vRightArm = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::RightArm);
  vRightArm.MapNode(RightArm1);
  vRightArm.MapNode(RightArm2);
  //////////////////
  // Right Kidney // 
  SELiquidCompartment& vRightKidney = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::RightKidney);
  vRightKidney.MapNode(RightKidney1);
  vRightKidney.MapNode(RightKidney2);
  ///////////////
  // Right Leg //
  SELiquidCompartment& vRightLeg = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::RightLeg);
  vRightLeg.MapNode(RightLeg1);
  vRightLeg.MapNode(RightLeg2);
  //////////
  // Skin //
  SELiquidCompartment& vSkin = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::Skin);
  vSkin.MapNode(Skin1);
  vSkin.MapNode(Skin2);
  /////////////////////
  // Small Intestine //
  SELiquidCompartment& vSmallIntestine = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::SmallIntestine);
  vSmallIntestine.MapNode(SmallIntestine);
  ////////////////
  // Splanchnic //
  SELiquidCompartment& vSplanchnic = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::Splanchnic);
  vSplanchnic.MapNode(Splanchnic);
  ////////////
  // Spleen //
  SELiquidCompartment& vSpleen = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::Spleen);
  vSpleen.MapNode(Spleen);
  //////////////
  // VenaCava //
  SELiquidCompartment& vVenaCava = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::VenaCava);
  vVenaCava.MapNode(VenaCava);
  ////////////
  // Ground //
  SELiquidCompartment& vGround = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::Ground);
  vGround.MapNode(Ground);

  //////////////////////////
  // Set up our hierarchy //
  //////////////////////////
  SELiquidCompartment& vKidneys = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::Kidneys);
  vKidneys.AddChild(vLeftKidney);
  vKidneys.AddChild(vRightKidney);
  SELiquidCompartment& vHeart = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::Heart);
  vHeart.AddChild(vMyocardium);
  vHeart.AddChild(vLeftHeart);
  vHeart.AddChild(vRightHeart);
  vHeart.AddChild(vPericardium);
  SELiquidCompartment& vLeftLung = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::LeftLung);
  vLeftLung.AddChild(vLeftPulmonaryArteries);
  vLeftLung.AddChild(vLeftPulmonaryCapillaries);
  vLeftLung.AddChild(vLeftPulmonaryVeins);
  SELiquidCompartment& vRightLung = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::RightLung);
  vRightLung.AddChild(vRightPulmonaryArteries);
  vRightLung.AddChild(vRightPulmonaryCapillaries);
  vRightLung.AddChild(vRightPulmonaryVeins);
  SELiquidCompartment& vLungs = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::Lungs);
  vLungs.AddChild(vLeftLung);
  vLungs.AddChild(vRightLung);
  SELiquidCompartment& vGut = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::Gut);
  vGut.AddChild(vSplanchnic);
  vGut.AddChild(vSmallIntestine);
  vGut.AddChild(vLargeIntestine);
  SELiquidCompartment& vAbdomen = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::Abdomen);
  vAbdomen.AddChild(vSplanchnic);
  vAbdomen.AddChild(vSmallIntestine);
  vAbdomen.AddChild(vLargeIntestine);
  vAbdomen.AddChild(vLeftKidney);
  vAbdomen.AddChild(vRightKidney);
  vAbdomen.AddChild(vSpleen);
  vAbdomen.AddChild(vLiver);
  vAbdomen.AddChild(vVenaCava);
  vAbdomen.AddChild(vAorta);
  vAbdomen.AddChild(vAbdominalCavity);


  //////////////////
  // Create Links //
  //////////////////

  /////////////////////
  // Heart and Lungs //
  SELiquidCompartmentLink& vVenaCavaToRightHeart = m_Compartments->CreateLiquidLink(vVenaCava, vRightHeart, pulse::VascularLink::VenaCavaToRightHeart);
  vVenaCavaToRightHeart.MapPath(VenaCavaToRightHeart2);
  SELiquidCompartmentLink& vRightHeartToLeftPulmonaryArteries = m_Compartments->CreateLiquidLink(vRightHeart, vLeftPulmonaryArteries, pulse::VascularLink::RightHeartToLeftPulmonaryArteries);
  vRightHeartToLeftPulmonaryArteries.MapPath(MainPulmonaryArteriesToLeftIntermediatePulmonaryArteries);
  SELiquidCompartmentLink& vLeftPulmonaryArteriesToCapillaries = m_Compartments->CreateLiquidLink(vLeftPulmonaryArteries, vLeftPulmonaryCapillaries, pulse::VascularLink::LeftPulmonaryArteriesToCapillaries);
  vLeftPulmonaryArteriesToCapillaries.MapPath(LeftPulmonaryArteriesToLeftPulmonaryCapillaries);
  SELiquidCompartmentLink& vLeftPulmonaryArteriesToVeins = m_Compartments->CreateLiquidLink(vLeftPulmonaryArteries, vLeftPulmonaryVeins, pulse::VascularLink::LeftPulmonaryArteriesToVeins);
  vLeftPulmonaryArteriesToVeins.MapPath(LeftPulmonaryArteriesToLeftPulmonaryVeins);
  SELiquidCompartmentLink& vLeftPulmonaryCapillariesToVeins = m_Compartments->CreateLiquidLink(vLeftPulmonaryCapillaries, vLeftPulmonaryVeins, pulse::VascularLink::LeftPulmonaryCapillariesToVeins);
  vLeftPulmonaryCapillariesToVeins.MapPath(LeftPulmonaryCapillariesToLeftPulmonaryVeins);
  SELiquidCompartmentLink& vLeftPulmonaryVeinsToLeftHeart = m_Compartments->CreateLiquidLink(vLeftPulmonaryVeins, vLeftHeart, pulse::VascularLink::LeftPulmonaryVeinsToLeftHeart);
  vLeftPulmonaryVeinsToLeftHeart.MapPath(LeftIntermediatePulmonaryVeinsToLeftHeart2);
  SELiquidCompartmentLink& vRightHeartToRightPulmonaryArteries = m_Compartments->CreateLiquidLink(vRightHeart, vRightPulmonaryArteries, pulse::VascularLink::RightHeartToRightPulmonaryArteries);
  vRightHeartToRightPulmonaryArteries.MapPath(MainPulmonaryArteriesToRightIntermediatePulmonaryArteries);
  SELiquidCompartmentLink& vRightPulmonaryArteriesToCapillaries = m_Compartments->CreateLiquidLink(vRightPulmonaryArteries, vRightPulmonaryCapillaries, pulse::VascularLink::RightPulmonaryArteriesToCapillaries);
  vRightPulmonaryArteriesToCapillaries.MapPath(RightPulmonaryArteriesToRightPulmonaryCapillaries);
  SELiquidCompartmentLink& vRightPulmonaryArteriesToVeins = m_Compartments->CreateLiquidLink(vRightPulmonaryArteries, vRightPulmonaryVeins, pulse::VascularLink::RightPulmonaryArteriesToVeins);
  vRightPulmonaryArteriesToVeins.MapPath(RightPulmonaryArteriesToRightPulmonaryVeins);
  SELiquidCompartmentLink& vRightPulmonaryCapillariesToVeins = m_Compartments->CreateLiquidLink(vRightPulmonaryCapillaries, vRightPulmonaryVeins, pulse::VascularLink::RightPulmonaryCapillariesToVeins);
  vRightPulmonaryCapillariesToVeins.MapPath(RightPulmonaryCapillariesToRightPulmonaryVeins);
  SELiquidCompartmentLink& vRightPulmonaryVeinsToLeftHeart = m_Compartments->CreateLiquidLink(vRightPulmonaryVeins, vLeftHeart, pulse::VascularLink::RightPulmonaryVeinsToLeftHeart);
  vRightPulmonaryVeinsToLeftHeart.MapPath(RightIntermediatePulmonaryVeinsToLeftHeart2);
  SELiquidCompartmentLink& vLeftHeartToAorta = m_Compartments->CreateLiquidLink(vLeftHeart, vAorta, pulse::VascularLink::LeftHeartToAorta);
  vLeftHeartToAorta.MapPath(LeftHeart1ToAorta2);
  //////////
  // Bone //
  SELiquidCompartmentLink& vAortaToBone = m_Compartments->CreateLiquidLink(vAorta, vBone, pulse::VascularLink::AortaToBone);
  vAortaToBone.MapPath(Aorta1ToBone1);
  SELiquidCompartmentLink& vBoneToVenaCava = m_Compartments->CreateLiquidLink(vBone, vVenaCava, pulse::VascularLink::BoneToVenaCava);
  vBoneToVenaCava.MapPath(Bone2ToVenaCava);
  ///////////
  // Brain //
  SELiquidCompartmentLink& vAortaToBrain = m_Compartments->CreateLiquidLink(vAorta, vBrain, pulse::VascularLink::AortaToBrain);
  vAortaToBrain.MapPath(Aorta1ToBrain1);
  SELiquidCompartmentLink& vBrainToVenaCava = m_Compartments->CreateLiquidLink(vBrain, vVenaCava, pulse::VascularLink::BrainToVenaCava);
  vBrainToVenaCava.MapPath(Brain2ToVenaCava);
  /////////
  // Fat //
  SELiquidCompartmentLink& vAortaToFat = m_Compartments->CreateLiquidLink(vAorta, vFat, pulse::VascularLink::AortaToFat);
  vAortaToFat.MapPath(Aorta1ToFat1);
  SELiquidCompartmentLink& vFatToVenaCava = m_Compartments->CreateLiquidLink(vFat, vVenaCava, pulse::VascularLink::FatToVenaCava);
  vFatToVenaCava.MapPath(Fat2ToVenaCava);
  /////////////////////
  // Large Intestine //
  SELiquidCompartmentLink& vAortaToLargeIntestine = m_Compartments->CreateLiquidLink(vAorta, vLargeIntestine, pulse::VascularLink::AortaToLargeIntestine);
  vAortaToLargeIntestine.MapPath(Aorta1ToLargeIntestine);
  SELiquidCompartmentLink& vLargeIntestineToLiver = m_Compartments->CreateLiquidLink(vLargeIntestine, vLiver, pulse::VascularLink::LargeIntestineToLiver);
  vLargeIntestineToLiver.MapPath(LargeIntestineToPortalVein);
  ///////////
  // Liver //
  SELiquidCompartmentLink& vAortaToLiver = m_Compartments->CreateLiquidLink(vAorta, vLiver, pulse::VascularLink::AortaToLiver);
  vAortaToLiver.MapPath(Aorta1ToLiver1);
  SELiquidCompartmentLink& vLiverToVenaCava = m_Compartments->CreateLiquidLink(vLiver, vVenaCava, pulse::VascularLink::LiverToVenaCava);
  vLiverToVenaCava.MapPath(Liver2ToVenaCava);
  //////////////
  // Left Arm //
  SELiquidCompartmentLink& vAortaToLeftArm = m_Compartments->CreateLiquidLink(vAorta, vLeftArm, pulse::VascularLink::AortaToLeftArm);
  vAortaToLeftArm.MapPath(Aorta1ToLeftArm1);
  SELiquidCompartmentLink& vLeftArmToVenaCava = m_Compartments->CreateLiquidLink(vLeftArm, vVenaCava, pulse::VascularLink::LeftArmToVenaCava);
  vLeftArmToVenaCava.MapPath(LeftArm2ToVenaCava);
  /////////////////
  // Left Kidney //
  SELiquidCompartmentLink& vAortaToLeftKidney = m_Compartments->CreateLiquidLink(vAorta, vLeftKidney, pulse::VascularLink::AortaToLeftKidney);
  vAortaToLeftKidney.MapPath(Aorta1ToLeftKidney1);
  SELiquidCompartmentLink& vLeftKidneyToVenaCava = m_Compartments->CreateLiquidLink(vLeftKidney, vVenaCava, pulse::VascularLink::LeftKidneyToVenaCava);
  vLeftKidneyToVenaCava.MapPath(LeftKidney2ToVenaCava);
  //////////////
  // Left Leg //
  SELiquidCompartmentLink& vAortaToLeftLeg = m_Compartments->CreateLiquidLink(vAorta, vLeftLeg, pulse::VascularLink::AortaToLeftLeg);
  vAortaToLeftLeg.MapPath(Aorta1ToLeftLeg1);
  SELiquidCompartmentLink& vLeftLegToVenaCava = m_Compartments->CreateLiquidLink(vLeftLeg, vVenaCava, pulse::VascularLink::LeftLegToVenaCava);
  vLeftLegToVenaCava.MapPath(LeftLeg2ToVenaCava);
  ////////////
  // Muscle //
  SELiquidCompartmentLink& vAortaToMuscle = m_Compartments->CreateLiquidLink(vAorta, vMuscle, pulse::VascularLink::AortaToMuscle);
  vAortaToMuscle.MapPath(Aorta1ToMuscle1);
  SELiquidCompartmentLink& vMuscleToVenaCava = m_Compartments->CreateLiquidLink(vMuscle, vVenaCava, pulse::VascularLink::MuscleToVenaCava);
  vMuscleToVenaCava.MapPath(Muscle2ToVenaCava);
  ////////////////
  // Myocardium //
  SELiquidCompartmentLink& vAortaToMyocardium = m_Compartments->CreateLiquidLink(vAorta, vMyocardium, pulse::VascularLink::AortaToMyocardium);
  vAortaToMyocardium.MapPath(Aorta1ToMyocardium1);
  SELiquidCompartmentLink& vMyocardiumToVenaCava = m_Compartments->CreateLiquidLink(vMyocardium, vVenaCava, pulse::VascularLink::MyocardiumToVenaCava);
  vMyocardiumToVenaCava.MapPath(Myocardium2ToVenaCava);
  ///////////////
  // Right Arm //
  SELiquidCompartmentLink& vAortaToRightArm = m_Compartments->CreateLiquidLink(vAorta, vRightArm, pulse::VascularLink::AortaToRightArm);
  vAortaToRightArm.MapPath(Aorta1ToRightArm1);
  SELiquidCompartmentLink& vRightArmToVenaCava = m_Compartments->CreateLiquidLink(vRightArm, vVenaCava, pulse::VascularLink::RightArmToVenaCava);
  vRightArmToVenaCava.MapPath(RightArm2ToVenaCava);
  //////////////////
  // Right Kidney // 
  SELiquidCompartmentLink& vAortaToRightKidney = m_Compartments->CreateLiquidLink(vAorta, vRightKidney, pulse::VascularLink::AortaToRightKidney);
  vAortaToRightKidney.MapPath(Aorta1ToRightKidney1);
  SELiquidCompartmentLink& vRightKidneyToVenaCava = m_Compartments->CreateLiquidLink(vRightKidney, vVenaCava, pulse::VascularLink::RightKidneyToVenaCava);
  vRightKidneyToVenaCava.MapPath(RightKidney2ToVenaCava);
  ///////////////
  // Right Leg //
  SELiquidCompartmentLink& vAortaToRightLeg = m_Compartments->CreateLiquidLink(vAorta, vRightLeg, pulse::VascularLink::AortaToRightLeg);
  vAortaToRightLeg.MapPath(Aorta1ToRightLeg1);
  SELiquidCompartmentLink& vRightLegToVenaCava = m_Compartments->CreateLiquidLink(vRightLeg, vVenaCava, pulse::VascularLink::RightLegToVenaCava);
  vRightLegToVenaCava.MapPath(RightLeg2ToVenaCava);
  //////////
  // Skin //
  SELiquidCompartmentLink& vAortaToSkin = m_Compartments->CreateLiquidLink(vAorta, vSkin, pulse::VascularLink::AortaToSkin);
  vAortaToSkin.MapPath(Aorta1ToSkin1);
  SELiquidCompartmentLink& vSkinToVenaCava = m_Compartments->CreateLiquidLink(vSkin, vVenaCava, pulse::VascularLink::SkinToVenaCava);
  vSkinToVenaCava.MapPath(Skin2ToVenaCava);
  /////////////////////
  // Small Intestine //
  SELiquidCompartmentLink& vAortaToSmallIntestine = m_Compartments->CreateLiquidLink(vAorta, vSmallIntestine, pulse::VascularLink::AortaToSmallIntestine);
  vAortaToSmallIntestine.MapPath(Aorta1ToSmallIntestine);
  SELiquidCompartmentLink& vSmallIntestineToLiver = m_Compartments->CreateLiquidLink(vSmallIntestine, vLiver, pulse::VascularLink::SmallIntestineToLiver);
  vSmallIntestineToLiver.MapPath(SmallIntestineToPortalVein);
  ////////////////
  // Splanchnic //
  SELiquidCompartmentLink& vAortaToSplanchnic = m_Compartments->CreateLiquidLink(vAorta, vSplanchnic, pulse::VascularLink::AortaToSplanchnic);
  vAortaToSplanchnic.MapPath(Aorta1ToSplanchnic);
  SELiquidCompartmentLink& vSplanchnicToLiver = m_Compartments->CreateLiquidLink(vSplanchnic, vLiver, pulse::VascularLink::SplanchnicToLiver);
  vSplanchnicToLiver.MapPath(SplanchnicToPortalVein);
  ////////////
  // Spleen //
  SELiquidCompartmentLink& vAortaToSpleen = m_Compartments->CreateLiquidLink(vAorta, vSpleen, pulse::VascularLink::AortaToSpleen);
  vAortaToSpleen.MapPath(Aorta1ToSpleen);
  SELiquidCompartmentLink& vSpleenToLiver = m_Compartments->CreateLiquidLink(vSpleen, vLiver, pulse::VascularLink::SpleenToLiver);
  vSpleenToLiver.MapPath(SpleenToPortalVein);

  /////////////////////
  // Bleeds and IV's //
  SELiquidCompartmentLink& vVenaCavaIV = m_Compartments->CreateLiquidLink(vGround, vVenaCava, pulse::VascularLink::VenaCavaIV);
  vVenaCavaIV.MapPath(IVToVenaCava);

  SELiquidCompartmentGraph& gCardiovascular = m_Compartments->GetCardiovascularGraph();
  gCardiovascular.AddCompartment(vRightHeart);
  gCardiovascular.AddCompartment(vRightPulmonaryArteries);
  gCardiovascular.AddCompartment(vLeftPulmonaryArteries);
  gCardiovascular.AddCompartment(vRightPulmonaryCapillaries);
  gCardiovascular.AddCompartment(vLeftPulmonaryCapillaries);
  gCardiovascular.AddCompartment(vRightPulmonaryVeins);
  gCardiovascular.AddCompartment(vLeftPulmonaryVeins);
  gCardiovascular.AddCompartment(vLeftHeart);
  gCardiovascular.AddCompartment(vAorta);
  gCardiovascular.AddCompartment(vBrain);
  gCardiovascular.AddCompartment(vBone);
  gCardiovascular.AddCompartment(vFat);
  gCardiovascular.AddCompartment(vLargeIntestine);
  gCardiovascular.AddCompartment(vLiver);
  gCardiovascular.AddCompartment(vLeftArm);
  gCardiovascular.AddCompartment(vLeftKidney);
  gCardiovascular.AddCompartment(vLeftLeg);
  gCardiovascular.AddCompartment(vMuscle);
  gCardiovascular.AddCompartment(vMyocardium);
  gCardiovascular.AddCompartment(vRightArm);
  gCardiovascular.AddCompartment(vRightKidney);
  gCardiovascular.AddCompartment(vRightLeg);
  gCardiovascular.AddCompartment(vSkin);
  gCardiovascular.AddCompartment(vSmallIntestine);
  gCardiovascular.AddCompartment(vSplanchnic);
  gCardiovascular.AddCompartment(vSpleen);
  gCardiovascular.AddCompartment(vVenaCava);
  gCardiovascular.AddLink(vVenaCavaToRightHeart);
  gCardiovascular.AddLink(vRightHeartToLeftPulmonaryArteries);
  gCardiovascular.AddLink(vLeftPulmonaryArteriesToCapillaries);
  gCardiovascular.AddLink(vLeftPulmonaryArteriesToVeins);
  gCardiovascular.AddLink(vLeftPulmonaryCapillariesToVeins);
  gCardiovascular.AddLink(vLeftPulmonaryVeinsToLeftHeart);
  gCardiovascular.AddLink(vRightHeartToRightPulmonaryArteries);
  gCardiovascular.AddLink(vRightPulmonaryArteriesToCapillaries);
  gCardiovascular.AddLink(vRightPulmonaryArteriesToVeins);
  gCardiovascular.AddLink(vRightPulmonaryCapillariesToVeins);
  gCardiovascular.AddLink(vRightPulmonaryVeinsToLeftHeart);
  gCardiovascular.AddLink(vLeftHeartToAorta);
  gCardiovascular.AddLink(vAortaToBone);
  gCardiovascular.AddLink(vBoneToVenaCava);
  gCardiovascular.AddLink(vAortaToBrain);
  gCardiovascular.AddLink(vBrainToVenaCava);
  gCardiovascular.AddLink(vAortaToFat);
  gCardiovascular.AddLink(vFatToVenaCava);
  gCardiovascular.AddLink(vAortaToLargeIntestine);
  gCardiovascular.AddLink(vLargeIntestineToLiver);
  gCardiovascular.AddLink(vAortaToLeftArm);
  gCardiovascular.AddLink(vLeftArmToVenaCava);
  gCardiovascular.AddLink(vAortaToLeftKidney);
  gCardiovascular.AddLink(vLeftKidneyToVenaCava);
  gCardiovascular.AddLink(vAortaToLeftLeg);
  gCardiovascular.AddLink(vLeftLegToVenaCava);
  gCardiovascular.AddLink(vAortaToLiver);
  gCardiovascular.AddLink(vLiverToVenaCava);
  gCardiovascular.AddLink(vAortaToMuscle);
  gCardiovascular.AddLink(vMuscleToVenaCava);
  gCardiovascular.AddLink(vAortaToMyocardium);
  gCardiovascular.AddLink(vMyocardiumToVenaCava);
  gCardiovascular.AddLink(vAortaToRightArm);
  gCardiovascular.AddLink(vRightArmToVenaCava);
  gCardiovascular.AddLink(vAortaToRightKidney);
  gCardiovascular.AddLink(vRightKidneyToVenaCava);
  gCardiovascular.AddLink(vAortaToRightLeg);
  gCardiovascular.AddLink(vRightLegToVenaCava);
  gCardiovascular.AddLink(vAortaToSkin);
  gCardiovascular.AddLink(vSkinToVenaCava);
  gCardiovascular.AddLink(vAortaToSmallIntestine);
  gCardiovascular.AddLink(vSmallIntestineToLiver);
  gCardiovascular.AddLink(vAortaToSplanchnic);
  gCardiovascular.AddLink(vSplanchnicToLiver);
  gCardiovascular.AddLink(vAortaToSpleen);
  gCardiovascular.AddLink(vSpleenToLiver);
  gCardiovascular.AddLink(vVenaCavaIV);
  gCardiovascular.StateChange();

  SELiquidCompartmentGraph& gCombinedCardiovascular = m_Compartments->GetActiveCardiovascularGraph();
  gCombinedCardiovascular.AddGraph(gCardiovascular);
  gCombinedCardiovascular.StateChange();
}

void PulseController::SetupRenal()
{
  Info("Setting Up Renal");
  //////////////////////////
  // Circuit Interdependence
  SEFluidCircuit& cCardiovascular = m_Circuits->GetCardiovascularCircuit();

  //assuming there is a left and right kidney node in cardiovascular AND that a baseline volume is set (as a function of patient mass): 
  double leftKidneyFluidVolume_mL = cCardiovascular.GetNode(pulse::CardiovascularNode::LeftKidney1)->GetVolumeBaseline(VolumeUnit::mL);
  double singleKidneyLargeVasculatureFluidVolume_mL = leftKidneyFluidVolume_mL / 2;    //Total large vasculature fluid volume
  double singleKidneySmallVasculatureFluidVolume_mL = leftKidneyFluidVolume_mL / 2;    //Total small vasculature fluid volume 

  //////////////////////////
  ///// Circuit Parameters//////
  double openSwitch_mmHg_s_Per_mL = m_Config->GetDefaultOpenFlowResistance(PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  //Resistances with some tuning multipliers
  double urineTuningMultiplier = 0.80; //0.85;
  double arteryTuningMultiplier = 0.35;

  double renalArteryResistance_mmHg_s_Per_mL = Convert(0.0250 * arteryTuningMultiplier, PressureTimePerVolumeUnit::mmHg_min_Per_mL, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  double afferentResistance_mmHg_s_Per_mL = Convert(0.0417, PressureTimePerVolumeUnit::mmHg_min_Per_mL, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  double efferentResistance_mmHg_s_Per_mL = Convert(0.0763, PressureTimePerVolumeUnit::mmHg_min_Per_mL, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  double glomerularResistance_mmHg_s_Per_mL = Convert(0.0019, PressureTimePerVolumeUnit::mmHg_min_Per_mL, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  double peritubularResistance_mmHg_s_Per_mL = Convert(0.0167, PressureTimePerVolumeUnit::mmHg_min_Per_mL, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  double renalVeinResistance_mmHg_s_Per_mL = Convert(0.0066, PressureTimePerVolumeUnit::mmHg_min_Per_mL, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  double glomerularFilterResistance_mmHg_s_Per_mL = Convert(0.1600 * urineTuningMultiplier, PressureTimePerVolumeUnit::mmHg_min_Per_mL, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  double tubulesResistance_mmHg_s_Per_mL = Convert(0.1920 * urineTuningMultiplier, PressureTimePerVolumeUnit::mmHg_min_Per_mL, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  double reabsoprtionResistance_mmHg_s_Per_mL = Convert(0.1613 * urineTuningMultiplier, PressureTimePerVolumeUnit::mmHg_min_Per_mL, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  //This one is tuned
  double ureterTuningMultiplier = 0.65;
  double ureterResistance_mmHg_s_Per_mL = Convert(30.0 * ureterTuningMultiplier, PressureTimePerVolumeUnit::mmHg_min_Per_mL, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  double urethraResistance_mmHg_s_Per_mL = openSwitch_mmHg_s_Per_mL;
  //Compliances
  //0.5 * CapacitanceKidney is the per-kidney value from 3 element Windkessel
  double totalCompliance = (0.5 * (0.91 * 1.7560) * 0.02);
  //The fractions here should add to 1.0;
  double renalArteryCompliance_mL_Per_mmHg = totalCompliance * 0.11;
  double renalVeinCompliance_mL_Per_mmHg = totalCompliance * 0.78;
  double glomerularCompliance_mL_Per_mmHg = totalCompliance * 0.11;
  ///\todo The bladder is currently not being modeled as a compliance
  //double bladderCompliance_mL_Per_mmHg = Convert(38.3, VolumePerPressureUnit::mL_Per_cmH2O, VolumePerPressureUnit::mL_Per_mmHg);

  //Large Vasculature (divide total large vasculature fluid volume three ways): 
  double tubulesVolume_mL = singleKidneyLargeVasculatureFluidVolume_mL / 3.0;
  double renalArteryVolume_mL = singleKidneyLargeVasculatureFluidVolume_mL / 3.0;
  double renalVeinVolume_mL = singleKidneyLargeVasculatureFluidVolume_mL / 3.0;

  //Small vasculature (divide total small vasculature fluid volume five ways):
  double peritubularVolume_mL = singleKidneySmallVasculatureFluidVolume_mL / 5.0;
  double efferentVolume_mL = singleKidneySmallVasculatureFluidVolume_mL / 5.0;
  double afferentVolume_mL = singleKidneySmallVasculatureFluidVolume_mL / 5.0;
  double bowmansVolume_mL = singleKidneySmallVasculatureFluidVolume_mL / 5.0;
  double glomerularVolume_mL = singleKidneySmallVasculatureFluidVolume_mL / 5.0;

  //Using width = 1.8 mm and length = 11 inches => 710.6 mm^3
  double ureterVolume_mL = 0.71;

  //Tuned constants
  double bladderVolume_mL = 1.0;
  //Unstressed Pressures - set to zero to use unstressed properly
  double renalArteryPressure_mmHg = 0.0;
  double renalVeinPressure_mmHg = 0.0;
  double glomerularPressure_mmHg = cCardiovascular.GetNode(pulse::CardiovascularNode::LeftKidney1)->GetPressure(PressureUnit::mmHg);
  //double bladderPressure_mmHg = bladderVolume_mL / bladderCompliance_mL_Per_mmHg;
  //Pressure Sources
  double glomerularOsmoticPressure_mmHg = -32.0;
  double bowmansOsmoticPressure_mmHg = 0.0;
  double tubulesOsmoticPressure_mmHg = -15.0;
  double peritubularOsmoticPressure_mmHg = -32.0;

  SEFluidCircuit& cRenal = m_Circuits->GetRenalCircuit();

  ////////////
  // Ground //
  SEFluidCircuitNode& Ground = cRenal.CreateNode(pulse::RenalNode::Ground);
  Ground.GetPressure().SetValue(0.0, PressureUnit::mmHg);
  Ground.SetAsReferenceNode();

  //////////////////
  // Create Nodes //
  //////////////////

  /////////////////
  // Right Blood //
  /////////////////
  //////////////////////////
  // RightAortaConnection //
  SEFluidCircuitNode& RightAortaConnection = cRenal.CreateNode(pulse::RenalNode::RightAortaConnection);
  //////////////////////
  // RightRenalArtery //
  SEFluidCircuitNode& RightRenalArtery = cRenal.CreateNode(pulse::RenalNode::RightRenalArtery);
  RightRenalArtery.GetVolumeBaseline().SetValue(renalArteryVolume_mL, VolumeUnit::mL);
  RightRenalArtery.GetPressure().SetValue(renalArteryPressure_mmHg, PressureUnit::mmHg);
  RightRenalArtery.GetNextPressure().SetValue(renalArteryPressure_mmHg, PressureUnit::mmHg);
  ////////////////////////////
  // RightAfferentArteriole //
  SEFluidCircuitNode& RightAfferentArteriole = cRenal.CreateNode(pulse::RenalNode::RightAfferentArteriole);
  RightAfferentArteriole.GetVolumeBaseline().SetValue(afferentVolume_mL, VolumeUnit::mL);
  ////////////////////////////////
  // RightGlomerularCapillaries //
  SEFluidCircuitNode& RightGlomerularCapillaries = cRenal.CreateNode(pulse::RenalNode::RightGlomerularCapillaries);
  RightGlomerularCapillaries.GetVolumeBaseline().SetValue(glomerularVolume_mL, VolumeUnit::mL);
  RightGlomerularCapillaries.GetPressure().SetValue(glomerularPressure_mmHg, PressureUnit::mmHg);
  ////////////////////////////
  // RightEfferentArteriole //
  SEFluidCircuitNode& RightEfferentArteriole = cRenal.CreateNode(pulse::RenalNode::RightEfferentArteriole);
  RightEfferentArteriole.GetVolumeBaseline().SetValue(efferentVolume_mL, VolumeUnit::mL);
  /////////////////////////////////
  // RightPeritubularCapillaries // 
  SEFluidCircuitNode& RightPeritubularCapillaries = cRenal.CreateNode(pulse::RenalNode::RightPeritubularCapillaries);
  RightPeritubularCapillaries.GetVolumeBaseline().SetValue(peritubularVolume_mL, VolumeUnit::mL);
  ////////////////////
  // RightRenalVein // 
  SEFluidCircuitNode& RightRenalVein = cRenal.CreateNode(pulse::RenalNode::RightRenalVein);
  RightRenalVein.GetVolumeBaseline().SetValue(renalVeinVolume_mL, VolumeUnit::mL);
  RightRenalVein.GetPressure().SetValue(renalVeinPressure_mmHg, PressureUnit::mmHg);
  /////////////////////////////
  // RightVenaCavaConnection //
  SEFluidCircuitNode& RightVenaCavaConnection = cRenal.CreateNode(pulse::RenalNode::RightVenaCavaConnection);
  //////////////////////////
  // RightBowmansCapsules //
  SEFluidCircuitNode& RightBowmansCapsules = cRenal.CreateNode(pulse::RenalNode::RightBowmansCapsules);
  RightBowmansCapsules.GetVolumeBaseline().SetValue(bowmansVolume_mL, VolumeUnit::mL);
  /////////////////////////////
  // RightNetBowmansCapsules //
  SEFluidCircuitNode& RightNetBowmansCapsules = cRenal.CreateNode(pulse::RenalNode::RightNetBowmansCapsules);
  ///////////////////////////////////
  // RightNetGlomerularCapillaries // 
  SEFluidCircuitNode& RightNetGlomerularCapillaries = cRenal.CreateNode(pulse::RenalNode::RightNetGlomerularCapillaries);
  ////////////////////////////////////
  // RightNetPeritubularCapillaries //
  SEFluidCircuitNode& RightNetPeritubularCapillaries = cRenal.CreateNode(pulse::RenalNode::RightNetPeritubularCapillaries);
  /////////////////
  // Right Urine //
  /////////////////
  //////////////////
  // RightTubules //
  SEFluidCircuitNode& RightTubules = cRenal.CreateNode(pulse::RenalNode::RightTubules);
  RightTubules.GetVolumeBaseline().SetValue(tubulesVolume_mL, VolumeUnit::mL);
  /////////////////////
  // RightNetTubules //
  SEFluidCircuitNode& RightNetTubules = cRenal.CreateNode(pulse::RenalNode::RightNetTubules);
  /////////////////
  // RightUreter //
  SEFluidCircuitNode& RightUreter = cRenal.CreateNode(pulse::RenalNode::RightUreter);
  RightUreter.GetVolumeBaseline().SetValue(ureterVolume_mL, VolumeUnit::mL);

  /////////////////
  // Left Blood //
  /////////////////
  //////////////////////////
  // LeftAortaConnection //
  SEFluidCircuitNode& LeftAortaConnection = cRenal.CreateNode(pulse::RenalNode::LeftAortaConnection);
  /////////////////////
  // LeftRenalArtery //
  SEFluidCircuitNode& LeftRenalArtery = cRenal.CreateNode(pulse::RenalNode::LeftRenalArtery);
  LeftRenalArtery.GetVolumeBaseline().SetValue(renalArteryVolume_mL, VolumeUnit::mL);
  LeftRenalArtery.GetPressure().SetValue(renalArteryPressure_mmHg, PressureUnit::mmHg);
  LeftRenalArtery.GetNextPressure().SetValue(renalArteryPressure_mmHg, PressureUnit::mmHg);
  ////////////////////////////
  // LeftAfferentArteriole //
  SEFluidCircuitNode& LeftAfferentArteriole = cRenal.CreateNode(pulse::RenalNode::LeftAfferentArteriole);
  LeftAfferentArteriole.GetVolumeBaseline().SetValue(afferentVolume_mL, VolumeUnit::mL);
  ////////////////////////////////
  // LeftGlomerularCapillaries //
  SEFluidCircuitNode& LeftGlomerularCapillaries = cRenal.CreateNode(pulse::RenalNode::LeftGlomerularCapillaries);
  LeftGlomerularCapillaries.GetVolumeBaseline().SetValue(glomerularVolume_mL, VolumeUnit::mL);
  LeftGlomerularCapillaries.GetPressure().SetValue(glomerularPressure_mmHg, PressureUnit::mmHg);
  ////////////////////////////
  // LeftEfferentArteriole //
  SEFluidCircuitNode& LeftEfferentArteriole = cRenal.CreateNode(pulse::RenalNode::LeftEfferentArteriole);
  LeftEfferentArteriole.GetVolumeBaseline().SetValue(efferentVolume_mL, VolumeUnit::mL);
  /////////////////////////////////
  // LeftPeritubularCapillaries // 
  SEFluidCircuitNode& LeftPeritubularCapillaries = cRenal.CreateNode(pulse::RenalNode::LeftPeritubularCapillaries);
  LeftPeritubularCapillaries.GetVolumeBaseline().SetValue(peritubularVolume_mL, VolumeUnit::mL);
  ////////////////////
  // LeftRenalVein // 
  SEFluidCircuitNode& LeftRenalVein = cRenal.CreateNode(pulse::RenalNode::LeftRenalVein);
  LeftRenalVein.GetVolumeBaseline().SetValue(renalVeinVolume_mL, VolumeUnit::mL);
  LeftRenalVein.GetPressure().SetValue(renalVeinPressure_mmHg, PressureUnit::mmHg);
  /////////////////////////////
  // LeftVenaCavaConnection //
  SEFluidCircuitNode& LeftVenaCavaConnection = cRenal.CreateNode(pulse::RenalNode::LeftVenaCavaConnection);
  //////////////////////////
  // LeftBowmansCapsules //
  SEFluidCircuitNode& LeftBowmansCapsules = cRenal.CreateNode(pulse::RenalNode::LeftBowmansCapsules);
  LeftBowmansCapsules.GetVolumeBaseline().SetValue(bowmansVolume_mL, VolumeUnit::mL);
  /////////////////////////////
  // LeftNetBowmansCapsules //
  SEFluidCircuitNode& LeftNetBowmansCapsules = cRenal.CreateNode(pulse::RenalNode::LeftNetBowmansCapsules);
  ///////////////////////////////////
  // LeftNetGlomerularCapillaries // 
  SEFluidCircuitNode& LeftNetGlomerularCapillaries = cRenal.CreateNode(pulse::RenalNode::LeftNetGlomerularCapillaries);
  ////////////////////////////////////
  // LeftNetPeritubularCapillaries //
  SEFluidCircuitNode& LeftNetPeritubularCapillaries = cRenal.CreateNode(pulse::RenalNode::LeftNetPeritubularCapillaries);
  /////////////////
  // Left Urine //
  /////////////////  
  //////////////////
  // LeftTubules //
  SEFluidCircuitNode& LeftTubules = cRenal.CreateNode(pulse::RenalNode::LeftTubules);
  LeftTubules.GetVolumeBaseline().SetValue(tubulesVolume_mL, VolumeUnit::mL);
  /////////////////////
  // LeftNetTubules //
  SEFluidCircuitNode& LeftNetTubules = cRenal.CreateNode(pulse::RenalNode::LeftNetTubules);
  /////////////////
  // LeftUreter //
  SEFluidCircuitNode& LeftUreter = cRenal.CreateNode(pulse::RenalNode::LeftUreter);
  LeftUreter.GetVolumeBaseline().SetValue(ureterVolume_mL, VolumeUnit::mL);

  /////////////
  // Bladder //
  SEFluidCircuitNode& Bladder = cRenal.CreateNode(pulse::RenalNode::Bladder);
  Bladder.GetVolumeBaseline().SetValue(bladderVolume_mL, VolumeUnit::mL);
  //Bladder.GetPressure().SetValue(0.0, PressureUnit::mmHg);

  //////////////////
  // Create Paths //
  //////////////////

  /////////////////
  // Right Blood //
  /////////////////
  ///////////////////////////////////////
  // RightAortaConnectionToRenalArtery //
  SEFluidCircuitPath& RightAortaConnectionToRenalArtery = cRenal.CreatePath(RightAortaConnection, RightRenalArtery, pulse::RenalPath::RightAortaConnectionToRenalArtery);
  //////////////////////
  // RightRenalArteryToAfferentArteriole //
  SEFluidCircuitPath& RightRenalArteryToAfferentArteriole = cRenal.CreatePath(RightRenalArtery, RightAfferentArteriole, pulse::RenalPath::RightRenalArteryToAfferentArteriole);
  RightRenalArteryToAfferentArteriole.GetResistanceBaseline().SetValue(renalArteryResistance_mmHg_s_Per_mL, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  ////////////////////////////////
  // RightRenalArteryCompliance //
  SEFluidCircuitPath& RightRenalArteryCompliance = cRenal.CreatePath(RightRenalArtery, Ground, pulse::RenalPath::RightRenalArteryCompliance);
  RightRenalArteryCompliance.GetComplianceBaseline().SetValue(renalArteryCompliance_mL_Per_mmHg, VolumePerPressureUnit::mL_Per_mmHg);
  ///////////////////////////////////////////////////
  // RightAfferentArterioleToGlomerularCapillaries //
  SEFluidCircuitPath& RightAfferentArterioleToGlomerularCapillaries = cRenal.CreatePath(RightAfferentArteriole, RightGlomerularCapillaries, pulse::RenalPath::RightAfferentArterioleToGlomerularCapillaries);
  RightAfferentArterioleToGlomerularCapillaries.GetResistanceBaseline().SetValue(afferentResistance_mmHg_s_Per_mL, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  ///////////////////////////////////////////////////
  // RightGlomerularCapillariesToEfferentArteriole //
  SEFluidCircuitPath& RightGlomerularCapillariesToEfferentArteriole = cRenal.CreatePath(RightGlomerularCapillaries, RightEfferentArteriole, pulse::RenalPath::RightGlomerularCapillariesToEfferentArteriole);
  RightGlomerularCapillariesToEfferentArteriole.GetResistanceBaseline().SetValue(glomerularResistance_mmHg_s_Per_mL, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  //////////////////////////////////////////
  // RightGlomerularCapillariesCompliance //
  SEFluidCircuitPath& RightGlomerularCapillariesCompliance = cRenal.CreatePath(RightGlomerularCapillaries, Ground, pulse::RenalPath::RightGlomerularCapillariesCompliance);
  RightGlomerularCapillariesCompliance.GetComplianceBaseline().SetValue(glomerularCompliance_mL_Per_mmHg, VolumePerPressureUnit::mL_Per_mmHg);
  ////////////////////////////////////////////////////
  // RightEfferentArterioleToPeritubularCapillaries //
  SEFluidCircuitPath& RightEfferentArterioleToPeritubularCapillaries = cRenal.CreatePath(RightEfferentArteriole, RightPeritubularCapillaries, pulse::RenalPath::RightEfferentArterioleToPeritubularCapillaries);
  RightEfferentArterioleToPeritubularCapillaries.GetResistanceBaseline().SetValue(efferentResistance_mmHg_s_Per_mL, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  ////////////////////////////////////////////
  // RightPeritubularCapillariesToRenalVein //
  SEFluidCircuitPath& RightPeritubularCapillariesToRenalVein = cRenal.CreatePath(RightPeritubularCapillaries, RightRenalVein, pulse::RenalPath::RightPeritubularCapillariesToRenalVein);
  RightPeritubularCapillariesToRenalVein.GetResistanceBaseline().SetValue(peritubularResistance_mmHg_s_Per_mL, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  ////////////////////////////////////////
  // RightRenalVeinToVenaCavaConnection //
  SEFluidCircuitPath& RightRenalVeinToVenaCavaConnection = cRenal.CreatePath(RightRenalVein, RightVenaCavaConnection, pulse::RenalPath::RightRenalVeinToVenaCavaConnection);
  RightRenalVeinToVenaCavaConnection.GetResistanceBaseline().SetValue(renalVeinResistance_mmHg_s_Per_mL, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  //////////////////////////////
  // RightRenalVeinCompliance //
  SEFluidCircuitPath& RightRenalVeinCompliance = cRenal.CreatePath(RightRenalVein, Ground, pulse::RenalPath::RightRenalVeinCompliance);
  RightRenalVeinCompliance.GetComplianceBaseline().SetValue(renalVeinCompliance_mL_Per_mmHg, VolumePerPressureUnit::mL_Per_mmHg);
  //////////////////////////////////////////////////////////
  // RightGlomerularCapillariesToNetGlomerularCapillaries //
  SEFluidCircuitPath& RightGlomerularCapillariesToNetGlomerularCapillaries = cRenal.CreatePath(RightGlomerularCapillaries, RightNetGlomerularCapillaries, pulse::RenalPath::RightGlomerularCapillariesToNetGlomerularCapillaries);
  RightGlomerularCapillariesToNetGlomerularCapillaries.GetPressureSourceBaseline().SetValue(glomerularOsmoticPressure_mmHg, PressureUnit::mmHg);
  ///////////////////////////////////////////////////////
  // RightNetGlomerularCapillariesToNetBowmansCapsules //
  SEFluidCircuitPath& RightNetGlomerularCapillariesToNetBowmansCapsules = cRenal.CreatePath(RightNetGlomerularCapillaries, RightNetBowmansCapsules, pulse::RenalPath::RightNetGlomerularCapillariesToNetBowmansCapsules);
  RightNetGlomerularCapillariesToNetBowmansCapsules.GetResistanceBaseline().SetValue(glomerularFilterResistance_mmHg_s_Per_mL, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  //////////////////////////////////////////////
  // RightBowmansCapsulesToNetBowmansCapsules //
  SEFluidCircuitPath& RightBowmansCapsulesToNetBowmansCapsules = cRenal.CreatePath(RightBowmansCapsules, RightNetBowmansCapsules, pulse::RenalPath::RightBowmansCapsulesToNetBowmansCapsules);
  RightBowmansCapsulesToNetBowmansCapsules.GetPressureSourceBaseline().SetValue(bowmansOsmoticPressure_mmHg, PressureUnit::mmHg);
  /////////////////
  // Right Urine //
  ///////////////// 
  ///////////////////////////////////
  // RightBowmansCapsulesToTubules //
  SEFluidCircuitPath& RightBowmansCapsulesToTubules = cRenal.CreatePath(RightBowmansCapsules, RightTubules, pulse::RenalPath::RightBowmansCapsulesToTubules);
  RightBowmansCapsulesToTubules.GetResistanceBaseline().SetValue(tubulesResistance_mmHg_s_Per_mL, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  //////////////////////////
  // RightTubulesToUreter //
  SEFluidCircuitPath& RightTubulesToUreter = cRenal.CreatePath(RightTubules, RightUreter, pulse::RenalPath::RightTubulesToUreter);
  RightTubulesToUreter.GetResistanceBaseline().SetValue(ureterResistance_mmHg_s_Per_mL, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  /////////////////////////////
  // RighTubulesToNetTubules //
  SEFluidCircuitPath& RightTubulesToNetTubules = cRenal.CreatePath(RightTubules, RightNetTubules, pulse::RenalPath::RightTubulesToNetTubules);
  RightTubulesToNetTubules.GetPressureSourceBaseline().SetValue(tubulesOsmoticPressure_mmHg, PressureUnit::mmHg);
  ////////////////////////////////////////////////
  // RightNetTubulesToNetPeritubularCapillaries //
  SEFluidCircuitPath& RightNetTubulesToNetPeritubularCapillaries = cRenal.CreatePath(RightNetTubules, RightNetPeritubularCapillaries, pulse::RenalPath::RightNetTubulesToNetPeritubularCapillaries);
  RightNetTubulesToNetPeritubularCapillaries.GetResistanceBaseline().SetValue(reabsoprtionResistance_mmHg_s_Per_mL, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  ////////////////////////////////////////////////////////////
  // RightPeritubularCapillariesToNetPeritubularCapillaries //
  SEFluidCircuitPath& RightPeritubularCapillariesToNetPeritubularCapillaries = cRenal.CreatePath(RightPeritubularCapillaries, RightNetPeritubularCapillaries, pulse::RenalPath::RightPeritubularCapillariesToNetPeritubularCapillaries);
  RightPeritubularCapillariesToNetPeritubularCapillaries.GetPressureSourceBaseline().SetValue(peritubularOsmoticPressure_mmHg, PressureUnit::mmHg);
  //////////////////////////
  // RightUreterToBladder //
  SEFluidCircuitPath& RightUreterToBladder = cRenal.CreatePath(RightUreter, Bladder, pulse::RenalPath::RightUreterToBladder);
  RightUreterToBladder.SetNextValve(eGate::Closed);

  /////////////////
  // Left Blood //
  /////////////////
  ///////////////////////////////////////
  // LeftAortaConnectionToRenalArtery //
  SEFluidCircuitPath& LeftAortaConnectionToRenalArtery = cRenal.CreatePath(LeftAortaConnection, LeftRenalArtery, pulse::RenalPath::LeftAortaConnectionToRenalArtery);
  //////////////////////
  // LeftRenalArteryToAfferentArteriole //
  SEFluidCircuitPath& LeftRenalArteryToAfferentArteriole = cRenal.CreatePath(LeftRenalArtery, LeftAfferentArteriole, pulse::RenalPath::LeftRenalArteryToAfferentArteriole);
  LeftRenalArteryToAfferentArteriole.GetResistanceBaseline().SetValue(renalArteryResistance_mmHg_s_Per_mL, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  ////////////////////////////////
  // LeftRenalArteryCompliance //
  SEFluidCircuitPath& LeftRenalArteryCompliance = cRenal.CreatePath(LeftRenalArtery, Ground, pulse::RenalPath::LeftRenalArteryCompliance);
  LeftRenalArteryCompliance.GetComplianceBaseline().SetValue(renalArteryCompliance_mL_Per_mmHg, VolumePerPressureUnit::mL_Per_mmHg);
  ///////////////////////////////////////////////////
  // LeftAfferentArterioleToGlomerularCapillaries //
  SEFluidCircuitPath& LeftAfferentArterioleToGlomerularCapillaries = cRenal.CreatePath(LeftAfferentArteriole, LeftGlomerularCapillaries, pulse::RenalPath::LeftAfferentArterioleToGlomerularCapillaries);
  LeftAfferentArterioleToGlomerularCapillaries.GetResistanceBaseline().SetValue(afferentResistance_mmHg_s_Per_mL, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  ///////////////////////////////////////////////////
  // LeftGlomerularCapillariesToEfferentArteriole //
  SEFluidCircuitPath& LeftGlomerularCapillariesToEfferentArteriole = cRenal.CreatePath(LeftGlomerularCapillaries, LeftEfferentArteriole, pulse::RenalPath::LeftGlomerularCapillariesToEfferentArteriole);
  LeftGlomerularCapillariesToEfferentArteriole.GetResistanceBaseline().SetValue(glomerularResistance_mmHg_s_Per_mL, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  //////////////////////////////////////////
  // LeftGlomerularCapillariesCompliance //
  SEFluidCircuitPath& LeftGlomerularCapillariesCompliance = cRenal.CreatePath(LeftGlomerularCapillaries, Ground, pulse::RenalPath::LeftGlomerularCapillariesCompliance);
  LeftGlomerularCapillariesCompliance.GetComplianceBaseline().SetValue(glomerularCompliance_mL_Per_mmHg, VolumePerPressureUnit::mL_Per_mmHg);
  ////////////////////////////////////////////////////
  // LeftEfferentArterioleToPeritubularCapillaries //
  SEFluidCircuitPath& LeftEfferentArterioleToPeritubularCapillaries = cRenal.CreatePath(LeftEfferentArteriole, LeftPeritubularCapillaries, pulse::RenalPath::LeftEfferentArterioleToPeritubularCapillaries);
  LeftEfferentArterioleToPeritubularCapillaries.GetResistanceBaseline().SetValue(efferentResistance_mmHg_s_Per_mL, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  ////////////////////////////////////////////
  // LeftPeritubularCapillariesToRenalVein //
  SEFluidCircuitPath& LeftPeritubularCapillariesToRenalVein = cRenal.CreatePath(LeftPeritubularCapillaries, LeftRenalVein, pulse::RenalPath::LeftPeritubularCapillariesToRenalVein);
  LeftPeritubularCapillariesToRenalVein.GetResistanceBaseline().SetValue(peritubularResistance_mmHg_s_Per_mL, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  ////////////////////////////////////////
  // LeftRenalVeinToVenaCavaConnection //
  SEFluidCircuitPath& LeftRenalVeinToVenaCavaConnection = cRenal.CreatePath(LeftRenalVein, LeftVenaCavaConnection, pulse::RenalPath::LeftRenalVeinToVenaCavaConnection);
  LeftRenalVeinToVenaCavaConnection.GetResistanceBaseline().SetValue(renalVeinResistance_mmHg_s_Per_mL, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  //////////////////////////////
  // LeftRenalVeinCompliance //
  SEFluidCircuitPath& LeftRenalVeinCompliance = cRenal.CreatePath(LeftRenalVein, Ground, pulse::RenalPath::LeftRenalVeinCompliance);
  LeftRenalVeinCompliance.GetComplianceBaseline().SetValue(renalVeinCompliance_mL_Per_mmHg, VolumePerPressureUnit::mL_Per_mmHg);
  //////////////////////////////////////////////////////////
  // LeftGlomerularCapillariesToNetGlomerularCapillaries //
  SEFluidCircuitPath& LeftGlomerularCapillariesToNetGlomerularCapillaries = cRenal.CreatePath(LeftGlomerularCapillaries, LeftNetGlomerularCapillaries, pulse::RenalPath::LeftGlomerularCapillariesToNetGlomerularCapillaries);
  LeftGlomerularCapillariesToNetGlomerularCapillaries.GetPressureSourceBaseline().SetValue(glomerularOsmoticPressure_mmHg, PressureUnit::mmHg);
  ///////////////////////////////////////////////////////
  // LeftNetGlomerularCapillariesToNetBowmansCapsules //
  SEFluidCircuitPath& LeftNetGlomerularCapillariesToNetBowmansCapsules = cRenal.CreatePath(LeftNetGlomerularCapillaries, LeftNetBowmansCapsules, pulse::RenalPath::LeftNetGlomerularCapillariesToNetBowmansCapsules);
  LeftNetGlomerularCapillariesToNetBowmansCapsules.GetResistanceBaseline().SetValue(glomerularFilterResistance_mmHg_s_Per_mL, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  //////////////////////////////////////////////
  // LeftBowmansCapsulesToNetBowmansCapsules //
  SEFluidCircuitPath& LeftBowmansCapsulesToNetBowmansCapsules = cRenal.CreatePath(LeftBowmansCapsules, LeftNetBowmansCapsules, pulse::RenalPath::LeftBowmansCapsulesToNetBowmansCapsules);
  LeftBowmansCapsulesToNetBowmansCapsules.GetPressureSourceBaseline().SetValue(bowmansOsmoticPressure_mmHg, PressureUnit::mmHg);
  /////////////////
  // Left Urine //
  /////////////////  
  ///////////////////////////////////
  // LeftBowmansCapsulesToTubules //
  SEFluidCircuitPath& LeftBowmansCapsulesToTubules = cRenal.CreatePath(LeftBowmansCapsules, LeftTubules, pulse::RenalPath::LeftBowmansCapsulesToTubules);
  LeftBowmansCapsulesToTubules.GetResistanceBaseline().SetValue(tubulesResistance_mmHg_s_Per_mL, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  //////////////////////////
  // LeftTubulesToUreter //
  SEFluidCircuitPath& LeftTubulesToUreter = cRenal.CreatePath(LeftTubules, LeftUreter, pulse::RenalPath::LeftTubulesToUreter);
  LeftTubulesToUreter.GetResistanceBaseline().SetValue(ureterResistance_mmHg_s_Per_mL, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  /////////////////////////////
  // LeftubulesToNetTubules //
  SEFluidCircuitPath& LeftTubulesToNetTubules = cRenal.CreatePath(LeftTubules, LeftNetTubules, pulse::RenalPath::LeftTubulesToNetTubules);
  LeftTubulesToNetTubules.GetPressureSourceBaseline().SetValue(tubulesOsmoticPressure_mmHg, PressureUnit::mmHg);
  ////////////////////////////////////////////////
  // LeftNetTubulesToNetPeritubularCapillaries //
  SEFluidCircuitPath& LeftNetTubulesToNetPeritubularCapillaries = cRenal.CreatePath(LeftNetTubules, LeftNetPeritubularCapillaries, pulse::RenalPath::LeftNetTubulesToNetPeritubularCapillaries);
  LeftNetTubulesToNetPeritubularCapillaries.GetResistanceBaseline().SetValue(reabsoprtionResistance_mmHg_s_Per_mL, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  ////////////////////////////////////////////////////////////
  // LeftPeritubularCapillariesToNetPeritubularCapillaries //
  SEFluidCircuitPath& LeftPeritubularCapillariesToNetPeritubularCapillaries = cRenal.CreatePath(LeftPeritubularCapillaries, LeftNetPeritubularCapillaries, pulse::RenalPath::LeftPeritubularCapillariesToNetPeritubularCapillaries);
  LeftPeritubularCapillariesToNetPeritubularCapillaries.GetPressureSourceBaseline().SetValue(peritubularOsmoticPressure_mmHg, PressureUnit::mmHg);
  //////////////////////////
  // LeftUreterToBladder //
  SEFluidCircuitPath& LeftUreterToBladder = cRenal.CreatePath(LeftUreter, Bladder, pulse::RenalPath::LeftUreterToBladder);
  LeftUreterToBladder.SetNextValve(eGate::Closed);

  ///////////////////////
  // BladderCompliance //
  SEFluidCircuitPath& BladderToGroundPressure = cRenal.CreatePath(Bladder, Ground, pulse::RenalPath::BladderToGroundPressure);
  /// \todo Use a compliance here - make sure you remove the current handling of bladder volume in the renal system as a pressure source
  //BladderCompliance.GetComplianceBaseline().SetValue(bladderCompliance_mL_Per_mmHg, VolumePerPressureUnit::mL_Per_mmHg);
  BladderToGroundPressure.GetPressureSourceBaseline().SetValue(-4.0, PressureUnit::mmHg); //Negative because source-target is for compliance
  //////////////
  // BladderGround //
  SEFluidCircuitPath& BladderToGroundUrinate = cRenal.CreatePath(Bladder, Ground, pulse::RenalPath::BladderToGroundUrinate);
  BladderToGroundUrinate.GetResistanceBaseline().SetValue(urethraResistance_mmHg_s_Per_mL, PressureTimePerVolumeUnit::mmHg_s_Per_mL);

  cRenal.SetNextAndCurrentFromBaselines();
  cRenal.StateChange();

  // Delete the three-element Windkessel kidney model 
  SEFluidCircuit& cCombinedCardiovascular = m_Circuits->GetActiveCardiovascularCircuit();
  m_Circuits->DeleteFluidNode(pulse::CardiovascularNode::LeftKidney1);
  m_Circuits->DeleteFluidNode(pulse::CardiovascularNode::LeftKidney2);
  m_Circuits->DeleteFluidNode(pulse::CardiovascularNode::RightKidney1);
  m_Circuits->DeleteFluidNode(pulse::CardiovascularNode::RightKidney2);
  m_Circuits->DeleteFluidPath(pulse::CardiovascularPath::Aorta1ToLeftKidney1);
  m_Circuits->DeleteFluidPath(pulse::CardiovascularPath::Aorta1ToRightKidney1);
  m_Circuits->DeleteFluidPath(pulse::CardiovascularPath::LeftKidney1ToLeftKidney2);
  m_Circuits->DeleteFluidPath(pulse::CardiovascularPath::RightKidney1ToRightKidney2);
  m_Circuits->DeleteFluidPath(pulse::CardiovascularPath::LeftKidney1ToGround);
  m_Circuits->DeleteFluidPath(pulse::CardiovascularPath::RightKidney1ToGround);
  m_Circuits->DeleteFluidPath(pulse::CardiovascularPath::LeftKidney2ToVenaCava);
  m_Circuits->DeleteFluidPath(pulse::CardiovascularPath::RightKidney2ToVenaCava);
  cCombinedCardiovascular.AddCircuit(cRenal);
  // Grab the nodes that we will be connecting between the 2 circuits
  SEFluidCircuitNode* Aorta1 = cCardiovascular.GetNode(pulse::CardiovascularNode::Aorta1);
  SEFluidCircuitNode* VenaCava = cCardiovascular.GetNode(pulse::CardiovascularNode::VenaCava);
  // Add the new connection paths
  SEFluidCircuitPath& NewAorta1ToRightKidney = cCombinedCardiovascular.CreatePath(*Aorta1, RightAortaConnection, pulse::CardiovascularPath::Aorta1ToRightKidney1);
  SEFluidCircuitPath& NewRightKidneyToVenaCava = cCombinedCardiovascular.CreatePath(RightVenaCavaConnection, *VenaCava, pulse::CardiovascularPath::RightKidney2ToVenaCava);
  SEFluidCircuitPath& NewAorta1ToLeftKidney = cCombinedCardiovascular.CreatePath(*Aorta1, LeftAortaConnection, pulse::CardiovascularPath::Aorta1ToLeftKidney1);
  SEFluidCircuitPath& NewLeftKidneyToVenaCava = cCombinedCardiovascular.CreatePath(LeftVenaCavaConnection, *VenaCava, pulse::CardiovascularPath::LeftKidney2ToVenaCava);
  // We need to move the resistances
  NewAorta1ToRightKidney.GetResistanceBaseline().Set(RightAortaConnectionToRenalArtery.GetResistanceBaseline());
  RightAortaConnectionToRenalArtery.GetResistanceBaseline().Invalidate();
  NewRightKidneyToVenaCava.GetResistanceBaseline().Set(RightRenalVeinToVenaCavaConnection.GetResistanceBaseline());
  RightRenalVeinToVenaCavaConnection.GetResistanceBaseline().Invalidate();
  NewAorta1ToLeftKidney.GetResistanceBaseline().Set(LeftAortaConnectionToRenalArtery.GetResistanceBaseline());
  LeftAortaConnectionToRenalArtery.GetResistanceBaseline().Invalidate();
  NewLeftKidneyToVenaCava.GetResistanceBaseline().Set(LeftRenalVeinToVenaCavaConnection.GetResistanceBaseline());
  LeftRenalVeinToVenaCavaConnection.GetResistanceBaseline().Invalidate();
  // Update the circuit
  cCombinedCardiovascular.SetNextAndCurrentFromBaselines();
  cCombinedCardiovascular.StateChange();

  ////////////////////////
  // Renal Compartments //
  ////////////////////////

  ///////////
  // Blood //
  ///////////

  ///////////////////////
  // RightRenalArtery //
  SELiquidCompartment& vRightRenalArtery = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::RightRenalArtery);
  vRightRenalArtery.MapNode(RightRenalArtery);
  //////////////////////////////
  // RightAfferentArteriole //
  SELiquidCompartment& vRightAfferentArteriole = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::RightAfferentArteriole);
  vRightAfferentArteriole.MapNode(RightAfferentArteriole);
  ////////////////////////////////
  // RightGlomerularCapillaries //
  SELiquidCompartment& vRightGlomerularCapillaries = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::RightGlomerularCapillaries);
  vRightGlomerularCapillaries.MapNode(RightGlomerularCapillaries);
  vRightGlomerularCapillaries.MapNode(RightNetGlomerularCapillaries);
  ////////////////////////////
  // RightEfferentArteriole //
  SELiquidCompartment& vRightEfferentArteriole = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::RightEfferentArteriole);
  vRightEfferentArteriole.MapNode(RightEfferentArteriole);
  /////////////////////////////////
  // RightPeritubularCapillaries //
  SELiquidCompartment& vRightPeritubularCapillaries = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::RightPeritubularCapillaries);
  vRightPeritubularCapillaries.MapNode(RightPeritubularCapillaries);
  vRightPeritubularCapillaries.MapNode(RightNetPeritubularCapillaries);
  ///////////////////
  // RightRenalVein //
  SELiquidCompartment& vRightRenalVein = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::RightRenalVein);
  vRightRenalVein.MapNode(RightRenalVein);
  //////////////////////////
  // RightBowmansCapsules //
  SELiquidCompartment& vRightBowmansCapsules = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::RightBowmansCapsules);
  vRightBowmansCapsules.MapNode(RightBowmansCapsules);
  vRightBowmansCapsules.MapNode(RightNetBowmansCapsules);
  //////////////////
  // RightTubules //
  SELiquidCompartment& vRightTubules = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::RightTubules);
  vRightTubules.MapNode(RightTubules);
  vRightTubules.MapNode(RightNetTubules);

  /////////////////////
  // LeftRenalArtery //
  SELiquidCompartment& vLeftRenalArtery = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::LeftRenalArtery);
  vLeftRenalArtery.MapNode(LeftRenalArtery);
  //////////////////////////////
  // LeftAfferentArteriole //
  SELiquidCompartment& vLeftAfferentArteriole = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::LeftAfferentArteriole);
  vLeftAfferentArteriole.MapNode(LeftAfferentArteriole);
  ////////////////////////////////
  // LeftGlomerularCapillaries //
  SELiquidCompartment& vLeftGlomerularCapillaries = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::LeftGlomerularCapillaries);
  vLeftGlomerularCapillaries.MapNode(LeftGlomerularCapillaries);
  vLeftGlomerularCapillaries.MapNode(LeftNetGlomerularCapillaries);
  ////////////////////////////
  // LeftEfferentArteriole //
  SELiquidCompartment& vLeftEfferentArteriole = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::LeftEfferentArteriole);
  vLeftEfferentArteriole.MapNode(LeftEfferentArteriole);
  ////////////////////////////////
  // LeftPeritubularCapillaries //
  SELiquidCompartment& vLeftPeritubularCapillaries = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::LeftPeritubularCapillaries);
  vLeftPeritubularCapillaries.MapNode(LeftPeritubularCapillaries);
  vLeftPeritubularCapillaries.MapNode(LeftNetPeritubularCapillaries);
  ///////////////////
  // LeftRenalVein //
  SELiquidCompartment& vLeftRenalVein = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::LeftRenalVein);
  vLeftRenalVein.MapNode(LeftRenalVein);
  /////////////////////////
  // LeftBowmansCapsules //
  SELiquidCompartment& vLeftBowmansCapsules = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::LeftBowmansCapsules);
  vLeftBowmansCapsules.MapNode(LeftBowmansCapsules);
  vLeftBowmansCapsules.MapNode(LeftNetBowmansCapsules);
  /////////////////
  // LeftTubules //
  SELiquidCompartment& vLeftTubules = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::LeftTubules);
  vLeftTubules.MapNode(LeftTubules);
  vLeftTubules.MapNode(LeftNetTubules);

  // Let's build out the hierarchy
  // Grab these, as cardiovascular already made them  
  SELiquidCompartment* vLeftKidney = m_Compartments->GetLiquidCompartment(pulse::VascularCompartment::LeftKidney);
  SELiquidCompartment& vLeftNephron = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::LeftNephron);
  vLeftKidney->GetNodeMapping().Clear();// Remove the nodes the cardiovascular was using to model the kidney
  vLeftKidney->AddChild(vLeftRenalArtery);
  vLeftKidney->AddChild(vLeftNephron);
  vLeftNephron.AddChild(vLeftAfferentArteriole);
  vLeftNephron.AddChild(vLeftGlomerularCapillaries);
  vLeftNephron.AddChild(vLeftEfferentArteriole);
  vLeftNephron.AddChild(vLeftPeritubularCapillaries);
  vLeftNephron.AddChild(vLeftBowmansCapsules);
  vLeftNephron.AddChild(vLeftTubules);
  vLeftKidney->AddChild(vLeftRenalVein);
  SELiquidCompartment* vRightKidney = m_Compartments->GetLiquidCompartment(pulse::VascularCompartment::RightKidney);
  SELiquidCompartment& vRightNephron = m_Compartments->CreateLiquidCompartment(pulse::VascularCompartment::RightNephron);
  vRightKidney->GetNodeMapping().Clear();// Remove the nodes the cardiovascular was using to model the kidney
  vRightKidney->AddChild(vRightRenalArtery);
  vRightKidney->AddChild(vRightNephron);
  vRightNephron.AddChild(vRightAfferentArteriole);
  vRightNephron.AddChild(vRightGlomerularCapillaries);
  vRightNephron.AddChild(vRightEfferentArteriole);
  vRightNephron.AddChild(vRightPeritubularCapillaries);
  vRightNephron.AddChild(vRightBowmansCapsules);
  vRightNephron.AddChild(vRightTubules);
  vRightKidney->AddChild(vRightRenalVein);

  ///////////
  // Urine //
  ///////////

  /////////////////
  // RightUreter //
  SELiquidCompartment& uRightUreter = m_Compartments->CreateLiquidCompartment(pulse::UrineCompartment::RightUreter);
  uRightUreter.MapNode(RightUreter);
  ////////////////
  // LeftUreter //
  SELiquidCompartment& uLeftUreter = m_Compartments->CreateLiquidCompartment(pulse::UrineCompartment::LeftUreter);
  uLeftUreter.MapNode(LeftUreter);
  ////////////////
  // Ureters //
  SELiquidCompartment& uUreters = m_Compartments->CreateLiquidCompartment(pulse::UrineCompartment::Ureters);
  uUreters.AddChild(uRightUreter);
  uUreters.AddChild(uLeftUreter);
  /////////////
  // Bladder //
  SELiquidCompartment& uBladder = m_Compartments->CreateLiquidCompartment(pulse::UrineCompartment::Bladder);
  uBladder.MapNode(Bladder);

  /////////////////////////////
  // Renal Compartment Links //
  /////////////////////////////

  // Graph Dependencies
  SELiquidCompartment& vAorta = *m_Compartments->GetLiquidCompartment(pulse::VascularCompartment::Aorta);
  SELiquidCompartment& vVenaCava = *m_Compartments->GetLiquidCompartment(pulse::VascularCompartment::VenaCava);
  SELiquidCompartment& vGround = *m_Compartments->GetLiquidCompartment(pulse::VascularCompartment::Ground);

  ///////////
  // Blood //
  ///////////

  ////////////////////////////
  // AortaToRightRenalArtery //
  m_Compartments->DeleteLiquidLink(pulse::VascularLink::AortaToRightKidney);// Replace this link
  SELiquidCompartmentLink& vAortaToRightRenalArtery = m_Compartments->CreateLiquidLink(vAorta, vRightRenalArtery, pulse::VascularLink::AortaToRightKidney);
  vAortaToRightRenalArtery.MapPath(RightAortaConnectionToRenalArtery);
  ////////////////////////////////////////
  // RightRenalArteryToAfferentArteriole //
  SELiquidCompartmentLink& vRightRenalArteryToAfferentArteriole = m_Compartments->CreateLiquidLink(vRightRenalArtery, vRightAfferentArteriole, pulse::VascularLink::RightRenalArteryToAfferentArteriole);
  vRightRenalArteryToAfferentArteriole.MapPath(RightRenalArteryToAfferentArteriole);
  //////////////////////////////////////////////////
  // RightAfferentArterioleToGlomerularCapillaries //
  SELiquidCompartmentLink& vRightAfferentArterioleToGlomerularCapillaries = m_Compartments->CreateLiquidLink(vRightAfferentArteriole, vRightGlomerularCapillaries, pulse::VascularLink::RightAfferentArterioleToGlomerularCapillaries);
  vRightAfferentArterioleToGlomerularCapillaries.MapPath(RightAfferentArterioleToGlomerularCapillaries);
  //////////////////////////////////////////////////
  // RightGlomerularCapillariesToEfferentArteriole //
  SELiquidCompartmentLink& vRightGlomerularCapillariesToEfferentArteriole = m_Compartments->CreateLiquidLink(vRightGlomerularCapillaries, vRightEfferentArteriole, pulse::VascularLink::RightGlomerularCapillariesToEfferentArteriole);
  vRightGlomerularCapillariesToEfferentArteriole.MapPath(RightGlomerularCapillariesToEfferentArteriole);
  /////////////////////////////////////////////////
  // RightGlomerularCapillariesToBowmansCapsules //
  SELiquidCompartmentLink& vRightGlomerularCapillariesToBowmansCapsules = m_Compartments->CreateLiquidLink(vRightGlomerularCapillaries, vRightBowmansCapsules, pulse::VascularLink::RightGlomerularCapillariesToBowmansCapsules);
  vRightGlomerularCapillariesToBowmansCapsules.MapPath(RightNetGlomerularCapillariesToNetBowmansCapsules);
  ///////////////////////////////////
  // RightBowmansCapsulesToTubules //
  SELiquidCompartmentLink& vRightBowmansCapsulesToTubules = m_Compartments->CreateLiquidLink(vRightBowmansCapsules, vRightTubules, pulse::VascularLink::RightBowmansCapsulesToTubules);
  vRightBowmansCapsulesToTubules.MapPath(RightBowmansCapsulesToTubules);
  //////////////////////////////////////////
  // RightTubulesToPeritubularCapillaries //
  SELiquidCompartmentLink& vRightTubulesToPeritubularCapillaries = m_Compartments->CreateLiquidLink(vRightTubules, vRightPeritubularCapillaries, pulse::VascularLink::RightTubulesToPeritubularCapillaries);
  vRightTubulesToPeritubularCapillaries.MapPath(RightNetTubulesToNetPeritubularCapillaries);
  ///////////////////////////////////////////////////
  // RightEfferentArterioleToPeritubularCapillaries //
  SELiquidCompartmentLink& vRightEfferentArterioleToPeritubularCapillaries = m_Compartments->CreateLiquidLink(vRightEfferentArteriole, vRightPeritubularCapillaries, pulse::VascularLink::RightEfferentArterioleToPeritubularCapillaries);
  vRightEfferentArterioleToPeritubularCapillaries.MapPath(RightEfferentArterioleToPeritubularCapillaries);
  ////////////////////////////////////////////
  // RightPeritubularCapillariesToRenalVein //
  SELiquidCompartmentLink& vRightPeritubularCapillariesToRenalVein = m_Compartments->CreateLiquidLink(vRightPeritubularCapillaries, vRightRenalVein, pulse::VascularLink::RightPeritubularCapillariesToRenalVein);
  vRightPeritubularCapillariesToRenalVein.MapPath(RightPeritubularCapillariesToRenalVein);
  /////////////////////////////
  // RightRenalVeinToVenaCava //
  m_Compartments->DeleteLiquidLink(pulse::VascularLink::RightKidneyToVenaCava);// Replace this vink
  SELiquidCompartmentLink& vRightRenalVeinToVenaCava = m_Compartments->CreateLiquidLink(vRightRenalVein, vVenaCava, pulse::VascularLink::RightKidneyToVenaCava);
  vRightRenalVeinToVenaCava.MapPath(RightRenalVeinToVenaCavaConnection);

  ////////////////////////////
  // AortaToLeftRenalArtery //
  m_Compartments->DeleteLiquidLink(pulse::VascularLink::AortaToLeftKidney);// Replace this uink
  SELiquidCompartmentLink& vAortaToLeftRenalArtery = m_Compartments->CreateLiquidLink(vAorta, vLeftRenalArtery, pulse::VascularLink::AortaToLeftKidney);
  vAortaToLeftRenalArtery.MapPath(LeftAortaConnectionToRenalArtery);
  ////////////////////////////////////////
  // LeftRenalArteryToAfferentArteriole //
  SELiquidCompartmentLink& vLeftRenalArteryToAfferentArteriole = m_Compartments->CreateLiquidLink(vLeftRenalArtery, vLeftAfferentArteriole, pulse::VascularLink::LeftRenalArteryToAfferentArteriole);
  vLeftRenalArteryToAfferentArteriole.MapPath(LeftRenalArteryToAfferentArteriole);
  //////////////////////////////////////////////////
  // LeftAfferentArterioleToGlomerularCapillaries //
  SELiquidCompartmentLink& vLeftAfferentArterioleToGlomerularCapillaries = m_Compartments->CreateLiquidLink(vLeftAfferentArteriole, vLeftGlomerularCapillaries, pulse::VascularLink::LeftAfferentArterioleToGlomerularCapillaries);
  vLeftAfferentArterioleToGlomerularCapillaries.MapPath(LeftAfferentArterioleToGlomerularCapillaries);
  //////////////////////////////////////////////////
  // LeftGlomerularCapillariesToEfferentArteriole //
  SELiquidCompartmentLink& vLeftGlomerularCapillariesToEfferentArteriole = m_Compartments->CreateLiquidLink(vLeftGlomerularCapillaries, vLeftEfferentArteriole, pulse::VascularLink::LeftGlomerularCapillariesToEfferentArteriole);
  vLeftGlomerularCapillariesToEfferentArteriole.MapPath(LeftGlomerularCapillariesToEfferentArteriole);
  ////////////////////////////////////////////////
  // LeftGlomerularCapillariesToBowmansCapsules //
  SELiquidCompartmentLink& vLeftGlomerularCapillariesToBowmansCapsules = m_Compartments->CreateLiquidLink(vLeftGlomerularCapillaries, vLeftBowmansCapsules, pulse::VascularLink::LeftGlomerularCapillariesToBowmansCapsules);
  vLeftGlomerularCapillariesToBowmansCapsules.MapPath(LeftNetGlomerularCapillariesToNetBowmansCapsules);
  //////////////////////////////////
  // LeftBowmansCapsulesToTubules //
  SELiquidCompartmentLink& vLeftBowmansCapsulesToTubules = m_Compartments->CreateLiquidLink(vLeftBowmansCapsules, vLeftTubules, pulse::VascularLink::LeftBowmansCapsulesToTubules);
  vLeftBowmansCapsulesToTubules.MapPath(LeftBowmansCapsulesToTubules);
  /////////////////////////////////////////
  // LeftTubulesToPeritubularCapillaries //
  SELiquidCompartmentLink& vLeftTubulesToPeritubularCapillaries = m_Compartments->CreateLiquidLink(vLeftTubules, vLeftPeritubularCapillaries, pulse::VascularLink::LeftTubulesToPeritubularCapillaries);
  vLeftTubulesToPeritubularCapillaries.MapPath(LeftNetTubulesToNetPeritubularCapillaries);
  ///////////////////////////////////////////////////
  // LeftEfferentArterioleToPeritubularCapillaries //
  SELiquidCompartmentLink& vLeftEfferentArterioleToPeritubularCapillaries = m_Compartments->CreateLiquidLink(vLeftEfferentArteriole, vLeftPeritubularCapillaries, pulse::VascularLink::LeftEfferentArterioleToPeritubularCapillaries);
  vLeftEfferentArterioleToPeritubularCapillaries.MapPath(LeftEfferentArterioleToPeritubularCapillaries);
  ///////////////////////////////////////////
  // LeftPeritubularCapillariesToRenalVein //
  SELiquidCompartmentLink& vLeftPeritubularCapillariesToRenalVein = m_Compartments->CreateLiquidLink(vLeftPeritubularCapillaries, vLeftRenalVein, pulse::VascularLink::LeftPeritubularCapillariesToRenalVein);
  vLeftPeritubularCapillariesToRenalVein.MapPath(LeftPeritubularCapillariesToRenalVein);
  /////////////////////////////
  // LeftRenalVeinToVenaCava //
  m_Compartments->DeleteLiquidLink(pulse::VascularLink::LeftKidneyToVenaCava);// Replace this link
  SELiquidCompartmentLink& vLeftRenalVeinToVenaCava = m_Compartments->CreateLiquidLink(vLeftRenalVein, vVenaCava, pulse::VascularLink::LeftKidneyToVenaCava);
  vLeftRenalVeinToVenaCava.MapPath(LeftRenalVeinToVenaCavaConnection);

  ///////////
  // Urine //
  ///////////

  //////////////////////////
  // RightTubulesToUreter //
  SELiquidCompartmentLink& uRightTubulesToUreter = m_Compartments->CreateLiquidLink(vRightTubules, uRightUreter, pulse::UrineLink::RightTubulesToUreter);
  uRightTubulesToUreter.MapPath(RightTubulesToUreter);
  //////////////////////////
  // RightUreterToBladder //
  SELiquidCompartmentLink& uRightUreterToBladder = m_Compartments->CreateLiquidLink(uRightUreter, uBladder, pulse::UrineLink::RightUreterToBladder);
  uRightUreterToBladder.MapPath(RightUreterToBladder);


  /////////////////////////
  // LeftTubulesToUreter //
  SELiquidCompartmentLink& uLeftTubulesToUreter = m_Compartments->CreateLiquidLink(vLeftTubules, uLeftUreter, pulse::UrineLink::LeftTubulesToUreter);
  uLeftTubulesToUreter.MapPath(LeftTubulesToUreter);
  /////////////////////////
  // LeftUreterToBladder //
  SELiquidCompartmentLink& uLeftUreterToBladder = m_Compartments->CreateLiquidLink(uLeftUreter, uBladder, pulse::UrineLink::LeftUreterToBladder);
  uLeftUreterToBladder.MapPath(LeftUreterToBladder);

  /////////////////////
  // BladderToGround //
  SELiquidCompartmentLink& uBladderToGround = m_Compartments->CreateLiquidLink(uBladder, vGround, pulse::UrineLink::BladderToGround);
  uBladderToGround.MapPath(BladderToGroundUrinate);
  SELiquidCompartmentLink& uBladderToGroundSource = m_Compartments->CreateLiquidLink(uBladder, vGround, pulse::UrineLink::BladderToGroundSource);
  uBladderToGroundSource.MapPath(BladderToGroundPressure);

  SELiquidCompartmentGraph& gRenal = m_Compartments->GetRenalGraph();
  gRenal.AddCompartment(vAorta);
  gRenal.AddCompartment(vVenaCava);
  // Left Blood
  gRenal.AddCompartment(vLeftRenalArtery);
  gRenal.AddCompartment(vLeftAfferentArteriole);
  gRenal.AddCompartment(vLeftGlomerularCapillaries);
  gRenal.AddCompartment(vLeftEfferentArteriole);
  gRenal.AddCompartment(vLeftPeritubularCapillaries);
  gRenal.AddCompartment(vLeftBowmansCapsules);
  gRenal.AddCompartment(vLeftTubules);
  gRenal.AddCompartment(vLeftRenalVein);
  gRenal.AddLink(vAortaToLeftRenalArtery);
  gRenal.AddLink(vLeftRenalArteryToAfferentArteriole);
  gRenal.AddLink(vLeftAfferentArterioleToGlomerularCapillaries);
  gRenal.AddLink(vLeftGlomerularCapillariesToEfferentArteriole);
  //gRenal.AddLink(vLeftGlomerularCapillariesToBowmansCapsules); //Active transport only
  gRenal.AddLink(vLeftBowmansCapsulesToTubules);
  //gRenal.AddLink(vLeftTubulesToPeritubularCapillaries); //Active transport only
  gRenal.AddLink(vLeftEfferentArterioleToPeritubularCapillaries);
  gRenal.AddLink(vLeftPeritubularCapillariesToRenalVein);
  gRenal.AddLink(vLeftRenalVeinToVenaCava);
  // Right Blood
  gRenal.AddCompartment(vRightRenalArtery);
  gRenal.AddCompartment(vRightAfferentArteriole);
  gRenal.AddCompartment(vRightGlomerularCapillaries);
  gRenal.AddCompartment(vRightEfferentArteriole);
  gRenal.AddCompartment(vRightPeritubularCapillaries);
  gRenal.AddCompartment(vRightBowmansCapsules);
  gRenal.AddCompartment(vRightTubules);
  gRenal.AddCompartment(vRightRenalVein);
  gRenal.AddLink(vAortaToRightRenalArtery);
  gRenal.AddLink(vRightRenalArteryToAfferentArteriole);
  gRenal.AddLink(vRightAfferentArterioleToGlomerularCapillaries);
  gRenal.AddLink(vRightGlomerularCapillariesToEfferentArteriole);
  //gRenal.AddLink(vRightGlomerularCapillariesToBowmansCapsules); //Active transport only
  gRenal.AddLink(vRightBowmansCapsulesToTubules);
  //gRenal.AddLink(vRightTubulesToPeritubularCapillaries); //Active transport only
  gRenal.AddLink(vRightEfferentArterioleToPeritubularCapillaries);
  gRenal.AddLink(vRightPeritubularCapillariesToRenalVein);
  gRenal.AddLink(vRightRenalVeinToVenaCava);
  // Left Urine
  gRenal.AddCompartment(uLeftUreter);
  gRenal.AddLink(uLeftTubulesToUreter);
  gRenal.AddLink(uLeftUreterToBladder);
  // Right Urine
  gRenal.AddCompartment(uRightUreter);
  gRenal.AddLink(uRightTubulesToUreter);
  gRenal.AddLink(uRightUreterToBladder);
  // Shared
  gRenal.AddCompartment(uBladder);
  gRenal.AddCompartment(vGround);
  gRenal.AddLink(uBladderToGround);
  gRenal.AddLink(uBladderToGroundSource);
  gRenal.StateChange();

  // We have discretized these compartments, so remove them
  SELiquidCompartmentGraph& gCombinedCardiovascular = m_Compartments->GetActiveCardiovascularGraph();
  gCombinedCardiovascular.RemoveCompartment(*vLeftKidney);
  gCombinedCardiovascular.RemoveCompartment(*vRightKidney);
  gCombinedCardiovascular.AddGraph(gRenal);
  gCombinedCardiovascular.StateChange();
}

void PulseController::SetupTissue()
{
  Info("Setting Up Tissue");
  SEFluidCircuit& cCardiovascular = m_Circuits->GetCardiovascularCircuit();
  SEFluidCircuit& cCombinedCardiovascular = m_Circuits->GetActiveCardiovascularCircuit();

  SEFluidCircuitNode* Ground = cCombinedCardiovascular.GetNode(pulse::CardiovascularNode::Ground);
  ///////////
  // Lymph //
  SEFluidCircuitNode& Lymph = cCombinedCardiovascular.CreateNode(pulse::TissueNode::Lymph);
  Lymph.GetPressure().SetValue(0, PressureUnit::mmHg);
  Lymph.GetVolumeBaseline().SetValue(0, VolumeUnit::mL);

  SEFluidCircuitNode* VenaCava = cCombinedCardiovascular.GetNode(pulse::CardiovascularNode::VenaCava);
  SEFluidCircuitPath& LymphToVenaCava = cCombinedCardiovascular.CreatePath(Lymph, *VenaCava, pulse::TissuePath::LymphToVenaCava);
  // No resistance - if it goes to lymph then it goes to vena cava
  SELiquidCompartment* cVenaCava = m_Compartments->GetLiquidCompartment(pulse::VascularCompartment::VenaCava);

  SELiquidCompartment& cLymph = m_Compartments->CreateLiquidCompartment(pulse::LymphCompartment::Lymph);
  cLymph.MapNode(Lymph);

  SELiquidCompartmentLink& lLymphToVenaCava = m_Compartments->CreateLiquidLink(cLymph, *cVenaCava, pulse::LymphLink::LymphToVenaCava);
  lLymphToVenaCava.MapPath(LymphToVenaCava);

  // The assumption for the vascular-tissue convection drag is that the resistance is inversely proportional to the tissue mass.
  // This is the proportionality constant
  double resistanceConstant = 20000.0; // Large resistance to prevent fluid from freely moving into the vascular space

  /// \todo Put Initial Circuit/Compartment data values into the configuration file.

  //Density (kg/L)
  double AdiposeTissueDensity = 1.03;
  double BoneTissueDensity = 1.3;
  double BrainTissueDensity = 1.0;
  double GutTissueDensity = 1.0;
  double RKidneyTissueDensity = 1.0;
  double LKidneyTissueDensity = 1.0;
  double LiverTissueDensity = 1.0;
  double RLungTissueDensity = 1.0;
  double LLungTissueDensity = 1.0;
  double MuscleTissueDensity = 1.0;
  double MyocardiumTissueDensity = 1.0;
  double SkinTissueDensity = 1.0;
  double SpleenTissueDensity = 1.0;

  // ExtracellcularWaterFraction        IntracellularWaterFraction    NeutralLipid                   NeutralPhospolipid             AlbuminRatio              AlphaAcidGlycoprotein       PlasmaLipoprotein        AcidicPhospohlipidConcentration
  double  AdiposeEWFraction = 0.135, AdiposeIWFraction = 0.017, AdiposeNLFraction = 0.79, AdiposeNPFraction = 0.002, AdiposeARatio = 0.049, AdiposeAAGRatio = 0.049, AdiposeLRatio = 0.068, AdiposeAPL = 0.4;
  double  BoneEWFraction = 0.1, BoneIWFraction = 0.346, BoneNLFraction = 0.074, BoneNPFraction = 0.0011, BoneARatio = 0.1, BoneAAGRatio = 0.1, BoneLRatio = 0.05, BoneAPL = 0.67;
  double  BrainEWFraction = 0.162, BrainIWFraction = 0.62, BrainNLFraction = 0.051, BrainNPFraction = 0.0565, BrainARatio = 0.048, BrainAAGRatio = 0.048, BrainLRatio = 0.041, BrainAPL = 0.4;
  double  GutEWFraction = 0.282, GutIWFraction = 0.475, GutNLFraction = 0.0487, GutNPFraction = 0.0163, GutARatio = 0.158, GutAAGRatio = 0.158, GutLRatio = 0.0141, GutAPL = 2.41;
  double  RKidneyEWFraction = 0.273, RKidneyIWFraction = 0.483, RKidneyNLFraction = 0.0207, RKidneyNPFraction = 0.0162, RKidneyARatio = 0.13, RKidneyAAGRatio = 0.13, RKidneyLRatio = 0.137, RKidneyAPL = 5.03;
  double  LKidneyEWFraction = 0.273, LKidneyIWFraction = 0.483, LKidneyNLFraction = 0.0207, LKidneyNPFraction = 0.0162, LKidneyARatio = 0.13, LKidneyAAGRatio = 0.13, LKidneyLRatio = 0.137, LKidneyAPL = 5.03;
  double  LiverEWFraction = 0.161, LiverIWFraction = 0.573, LiverNLFraction = 0.0348, LiverNPFraction = 0.0252, LiverARatio = 0.086, LiverAAGRatio = 0.086, LiverLRatio = 0.161, LiverAPL = 4.56;
  double  RLungEWFraction = 0.336, RLungIWFraction = 0.446, RLungNLFraction = 0.003, RLungNPFraction = 0.009, RLungARatio = 0.212, RLungAAGRatio = 0.212, RLungLRatio = 0.168, RLungAPL = 3.91;
  double  LLungEWFraction = 0.336, LLungIWFraction = 0.446, LLungNLFraction = 0.003, LLungNPFraction = 0.009, LLungARatio = 0.212, LLungAAGRatio = 0.212, LLungLRatio = 0.168, LLungAPL = 3.91;
  double  MuscleEWFraction = 0.118, MuscleIWFraction = 0.63, MuscleNLFraction = 0.0238, MuscleNPFraction = 0.0072, MuscleARatio = 0.064, MuscleAAGRatio = 0.064, MuscleLRatio = 0.059, MuscleAPL = 1.53;
  double  MyocardiumEWFraction = 0.32, MyocardiumIWFraction = 0.456, MyocardiumNLFraction = 0.0115, MyocardiumNPFraction = 0.0166, MyocardiumARatio = 0.157, MyocardiumAAGRatio = 0.157, MyocardiumLRatio = 0.16, MyocardiumAPL = 2.25;
  double  SkinEWFraction = 0.382, SkinIWFraction = 0.291, SkinNLFraction = 0.0284, SkinNPFraction = 0.0111, SkinARatio = 0.277, SkinAAGRatio = 0.277, SkinLRatio = 0.096, SkinAPL = 1.32;
  double  SpleenEWFraction = 0.207, SpleenIWFraction = 0.579, SpleenNLFraction = 0.0201, SpleenNPFraction = 0.0198, SpleenARatio = 0.277, SpleenAAGRatio = 0.277, SpleenLRatio = 0.096, SpleenAPL = 3.18;

  //Typical ICRP Male
  //Total Mass (kg)              
  double AdiposeTissueMass = 14.5;
  double BoneTissueMass = 10.5;
  double BrainTissueMass = 1.45;
  double GutTissueMass = 1.02;
  double RKidneyTissueMass = 0.155;
  double LKidneyTissueMass = 0.155;
  double LiverTissueMass = 1.8;
  double RLungTissueMass = 0.25;
  double LLungTissueMass = 0.25;
  double MuscleTissueMass = 29.0;
  double MyocardiumTissueMass = 0.33;
  double SkinTissueMass = 3.3;
  double SpleenTissueMass = 0.15;

  //Typical ICRP Female - From ICRP
  //Total Mass (kg)
  if (m_InitialPatient->GetSex() == ePatient_Sex::Female)
  {
    AdiposeTissueMass = 19.0;
    BoneTissueMass = 7.8;
    BrainTissueMass = 1.3;
    GutTissueMass = 0.96;
    RKidneyTissueMass = 0.1375;
    LKidneyTissueMass = 0.1375;
    LiverTissueMass = 1.4;
    RLungTissueMass = 0.21;
    LLungTissueMass = 0.21;
    MuscleTissueMass = 17.5;
    MyocardiumTissueMass = 0.25;
    SkinTissueMass = 2.3;
    SpleenTissueMass = 0.13;
  }

  //Scale things based on patient parameters -------------------------------

  //Modify adipose (i.e. fat) directly using the body fat fraction
  AdiposeTissueMass = m_InitialPatient->GetBodyFatFraction().GetValue() * m_InitialPatient->GetWeight().GetValue(MassUnit::kg);

  //Modify skin based on total surface area
  //Male
  double standardPatientWeight_lb = 170.0;
  double standardPatientHeight_in = 71.0;
  if (m_InitialPatient->GetSex() == ePatient_Sex::Female)
  {
    //Female
    standardPatientWeight_lb = 130.0;
    standardPatientHeight_in = 64.0;
  }
  double typicalSkinSurfaceArea_m2 = 0.20247 * pow(Convert(standardPatientWeight_lb, MassUnit::lb, MassUnit::kg), 0.425) * pow(Convert(standardPatientHeight_in, LengthUnit::in, LengthUnit::m), 0.725);
  double patientSkinArea_m2 = m_InitialPatient->GetSkinSurfaceArea(AreaUnit::m2);
  SkinTissueMass = SkinTissueMass * patientSkinArea_m2 / typicalSkinSurfaceArea_m2;

  //Modify most based on lean body mass
  //Hume, R (Jul 1966). "Prediction of lean body mass from height and weight." Journal of clinical pathology. 19 (4): 389�91. doi:10.1136/jcp.19.4.389. PMC 473290. PMID 5929341.
  //double typicalLeanBodyMass_kg = 0.32810 * Convert(standardPatientWeight_lb, MassUnit::lb, MassUnit::kg) + 0.33929 * Convert(standardPatientHeight_in, LengthUnit::in, LengthUnit::cm) - 29.5336; //Male
  //if (m_InitialPatient->GetSex() == ePatient_Sex::Female)
  //{
   // typicalLeanBodyMass_kg = 0.29569 * Convert(standardPatientWeight_lb, MassUnit::lb, MassUnit::kg) + 0.41813 * Convert(standardPatientHeight_in, LengthUnit::in, LengthUnit::cm) - 43.2933; //Female
  //}

  //Male
  double standardFatFraction = 0.21;
  if (m_InitialPatient->GetSex() == ePatient_Sex::Female)
  {
    //Female
    standardFatFraction = 0.28;
  }
  double standardLeanBodyMass_kg = Convert(standardPatientWeight_lb, MassUnit::lb, MassUnit::kg) * (1.0 - standardFatFraction);
  double patientLeanBodyMass_kg = m_InitialPatient->GetLeanBodyMass(MassUnit::kg);
  double leanBodyMassFractionOfTypical = patientLeanBodyMass_kg / standardLeanBodyMass_kg;

  BoneTissueMass *= leanBodyMassFractionOfTypical;
  GutTissueMass *= leanBodyMassFractionOfTypical;
  RKidneyTissueMass *= leanBodyMassFractionOfTypical;
  LKidneyTissueMass *= leanBodyMassFractionOfTypical;
  LiverTissueMass *= leanBodyMassFractionOfTypical;
  RLungTissueMass *= leanBodyMassFractionOfTypical;
  LLungTissueMass *= leanBodyMassFractionOfTypical;
  MuscleTissueMass *= leanBodyMassFractionOfTypical;
  MyocardiumTissueMass *= leanBodyMassFractionOfTypical;
  SpleenTissueMass *= leanBodyMassFractionOfTypical;

  //Note: Brain doesn't change

  //Total Volume(L)
  double AdiposeTissueVolume = AdiposeTissueMass / AdiposeTissueDensity;
  double BoneTissueVolume = BoneTissueMass / BoneTissueDensity;
  double BrainTissueVolume = BrainTissueMass / BrainTissueDensity;
  double GutTissueVolume = GutTissueMass / GutTissueDensity;
  double RKidneyTissueVolume = RKidneyTissueMass / RKidneyTissueDensity;
  double LKidneyTissueVolume = LKidneyTissueMass / LKidneyTissueDensity;
  double LiverTissueVolume = LiverTissueMass / LiverTissueDensity;
  double RLungTissueVolume = RLungTissueMass / RLungTissueDensity;
  double LLungTissueVolume = LLungTissueMass / LLungTissueDensity;
  double MuscleTissueVolume = MuscleTissueMass / MuscleTissueDensity;
  double MyocardiumTissueVolume = MyocardiumTissueMass / MyocardiumTissueDensity;
  double SkinTissueVolume = SkinTissueMass / SkinTissueDensity;
  double SpleenTissueVolume = SpleenTissueMass / SpleenTissueDensity;

  //Create the circuit -------------------------------

  // Note on colloid osmotic pressure
  // Vascular oncotic pressure sources are currently set to zero, and a constant oncotic pressure is incorporated
  // into the hydrostatic pressure stored in the tissue compliance elements.
  // In a future release the oncotic pressure sources will be set dynamically based on the Landis-Pappenheimer equation

  /////////
  // Fat //
  SEFluidCircuitNode* Fat1 = cCombinedCardiovascular.GetNode(pulse::CardiovascularNode::Fat1);
  SEFluidCircuitNode& FatT1 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::FatT1);
  SEFluidCircuitNode& FatT2 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::FatT2);
  SEFluidCircuitNode& FatT3 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::FatT3);
  FatT1.GetPressure().Set(Fat1->GetPressure());
  FatT3.GetPressure().Set(Ground->GetPressure());
  FatT1.GetVolumeBaseline().SetValue(AdiposeEWFraction * AdiposeTissueVolume * 1000.0, VolumeUnit::mL);

  SEFluidCircuitPath& Fat1ToFatT2 = cCombinedCardiovascular.CreatePath(*Fat1, FatT2, pulse::TissuePath::Fat1ToFatT2);
  Fat1ToFatT2.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::mmHg);
  SEFluidCircuitPath& FatT2ToFatT1 = cCombinedCardiovascular.CreatePath(FatT2, FatT1, pulse::TissuePath::FatT2ToFatT1);
  FatT2ToFatT1.GetResistanceBaseline().SetValue((1 / AdiposeTissueMass) * resistanceConstant, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& FatT1ToFatT3 = cCombinedCardiovascular.CreatePath(FatT1, FatT3, pulse::TissuePath::FatT1ToFatT3);
  FatT1ToFatT3.GetComplianceBaseline().SetValue(FatT1.GetVolumeBaseline(VolumeUnit::mL) / FatT1.GetPressure(PressureUnit::mmHg), VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& GroundToFatT3 = cCombinedCardiovascular.CreatePath(*Ground, FatT3, pulse::TissuePath::GroundToFatT3);
  GroundToFatT3.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::mmHg);

  SEFluidCircuitPath& FatT1ToLymph = cCombinedCardiovascular.CreatePath(FatT1, Lymph, pulse::TissuePath::FatT1ToLymph);
  FatT1ToLymph.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);

  SETissueCompartment& FatTissue = m_Compartments->CreateTissueCompartment(pulse::TissueCompartment::Fat);
  SELiquidCompartment& FatExtracellular = m_Compartments->CreateLiquidCompartment(pulse::ExtravascularCompartment::FatExtracellular);
  SELiquidCompartment& FatIntracellular = m_Compartments->CreateLiquidCompartment(pulse::ExtravascularCompartment::FatIntracellular);
  FatTissue.GetMatrixVolume().SetValue((1 - AdiposeEWFraction - AdiposeIWFraction) * AdiposeTissueVolume * 1000.0, VolumeUnit::mL);
  FatExtracellular.MapNode(FatT1);
  FatExtracellular.MapNode(FatT2);
  FatExtracellular.MapNode(FatT3);
  FatExtracellular.GetWaterVolumeFraction().SetValue(AdiposeEWFraction);
  FatIntracellular.GetVolume().SetValue(AdiposeIWFraction * AdiposeTissueVolume * 1000.0, VolumeUnit::mL);
  FatIntracellular.GetWaterVolumeFraction().SetValue(AdiposeIWFraction);
  FatTissue.GetAcidicPhospohlipidConcentration().SetValue(AdiposeAPL, MassPerMassUnit::mg_Per_g);
  FatTissue.GetNeutralLipidsVolumeFraction().SetValue(AdiposeNLFraction);
  FatTissue.GetNeutralPhospholipidsVolumeFraction().SetValue(AdiposeNPFraction);
  FatTissue.GetTissueToPlasmaAlbuminRatio().SetValue(AdiposeARatio);
  FatTissue.GetTissueToPlasmaAlphaAcidGlycoproteinRatio().SetValue(AdiposeAAGRatio);
  FatTissue.GetTissueToPlasmaLipoproteinRatio().SetValue(AdiposeLRatio);
  FatTissue.GetTotalMass().SetValue(AdiposeTissueMass, MassUnit::kg);

  SELiquidCompartmentLink& FatVascularToTissue = m_Compartments->CreateLiquidLink(*m_Compartments->GetLiquidCompartment(pulse::VascularCompartment::Fat),
    FatExtracellular, pulse::VascularLink::FatVascularToTissue);
  FatVascularToTissue.MapPath(Fat1ToFatT2);

  SELiquidCompartmentLink& FatTissueToLymph = m_Compartments->CreateLiquidLink(FatExtracellular, cLymph, pulse::LymphLink::FatTissueToLymph);
  FatTissueToLymph.MapPath(FatT1ToLymph);

  //////////
  // Bone //
  SEFluidCircuitNode* Bone1 = cCombinedCardiovascular.GetNode(pulse::CardiovascularNode::Bone1);
  SEFluidCircuitNode& BoneT1 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::BoneT1);
  SEFluidCircuitNode& BoneT2 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::BoneT2);
  SEFluidCircuitNode& BoneT3 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::BoneT3);
  BoneT1.GetPressure().Set(Bone1->GetPressure());
  BoneT3.GetPressure().Set(Ground->GetPressure());
  BoneT1.GetVolumeBaseline().SetValue(BoneEWFraction * BoneTissueVolume * 1000.0, VolumeUnit::mL);

  SEFluidCircuitPath& Bone1ToBoneT2 = cCombinedCardiovascular.CreatePath(*Bone1, BoneT2, pulse::TissuePath::Bone1ToBoneT2);
  Bone1ToBoneT2.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::mmHg);
  SEFluidCircuitPath& BoneT2ToBoneT1 = cCombinedCardiovascular.CreatePath(BoneT2, BoneT1, pulse::TissuePath::BoneT2ToBoneT1);
  BoneT2ToBoneT1.GetResistanceBaseline().SetValue((1 / BoneTissueMass) * resistanceConstant, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& BoneT1ToBoneT3 = cCombinedCardiovascular.CreatePath(BoneT1, BoneT3, pulse::TissuePath::BoneT1ToBoneT3);
  BoneT1ToBoneT3.GetComplianceBaseline().SetValue(BoneT1.GetVolumeBaseline(VolumeUnit::mL) / BoneT1.GetPressure(PressureUnit::mmHg), VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& GroundToBoneT3 = cCombinedCardiovascular.CreatePath(*Ground, BoneT3, pulse::TissuePath::GroundToBoneT3);
  GroundToBoneT3.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::mmHg);

  SEFluidCircuitPath& BoneT1ToLymph = cCombinedCardiovascular.CreatePath(BoneT1, Lymph, pulse::TissuePath::BoneT1ToLymph);
  BoneT1ToLymph.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);

  SETissueCompartment& BoneTissue = m_Compartments->CreateTissueCompartment(pulse::TissueCompartment::Bone);
  SELiquidCompartment& BoneExtracellular = m_Compartments->CreateLiquidCompartment(pulse::ExtravascularCompartment::BoneExtracellular);
  SELiquidCompartment& BoneIntracellular = m_Compartments->CreateLiquidCompartment(pulse::ExtravascularCompartment::BoneIntracellular);
  BoneTissue.GetMatrixVolume().SetValue((1 - BoneEWFraction - BoneIWFraction) * BoneTissueVolume * 1000.0, VolumeUnit::mL);
  BoneExtracellular.MapNode(BoneT1);
  BoneExtracellular.MapNode(BoneT2);
  BoneExtracellular.MapNode(BoneT3);
  BoneExtracellular.GetWaterVolumeFraction().SetValue(BoneEWFraction);
  BoneIntracellular.GetVolume().SetValue(BoneIWFraction * BoneTissueVolume * 1000.0, VolumeUnit::mL);
  BoneIntracellular.GetWaterVolumeFraction().SetValue(BoneIWFraction);
  BoneTissue.GetAcidicPhospohlipidConcentration().SetValue(BoneAPL, MassPerMassUnit::mg_Per_g);
  BoneTissue.GetNeutralLipidsVolumeFraction().SetValue(BoneNLFraction);
  BoneTissue.GetNeutralPhospholipidsVolumeFraction().SetValue(BoneNPFraction);
  BoneTissue.GetTissueToPlasmaAlphaAcidGlycoproteinRatio().SetValue(BoneAAGRatio);
  BoneTissue.GetTissueToPlasmaAlbuminRatio().SetValue(BoneARatio);
  BoneTissue.GetTissueToPlasmaLipoproteinRatio().SetValue(BoneLRatio);
  BoneTissue.GetTotalMass().SetValue(BoneTissueMass, MassUnit::kg);

  SELiquidCompartmentLink& BoneVascularToTissue = m_Compartments->CreateLiquidLink(*m_Compartments->GetLiquidCompartment(pulse::VascularCompartment::Bone),
    BoneExtracellular, pulse::VascularLink::BoneVascularToTissue);
  BoneVascularToTissue.MapPath(Bone1ToBoneT2);

  SELiquidCompartmentLink& BoneTissueToLymph = m_Compartments->CreateLiquidLink(BoneExtracellular, cLymph, pulse::LymphLink::BoneTissueToLymph);
  BoneTissueToLymph.MapPath(BoneT1ToLymph);

  ///////////
  // Brain //
  SEFluidCircuitNode* Brain1 = cCombinedCardiovascular.GetNode(pulse::CardiovascularNode::Brain1);
  SEFluidCircuitNode& BrainT1 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::BrainT1);
  SEFluidCircuitNode& BrainT2 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::BrainT2);
  SEFluidCircuitNode& BrainT3 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::BrainT3);
  BrainT1.GetPressure().Set(Brain1->GetPressure());
  BrainT3.GetPressure().Set(Ground->GetPressure());
  BrainT1.GetVolumeBaseline().SetValue(BrainEWFraction * BrainTissueVolume * 1000.0, VolumeUnit::mL);

  SEFluidCircuitPath& Brain1ToBrainT2 = cCombinedCardiovascular.CreatePath(*Brain1, BrainT2, pulse::TissuePath::Brain1ToBrainT2);
  Brain1ToBrainT2.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::mmHg);
  SEFluidCircuitPath& BrainT2ToBrainT1 = cCombinedCardiovascular.CreatePath(BrainT2, BrainT1, pulse::TissuePath::BrainT2ToBrainT1);
  BrainT2ToBrainT1.GetResistanceBaseline().SetValue((1 / BrainTissueMass) * resistanceConstant, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& BrainT1ToBrainT3 = cCombinedCardiovascular.CreatePath(BrainT1, BrainT3, pulse::TissuePath::BrainT1ToBrainT3);
  BrainT1ToBrainT3.GetComplianceBaseline().SetValue(BrainT1.GetVolumeBaseline(VolumeUnit::mL) / BrainT1.GetPressure(PressureUnit::mmHg), VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& GroundToBrainT3 = cCombinedCardiovascular.CreatePath(*Ground, BrainT3, pulse::TissuePath::GroundToBrainT3);
  GroundToBrainT3.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::mmHg);

  SEFluidCircuitPath& BrainT1ToLymph = cCombinedCardiovascular.CreatePath(BrainT1, Lymph, pulse::TissuePath::BrainT1ToLymph);
  BrainT1ToLymph.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);

  SETissueCompartment& BrainTissue = m_Compartments->CreateTissueCompartment(pulse::TissueCompartment::Brain);
  SELiquidCompartment& BrainExtracellular = m_Compartments->CreateLiquidCompartment(pulse::ExtravascularCompartment::BrainExtracellular);
  SELiquidCompartment& BrainIntracellular = m_Compartments->CreateLiquidCompartment(pulse::ExtravascularCompartment::BrainIntracellular);
  BrainTissue.GetMatrixVolume().SetValue((1 - BrainEWFraction - BrainIWFraction) * BrainTissueVolume * 1000.0, VolumeUnit::mL);
  BrainExtracellular.MapNode(BrainT1);
  BrainExtracellular.MapNode(BrainT2);
  BrainExtracellular.MapNode(BrainT3);
  BrainExtracellular.GetWaterVolumeFraction().SetValue(BrainEWFraction);
  BrainIntracellular.GetVolume().SetValue(BrainIWFraction * BrainTissueVolume * 1000.0, VolumeUnit::mL);
  BrainIntracellular.GetWaterVolumeFraction().SetValue(BrainIWFraction);
  BrainTissue.GetAcidicPhospohlipidConcentration().SetValue(BrainAPL, MassPerMassUnit::mg_Per_g);
  BrainTissue.GetNeutralLipidsVolumeFraction().SetValue(BrainNLFraction);
  BrainTissue.GetNeutralPhospholipidsVolumeFraction().SetValue(BrainNPFraction);
  BrainTissue.GetTissueToPlasmaAlphaAcidGlycoproteinRatio().SetValue(BrainAAGRatio);
  BrainTissue.GetTissueToPlasmaAlbuminRatio().SetValue(BrainARatio);
  BrainTissue.GetTissueToPlasmaLipoproteinRatio().SetValue(BrainLRatio);
  BrainTissue.GetTotalMass().SetValue(BrainTissueMass, MassUnit::kg);

  SELiquidCompartmentLink& BrainVascularToTissue = m_Compartments->CreateLiquidLink(*m_Compartments->GetLiquidCompartment(pulse::VascularCompartment::Brain),
    BrainExtracellular, pulse::VascularLink::BrainVascularToTissue);
  BrainVascularToTissue.MapPath(Brain1ToBrainT2);

  SELiquidCompartmentLink& BrainTissueToLymph = m_Compartments->CreateLiquidLink(BrainExtracellular, cLymph, pulse::LymphLink::BrainTissueToLymph);
  BrainTissueToLymph.MapPath(BrainT1ToLymph);

  /////////
  // Gut //
  SEFluidCircuitNode* SmallIntestine = cCardiovascular.GetNode(pulse::CardiovascularNode::SmallIntestine1);
  SEFluidCircuitNode* LargeIntestine = cCardiovascular.GetNode(pulse::CardiovascularNode::LargeIntestine1);
  SEFluidCircuitNode* Splanchnic = cCardiovascular.GetNode(pulse::CardiovascularNode::Splanchnic1);
  SEFluidCircuitNode& GutT1 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::GutT1);
  SEFluidCircuitNode& GutT2 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::GutT2);
  SEFluidCircuitNode& GutT3 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::GutT3);
  GutT1.GetPressure().Set(SmallIntestine->GetPressure());
  GutT3.GetPressure().Set(Ground->GetPressure());
  GutT1.GetVolumeBaseline().SetValue(GutEWFraction * GutTissueVolume * 1000.0, VolumeUnit::mL);

  SEFluidCircuitPath& SmallIntestine1ToGutT2 = cCombinedCardiovascular.CreatePath(*SmallIntestine, GutT2, pulse::TissuePath::SmallIntestine1ToGutT2);
  SmallIntestine1ToGutT2.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::mmHg);
  SEFluidCircuitPath& LargeIntestine1ToGutT2 = cCombinedCardiovascular.CreatePath(*LargeIntestine, GutT2, pulse::TissuePath::LargeIntestine1ToGutT2);
  LargeIntestine1ToGutT2.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::mmHg);
  SEFluidCircuitPath& Splanchnic1ToGutT2 = cCombinedCardiovascular.CreatePath(*Splanchnic, GutT2, pulse::TissuePath::Splanchnic1ToGutT2);
  Splanchnic1ToGutT2.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::mmHg);
  SEFluidCircuitPath& GutT2ToGutT1 = cCombinedCardiovascular.CreatePath(GutT2, GutT1, pulse::TissuePath::GutT2ToGutT1);
  GutT2ToGutT1.GetResistanceBaseline().SetValue((1 / GutTissueMass) * resistanceConstant, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& GutT1ToGutT3 = cCombinedCardiovascular.CreatePath(GutT1, GutT3, pulse::TissuePath::GutT1ToGutT3);
  GutT1ToGutT3.GetComplianceBaseline().SetValue(GutT1.GetVolumeBaseline(VolumeUnit::mL) / GutT1.GetPressure(PressureUnit::mmHg), VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& GroundToGutT3 = cCombinedCardiovascular.CreatePath(*Ground, GutT3, pulse::TissuePath::GroundToGutT3);
  GroundToGutT3.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::mmHg);

  SEFluidCircuitPath& GutT1ToLymph = cCombinedCardiovascular.CreatePath(GutT1, Lymph, pulse::TissuePath::GutT1ToLymph);
  GutT1ToLymph.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);

  SETissueCompartment& GutTissue = m_Compartments->CreateTissueCompartment(pulse::TissueCompartment::Gut);
  SELiquidCompartment& GutExtracellular = m_Compartments->CreateLiquidCompartment(pulse::ExtravascularCompartment::GutExtracellular);
  SELiquidCompartment& GutIntracellular = m_Compartments->CreateLiquidCompartment(pulse::ExtravascularCompartment::GutIntracellular);
  GutTissue.GetMatrixVolume().SetValue((1 - GutEWFraction - GutIWFraction) * GutTissueVolume * 1000.0, VolumeUnit::mL);
  GutExtracellular.MapNode(GutT1);
  GutExtracellular.MapNode(GutT2);
  GutExtracellular.MapNode(GutT3);
  GutExtracellular.GetWaterVolumeFraction().SetValue(GutEWFraction);
  GutIntracellular.GetVolume().SetValue(GutIWFraction * GutTissueVolume * 1000.0, VolumeUnit::mL);
  GutIntracellular.GetWaterVolumeFraction().SetValue(GutIWFraction);
  GutTissue.GetAcidicPhospohlipidConcentration().SetValue(GutAPL, MassPerMassUnit::mg_Per_g);
  GutTissue.GetNeutralLipidsVolumeFraction().SetValue(GutNLFraction);
  GutTissue.GetNeutralPhospholipidsVolumeFraction().SetValue(GutNPFraction);
  GutTissue.GetTissueToPlasmaAlphaAcidGlycoproteinRatio().SetValue(GutAAGRatio);
  GutTissue.GetTissueToPlasmaAlbuminRatio().SetValue(GutARatio);
  GutTissue.GetTissueToPlasmaLipoproteinRatio().SetValue(GutLRatio);
  GutTissue.GetTotalMass().SetValue(GutTissueMass, MassUnit::kg);

  SELiquidCompartmentLink& SmallIntestineVascularToTissue = m_Compartments->CreateLiquidLink(*m_Compartments->GetLiquidCompartment(pulse::VascularCompartment::SmallIntestine),
    GutExtracellular, pulse::VascularLink::SmallIntestineVascularToTissue);
  SmallIntestineVascularToTissue.MapPath(SmallIntestine1ToGutT2);

  SELiquidCompartmentLink& LargeIntestineVascularToTissue = m_Compartments->CreateLiquidLink(*m_Compartments->GetLiquidCompartment(pulse::VascularCompartment::LargeIntestine),
    GutExtracellular, pulse::VascularLink::LargeIntestineVascularToTissue);
  LargeIntestineVascularToTissue.MapPath(LargeIntestine1ToGutT2);

  SELiquidCompartmentLink& SplanchnicVascularToTissue = m_Compartments->CreateLiquidLink(*m_Compartments->GetLiquidCompartment(pulse::VascularCompartment::Splanchnic),
    GutExtracellular, pulse::VascularLink::SplanchnicVascularToTissue);
  SplanchnicVascularToTissue.MapPath(Splanchnic1ToGutT2);

  SELiquidCompartmentLink& GutTissueToLymph = m_Compartments->CreateLiquidLink(GutExtracellular, cLymph, pulse::LymphLink::GutTissueToLymph);
  GutTissueToLymph.MapPath(GutT1ToLymph);

  /////////////////
  // Left Kidney //
  SEFluidCircuitNode* LeftKidney1;
  if (!m_Config->IsRenalEnabled())
    LeftKidney1 = cCombinedCardiovascular.GetNode(pulse::CardiovascularNode::LeftKidney1);
  else
    LeftKidney1 = cCombinedCardiovascular.GetNode(pulse::RenalNode::LeftGlomerularCapillaries);

  SEFluidCircuitNode& LeftKidneyT1 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::LeftKidneyT1);
  SEFluidCircuitNode& LeftKidneyT2 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::LeftKidneyT2);
  SEFluidCircuitNode& LeftKidneyT3 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::LeftKidneyT3);
  LeftKidneyT1.GetPressure().Set(LeftKidney1->GetPressure());
  LeftKidneyT3.GetPressure().Set(Ground->GetPressure());
  LeftKidneyT1.GetVolumeBaseline().SetValue(LKidneyEWFraction * LKidneyTissueVolume * 1000.0, VolumeUnit::mL);

  SEFluidCircuitPath& LeftKidney1ToLeftKidneyT2 = cCombinedCardiovascular.CreatePath(*LeftKidney1, LeftKidneyT2, pulse::TissuePath::LeftKidney1ToLeftKidneyT2);
  LeftKidney1ToLeftKidneyT2.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::mmHg);
  SEFluidCircuitPath& LeftKidneyT2ToLeftKidneyT1 = cCombinedCardiovascular.CreatePath(LeftKidneyT2, LeftKidneyT1, pulse::TissuePath::LeftKidneyT2ToLeftKidneyT1);
  LeftKidneyT2ToLeftKidneyT1.GetResistanceBaseline().SetValue((1 / LKidneyTissueMass) * resistanceConstant, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& LeftKidneyT1ToLeftKidneyT3 = cCombinedCardiovascular.CreatePath(LeftKidneyT1, LeftKidneyT3, pulse::TissuePath::LeftKidneyT1ToLeftKidneyT3);
  LeftKidneyT1ToLeftKidneyT3.GetComplianceBaseline().SetValue(LeftKidneyT1.GetVolumeBaseline(VolumeUnit::mL) / LeftKidneyT1.GetPressure(PressureUnit::mmHg), VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& GroundToLeftKidneyT3 = cCombinedCardiovascular.CreatePath(*Ground, LeftKidneyT3, pulse::TissuePath::GroundToLeftKidneyT3);
  GroundToLeftKidneyT3.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::mmHg);

  SEFluidCircuitPath& LeftKidneyT1ToLymph = cCombinedCardiovascular.CreatePath(LeftKidneyT1, Lymph, pulse::TissuePath::LeftKidneyT1ToLymph);
  LeftKidneyT1ToLymph.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);

  SETissueCompartment& LeftKidneyTissue = m_Compartments->CreateTissueCompartment(pulse::TissueCompartment::LeftKidney);
  SELiquidCompartment& LeftKidneyExtracellular = m_Compartments->CreateLiquidCompartment(pulse::ExtravascularCompartment::LeftKidneyExtracellular);
  SELiquidCompartment& LeftKidneyIntracellular = m_Compartments->CreateLiquidCompartment(pulse::ExtravascularCompartment::LeftKidneyIntracellular);
  LeftKidneyTissue.GetMatrixVolume().SetValue((1 - LKidneyEWFraction - LKidneyIWFraction) * LKidneyTissueVolume * 1000.0, VolumeUnit::mL);
  LeftKidneyExtracellular.MapNode(LeftKidneyT1);
  LeftKidneyExtracellular.MapNode(LeftKidneyT2);
  LeftKidneyExtracellular.MapNode(LeftKidneyT3);
  LeftKidneyExtracellular.GetWaterVolumeFraction().SetValue(LKidneyEWFraction);
  LeftKidneyIntracellular.GetVolume().SetValue(LKidneyIWFraction * LKidneyTissueVolume * 1000.0, VolumeUnit::mL);
  LeftKidneyIntracellular.GetWaterVolumeFraction().SetValue(LKidneyIWFraction);
  LeftKidneyTissue.GetAcidicPhospohlipidConcentration().SetValue(LKidneyAPL, MassPerMassUnit::mg_Per_g);
  LeftKidneyTissue.GetNeutralLipidsVolumeFraction().SetValue(LKidneyNLFraction);
  LeftKidneyTissue.GetNeutralPhospholipidsVolumeFraction().SetValue(LKidneyNPFraction);
  LeftKidneyTissue.GetTissueToPlasmaAlphaAcidGlycoproteinRatio().SetValue(LKidneyAAGRatio);
  LeftKidneyTissue.GetTissueToPlasmaAlbuminRatio().SetValue(LKidneyARatio);
  LeftKidneyTissue.GetTissueToPlasmaLipoproteinRatio().SetValue(LKidneyLRatio);
  LeftKidneyTissue.GetTotalMass().SetValue(LKidneyTissueMass, MassUnit::kg);

  SELiquidCompartmentLink& LeftKidneyVascularToTissue = m_Compartments->CreateLiquidLink(*m_Compartments->GetLiquidCompartment(pulse::VascularCompartment::LeftKidney),
    LeftKidneyExtracellular, pulse::VascularLink::LeftKidneyVascularToTissue);
  LeftKidneyVascularToTissue.MapPath(LeftKidney1ToLeftKidneyT2);

  SELiquidCompartmentLink& LeftKidneyTissueToLymph = m_Compartments->CreateLiquidLink(LeftKidneyExtracellular, cLymph, pulse::LymphLink::LeftKidneyTissueToLymph);
  LeftKidneyTissueToLymph.MapPath(LeftKidneyT1ToLymph);

  ///////////////
  // Left Lung //
  SEFluidCircuitNode* LeftLung1 = cCardiovascular.GetNode(pulse::CardiovascularNode::LeftPulmonaryCapillaries);
  SEFluidCircuitNode& LeftLungT1 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::LeftLungT1);
  SEFluidCircuitNode& LeftLungT2 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::LeftLungT2);
  SEFluidCircuitNode& LeftLungT3 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::LeftLungT3);
  LeftLungT1.GetPressure().Set(LeftLung1->GetPressure());
  LeftLungT3.GetPressure().Set(Ground->GetPressure());
  LeftLungT1.GetVolumeBaseline().SetValue(LLungEWFraction * LLungTissueVolume * 1000.0, VolumeUnit::mL);

  SEFluidCircuitPath& LeftLung1ToLeftLungT2 = cCombinedCardiovascular.CreatePath(*LeftLung1, LeftLungT2, pulse::TissuePath::LeftLung1ToLeftLungT2);
  LeftLung1ToLeftLungT2.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::mmHg);
  SEFluidCircuitPath& LeftLungT2ToLeftLungT1 = cCombinedCardiovascular.CreatePath(LeftLungT2, LeftLungT1, pulse::TissuePath::LeftLungT2ToLeftLungT1);
  LeftLungT2ToLeftLungT1.GetResistanceBaseline().SetValue((1 / LLungTissueMass) * resistanceConstant, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& LeftLungT1ToLeftLungT3 = cCombinedCardiovascular.CreatePath(LeftLungT1, LeftLungT3, pulse::TissuePath::LeftLungT1ToLeftLungT3);
  LeftLungT1ToLeftLungT3.GetComplianceBaseline().SetValue(LeftLungT1.GetVolumeBaseline(VolumeUnit::mL) / LeftLungT1.GetPressure(PressureUnit::mmHg), VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& GroundToLeftLungT3 = cCombinedCardiovascular.CreatePath(*Ground, LeftLungT3, pulse::TissuePath::GroundToLeftLungT3);
  GroundToLeftLungT3.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::mmHg);

  SEFluidCircuitPath& LeftLungT1ToLymph = cCombinedCardiovascular.CreatePath(LeftLungT1, Lymph, pulse::TissuePath::LeftLungT1ToLymph);
  LeftLungT1ToLymph.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);

  SETissueCompartment& LeftLungTissue = m_Compartments->CreateTissueCompartment(pulse::TissueCompartment::LeftLung);
  SELiquidCompartment& LeftLungExtracellular = m_Compartments->CreateLiquidCompartment(pulse::ExtravascularCompartment::LeftLungExtracellular);
  SELiquidCompartment& LeftLungIntracellular = m_Compartments->CreateLiquidCompartment(pulse::ExtravascularCompartment::LeftLungIntracellular);
  LeftLungTissue.GetMatrixVolume().SetValue((1 - LLungEWFraction - LLungIWFraction) * LLungTissueVolume * 1000.0, VolumeUnit::mL);
  LeftLungExtracellular.MapNode(LeftLungT1);
  LeftLungExtracellular.MapNode(LeftLungT2);
  LeftLungExtracellular.MapNode(LeftLungT3);
  LeftLungExtracellular.GetWaterVolumeFraction().SetValue(LLungEWFraction);
  LeftLungIntracellular.GetVolume().SetValue(LLungIWFraction * LLungTissueVolume * 1000.0, VolumeUnit::mL);
  LeftLungIntracellular.GetWaterVolumeFraction().SetValue(LLungIWFraction);
  LeftLungTissue.GetAcidicPhospohlipidConcentration().SetValue(LLungAPL, MassPerMassUnit::mg_Per_g);
  LeftLungTissue.GetNeutralLipidsVolumeFraction().SetValue(LLungNLFraction);
  LeftLungTissue.GetNeutralPhospholipidsVolumeFraction().SetValue(LLungNPFraction);
  LeftLungTissue.GetTissueToPlasmaAlbuminRatio().SetValue(LLungARatio);
  LeftLungTissue.GetTissueToPlasmaLipoproteinRatio().SetValue(LLungLRatio);
  LeftLungTissue.GetTissueToPlasmaAlphaAcidGlycoproteinRatio().SetValue(LLungAAGRatio);
  LeftLungTissue.GetTotalMass().SetValue(LLungTissueMass, MassUnit::kg);

  SELiquidCompartmentLink& LeftLungVascularToTissue = m_Compartments->CreateLiquidLink(*m_Compartments->GetLiquidCompartment(pulse::VascularCompartment::LeftLung),
    LeftLungExtracellular, pulse::VascularLink::LeftLungVascularToTissue);
  LeftLungVascularToTissue.MapPath(LeftLung1ToLeftLungT2);

  SELiquidCompartmentLink& LeftLungTissueToLymph = m_Compartments->CreateLiquidLink(LeftLungExtracellular, cLymph, pulse::LymphLink::LeftLungTissueToLymph);
  LeftLungTissueToLymph.MapPath(LeftLungT1ToLymph);

  ///////////
  // Liver //
  SEFluidCircuitNode* Liver1 = cCardiovascular.GetNode(pulse::CardiovascularNode::Liver1);
  SEFluidCircuitNode& LiverT1 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::LiverT1);
  SEFluidCircuitNode& LiverT2 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::LiverT2);
  SEFluidCircuitNode& LiverT3 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::LiverT3);
  LiverT1.GetPressure().Set(Liver1->GetPressure());
  LiverT3.GetPressure().Set(Ground->GetPressure());
  LiverT1.GetVolumeBaseline().SetValue(LiverEWFraction * LiverTissueVolume * 1000.0, VolumeUnit::mL);

  SEFluidCircuitPath& Liver1ToLiverT2 = cCombinedCardiovascular.CreatePath(*Liver1, LiverT2, pulse::TissuePath::Liver1ToLiverT2);
  Liver1ToLiverT2.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::mmHg);
  SEFluidCircuitPath& LiverT2ToLiverT1 = cCombinedCardiovascular.CreatePath(LiverT2, LiverT1, pulse::TissuePath::LiverT2ToLiverT1);
  LiverT2ToLiverT1.GetResistanceBaseline().SetValue((1 / LiverTissueMass) * resistanceConstant, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& LiverT1ToLiverT3 = cCombinedCardiovascular.CreatePath(LiverT1, LiverT3, pulse::TissuePath::LiverT1ToLiverT3);
  LiverT1ToLiverT3.GetComplianceBaseline().SetValue(LiverT1.GetVolumeBaseline(VolumeUnit::mL) / LiverT1.GetPressure(PressureUnit::mmHg), VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& GroundToLiverT3 = cCombinedCardiovascular.CreatePath(*Ground, LiverT3, pulse::TissuePath::GroundToLiverT3);
  GroundToLiverT3.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::mmHg);

  SEFluidCircuitPath& LiverT1ToLymph = cCombinedCardiovascular.CreatePath(LiverT1, Lymph, pulse::TissuePath::LiverT1ToLymph);
  LiverT1ToLymph.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);

  SETissueCompartment& LiverTissue = m_Compartments->CreateTissueCompartment(pulse::TissueCompartment::Liver);
  SELiquidCompartment& LiverExtracellular = m_Compartments->CreateLiquidCompartment(pulse::ExtravascularCompartment::LiverExtracellular);
  SELiquidCompartment& LiverIntracellular = m_Compartments->CreateLiquidCompartment(pulse::ExtravascularCompartment::LiverIntracellular);
  LiverTissue.GetMatrixVolume().SetValue((1 - LiverEWFraction - LiverIWFraction) * LiverTissueVolume * 1000.0, VolumeUnit::mL);
  LiverExtracellular.MapNode(LiverT1);
  LiverExtracellular.MapNode(LiverT2);
  LiverExtracellular.MapNode(LiverT3);
  LiverExtracellular.GetWaterVolumeFraction().SetValue(LiverEWFraction);
  LiverIntracellular.GetVolume().SetValue(LiverIWFraction * LiverTissueVolume * 1000.0, VolumeUnit::mL);
  LiverIntracellular.GetWaterVolumeFraction().SetValue(LiverIWFraction);
  LiverTissue.GetAcidicPhospohlipidConcentration().SetValue(LiverAPL, MassPerMassUnit::mg_Per_g);
  LiverTissue.GetNeutralLipidsVolumeFraction().SetValue(LiverNLFraction);
  LiverTissue.GetNeutralPhospholipidsVolumeFraction().SetValue(LiverNPFraction);
  LiverTissue.GetTissueToPlasmaAlphaAcidGlycoproteinRatio().SetValue(LiverAAGRatio);
  LiverTissue.GetTissueToPlasmaAlbuminRatio().SetValue(LiverARatio);
  LiverTissue.GetTissueToPlasmaLipoproteinRatio().SetValue(LiverLRatio);
  LiverTissue.GetTotalMass().SetValue(LiverTissueMass, MassUnit::kg);

  SELiquidCompartmentLink& LiverVascularToTissue = m_Compartments->CreateLiquidLink(*m_Compartments->GetLiquidCompartment(pulse::VascularCompartment::Liver),
    LiverExtracellular, pulse::VascularLink::LiverVascularToTissue);
  LiverVascularToTissue.MapPath(Liver1ToLiverT2);

  SELiquidCompartmentLink& LiverTissueToLymph = m_Compartments->CreateLiquidLink(LiverExtracellular, cLymph, pulse::LymphLink::LiverTissueToLymph);
  LiverTissueToLymph.MapPath(LiverT1ToLymph);

  ////////////
  // Muscle //
  SEFluidCircuitNode* Muscle1 = cCardiovascular.GetNode(pulse::CardiovascularNode::Muscle1);
  SEFluidCircuitNode& MuscleT1 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::MuscleT1);
  SEFluidCircuitNode& MuscleT2 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::MuscleT2);
  SEFluidCircuitNode& MuscleT3 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::MuscleT3);
  MuscleT1.GetPressure().Set(Muscle1->GetPressure());
  MuscleT3.GetPressure().Set(Ground->GetPressure());
  MuscleT1.GetVolumeBaseline().SetValue(MuscleEWFraction * MuscleTissueVolume * 1000.0, VolumeUnit::mL);

  SEFluidCircuitPath& Muscle1ToMuscleT2 = cCombinedCardiovascular.CreatePath(*Muscle1, MuscleT2, pulse::TissuePath::Muscle1ToMuscleT2);
  Muscle1ToMuscleT2.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::mmHg);
  SEFluidCircuitPath& MuscleT2ToMuscleT1 = cCombinedCardiovascular.CreatePath(MuscleT2, MuscleT1, pulse::TissuePath::MuscleT2ToMuscleT1);
  MuscleT2ToMuscleT1.GetResistanceBaseline().SetValue((1 / MuscleTissueMass) * resistanceConstant, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& MuscleT1ToMuscleT3 = cCombinedCardiovascular.CreatePath(MuscleT1, MuscleT3, pulse::TissuePath::MuscleT1ToMuscleT3);
  MuscleT1ToMuscleT3.GetComplianceBaseline().SetValue(MuscleT1.GetVolumeBaseline(VolumeUnit::mL) / MuscleT1.GetPressure(PressureUnit::mmHg), VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& GroundToMuscleT3 = cCombinedCardiovascular.CreatePath(*Ground, MuscleT3, pulse::TissuePath::GroundToMuscleT3);
  GroundToMuscleT3.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::mmHg);

  SEFluidCircuitPath& MuscleT1ToLymph = cCombinedCardiovascular.CreatePath(MuscleT1, Lymph, pulse::TissuePath::MuscleT1ToLymph);
  MuscleT1ToLymph.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);

  SETissueCompartment& MuscleTissue = m_Compartments->CreateTissueCompartment(pulse::TissueCompartment::Muscle);
  SELiquidCompartment& MuscleExtracellular = m_Compartments->CreateLiquidCompartment(pulse::ExtravascularCompartment::MuscleExtracellular);
  SELiquidCompartment& MuscleIntracellular = m_Compartments->CreateLiquidCompartment(pulse::ExtravascularCompartment::MuscleIntracellular);
  MuscleTissue.GetMatrixVolume().SetValue((1 - MuscleEWFraction - MuscleIWFraction) * MuscleTissueVolume * 1000.0, VolumeUnit::mL);
  MuscleExtracellular.MapNode(MuscleT1);
  MuscleExtracellular.MapNode(MuscleT2);
  MuscleExtracellular.MapNode(MuscleT3);
  MuscleExtracellular.GetWaterVolumeFraction().SetValue(MuscleEWFraction);
  MuscleIntracellular.GetVolume().SetValue(MuscleIWFraction * MuscleTissueVolume * 1000.0, VolumeUnit::mL);
  MuscleIntracellular.GetWaterVolumeFraction().SetValue(MuscleIWFraction);
  MuscleTissue.GetAcidicPhospohlipidConcentration().SetValue(MuscleAPL, MassPerMassUnit::mg_Per_g);
  MuscleTissue.GetNeutralLipidsVolumeFraction().SetValue(MuscleNLFraction);
  MuscleTissue.GetNeutralPhospholipidsVolumeFraction().SetValue(MuscleNPFraction);
  MuscleTissue.GetTissueToPlasmaAlphaAcidGlycoproteinRatio().SetValue(MuscleAAGRatio);
  MuscleTissue.GetTissueToPlasmaAlbuminRatio().SetValue(MuscleARatio);
  MuscleTissue.GetTissueToPlasmaLipoproteinRatio().SetValue(MuscleLRatio);
  MuscleTissue.GetTotalMass().SetValue(MuscleTissueMass, MassUnit::kg);

  SELiquidCompartmentLink& MuscleVascularToTissue = m_Compartments->CreateLiquidLink(*m_Compartments->GetLiquidCompartment(pulse::VascularCompartment::Muscle),
    MuscleExtracellular, pulse::VascularLink::MuscleVascularToTissue);
  MuscleVascularToTissue.MapPath(Muscle1ToMuscleT2);

  SELiquidCompartmentLink& MuscleTissueToLymph = m_Compartments->CreateLiquidLink(MuscleExtracellular, cLymph, pulse::LymphLink::MuscleTissueToLymph);
  MuscleTissueToLymph.MapPath(MuscleT1ToLymph);

  ////////////////
  // Myocardium //
  SEFluidCircuitNode* Myocardium1 = cCardiovascular.GetNode(pulse::CardiovascularNode::Myocardium1);
  SEFluidCircuitNode& MyocardiumT1 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::MyocardiumT1);
  SEFluidCircuitNode& MyocardiumT2 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::MyocardiumT2);
  SEFluidCircuitNode& MyocardiumT3 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::MyocardiumT3);
  MyocardiumT1.GetPressure().Set(Myocardium1->GetPressure());
  MyocardiumT3.GetPressure().Set(Ground->GetPressure());
  MyocardiumT1.GetVolumeBaseline().SetValue(MyocardiumEWFraction * MyocardiumTissueVolume * 1000.0, VolumeUnit::mL);

  SEFluidCircuitPath& Myocardium1ToMyocardiumT2 = cCombinedCardiovascular.CreatePath(*Myocardium1, MyocardiumT2, pulse::TissuePath::Myocardium1ToMyocardiumT2);
  Myocardium1ToMyocardiumT2.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::mmHg);
  SEFluidCircuitPath& MyocardiumT2ToMyocardiumT1 = cCombinedCardiovascular.CreatePath(MyocardiumT2, MyocardiumT1, pulse::TissuePath::MyocardiumT2ToMyocardiumT1);
  MyocardiumT2ToMyocardiumT1.GetResistanceBaseline().SetValue((1 / MyocardiumTissueMass) * resistanceConstant, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& MyocardiumT1ToMyocardiumT3 = cCombinedCardiovascular.CreatePath(MyocardiumT1, MyocardiumT3, pulse::TissuePath::MyocardiumT1ToMyocardiumT3);
  MyocardiumT1ToMyocardiumT3.GetComplianceBaseline().SetValue(MyocardiumT1.GetVolumeBaseline(VolumeUnit::mL) / MyocardiumT1.GetPressure(PressureUnit::mmHg), VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& GroundToMyocardiumT3 = cCombinedCardiovascular.CreatePath(*Ground, MyocardiumT3, pulse::TissuePath::GroundToMyocardiumT3);
  GroundToMyocardiumT3.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::mmHg);

  SEFluidCircuitPath& MyocardiumT1ToLymph = cCombinedCardiovascular.CreatePath(MyocardiumT1, Lymph, pulse::TissuePath::MyocardiumT1ToLymph);
  MyocardiumT1ToLymph.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);

  SETissueCompartment& MyocardiumTissue = m_Compartments->CreateTissueCompartment(pulse::TissueCompartment::Myocardium);
  SELiquidCompartment& MyocardiumExtracellular = m_Compartments->CreateLiquidCompartment(pulse::ExtravascularCompartment::MyocardiumExtracellular);
  SELiquidCompartment& MyocardiumIntracellular = m_Compartments->CreateLiquidCompartment(pulse::ExtravascularCompartment::MyocardiumIntracellular);
  MyocardiumTissue.GetMatrixVolume().SetValue((1 - MyocardiumEWFraction - MyocardiumIWFraction) * MyocardiumTissueVolume * 1000.0, VolumeUnit::mL);
  MyocardiumExtracellular.MapNode(MyocardiumT1);
  MyocardiumExtracellular.MapNode(MyocardiumT2);
  MyocardiumExtracellular.MapNode(MyocardiumT3);
  MyocardiumExtracellular.GetWaterVolumeFraction().SetValue(MyocardiumEWFraction);
  MyocardiumIntracellular.GetVolume().SetValue(MyocardiumIWFraction * MyocardiumTissueVolume * 1000.0, VolumeUnit::mL);
  MyocardiumIntracellular.GetWaterVolumeFraction().SetValue(MyocardiumIWFraction);
  MyocardiumTissue.GetAcidicPhospohlipidConcentration().SetValue(MyocardiumAPL, MassPerMassUnit::mg_Per_g);
  MyocardiumTissue.GetNeutralLipidsVolumeFraction().SetValue(MyocardiumNLFraction);
  MyocardiumTissue.GetNeutralPhospholipidsVolumeFraction().SetValue(MyocardiumNPFraction);
  MyocardiumTissue.GetTissueToPlasmaAlphaAcidGlycoproteinRatio().SetValue(MyocardiumAAGRatio);
  MyocardiumTissue.GetTissueToPlasmaAlbuminRatio().SetValue(MyocardiumARatio);
  MyocardiumTissue.GetTissueToPlasmaLipoproteinRatio().SetValue(MyocardiumLRatio);
  MyocardiumTissue.GetTotalMass().SetValue(MyocardiumTissueMass, MassUnit::kg);

  SELiquidCompartmentLink& MyocardiumVascularToTissue = m_Compartments->CreateLiquidLink(*m_Compartments->GetLiquidCompartment(pulse::VascularCompartment::Myocardium),
    MyocardiumExtracellular, pulse::VascularLink::MyocardiumVascularToTissue);
  MyocardiumVascularToTissue.MapPath(Myocardium1ToMyocardiumT2);

  SELiquidCompartmentLink& MyocardiumTissueToLymph = m_Compartments->CreateLiquidLink(MyocardiumExtracellular, cLymph, pulse::LymphLink::MyocardiumTissueToLymph);
  MyocardiumTissueToLymph.MapPath(MyocardiumT1ToLymph);

  //////////////////
  // Right Kidney //
  SEFluidCircuitNode* RightKidney1;
  if (!m_Config->IsRenalEnabled())
    RightKidney1 = cCombinedCardiovascular.GetNode(pulse::CardiovascularNode::RightKidney1);
  else
    RightKidney1 = cCombinedCardiovascular.GetNode(pulse::RenalNode::RightGlomerularCapillaries);

  SEFluidCircuitNode& RightKidneyT1 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::RightKidneyT1);
  SEFluidCircuitNode& RightKidneyT2 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::RightKidneyT2);
  SEFluidCircuitNode& RightKidneyT3 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::RightKidneyT3);
  RightKidneyT1.GetPressure().Set(RightKidney1->GetPressure());
  RightKidneyT3.GetPressure().Set(Ground->GetPressure());
  RightKidneyT1.GetVolumeBaseline().SetValue(RKidneyEWFraction * RKidneyTissueVolume * 1000.0, VolumeUnit::mL);

  SEFluidCircuitPath& RightKidney1ToRightKidneyT2 = cCombinedCardiovascular.CreatePath(*RightKidney1, RightKidneyT2, pulse::TissuePath::RightKidney1ToRightKidneyT2);
  RightKidney1ToRightKidneyT2.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::mmHg);
  SEFluidCircuitPath& RightKidneyT2ToRightKidneyT1 = cCombinedCardiovascular.CreatePath(RightKidneyT2, RightKidneyT1, pulse::TissuePath::RightKidneyT2ToRightKidneyT1);
  RightKidneyT2ToRightKidneyT1.GetResistanceBaseline().SetValue((1 / RKidneyTissueMass) * resistanceConstant, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& RightKidneyT1ToRightKidneyT3 = cCombinedCardiovascular.CreatePath(RightKidneyT1, RightKidneyT3, pulse::TissuePath::RightKidneyT1ToRightKidneyT3);
  RightKidneyT1ToRightKidneyT3.GetComplianceBaseline().SetValue(RightKidneyT1.GetVolumeBaseline(VolumeUnit::mL) / RightKidneyT1.GetPressure(PressureUnit::mmHg), VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& GroundToRightKidneyT3 = cCombinedCardiovascular.CreatePath(*Ground, RightKidneyT3, pulse::TissuePath::GroundToRightKidneyT3);
  GroundToRightKidneyT3.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::mmHg);

  SEFluidCircuitPath& RightKidneyT1ToLymph = cCombinedCardiovascular.CreatePath(RightKidneyT1, Lymph, pulse::TissuePath::RightKidneyT1ToLymph);
  RightKidneyT1ToLymph.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);

  SETissueCompartment& RightKidneyTissue = m_Compartments->CreateTissueCompartment(pulse::TissueCompartment::RightKidney);
  SELiquidCompartment& RightKidneyExtracellular = m_Compartments->CreateLiquidCompartment(pulse::ExtravascularCompartment::RightKidneyExtracellular);
  SELiquidCompartment& RightKidneyIntracellular = m_Compartments->CreateLiquidCompartment(pulse::ExtravascularCompartment::RightKidneyIntracellular);
  RightKidneyTissue.GetMatrixVolume().SetValue((1 - RKidneyEWFraction - RKidneyIWFraction) * RKidneyTissueVolume * 1000.0, VolumeUnit::mL);
  RightKidneyExtracellular.MapNode(RightKidneyT1);
  RightKidneyExtracellular.MapNode(RightKidneyT2);
  RightKidneyExtracellular.MapNode(RightKidneyT3);
  RightKidneyExtracellular.GetWaterVolumeFraction().SetValue(RKidneyEWFraction);
  RightKidneyIntracellular.GetVolume().SetValue(RKidneyIWFraction * RKidneyTissueVolume * 1000.0, VolumeUnit::mL);
  RightKidneyIntracellular.GetWaterVolumeFraction().SetValue(RKidneyIWFraction);
  RightKidneyTissue.GetAcidicPhospohlipidConcentration().SetValue(RKidneyAPL, MassPerMassUnit::mg_Per_g);
  RightKidneyTissue.GetNeutralLipidsVolumeFraction().SetValue(RKidneyNLFraction);
  RightKidneyTissue.GetNeutralPhospholipidsVolumeFraction().SetValue(RKidneyNPFraction);
  RightKidneyTissue.GetTissueToPlasmaAlphaAcidGlycoproteinRatio().SetValue(RKidneyAAGRatio);
  RightKidneyTissue.GetTissueToPlasmaAlbuminRatio().SetValue(RKidneyARatio);
  RightKidneyTissue.GetTissueToPlasmaLipoproteinRatio().SetValue(RKidneyLRatio);
  RightKidneyTissue.GetTotalMass().SetValue(RKidneyTissueMass, MassUnit::kg);

  SELiquidCompartmentLink& RightKidneyVascularToTissue = m_Compartments->CreateLiquidLink(*m_Compartments->GetLiquidCompartment(pulse::VascularCompartment::RightKidney),
    RightKidneyExtracellular, pulse::VascularLink::RightKidneyVascularToTissue);
  RightKidneyVascularToTissue.MapPath(RightKidney1ToRightKidneyT2);

  SELiquidCompartmentLink& RightKidneyTissueToLymph = m_Compartments->CreateLiquidLink(RightKidneyExtracellular, cLymph, pulse::LymphLink::RightKidneyTissueToLymph);
  RightKidneyTissueToLymph.MapPath(RightKidneyT1ToLymph);

  ////////////////
  // Right Lung //
  SEFluidCircuitNode* RightLung1 = cCardiovascular.GetNode(pulse::CardiovascularNode::RightPulmonaryCapillaries);
  SEFluidCircuitNode& RightLungT1 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::RightLungT1);
  SEFluidCircuitNode& RightLungT2 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::RightLungT2);
  SEFluidCircuitNode& RightLungT3 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::RightLungT3);
  RightLungT1.GetPressure().Set(RightLung1->GetPressure());
  RightLungT3.GetPressure().Set(Ground->GetPressure());
  RightLungT1.GetVolumeBaseline().SetValue(RLungEWFraction * RLungTissueVolume * 1000.0, VolumeUnit::mL);

  SEFluidCircuitPath& RightLung1ToRightLungT2 = cCombinedCardiovascular.CreatePath(*RightLung1, RightLungT2, pulse::TissuePath::RightLung1ToRightLungT2);
  RightLung1ToRightLungT2.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::mmHg);
  SEFluidCircuitPath& RightLungT2ToRightLungT1 = cCombinedCardiovascular.CreatePath(RightLungT2, RightLungT1, pulse::TissuePath::RightLungT2ToRightLungT1);
  RightLungT2ToRightLungT1.GetResistanceBaseline().SetValue((1 / RLungTissueMass) * resistanceConstant, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& RightLungT1ToRightLungT3 = cCombinedCardiovascular.CreatePath(RightLungT1, RightLungT3, pulse::TissuePath::RightLungT1ToRightLungT3);
  RightLungT1ToRightLungT3.GetComplianceBaseline().SetValue(RightLungT1.GetVolumeBaseline(VolumeUnit::mL) / RightLungT1.GetPressure(PressureUnit::mmHg), VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& GroundToRightLungT3 = cCombinedCardiovascular.CreatePath(*Ground, RightLungT3, pulse::TissuePath::GroundToRightLungT3);
  GroundToRightLungT3.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::mmHg);

  SEFluidCircuitPath& RightLungT1ToLymph = cCombinedCardiovascular.CreatePath(RightLungT1, Lymph, pulse::TissuePath::RightLungT1ToLymph);
  RightLungT1ToLymph.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);

  SETissueCompartment& RightLungTissue = m_Compartments->CreateTissueCompartment(pulse::TissueCompartment::RightLung);
  SELiquidCompartment& RightLungExtracellular = m_Compartments->CreateLiquidCompartment(pulse::ExtravascularCompartment::RightLungExtracellular);
  SELiquidCompartment& RightLungIntracellular = m_Compartments->CreateLiquidCompartment(pulse::ExtravascularCompartment::RightLungIntracellular);
  RightLungTissue.GetMatrixVolume().SetValue((1 - RLungEWFraction - RLungIWFraction) * RLungTissueVolume * 1000.0, VolumeUnit::mL);
  RightLungExtracellular.MapNode(RightLungT1);
  RightLungExtracellular.MapNode(RightLungT2);
  RightLungExtracellular.MapNode(RightLungT3);
  RightLungExtracellular.GetWaterVolumeFraction().SetValue(RLungEWFraction);
  RightLungIntracellular.GetVolume().SetValue(RLungIWFraction * RLungTissueVolume * 1000.0, VolumeUnit::mL);
  RightLungIntracellular.GetWaterVolumeFraction().SetValue(RLungIWFraction);
  RightLungTissue.GetAcidicPhospohlipidConcentration().SetValue(RLungAPL, MassPerMassUnit::mg_Per_g);
  RightLungTissue.GetNeutralLipidsVolumeFraction().SetValue(RLungNLFraction);
  RightLungTissue.GetNeutralPhospholipidsVolumeFraction().SetValue(RLungNPFraction);
  RightLungTissue.GetTissueToPlasmaAlphaAcidGlycoproteinRatio().SetValue(RLungAAGRatio);
  RightLungTissue.GetTissueToPlasmaAlbuminRatio().SetValue(RLungARatio);
  RightLungTissue.GetTissueToPlasmaLipoproteinRatio().SetValue(RLungLRatio);
  RightLungTissue.GetTotalMass().SetValue(RLungTissueMass, MassUnit::kg);

  SELiquidCompartmentLink& RightLungVascularToTissue = m_Compartments->CreateLiquidLink(*m_Compartments->GetLiquidCompartment(pulse::VascularCompartment::RightLung),
    RightLungExtracellular, pulse::VascularLink::RightLungVascularToTissue);
  RightLungVascularToTissue.MapPath(RightLung1ToRightLungT2);

  SELiquidCompartmentLink& RightLungTissueToLymph = m_Compartments->CreateLiquidLink(RightLungExtracellular, cLymph, pulse::LymphLink::RightLungTissueToLymph);
  RightLungTissueToLymph.MapPath(RightLungT1ToLymph);

  //////////
  // Skin //
  SEFluidCircuitNode* Skin1 = cCardiovascular.GetNode(pulse::CardiovascularNode::Skin1);
  SEFluidCircuitNode& SkinT1 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::SkinT1);
  SEFluidCircuitNode& SkinT2 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::SkinT2);
  SEFluidCircuitNode& SkinT3 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::SkinT3);
  SkinT1.GetPressure().Set(Skin1->GetPressure());
  SkinT3.GetPressure().Set(Ground->GetPressure());
  SkinT1.GetVolumeBaseline().SetValue(SkinEWFraction * SkinTissueVolume * 1000.0, VolumeUnit::mL);

  SEFluidCircuitPath& Skin1ToSkinT2 = cCombinedCardiovascular.CreatePath(*Skin1, SkinT2, pulse::TissuePath::Skin1ToSkinT2);
  Skin1ToSkinT2.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::mmHg);
  SEFluidCircuitPath& SkinT2ToSkinT1 = cCombinedCardiovascular.CreatePath(SkinT2, SkinT1, pulse::TissuePath::SkinT2ToSkinT1);
  SkinT2ToSkinT1.GetResistanceBaseline().SetValue((1 / SkinTissueMass) * resistanceConstant, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& SkinT1ToSkinT3 = cCombinedCardiovascular.CreatePath(SkinT1, SkinT3, pulse::TissuePath::SkinT1ToSkinT3);
  SkinT1ToSkinT3.GetComplianceBaseline().SetValue(SkinT1.GetVolumeBaseline(VolumeUnit::mL) / SkinT1.GetPressure(PressureUnit::mmHg), VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& GroundToSkinT3 = cCombinedCardiovascular.CreatePath(*Ground, SkinT3, pulse::TissuePath::GroundToSkinT3);
  GroundToSkinT3.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::mmHg);

  SEFluidCircuitPath& SkinT1ToLymph = cCombinedCardiovascular.CreatePath(SkinT1, Lymph, pulse::TissuePath::SkinT1ToLymph);
  SkinT1ToLymph.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);

  SEFluidCircuitPath& SkinT1ToGround = cCombinedCardiovascular.CreatePath(SkinT1, *Ground, pulse::TissuePath::SkinSweating);
  SkinT1ToGround.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);

  SETissueCompartment& SkinTissue = m_Compartments->CreateTissueCompartment(pulse::TissueCompartment::Skin);
  SELiquidCompartment& SkinExtracellular = m_Compartments->CreateLiquidCompartment(pulse::ExtravascularCompartment::SkinExtracellular);
  SELiquidCompartment& SkinIntracellular = m_Compartments->CreateLiquidCompartment(pulse::ExtravascularCompartment::SkinIntracellular);
  SkinTissue.GetMatrixVolume().SetValue((1 - SkinEWFraction - SkinIWFraction) * SkinTissueVolume * 1000.0, VolumeUnit::mL);
  SkinExtracellular.MapNode(SkinT1);
  SkinExtracellular.MapNode(SkinT2);
  SkinExtracellular.MapNode(SkinT3);
  SkinExtracellular.GetWaterVolumeFraction().SetValue(SkinEWFraction);
  SkinIntracellular.GetVolume().SetValue(SkinIWFraction * SkinTissueVolume * 1000.0, VolumeUnit::mL);
  SkinIntracellular.GetWaterVolumeFraction().SetValue(SkinIWFraction);
  SkinTissue.GetAcidicPhospohlipidConcentration().SetValue(SkinAPL, MassPerMassUnit::mg_Per_g);
  SkinTissue.GetNeutralLipidsVolumeFraction().SetValue(SkinNLFraction);
  SkinTissue.GetNeutralPhospholipidsVolumeFraction().SetValue(SkinNPFraction);
  SkinTissue.GetTissueToPlasmaAlphaAcidGlycoproteinRatio().SetValue(SkinAAGRatio);
  SkinTissue.GetTissueToPlasmaAlbuminRatio().SetValue(SkinARatio);
  SkinTissue.GetTissueToPlasmaLipoproteinRatio().SetValue(SkinLRatio);
  SkinTissue.GetTotalMass().SetValue(SkinTissueMass, MassUnit::kg);

  SELiquidCompartmentLink& SkinVascularToTissue = m_Compartments->CreateLiquidLink(*m_Compartments->GetLiquidCompartment(pulse::VascularCompartment::Skin),
    SkinExtracellular, pulse::VascularLink::SkinVascularToTissue);
  SkinVascularToTissue.MapPath(Skin1ToSkinT2);

  SELiquidCompartmentLink& SkinTissueToLymph = m_Compartments->CreateLiquidLink(SkinExtracellular, cLymph, pulse::LymphLink::SkinTissueToLymph);
  SkinTissueToLymph.MapPath(SkinT1ToLymph);

  ////////////
  // Spleen //
  SEFluidCircuitNode* Spleen = cCardiovascular.GetNode(pulse::CardiovascularNode::Spleen1);
  SEFluidCircuitNode& SpleenT1 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::SpleenT1);
  SEFluidCircuitNode& SpleenT2 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::SpleenT2);
  SEFluidCircuitNode& SpleenT3 = cCombinedCardiovascular.CreateNode(pulse::TissueNode::SpleenT3);
  SpleenT1.GetPressure().Set(Spleen->GetPressure());
  SpleenT3.GetPressure().Set(Ground->GetPressure());
  SpleenT1.GetVolumeBaseline().SetValue(SpleenEWFraction * SpleenTissueVolume * 1000.0, VolumeUnit::mL);

  SEFluidCircuitPath& Spleen1ToSpleenT2 = cCombinedCardiovascular.CreatePath(*Spleen, SpleenT2, pulse::TissuePath::Spleen1ToSpleenT2);
  Spleen1ToSpleenT2.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::mmHg);
  SEFluidCircuitPath& SpleenT2ToSpleenT1 = cCombinedCardiovascular.CreatePath(SpleenT2, SpleenT1, pulse::TissuePath::SpleenT2ToSpleenT1);
  SpleenT2ToSpleenT1.GetResistanceBaseline().SetValue((1 / SpleenTissueMass) * resistanceConstant, PressureTimePerVolumeUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& SpleenT1ToSpleenT3 = cCombinedCardiovascular.CreatePath(SpleenT1, SpleenT3, pulse::TissuePath::SpleenT1ToSpleenT3);
  SpleenT1ToSpleenT3.GetComplianceBaseline().SetValue(SpleenT1.GetVolumeBaseline(VolumeUnit::mL) / SpleenT1.GetPressure(PressureUnit::mmHg), VolumePerPressureUnit::mL_Per_mmHg);
  SEFluidCircuitPath& GroundToSpleenT3 = cCombinedCardiovascular.CreatePath(*Ground, SpleenT3, pulse::TissuePath::GroundToSpleenT3);
  GroundToSpleenT3.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::mmHg);

  SEFluidCircuitPath& SpleenT1ToLymph = cCombinedCardiovascular.CreatePath(SpleenT1, Lymph, pulse::TissuePath::SpleenT1ToLymph);
  SpleenT1ToLymph.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);

  SETissueCompartment& SpleenTissue = m_Compartments->CreateTissueCompartment(pulse::TissueCompartment::Spleen);
  SELiquidCompartment& SpleenExtracellular = m_Compartments->CreateLiquidCompartment(pulse::ExtravascularCompartment::SpleenExtracellular);
  SELiquidCompartment& SpleenIntracellular = m_Compartments->CreateLiquidCompartment(pulse::ExtravascularCompartment::SpleenIntracellular);
  SpleenTissue.GetMatrixVolume().SetValue((1 - SpleenEWFraction - SpleenIWFraction) * SpleenTissueVolume * 1000.0, VolumeUnit::mL);
  SpleenExtracellular.MapNode(SpleenT1);
  SpleenExtracellular.MapNode(SpleenT2);
  SpleenExtracellular.MapNode(SpleenT3);
  SpleenExtracellular.GetWaterVolumeFraction().SetValue(SpleenEWFraction);
  SpleenIntracellular.GetVolume().SetValue(SpleenIWFraction * SpleenTissueVolume * 1000.0, VolumeUnit::mL);
  SpleenIntracellular.GetWaterVolumeFraction().SetValue(SpleenIWFraction);
  SpleenTissue.GetAcidicPhospohlipidConcentration().SetValue(SpleenAPL, MassPerMassUnit::mg_Per_g);
  SpleenTissue.GetNeutralLipidsVolumeFraction().SetValue(SpleenNLFraction);
  SpleenTissue.GetNeutralPhospholipidsVolumeFraction().SetValue(SpleenNPFraction);
  SpleenTissue.GetTissueToPlasmaAlphaAcidGlycoproteinRatio().SetValue(SpleenAAGRatio);
  SpleenTissue.GetTissueToPlasmaAlbuminRatio().SetValue(SpleenARatio);
  SpleenTissue.GetTissueToPlasmaLipoproteinRatio().SetValue(SpleenLRatio);
  SpleenTissue.GetTotalMass().SetValue(SpleenTissueMass, MassUnit::kg);

  SELiquidCompartmentLink& SpleenVascularToTissue = m_Compartments->CreateLiquidLink(*m_Compartments->GetLiquidCompartment(pulse::VascularCompartment::Spleen),
    SpleenExtracellular, pulse::VascularLink::SpleenVascularToTissue);
  SpleenVascularToTissue.MapPath(Spleen1ToSpleenT2);

  SELiquidCompartmentLink& SpleenTissueToLymph = m_Compartments->CreateLiquidLink(SpleenExtracellular, cLymph, pulse::LymphLink::SpleenTissueToLymph);
  SpleenTissueToLymph.MapPath(SpleenT1ToLymph);

  cCombinedCardiovascular.SetNextAndCurrentFromBaselines();
  cCombinedCardiovascular.StateChange();
}

void PulseController::SetupCerebrospinalFluid()
{
  // TODO Rachel
  Info("Setting Up Cerebrospinal Fluid");
  SEFluidCircuit& cCombinedCardiovascular = m_Circuits->GetActiveCardiovascularCircuit();

  SEFluidCircuitNode* Ground = cCombinedCardiovascular.GetNode(pulse::CardiovascularNode::Ground);
  SEFluidCircuitNode* Brain1 = cCombinedCardiovascular.GetNode(pulse::CardiovascularNode::Brain1);

  double brainVascularCompliance = cCombinedCardiovascular.GetPath(pulse::CardiovascularPath::Brain1ToGround)->GetComplianceBaseline().GetValue(VolumePerPressureUnit::mL_Per_mmHg);
  double brainVascularPressure = Brain1->GetPressure().GetValue(PressureUnit::mmHg);

  m_Circuits->DeleteFluidPath(pulse::CardiovascularPath::Brain1ToGround);

  SEFluidCircuitNode& VascularCSFBarrier = cCombinedCardiovascular.CreateNode(pulse::CerebrospinalFluidNode::VascularCSFBarrier);
  VascularCSFBarrier.GetPressure().SetValue(brainVascularPressure, PressureUnit::mmHg);

  //Initialize these values based on height/weight
  SEFluidCircuitNode& IntracranialSpace1 = cCombinedCardiovascular.CreateNode(pulse::CerebrospinalFluidNode::IntracranialSpace1);
  IntracranialSpace1.GetPressure().SetValue(7.0, PressureUnit::mmHg);
  IntracranialSpace1.GetVolumeBaseline().SetValue(0.0, VolumeUnit::mL);
  SEFluidCircuitNode& IntracranialSpace2 = cCombinedCardiovascular.CreateNode(pulse::CerebrospinalFluidNode::IntracranialSpace2);
  IntracranialSpace2.GetPressure().SetValue(7.0, PressureUnit::mmHg);
  IntracranialSpace2.GetVolumeBaseline().SetValue(100.0, VolumeUnit::mL);

  SEFluidCircuitPath& Brain1ToVascularCSFBarrier = cCombinedCardiovascular.CreatePath(*Brain1, VascularCSFBarrier, pulse::CerebrospinalFluidPath::Brain1ToVascularCSFBarrier);
  Brain1ToVascularCSFBarrier.GetComplianceBaseline().SetValue(brainVascularCompliance, VolumePerPressureUnit::mL_Per_mmHg); //Vascular Volume

  cCombinedCardiovascular.CreatePath(VascularCSFBarrier, IntracranialSpace1, pulse::CerebrospinalFluidPath::VascularCSFBarrierToIntracranialSpace1);

  SEFluidCircuitPath& GroundToIntracranialSpace1 = cCombinedCardiovascular.CreatePath(*Ground, IntracranialSpace1, pulse::CerebrospinalFluidPath::GroundToIntracranialSpace1);
  GroundToIntracranialSpace1.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);  //Absorption/Production Path

  SEFluidCircuitPath& IntracranialSpace1ToIntracranialSpace2 = cCombinedCardiovascular.CreatePath(IntracranialSpace1, IntracranialSpace2, pulse::CerebrospinalFluidPath::IntracranialSpace1ToIntracranialSpace2);
  IntracranialSpace1ToIntracranialSpace2.GetComplianceBaseline().SetValue(70.0, VolumePerPressureUnit::mL_Per_mmHg); //CSF Volume

  cCombinedCardiovascular.CreatePath(IntracranialSpace2, *Ground, pulse::CerebrospinalFluidPath::IntracranialSpace2ToGround);

  cCombinedCardiovascular.SetNextAndCurrentFromBaselines();
  cCombinedCardiovascular.StateChange();

  // Compartment
  SELiquidCompartment& cIntracranialSpace = m_Compartments->CreateLiquidCompartment(pulse::CerebrospinalFluidCompartment::IntracranialSpace);
  cIntracranialSpace.MapNode(IntracranialSpace1);
  cIntracranialSpace.MapNode(IntracranialSpace2);

  SELiquidCompartmentGraph& gCombinedCardiovascular = m_Compartments->GetActiveCardiovascularGraph();
  gCombinedCardiovascular.AddCompartment(cIntracranialSpace);
  gCombinedCardiovascular.StateChange();
}

void PulseController::SetupRespiratory()
{
  Info("Setting Up Respiratory");
  double RightLungRatio = m_InitialPatient->GetRightLungRatio().GetValue();
  double LeftLungRatio = 1 - RightLungRatio;

  SEFluidCircuit& cRespiratory = m_Circuits->GetRespiratoryCircuit();
  SEFluidCircuitNode* Ambient = m_Circuits->GetFluidNode(pulse::EnvironmentNode::Ambient);
  cRespiratory.AddNode(*Ambient);

  //Input parameters
  const double RespiratorySystemCompliance_L_Per_cmH20 = 0.1; /// \cite Levitzky2013pulmonary
  const double RespiratorySideCompliance_L_Per_cmH2O = RespiratorySystemCompliance_L_Per_cmH20 / 2.0; //compliances in parallel sum, so divide by 2 for each lung
  const double LungCompliance_L_Per_cmH2O = 2.0 * RespiratorySideCompliance_L_Per_cmH2O; //compliances in series, so multiply by 2 for equal split
  const double ChestWallCompliance_L_Per_cmH2O = LungCompliance_L_Per_cmH2O; // =0.1 L/cmH2O each /// \cite kacmarek2016egan p233
  const double IntrapleuralPressure_cmH2O = -5.0; /// \cite Levitzky2013pulmonary
  const double TotalAirwayResistance_cmH2O_s_Per_L = 1.5; /// \cite Levitzky2013pulmonary

  //Should add up to 100% of total airway resistance
  /// \cite kacmarek2016egan
  const double TracheaResistancePercent = 0.5;
  const double BronchiResistancePercent = 0.3;
  const double AlveoliDuctResistancePercent = 0.2;

  //Based on equivalent resistance circuit math
  double TracheaResistance = TotalAirwayResistance_cmH2O_s_Per_L - (BronchiResistancePercent * TotalAirwayResistance_cmH2O_s_Per_L + AlveoliDuctResistancePercent * TotalAirwayResistance_cmH2O_s_Per_L) / 2;
  double BronchiResistance = 2 * (TotalAirwayResistance_cmH2O_s_Per_L - TracheaResistance) - AlveoliDuctResistancePercent * TotalAirwayResistance_cmH2O_s_Per_L;
  double AlveoliDuctResistance = 2 * (TotalAirwayResistance_cmH2O_s_Per_L - TracheaResistance) - BronchiResistance;

  double FunctionalResidualCapacity_L = m_InitialPatient->GetFunctionalResidualCapacity(VolumeUnit::L);
  double anatomicDeadSpaceVolume_L = 0.002 * m_InitialPatient->GetWeight(MassUnit::kg); //Should not change with diseases /// \cite Levitzky2013pulmonary
  double alveolarDeadSpaceVolume_L = 0.001;  //Should change with certain diseases /// \cite Levitzky2013pulmonary
  double physiologicDeadSpaceVolume_L = anatomicDeadSpaceVolume_L + alveolarDeadSpaceVolume_L;
  //double PleuralVolume_L = 20.0 / 1000.0; //this is a liquid volume  /// \cite Levitzky2013pulmonary
  double PleuralVolume_L = FunctionalResidualCapacity_L; //Make this a gas volume to mimic the liquid volume

  double AmbientPresure = 1033.23; // = 1 atm
  double OpenResistance_cmH2O_s_Per_L = m_Config->GetDefaultOpenFlowResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);

  // Airway
  SEFluidCircuitNode& Airway = cRespiratory.CreateNode(pulse::RespiratoryNode::Airway);
  Airway.GetPressure().SetValue(AmbientPresure, PressureUnit::cmH2O);
  Airway.GetVolumeBaseline().SetValue(0.0206, VolumeUnit::L);
  // Carina
  SEFluidCircuitNode& Carina = cRespiratory.CreateNode(pulse::RespiratoryNode::Carina);
  Carina.GetPressure().SetValue(AmbientPresure, PressureUnit::cmH2O);
  Carina.GetVolumeBaseline().SetValue(0.05 * FunctionalResidualCapacity_L / 2.4, VolumeUnit::L); //Trachea Volume
  // Right Anatomic Dead Space
  SEFluidCircuitNode& RightAnatomicDeadSpace = cRespiratory.CreateNode(pulse::RespiratoryNode::RightAnatomicDeadSpace);
  RightAnatomicDeadSpace.GetPressure().SetValue(AmbientPresure, PressureUnit::cmH2O);
  RightAnatomicDeadSpace.GetVolumeBaseline().SetValue(RightLungRatio * anatomicDeadSpaceVolume_L, VolumeUnit::L);
  // Left Anatomic Dead Space
  SEFluidCircuitNode& LeftAnatomicDeadSpace = cRespiratory.CreateNode(pulse::RespiratoryNode::LeftAnatomicDeadSpace);
  LeftAnatomicDeadSpace.GetPressure().SetValue(AmbientPresure, PressureUnit::cmH2O);
  LeftAnatomicDeadSpace.GetVolumeBaseline().SetValue(LeftLungRatio * anatomicDeadSpaceVolume_L, VolumeUnit::L);
  // Right Alveolar Dead Space
  SEFluidCircuitNode& RightAlveolarDeadSpace = cRespiratory.CreateNode(pulse::RespiratoryNode::RightAlveolarDeadSpace);
  RightAlveolarDeadSpace.GetPressure().SetValue(AmbientPresure, PressureUnit::cmH2O);
  RightAlveolarDeadSpace.GetVolumeBaseline().SetValue(RightLungRatio * alveolarDeadSpaceVolume_L, VolumeUnit::L);
  // Left Alveolar Dead Space
  SEFluidCircuitNode& LeftAlveolarDeadSpace = cRespiratory.CreateNode(pulse::RespiratoryNode::LeftAlveolarDeadSpace);
  LeftAlveolarDeadSpace.GetPressure().SetValue(AmbientPresure, PressureUnit::cmH2O);
  LeftAlveolarDeadSpace.GetVolumeBaseline().SetValue(LeftLungRatio * alveolarDeadSpaceVolume_L, VolumeUnit::L);
  // Right Alveoli
  SEFluidCircuitNode& RightAlveoli = cRespiratory.CreateNode(pulse::RespiratoryNode::RightAlveoli);
  RightAlveoli.GetPressure().SetValue(AmbientPresure, PressureUnit::cmH2O);
  RightAlveoli.GetVolumeBaseline().SetValue(RightLungRatio * FunctionalResidualCapacity_L - RightLungRatio * physiologicDeadSpaceVolume_L, VolumeUnit::L);
  // Left Alveoli
  SEFluidCircuitNode& LeftAlveoli = cRespiratory.CreateNode(pulse::RespiratoryNode::LeftAlveoli);
  LeftAlveoli.GetPressure().SetValue(AmbientPresure, PressureUnit::cmH2O);
  LeftAlveoli.GetVolumeBaseline().SetValue(LeftLungRatio * FunctionalResidualCapacity_L - LeftLungRatio * physiologicDeadSpaceVolume_L, VolumeUnit::L);
  // Node for right alveoli leak
  SEFluidCircuitNode& RightAlveoliLeak = cRespiratory.CreateNode(pulse::RespiratoryNode::RightAlveoliLeak);
  RightAlveoliLeak.GetPressure().SetValue(AmbientPresure, PressureUnit::cmH2O);
  // Node for left alveoli leak
  SEFluidCircuitNode& LeftAlveoliLeak = cRespiratory.CreateNode(pulse::RespiratoryNode::LeftAlveoliLeak);
  LeftAlveoliLeak.GetPressure().SetValue(AmbientPresure, PressureUnit::cmH2O);
  // Right Pleural Connection - no volume, so it doesn't get modified by compliances
  SEFluidCircuitNode& RightPleuralConnection = cRespiratory.CreateNode(pulse::RespiratoryNode::RightPleuralConnection);
  RightPleuralConnection.GetPressure().SetValue(AmbientPresure + IntrapleuralPressure_cmH2O, PressureUnit::cmH2O);
  // Left Pleural Connection - no volume, so it doesn't get modified by compliances
  SEFluidCircuitNode& LeftPleuralConnection = cRespiratory.CreateNode(pulse::RespiratoryNode::LeftPleuralConnection);
  LeftPleuralConnection.GetPressure().SetValue(AmbientPresure + IntrapleuralPressure_cmH2O, PressureUnit::cmH2O);
  // Right Pleural
  SEFluidCircuitNode& RightPleural = cRespiratory.CreateNode(pulse::RespiratoryNode::RightPleural);
  RightPleural.GetPressure().SetValue(AmbientPresure + IntrapleuralPressure_cmH2O, PressureUnit::cmH2O);
  RightPleural.GetVolumeBaseline().SetValue(RightLungRatio * PleuralVolume_L, VolumeUnit::L);
  // Left Pleural 
  SEFluidCircuitNode& LeftPleural = cRespiratory.CreateNode(pulse::RespiratoryNode::LeftPleural);
  LeftPleural.GetPressure().SetValue(AmbientPresure + IntrapleuralPressure_cmH2O, PressureUnit::cmH2O);
  LeftPleural.GetVolumeBaseline().SetValue(LeftLungRatio * PleuralVolume_L, VolumeUnit::L);
  // Node for left chest leak
  SEFluidCircuitNode& LeftChestLeak = cRespiratory.CreateNode(pulse::RespiratoryNode::LeftChestLeak);
  LeftChestLeak.GetPressure().SetValue(AmbientPresure, PressureUnit::cmH2O);
  // Node for right Alveoli leak
  SEFluidCircuitNode& RightChestLeak = cRespiratory.CreateNode(pulse::RespiratoryNode::RightChestLeak);
  RightChestLeak.GetPressure().SetValue(AmbientPresure, PressureUnit::cmH2O);
  // Stomach
  SEFluidCircuitNode& Stomach = cRespiratory.CreateNode(pulse::RespiratoryNode::Stomach);
  Stomach.GetPressure().SetValue(AmbientPresure, PressureUnit::cmH2O);
  Stomach.GetVolumeBaseline().SetValue(0.1, VolumeUnit::L);
  // Respiratory Muscle - corresponds to a node representing the inspiratory muscles, particularly diaphragm 
  SEFluidCircuitNode& RespiratoryMuscle = cRespiratory.CreateNode(pulse::RespiratoryNode::RespiratoryMuscle);
  RespiratoryMuscle.GetPressure().SetValue(AmbientPresure, PressureUnit::cmH2O);

  // Environment to Airway connections, the path has no element.
  SEFluidCircuitPath& EnvironmentToAirway = cRespiratory.CreatePath(*Ambient, Airway, pulse::RespiratoryPath::EnvironmentToAirway);
  SEFluidCircuitPath& AirwayToCarina = cRespiratory.CreatePath(Airway, Carina, pulse::RespiratoryPath::AirwayToCarina);
  AirwayToCarina.GetResistanceBaseline().SetValue(TracheaResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  SEFluidCircuitPath& CarinaToRightAnatomicDeadSpace = cRespiratory.CreatePath(Carina, RightAnatomicDeadSpace, pulse::RespiratoryPath::CarinaToRightAnatomicDeadSpace);
  CarinaToRightAnatomicDeadSpace.GetResistanceBaseline().SetValue(BronchiResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  SEFluidCircuitPath& CarinaToLeftAnatomicDeadSpace = cRespiratory.CreatePath(Carina, LeftAnatomicDeadSpace, pulse::RespiratoryPath::CarinaToLeftAnatomicDeadSpace);
  CarinaToLeftAnatomicDeadSpace.GetResistanceBaseline().SetValue(BronchiResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  SEFluidCircuitPath& RightAnatomicDeadSpaceToRightAlveolarDeadSpace = cRespiratory.CreatePath(RightAnatomicDeadSpace, RightAlveolarDeadSpace, pulse::RespiratoryPath::RightAnatomicDeadSpaceToRightAlveolarDeadSpace);
  RightAnatomicDeadSpaceToRightAlveolarDeadSpace.GetResistanceBaseline().SetValue(AlveoliDuctResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  SEFluidCircuitPath& LeftAnatomicDeadSpaceToLeftAlveolarDeadSpace = cRespiratory.CreatePath(LeftAnatomicDeadSpace, LeftAlveolarDeadSpace, pulse::RespiratoryPath::LeftAnatomicDeadSpaceToLeftAlveolarDeadSpace);
  LeftAnatomicDeadSpaceToLeftAlveolarDeadSpace.GetResistanceBaseline().SetValue(AlveoliDuctResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  SEFluidCircuitPath& RightAlveolarDeadSpaceToRightAlveoli = cRespiratory.CreatePath(RightAlveolarDeadSpace, RightAlveoli, pulse::RespiratoryPath::RightAlveolarDeadSpaceToRightAlveoli);
  SEFluidCircuitPath& LeftAlveolarDeadSpaceToLeftAlveoli = cRespiratory.CreatePath(LeftAlveolarDeadSpace, LeftAlveoli, pulse::RespiratoryPath::LeftAlveolarDeadSpaceToLeftAlveoli);
  SEFluidCircuitPath& RightAlveoliToRightPleuralConnection = cRespiratory.CreatePath(RightAlveoli, RightPleuralConnection, pulse::RespiratoryPath::RightAlveoliToRightPleuralConnection);
  RightAlveoliToRightPleuralConnection.GetComplianceBaseline().SetValue(LungCompliance_L_Per_cmH2O, VolumePerPressureUnit::L_Per_cmH2O);
  RightAlveoliToRightPleuralConnection.SetNextPolarizedState(eGate::Closed);
  SEFluidCircuitPath& LeftAlveoliToLeftPleuralConnection = cRespiratory.CreatePath(LeftAlveoli, LeftPleuralConnection, pulse::RespiratoryPath::LeftAlveoliToLeftPleuralConnection);
  LeftAlveoliToLeftPleuralConnection.GetComplianceBaseline().SetValue(LungCompliance_L_Per_cmH2O, VolumePerPressureUnit::L_Per_cmH2O);
  LeftAlveoliToLeftPleuralConnection.SetNextPolarizedState(eGate::Closed);
  //Need a no element path to be able to include a node with no volume, so it doesn't get modified by compliances
  cRespiratory.CreatePath(RightPleuralConnection, RightPleural, pulse::RespiratoryPath::RightPleuralConnectionToRightPleural);
  cRespiratory.CreatePath(LeftPleuralConnection, LeftPleural, pulse::RespiratoryPath::LeftPleuralConnectionToLeftPleural);
  //----------------------------------------------------------------------------------------------------------------------------------------------
  // Path between alveoli and pleural - for right pleural leak
  SEFluidCircuitPath& RightAlveoliToRightAlveoliLeak = cRespiratory.CreatePath(RightAlveoli, RightAlveoliLeak, pulse::RespiratoryPath::RightAlveoliToRightAlveoliLeak);
  RightAlveoliToRightAlveoliLeak.SetNextValve(eGate::Closed);
  SEFluidCircuitPath& RightAlveoliLeakToRightPleural = cRespiratory.CreatePath(RightAlveoliLeak, RightPleural, pulse::RespiratoryPath::RightAlveoliLeakToRightPleural);
  RightAlveoliLeakToRightPleural.GetResistanceBaseline().SetValue(OpenResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  // Path between alveoli and pleural - for left pleural leak
  SEFluidCircuitPath& LeftAlveoliToLeftAlveoliLeak = cRespiratory.CreatePath(LeftAlveoli, LeftAlveoliLeak, pulse::RespiratoryPath::LeftAlveoliToLeftAlveoliLeak);
  LeftAlveoliToLeftAlveoliLeak.SetNextValve(eGate::Closed);
  SEFluidCircuitPath& LeftAlveoliLeakToLeftPleural = cRespiratory.CreatePath(LeftAlveoliLeak, LeftPleural, pulse::RespiratoryPath::LeftAlveoliLeakToLeftPleural);
  LeftAlveoliLeakToLeftPleural.GetResistanceBaseline().SetValue(OpenResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  // Path for needle decompression - right side
  SEFluidCircuitPath& RightPleuralToEnvironment = cRespiratory.CreatePath(RightPleural, *Ambient, pulse::RespiratoryPath::RightPleuralToEnvironment);
  RightPleuralToEnvironment.GetResistanceBaseline().SetValue(OpenResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  //Path for needle decompression - left side
  SEFluidCircuitPath& LeftPleuralToEnvironment = cRespiratory.CreatePath(LeftPleural, *Ambient, pulse::RespiratoryPath::LeftPleuralToEnvironment);
  LeftPleuralToEnvironment.GetResistanceBaseline().SetValue(OpenResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  // Path for open (chest wound) pneumothorax circuit  - right side
  SEFluidCircuitPath& EnvironmentToRightChestLeak = cRespiratory.CreatePath(*Ambient, RightChestLeak, pulse::RespiratoryPath::EnvironmentToRightChestLeak);
  EnvironmentToRightChestLeak.GetResistanceBaseline().SetValue(OpenResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  SEFluidCircuitPath& RightChestLeakToRightPleural = cRespiratory.CreatePath(RightChestLeak, RightPleural, pulse::RespiratoryPath::RightChestLeakToRightPleural);
  RightChestLeakToRightPleural.SetNextValve(eGate::Closed);
  // Path for open (chest wound) pneumothorax circuit - left side
  SEFluidCircuitPath& EnvironmentToLeftChestLeak = cRespiratory.CreatePath(*Ambient, LeftChestLeak, pulse::RespiratoryPath::EnvironmentToLeftChestLeak);
  EnvironmentToLeftChestLeak.GetResistanceBaseline().SetValue(OpenResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  SEFluidCircuitPath& LeftChestLeakToLeftPleural = cRespiratory.CreatePath(LeftChestLeak, LeftPleural, pulse::RespiratoryPath::LeftChestLeakToLeftPleural);
  LeftChestLeakToLeftPleural.SetNextValve(eGate::Closed);
  // Paths for the Driver
  SEFluidCircuitPath& EnvironmentToRespiratoryMuscle = cRespiratory.CreatePath(*Ambient, RespiratoryMuscle, pulse::RespiratoryPath::EnvironmentToRespiratoryMuscle);
  EnvironmentToRespiratoryMuscle.GetPressureSourceBaseline().SetValue(RespiratoryMuscle.GetPressure(PressureUnit::cmH2O) - AmbientPresure, PressureUnit::cmH2O);
  // Esophageal (Stomach) path
  SEFluidCircuitPath& AirwayToStomach = cRespiratory.CreatePath(Airway, Stomach, pulse::RespiratoryPath::AirwayToStomach);
  AirwayToStomach.GetResistanceBaseline().SetValue(OpenResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  SEFluidCircuitPath& StomachToEnvironment = cRespiratory.CreatePath(Stomach, *Ambient, pulse::RespiratoryPath::StomachToEnvironment);
  StomachToEnvironment.GetComplianceBaseline().SetValue(0.05, VolumePerPressureUnit::L_Per_cmH2O);
  StomachToEnvironment.SetNextPolarizedState(eGate::Closed);
  // Paths to RespiratoryMuscle
  SEFluidCircuitPath& RightPleuralToRespiratoryMuscle = cRespiratory.CreatePath(RightPleural, RespiratoryMuscle, pulse::RespiratoryPath::RightPleuralToRespiratoryMuscle);
  RightPleuralToRespiratoryMuscle.GetComplianceBaseline().SetValue(ChestWallCompliance_L_Per_cmH2O, VolumePerPressureUnit::L_Per_cmH2O);
  SEFluidCircuitPath& LeftPleuralToRespiratoryMuscle = cRespiratory.CreatePath(LeftPleural, RespiratoryMuscle, pulse::RespiratoryPath::LeftPleuralToRespiratoryMuscle);
  LeftPleuralToRespiratoryMuscle.GetComplianceBaseline().SetValue(ChestWallCompliance_L_Per_cmH2O, VolumePerPressureUnit::L_Per_cmH2O);

  cRespiratory.SetNextAndCurrentFromBaselines();
  cRespiratory.StateChange();

  // Setup Compartments // 

  // Pulmonary Compartments
  SEGasCompartment& pAirway = m_Compartments->CreateGasCompartment(pulse::PulmonaryCompartment::Airway);
  pAirway.MapNode(Airway);
  SEGasCompartment& pStomach = m_Compartments->CreateGasCompartment(pulse::PulmonaryCompartment::Stomach);
  pStomach.MapNode(Stomach);
  SEGasCompartment& pCarina = m_Compartments->CreateGasCompartment(pulse::PulmonaryCompartment::Carina);
  pCarina.MapNode(Carina);
  SEGasCompartment& pLeftAnatomicDeadSpace = m_Compartments->CreateGasCompartment(pulse::PulmonaryCompartment::LeftAnatomicDeadSpace);
  pLeftAnatomicDeadSpace.MapNode(LeftAnatomicDeadSpace);
  SEGasCompartment& pLeftAlveolarDeadSpace = m_Compartments->CreateGasCompartment(pulse::PulmonaryCompartment::LeftAlveolarDeadSpace);
  pLeftAlveolarDeadSpace.MapNode(LeftAlveolarDeadSpace);
  SEGasCompartment& pLeftAlveoli = m_Compartments->CreateGasCompartment(pulse::PulmonaryCompartment::LeftAlveoli);
  pLeftAlveoli.MapNode(LeftAlveoli);
  SEGasCompartment& pLeftPleuralCavity = m_Compartments->CreateGasCompartment(pulse::PulmonaryCompartment::LeftPleuralCavity);
  pLeftPleuralCavity.MapNode(LeftPleural);
  SEGasCompartment& pRightAnatomicDeadSpace = m_Compartments->CreateGasCompartment(pulse::PulmonaryCompartment::RightAnatomicDeadSpace);
  pRightAnatomicDeadSpace.MapNode(RightAnatomicDeadSpace);
  SEGasCompartment& pRightAlveolarDeadSpace = m_Compartments->CreateGasCompartment(pulse::PulmonaryCompartment::RightAlveolarDeadSpace);
  pRightAlveolarDeadSpace.MapNode(RightAlveolarDeadSpace);
  SEGasCompartment& pRightAlveoli = m_Compartments->CreateGasCompartment(pulse::PulmonaryCompartment::RightAlveoli);
  pRightAlveoli.MapNode(RightAlveoli);
  SEGasCompartment& pRightPleuralCavity = m_Compartments->CreateGasCompartment(pulse::PulmonaryCompartment::RightPleuralCavity);
  pRightPleuralCavity.MapNode(RightPleural);
  SEGasCompartment& pLeftAlveoliLeak = m_Compartments->CreateGasCompartment(pulse::PulmonaryCompartment::LeftAlveoliLeak);
  pLeftAlveoliLeak.MapNode(LeftAlveoliLeak);
  SEGasCompartment& pLeftChestLeak = m_Compartments->CreateGasCompartment(pulse::PulmonaryCompartment::LeftChestLeak);
  pLeftChestLeak.MapNode(LeftChestLeak);
  SEGasCompartment& pRightAlveoliLeak = m_Compartments->CreateGasCompartment(pulse::PulmonaryCompartment::RightAlveoliLeak);
  pRightAlveoliLeak.MapNode(RightAlveoliLeak);
  SEGasCompartment& pRightChestLeak = m_Compartments->CreateGasCompartment(pulse::PulmonaryCompartment::RightChestLeak);
  pRightChestLeak.MapNode(RightChestLeak);

  // Set up hierarchy
  SEGasCompartment& pLeftLung = m_Compartments->CreateGasCompartment(pulse::PulmonaryCompartment::LeftLung);
  pLeftLung.AddChild(pLeftAnatomicDeadSpace);
  pLeftLung.AddChild(pLeftAlveolarDeadSpace);
  pLeftLung.AddChild(pLeftAlveoli);
  SEGasCompartment& pRightLung = m_Compartments->CreateGasCompartment(pulse::PulmonaryCompartment::RightLung);
  pRightLung.AddChild(pRightAnatomicDeadSpace);
  pRightLung.AddChild(pRightAlveolarDeadSpace);
  pRightLung.AddChild(pRightAlveoli);
  SEGasCompartment& pLungs = m_Compartments->CreateGasCompartment(pulse::PulmonaryCompartment::Lungs);
  pLungs.AddChild(pLeftLung);
  pLungs.AddChild(pRightLung);
  SEGasCompartment& pPleuralCavity = m_Compartments->CreateGasCompartment(pulse::PulmonaryCompartment::PleuralCavity);
  pPleuralCavity.AddChild(pLeftPleuralCavity);
  pPleuralCavity.AddChild(pRightPleuralCavity);

  // Setup Links //
  SEGasCompartment* gEnvironment = m_Compartments->GetGasCompartment(pulse::EnvironmentCompartment::Ambient);
  SEGasCompartmentLink& pEnvironmentToAirway = m_Compartments->CreateGasLink(*gEnvironment, pAirway, pulse::PulmonaryLink::EnvironmentToAirway);
  pEnvironmentToAirway.MapPath(EnvironmentToAirway);
  SEGasCompartmentLink& pAirwayToCarina = m_Compartments->CreateGasLink(pAirway, pCarina, pulse::PulmonaryLink::AirwayToCarina);
  pAirwayToCarina.MapPath(AirwayToCarina);
  SEGasCompartmentLink& pAirwayToStomach = m_Compartments->CreateGasLink(pAirway, pStomach, pulse::PulmonaryLink::AirwayToStomach);
  pAirwayToStomach.MapPath(AirwayToStomach);
  SEGasCompartmentLink& pCarinaToLeftAnatomicDeadSpace = m_Compartments->CreateGasLink(pCarina, pLeftAnatomicDeadSpace, pulse::PulmonaryLink::CarinaToLeftAnatomicDeadSpace);
  pCarinaToLeftAnatomicDeadSpace.MapPath(CarinaToLeftAnatomicDeadSpace);
  SEGasCompartmentLink& pLeftAnatomicDeadSpaceToLeftAlveolarDeadSpace = m_Compartments->CreateGasLink(pLeftAnatomicDeadSpace, pLeftAlveolarDeadSpace, pulse::PulmonaryLink::LeftAnatomicDeadSpaceToLeftAlveolarDeadSpace);
  pLeftAnatomicDeadSpaceToLeftAlveolarDeadSpace.MapPath(LeftAnatomicDeadSpaceToLeftAlveolarDeadSpace);
  SEGasCompartmentLink& pLeftAlveolarDeadSpaceToLeftAlveoli = m_Compartments->CreateGasLink(pLeftAlveolarDeadSpace, pLeftAlveoli, pulse::PulmonaryLink::LeftAlveolarDeadSpaceToLeftAlveoli);
  pLeftAlveolarDeadSpaceToLeftAlveoli.MapPath(LeftAlveolarDeadSpaceToLeftAlveoli);
  SEGasCompartmentLink& pCarinaToRightAnatomicDeadSpace = m_Compartments->CreateGasLink(pCarina, pRightAnatomicDeadSpace, pulse::PulmonaryLink::CarinaToRightAnatomicDeadSpace);
  pCarinaToRightAnatomicDeadSpace.MapPath(CarinaToRightAnatomicDeadSpace);
  SEGasCompartmentLink& pRightAnatomicDeadSpaceToRightAlveolarDeadSpace = m_Compartments->CreateGasLink(pRightAnatomicDeadSpace, pRightAlveolarDeadSpace, pulse::PulmonaryLink::RightAnatomicDeadSpaceToRightAlveolarDeadSpace);
  pRightAnatomicDeadSpaceToRightAlveolarDeadSpace.MapPath(RightAnatomicDeadSpaceToRightAlveolarDeadSpace);
  SEGasCompartmentLink& pRightAlveolarDeadSpaceToRightAlveoli = m_Compartments->CreateGasLink(pRightAlveolarDeadSpace, pRightAlveoli, pulse::PulmonaryLink::RightAlveolarDeadSpaceToRightAlveoli);
  pRightAlveolarDeadSpaceToRightAlveoli.MapPath(RightAlveolarDeadSpaceToRightAlveoli);
  SEGasCompartmentLink& pEnvironmentToLeftChestLeak = m_Compartments->CreateGasLink(*gEnvironment, pLeftChestLeak, pulse::PulmonaryLink::EnvironmentToLeftChestLeak);
  pEnvironmentToLeftChestLeak.MapPath(EnvironmentToLeftChestLeak);
  SEGasCompartmentLink& pEnvironmentToRightChestLeak = m_Compartments->CreateGasLink(*gEnvironment, pRightChestLeak, pulse::PulmonaryLink::EnvironmentToRightChestLeak);
  pEnvironmentToRightChestLeak.MapPath(EnvironmentToRightChestLeak);
  SEGasCompartmentLink& pLeftAlveoliLeakToLeftPleural = m_Compartments->CreateGasLink(pLeftAlveoliLeak, pLeftPleuralCavity, pulse::PulmonaryLink::LeftAlveoliLeakToLeftPleural);
  pLeftAlveoliLeakToLeftPleural.MapPath(LeftAlveoliLeakToLeftPleural);
  SEGasCompartmentLink& pLeftAlveoliToLeftAlveoliLeak = m_Compartments->CreateGasLink(pLeftAlveoli, pLeftAlveoliLeak, pulse::PulmonaryLink::LeftAlveoliToLeftAlveoliLeak);
  pLeftAlveoliToLeftAlveoliLeak.MapPath(LeftAlveoliToLeftAlveoliLeak);
  SEGasCompartmentLink& pLeftChestLeakToLeftPleural = m_Compartments->CreateGasLink(pLeftChestLeak, pLeftPleuralCavity, pulse::PulmonaryLink::LeftChestLeakToLeftPleural);
  pLeftChestLeakToLeftPleural.MapPath(LeftChestLeakToLeftPleural);
  SEGasCompartmentLink& pLeftPleuralToEnvironment = m_Compartments->CreateGasLink(pLeftPleuralCavity, *gEnvironment, pulse::PulmonaryLink::LeftPleuralToEnvironment);
  pLeftPleuralToEnvironment.MapPath(LeftPleuralToEnvironment);
  SEGasCompartmentLink& pRightAlveoliLeakToRightPleural = m_Compartments->CreateGasLink(pRightAlveoliLeak, pRightPleuralCavity, pulse::PulmonaryLink::RightAlveoliLeakToRightPleural);
  pRightAlveoliLeakToRightPleural.MapPath(RightAlveoliLeakToRightPleural);
  SEGasCompartmentLink& pRightAlveoliToRightAlveoliLeak = m_Compartments->CreateGasLink(pRightAlveoli, pRightAlveoliLeak, pulse::PulmonaryLink::RightAlveoliToRightAlveoliLeak);
  pRightAlveoliToRightAlveoliLeak.MapPath(RightAlveoliToRightAlveoliLeak);
  SEGasCompartmentLink& pRightChestLeakToRightPleural = m_Compartments->CreateGasLink(pRightChestLeak, pRightPleuralCavity, pulse::PulmonaryLink::RightChestLeakToRightPleural);
  pRightChestLeakToRightPleural.MapPath(RightChestLeakToRightPleural);
  SEGasCompartmentLink& pRightPleuralToEnvironment = m_Compartments->CreateGasLink(pRightPleuralCavity, *gEnvironment, pulse::PulmonaryLink::RightPleuralToEnvironment);
  pRightPleuralToEnvironment.MapPath(RightPleuralToEnvironment);

  // Create the respiratory graph for transport //
  SEGasCompartmentGraph& gRespiratory = m_Compartments->GetRespiratoryGraph();
  gRespiratory.AddCompartment(*gEnvironment);
  gRespiratory.AddCompartment(pAirway);
  gRespiratory.AddCompartment(pStomach);
  gRespiratory.AddCompartment(pCarina);
  gRespiratory.AddCompartment(pLeftAnatomicDeadSpace);
  gRespiratory.AddCompartment(pLeftAlveolarDeadSpace);
  gRespiratory.AddCompartment(pLeftAlveoli);
  gRespiratory.AddCompartment(pRightAnatomicDeadSpace);
  gRespiratory.AddCompartment(pRightAlveolarDeadSpace);
  gRespiratory.AddCompartment(pRightPleuralCavity);
  gRespiratory.AddCompartment(pLeftPleuralCavity);
  gRespiratory.AddCompartment(pRightAlveoli);
  gRespiratory.AddCompartment(pLeftAlveoliLeak);
  gRespiratory.AddCompartment(pLeftChestLeak);
  gRespiratory.AddCompartment(pRightAlveoliLeak);
  gRespiratory.AddCompartment(pRightChestLeak);
  gRespiratory.AddLink(pEnvironmentToAirway);
  gRespiratory.AddLink(pAirwayToCarina);
  gRespiratory.AddLink(pAirwayToStomach);
  gRespiratory.AddLink(pCarinaToLeftAnatomicDeadSpace);
  gRespiratory.AddLink(pLeftAnatomicDeadSpaceToLeftAlveolarDeadSpace);
  gRespiratory.AddLink(pLeftAlveolarDeadSpaceToLeftAlveoli);
  gRespiratory.AddLink(pCarinaToRightAnatomicDeadSpace);
  gRespiratory.AddLink(pRightAnatomicDeadSpaceToRightAlveolarDeadSpace);
  gRespiratory.AddLink(pRightAlveolarDeadSpaceToRightAlveoli);
  gRespiratory.AddLink(pEnvironmentToLeftChestLeak);
  gRespiratory.AddLink(pEnvironmentToRightChestLeak);
  gRespiratory.AddLink(pLeftAlveoliLeakToLeftPleural);
  gRespiratory.AddLink(pLeftAlveoliToLeftAlveoliLeak);
  gRespiratory.AddLink(pLeftChestLeakToLeftPleural);
  gRespiratory.AddLink(pLeftPleuralToEnvironment);
  gRespiratory.AddLink(pRightAlveoliLeakToRightPleural);
  gRespiratory.AddLink(pRightAlveoliToRightAlveoliLeak);
  gRespiratory.AddLink(pRightChestLeakToRightPleural);
  gRespiratory.AddLink(pRightPleuralToEnvironment);
  gRespiratory.StateChange();

  // Generically set up the Aerosol Graph, this is a mirror of the Respiratory Gas Graph, only it's a liquid graph
  SELiquidCompartmentGraph& lAerosol = m_Compartments->GetAerosolGraph();
  SELiquidCompartment* lEnvironment = m_Compartments->GetLiquidCompartment(pulse::EnvironmentCompartment::Ambient);
  lAerosol.AddCompartment(*lEnvironment);
  // First Create the compartments and map the same nodes
  for (auto name : pulse::PulmonaryCompartment::GetValues())
  {
    SEGasCompartment* gasCmpt = m_Compartments->GetGasCompartment(name);
    SELiquidCompartment& liquidCmpt = m_Compartments->CreateLiquidCompartment(name);
    if (gasCmpt->HasNodeMapping())
    {
      for (auto node : gasCmpt->GetNodeMapping().GetNodes())
        liquidCmpt.MapNode(*node);
    }
  }
  // Hook up any hierarchies
  for (auto name : pulse::PulmonaryCompartment::GetValues())
  {
    SEGasCompartment* gasCmpt = m_Compartments->GetGasCompartment(name);
    SELiquidCompartment* liquidCmpt = m_Compartments->GetLiquidCompartment(name);
    if (gasCmpt->HasChildren())
    {
      for (auto child : gasCmpt->GetChildren())
        liquidCmpt->AddChild(*m_Compartments->GetLiquidCompartment(child->GetName()));
    }
  }
  // Add leaf compartments to the graph
  for (auto name : pulse::PulmonaryCompartment::GetValues())
  {
    SELiquidCompartment* liquidCmpt = m_Compartments->GetLiquidCompartment(name);
    if (!liquidCmpt->HasChildren())
      lAerosol.AddCompartment(*liquidCmpt);
  }
  // Create Links
  for (auto name : pulse::PulmonaryLink::GetValues())
  {
    SEGasCompartmentLink* gasLink = m_Compartments->GetGasLink(name);
    SELiquidCompartment* src = m_Compartments->GetLiquidCompartment(gasLink->GetSourceCompartment().GetName());
    SELiquidCompartment* tgt = m_Compartments->GetLiquidCompartment(gasLink->GetTargetCompartment().GetName());
    SELiquidCompartmentLink& liquidLink = m_Compartments->CreateLiquidLink(*src, *tgt, name);
    if (gasLink->HasPath())
      liquidLink.MapPath(*gasLink->GetPath());
    lAerosol.AddLink(liquidLink);
  }
  lAerosol.StateChange();
}

void PulseController::SetupGastrointestinal()
{
  Info("Setting Up Gastrointestinal");
  // Circuit
  SEFluidCircuit& cCombinedCardiovascular = m_Circuits->GetActiveCardiovascularCircuit();

  SEFluidCircuitNode& SmallIntestineC1 = cCombinedCardiovascular.CreateNode(pulse::ChymeNode::SmallIntestineC1);
  SmallIntestineC1.GetPressure().SetValue(0, PressureUnit::mmHg);
  SmallIntestineC1.GetVolumeBaseline().SetValue(10, VolumeUnit::mL);

  SEFluidCircuitNode* SmallIntestine1 = cCombinedCardiovascular.GetNode(pulse::CardiovascularNode::SmallIntestine1);
  SEFluidCircuitNode* Ground = cCombinedCardiovascular.GetNode(pulse::CardiovascularNode::Ground);

  SEFluidCircuitPath& SmallIntestineC1ToSmallIntestine1 = cCombinedCardiovascular.CreatePath(SmallIntestineC1, *SmallIntestine1, pulse::ChymePath::SmallIntestineC1ToSmallIntestine1);
  SmallIntestineC1ToSmallIntestine1.GetFlowSourceBaseline().SetValue(0, VolumePerTimeUnit::mL_Per_min);
  cCombinedCardiovascular.CreatePath(*Ground, SmallIntestineC1, pulse::ChymePath::GroundToSmallIntestineC1);

  if (m_Config->IsTissueEnabled())
  {
    SEFluidCircuitNode* GutT1 = cCombinedCardiovascular.GetNode(pulse::TissueNode::GutT1);
    SEFluidCircuitPath& GutT1ToGround = cCombinedCardiovascular.CreatePath(*GutT1, *Ground, pulse::ChymePath::GutT1ToGround);
    GutT1ToGround.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);
  }

  cCombinedCardiovascular.SetNextAndCurrentFromBaselines();
  cCombinedCardiovascular.StateChange();

  // Compartment
  SELiquidCompartment& cSmallIntestine = m_Compartments->CreateLiquidCompartment(pulse::ChymeCompartment::SmallIntestine);
  cSmallIntestine.MapNode(SmallIntestineC1);

  SELiquidCompartment* vSmallIntestine = m_Compartments->GetLiquidCompartment(pulse::VascularCompartment::SmallIntestine);
  SELiquidCompartmentLink& lSmallIntestineChymeToVasculature = m_Compartments->CreateLiquidLink(cSmallIntestine, *vSmallIntestine, pulse::ChymeLink::SmallIntestineChymeToVasculature);
  lSmallIntestineChymeToVasculature.MapPath(SmallIntestineC1ToSmallIntestine1);

  SELiquidCompartmentGraph& gCombinedCardiovascular = m_Compartments->GetActiveCardiovascularGraph();
  gCombinedCardiovascular.AddCompartment(cSmallIntestine);
  gCombinedCardiovascular.AddLink(lSmallIntestineChymeToVasculature);
  gCombinedCardiovascular.StateChange();
}

void PulseController::SetupAnesthesiaMachine()
{
  Info("Setting Up Anesthesia Machine");
  /////////////////////// Circuit Interdependencies
  double AmbientPresure = 1033.23; // = 1 atm // Also defined in SetupRespiratoryCircuit
  SEFluidCircuit& cRespiratory = m_Circuits->GetRespiratoryCircuit();
  SEGasCompartmentGraph& gRespiratory = m_Compartments->GetRespiratoryGraph();
  ///////////////////////

  double ventilatorVolume_L = 1.0;
  double ventilatorCompliance_L_Per_cmH2O = 0.1;
  double dValveOpenResistance = m_Config->GetMachineOpenResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  double dSwitchOpenResistance = m_Config->GetDefaultOpenFlowResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  double dSwitchClosedResistance = m_Config->GetDefaultClosedFlowResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  double dLowResistance = 0.01;

  SEFluidCircuit& cAnesthesia = m_Circuits->GetAnesthesiaMachineCircuit();
  SEFluidCircuitNode* Ambient = m_Circuits->GetFluidNode(pulse::EnvironmentNode::Ambient);
  cAnesthesia.AddNode(*Ambient);

  ////////////////
  // Ventilator //
  SEFluidCircuitNode& Ventilator = cAnesthesia.CreateNode(pulse::AnesthesiaMachineNode::Ventilator);
  Ventilator.GetVolumeBaseline().SetValue(ventilatorVolume_L, VolumeUnit::L);
  Ventilator.GetPressure().SetValue(AmbientPresure, PressureUnit::cmH2O);
  /////////////////
  // ReliefValve //
  SEFluidCircuitNode& ReliefValve = cAnesthesia.CreateNode(pulse::AnesthesiaMachineNode::ReliefValve);
  ReliefValve.GetPressure().SetValue(AmbientPresure, PressureUnit::cmH2O);
  //////////////
  // Selector //
  SEFluidCircuitNode& Selector = cAnesthesia.CreateNode(pulse::AnesthesiaMachineNode::Selector);
  Selector.GetPressure().SetValue(AmbientPresure, PressureUnit::cmH2O);
  Selector.GetVolumeBaseline().SetValue(0.1, VolumeUnit::L);
  //////////////
  // Scrubber //
  SEFluidCircuitNode& Scrubber = cAnesthesia.CreateNode(pulse::AnesthesiaMachineNode::Scrubber);
  Scrubber.GetPressure().SetValue(AmbientPresure, PressureUnit::cmH2O);
  Scrubber.GetVolumeBaseline().SetValue(0.1, VolumeUnit::L);
  ////////////
  // YPiece //
  SEFluidCircuitNode& Ypiece = cAnesthesia.CreateNode(pulse::AnesthesiaMachineNode::YPiece);
  Ypiece.GetPressure().SetValue(AmbientPresure, PressureUnit::cmH2O);
  Ypiece.GetVolumeBaseline().SetValue(0.01, VolumeUnit::L);
  //////////////
  // GasInlet //
  SEFluidCircuitNode& GasInlet = cAnesthesia.CreateNode(pulse::AnesthesiaMachineNode::GasInlet);
  GasInlet.GetPressure().SetValue(AmbientPresure, PressureUnit::cmH2O);
  GasInlet.GetVolumeBaseline().SetValue(0.1, VolumeUnit::L);
  ///////////////
  // GasSource //
  SEFluidCircuitNode& GasSource = cAnesthesia.CreateNode(pulse::AnesthesiaMachineNode::GasSource);
  GasSource.GetPressure().SetValue(AmbientPresure, PressureUnit::cmH2O);
  GasSource.GetVolumeBaseline().SetValue(std::numeric_limits<double>::infinity(), VolumeUnit::mL);
  //////////////////////////
  // AnesthesiaConnection //
  SEFluidCircuitNode& AnesthesiaConnection = cAnesthesia.CreateNode(pulse::AnesthesiaMachineNode::Connection);
  AnesthesiaConnection.GetPressure().SetValue(AmbientPresure, PressureUnit::cmH2O);
  AnesthesiaConnection.GetVolumeBaseline().SetValue(0.01, VolumeUnit::L);
  /////////////////////
  // InspiratoryLimb //
  SEFluidCircuitNode& InspiratoryLimb = cAnesthesia.CreateNode(pulse::AnesthesiaMachineNode::InspiratoryLimb);
  InspiratoryLimb.GetPressure().SetValue(AmbientPresure, PressureUnit::cmH2O);
  InspiratoryLimb.GetVolumeBaseline().SetValue(0.1, VolumeUnit::L);
  ////////////////////
  // ExpiratoryLimb //
  SEFluidCircuitNode& ExpiratoryLimb = cAnesthesia.CreateNode(pulse::AnesthesiaMachineNode::ExpiratoryLimb);
  ExpiratoryLimb.GetPressure().SetValue(AmbientPresure, PressureUnit::cmH2O);
  ExpiratoryLimb.GetVolumeBaseline().SetValue(0.1, VolumeUnit::L);
  /////////////////////////////
  // EnvironmentToVentilator //
  SEFluidCircuitPath& EnvironmentToVentilator = cAnesthesia.CreatePath(*Ambient, Ventilator, pulse::AnesthesiaMachinePath::EnvironmentToVentilator);
  EnvironmentToVentilator.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::cmH2O);
  //////////////////////////////
  // EnvironmentToReliefValve //
  SEFluidCircuitPath& EnvironmentToReliefValve = cAnesthesia.CreatePath(*Ambient, ReliefValve, pulse::AnesthesiaMachinePath::EnvironmentToReliefValve);
  EnvironmentToReliefValve.GetPressureSourceBaseline().SetValue(100.0, PressureUnit::cmH2O);
  /////////////////////////////
  // VentilatorToEnvironment //
  SEFluidCircuitPath& VentilatorToEnviornment = cAnesthesia.CreatePath(Ventilator, *Ambient, pulse::AnesthesiaMachinePath::VentilatorToEnvironment);
  VentilatorToEnviornment.GetComplianceBaseline().SetValue(ventilatorCompliance_L_Per_cmH2O, VolumePerPressureUnit::L_Per_cmH2O);
  ////////////////////////////////////
  // VentilatorToSelector //
  SEFluidCircuitPath& VentilatorConnectionToSelector = cAnesthesia.CreatePath(Ventilator, Selector, pulse::AnesthesiaMachinePath::VentilatorToSelector);
  VentilatorConnectionToSelector.GetResistanceBaseline().SetValue(dSwitchClosedResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  ///////////////////////////
  // SelectorToReliefValve //
  SEFluidCircuitPath& SelectorToReliefValve = cAnesthesia.CreatePath(Selector, ReliefValve, pulse::AnesthesiaMachinePath::SelectorToReliefValve);
  SelectorToReliefValve.SetNextValve(eGate::Open);
  ////////////////////////
  // SelectorToScrubber //
  SEFluidCircuitPath& SelectorToScrubber = cAnesthesia.CreatePath(Selector, Scrubber, pulse::AnesthesiaMachinePath::SelectorToScrubber);
  SelectorToScrubber.GetResistanceBaseline().SetValue(dLowResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  ////////////////////////
  // ScrubberToGasInlet //
  SEFluidCircuitPath& ScrubberToGasInlet = cAnesthesia.CreatePath(Scrubber, GasInlet, pulse::AnesthesiaMachinePath::ScrubberToGasInlet);
  ScrubberToGasInlet.GetResistanceBaseline().SetValue(dLowResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  ////////////////////////////
  // EnvironmentToGasSource //
  SEFluidCircuitPath& EnvironmentToGasSource = cAnesthesia.CreatePath(*Ambient, GasSource, pulse::AnesthesiaMachinePath::EnvironmentToGasSource);
  EnvironmentToGasSource.GetPressureSourceBaseline().SetValue(2000.0, PressureUnit::psi);
  /////////////////////////
  // GasSourceToGasInlet //
  SEFluidCircuitPath& GasSourceToGasInlet = cAnesthesia.CreatePath(GasSource, GasInlet, pulse::AnesthesiaMachinePath::GasSourceToGasInlet);
  GasSourceToGasInlet.GetResistanceBaseline().SetValue(dValveOpenResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  ///////////////////////////////
  // GasInletToInspiratoryLimb //
  SEFluidCircuitPath& GasInletToInspiratoryLimb = cAnesthesia.CreatePath(GasInlet, InspiratoryLimb, pulse::AnesthesiaMachinePath::GasInletToInspiratoryLimb);
  GasInletToInspiratoryLimb.GetResistanceBaseline().SetValue(dLowResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  /////////////////////////////
  // InspiratoryLimbToYPiece //
  SEFluidCircuitPath& InspiratoryLimbToYPiece = cAnesthesia.CreatePath(InspiratoryLimb, Ypiece, pulse::AnesthesiaMachinePath::InspiratoryLimbToYPiece);
  InspiratoryLimbToYPiece.GetResistanceBaseline().SetValue(dLowResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  ////////////////////////////
  // YPieceToExpiratoryLimb //
  SEFluidCircuitPath& YPieceToExpiratoryLimb = cAnesthesia.CreatePath(Ypiece, ExpiratoryLimb, pulse::AnesthesiaMachinePath::YPieceToExpiratoryLimb);
  YPieceToExpiratoryLimb.GetResistanceBaseline().SetValue(dLowResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  //////////////////////////////
  // ExpiratoryLimbToSelector //
  SEFluidCircuitPath& ExpiratoryLimbToSelector = cAnesthesia.CreatePath(ExpiratoryLimb, Selector, pulse::AnesthesiaMachinePath::ExpiratoryLimbToSelector);
  ExpiratoryLimbToSelector.GetResistanceBaseline().SetValue(dLowResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  //////////////////////////////////
  // YPieceToAnesthesiaConnection //
  SEFluidCircuitPath& YPieceToAnesthesiaConnection = cAnesthesia.CreatePath(Ypiece, AnesthesiaConnection, pulse::AnesthesiaMachinePath::YPieceToConnection);
  ///////////////////////////////////////
  // AnesthesiaConnectionToEnvironment //
  SEFluidCircuitPath& AnesthesiaConnectionToEnvironment = cAnesthesia.CreatePath(AnesthesiaConnection, *Ambient, pulse::AnesthesiaMachinePath::ConnectionToEnvironment);
  AnesthesiaConnectionToEnvironment.GetResistanceBaseline().SetValue(dSwitchOpenResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);

  cAnesthesia.SetNextAndCurrentFromBaselines();
  cAnesthesia.StateChange();

  //Combined Respiratory and Anesthesia Machine Circuit
  SEFluidCircuit& cCombinedAnesthesia = m_Circuits->GetRespiratoryAndAnesthesiaMachineCircuit();
  cCombinedAnesthesia.AddCircuit(cRespiratory);
  cCombinedAnesthesia.AddCircuit(cAnesthesia);
  SEFluidCircuitNode& Airway = *cCombinedAnesthesia.GetNode(pulse::RespiratoryNode::Airway);
  SEFluidCircuitPath& AnesthesiaConnectionToAirway = cCombinedAnesthesia.CreatePath(AnesthesiaConnection, Airway, pulse::CombinedAnesthesiaMachinePath::ConnectionToAirway);
  cCombinedAnesthesia.RemovePath(pulse::RespiratoryPath::EnvironmentToAirway);
  cCombinedAnesthesia.SetNextAndCurrentFromBaselines();
  cCombinedAnesthesia.StateChange();

  // Grab the Environment Compartment
  SEGasCompartment* eEnvironment = m_Compartments->GetGasCompartment(pulse::EnvironmentCompartment::Ambient);
  // Anesthesia Machine Compartments
  SEGasCompartment& aAnesthesiaConnection = m_Compartments->CreateGasCompartment(pulse::AnesthesiaMachineCompartment::Connection);
  aAnesthesiaConnection.MapNode(AnesthesiaConnection);
  SEGasCompartment& aExpiratoryLimb = m_Compartments->CreateGasCompartment(pulse::AnesthesiaMachineCompartment::ExpiratoryLimb);
  aExpiratoryLimb.MapNode(ExpiratoryLimb);
  SEGasCompartment& aGasInlet = m_Compartments->CreateGasCompartment(pulse::AnesthesiaMachineCompartment::GasInlet);
  aGasInlet.MapNode(GasInlet);
  SEGasCompartment& aGasSource = m_Compartments->CreateGasCompartment(pulse::AnesthesiaMachineCompartment::GasSource);
  aGasSource.MapNode(GasSource);
  SEGasCompartment& aInspiratoryLimb = m_Compartments->CreateGasCompartment(pulse::AnesthesiaMachineCompartment::InspiratoryLimb);
  aInspiratoryLimb.MapNode(InspiratoryLimb);
  SEGasCompartment& aReliefValve = m_Compartments->CreateGasCompartment(pulse::AnesthesiaMachineCompartment::ReliefValve);
  aReliefValve.MapNode(ReliefValve);
  SEGasCompartment& aScrubber = m_Compartments->CreateGasCompartment(pulse::AnesthesiaMachineCompartment::Scrubber);
  aScrubber.MapNode(Scrubber);
  SEGasCompartment& aSelector = m_Compartments->CreateGasCompartment(pulse::AnesthesiaMachineCompartment::Selector);
  aSelector.MapNode(Selector);
  SEGasCompartment& aVentilator = m_Compartments->CreateGasCompartment(pulse::AnesthesiaMachineCompartment::Ventilator);
  aVentilator.MapNode(Ventilator);
  SEGasCompartment& aYPiece = m_Compartments->CreateGasCompartment(pulse::AnesthesiaMachineCompartment::YPiece);
  aYPiece.MapNode(Ypiece);

  // Setup Links //
  SEGasCompartmentLink& aEnvironmentToReliefValve = m_Compartments->CreateGasLink(*eEnvironment, aReliefValve, pulse::AnesthesiaMachineLink::EnvironmentToReliefValve);
  aEnvironmentToReliefValve.MapPath(EnvironmentToReliefValve);
  SEGasCompartmentLink& aVentilatorToSelector = m_Compartments->CreateGasLink(aVentilator, aSelector, pulse::AnesthesiaMachineLink::VentilatorToSelector);
  aVentilatorToSelector.MapPath(VentilatorConnectionToSelector);
  SEGasCompartmentLink& aSelectorToReliefValve = m_Compartments->CreateGasLink(aSelector, aReliefValve, pulse::AnesthesiaMachineLink::SelectorToReliefValve);
  aSelectorToReliefValve.MapPath(SelectorToReliefValve);
  SEGasCompartmentLink& aSelectorToScrubber = m_Compartments->CreateGasLink(aSelector, aScrubber, pulse::AnesthesiaMachineLink::SelectorToScrubber);
  aSelectorToScrubber.MapPath(SelectorToScrubber);
  SEGasCompartmentLink& aScrubberToGasInlet = m_Compartments->CreateGasLink(aScrubber, aGasInlet, pulse::AnesthesiaMachineLink::ScrubberToGasInlet);
  aScrubberToGasInlet.MapPath(ScrubberToGasInlet);
  SEGasCompartmentLink& aGasSourceToGasInlet = m_Compartments->CreateGasLink(aGasSource, aGasInlet, pulse::AnesthesiaMachineLink::GasSourceToGasInlet);
  aGasSourceToGasInlet.MapPath(GasSourceToGasInlet);
  SEGasCompartmentLink& aGasInletToInspiratoryLimb = m_Compartments->CreateGasLink(aGasInlet, aInspiratoryLimb, pulse::AnesthesiaMachineLink::GasInletToInspiratoryLimb);
  aGasInletToInspiratoryLimb.MapPath(GasInletToInspiratoryLimb);
  SEGasCompartmentLink& aInspiratoryLimbToYPiece = m_Compartments->CreateGasLink(aInspiratoryLimb, aYPiece, pulse::AnesthesiaMachineLink::InspiratoryLimbToYPiece);
  aInspiratoryLimbToYPiece.MapPath(InspiratoryLimbToYPiece);
  SEGasCompartmentLink& aYPieceToExpiratoryLimb = m_Compartments->CreateGasLink(aYPiece, aExpiratoryLimb, pulse::AnesthesiaMachineLink::YPieceToExpiratoryLimb);
  aYPieceToExpiratoryLimb.MapPath(YPieceToExpiratoryLimb);
  SEGasCompartmentLink& aExpiratoryLimbToSelector = m_Compartments->CreateGasLink(aExpiratoryLimb, aSelector, pulse::AnesthesiaMachineLink::ExpiratoryLimbToSelector);
  aExpiratoryLimbToSelector.MapPath(ExpiratoryLimbToSelector);
  SEGasCompartmentLink& aYPieceToAnesthesiaConnection = m_Compartments->CreateGasLink(aYPiece, aAnesthesiaConnection, pulse::AnesthesiaMachineLink::YPieceToConnection);
  aYPieceToAnesthesiaConnection.MapPath(YPieceToAnesthesiaConnection);
  SEGasCompartmentLink& aAnesthesiaConnectionLeak = m_Compartments->CreateGasLink(aAnesthesiaConnection, *eEnvironment, pulse::AnesthesiaMachineLink::ConnectionLeak);
  aAnesthesiaConnectionLeak.MapPath(AnesthesiaConnectionToEnvironment);

  SEGasCompartmentGraph& gAnesthesia = m_Compartments->GetAnesthesiaMachineGraph();
  gAnesthesia.AddCompartment(*eEnvironment);
  gAnesthesia.AddCompartment(aAnesthesiaConnection);
  gAnesthesia.AddCompartment(aExpiratoryLimb);
  gAnesthesia.AddCompartment(aGasInlet);
  gAnesthesia.AddCompartment(aGasSource);
  gAnesthesia.AddCompartment(aInspiratoryLimb);
  gAnesthesia.AddCompartment(aReliefValve);
  gAnesthesia.AddCompartment(aScrubber);
  gAnesthesia.AddCompartment(aSelector);
  gAnesthesia.AddCompartment(aVentilator);
  gAnesthesia.AddCompartment(aYPiece);
  gAnesthesia.AddLink(aEnvironmentToReliefValve);
  gAnesthesia.AddLink(aVentilatorToSelector);
  gAnesthesia.AddLink(aSelectorToReliefValve);
  gAnesthesia.AddLink(aSelectorToScrubber);
  gAnesthesia.AddLink(aScrubberToGasInlet);
  gAnesthesia.AddLink(aGasSourceToGasInlet);
  gAnesthesia.AddLink(aGasInletToInspiratoryLimb);
  gAnesthesia.AddLink(aInspiratoryLimbToYPiece);
  gAnesthesia.AddLink(aYPieceToExpiratoryLimb);
  gAnesthesia.AddLink(aExpiratoryLimbToSelector);
  gAnesthesia.AddLink(aYPieceToAnesthesiaConnection);
  gAnesthesia.AddLink(aAnesthesiaConnectionLeak);
  gAnesthesia.StateChange();

  //Now do the combined transport setup
  // Grab the Airway from pulmonary
  SEGasCompartment* pAirway = m_Compartments->GetGasCompartment(pulse::PulmonaryCompartment::Airway);
  SEGasCompartmentLink& aAnesthesiaConnectionToAirway = m_Compartments->CreateGasLink(aAnesthesiaConnection, *pAirway, pulse::AnesthesiaMachineLink::ConnectionToAirway);
  aAnesthesiaConnectionToAirway.MapPath(AnesthesiaConnectionToAirway);

  SEGasCompartmentGraph& gCombinedRespiratoryAnesthesia = m_Compartments->GetRespiratoryAndAnesthesiaMachineGraph();
  gCombinedRespiratoryAnesthesia.AddGraph(gRespiratory);
  gCombinedRespiratoryAnesthesia.AddGraph(gAnesthesia);
  gCombinedRespiratoryAnesthesia.RemoveLink(pulse::PulmonaryLink::EnvironmentToAirway);
  gCombinedRespiratoryAnesthesia.AddLink(aAnesthesiaConnectionToAirway);
  gCombinedRespiratoryAnesthesia.StateChange();
}

void PulseController::SetupInhaler()
{
  Info("Setting Up Inhaler");
  /////////////////////// Circuit Interdependencies
  double dLowResistance = 0.01; // Also defined in SetupRespiratoryCircuit
  SEFluidCircuit& cRespiratory = m_Circuits->GetRespiratoryCircuit();
  SEGasCompartmentGraph& gRespiratory = m_Compartments->GetRespiratoryGraph();
  SELiquidCompartmentGraph& lAerosol = m_Compartments->GetAerosolGraph();
  ///////////////////////

  //Combined Respiratory and Inhaler Circuit
  SEFluidCircuit& m_CombinedInhaler = m_Circuits->GetRespiratoryAndInhalerCircuit();
  m_CombinedInhaler.AddCircuit(cRespiratory);
  // Grab connection points/nodes
  SEFluidCircuitNode& Airway = *cRespiratory.GetNode(pulse::RespiratoryNode::Airway);
  SEFluidCircuitNode& Ambient = *cRespiratory.GetNode(pulse::EnvironmentNode::Ambient);
  // Define node on the combined graph, this is a simple circuit, no reason to make a independent circuit at this point
  SEFluidCircuitNode& Mouthpiece = m_CombinedInhaler.CreateNode(pulse::InhalerNode::Mouthpiece);
  Mouthpiece.GetPressure().SetValue(0.0, PressureUnit::cmH2O);
  Mouthpiece.GetNextPressure().SetValue(0.0, PressureUnit::cmH2O);
  double dInhalerBaseVolume_L = 0.030;  // 30 milliliters
  Mouthpiece.GetVolumeBaseline().SetValue(dInhalerBaseVolume_L, VolumeUnit::L);
  // Define path on the combined graph, this is a simple circuit, no reason to make a independent circuit at this point
  SEFluidCircuitPath& EnvironmentToMouthpiece = m_CombinedInhaler.CreatePath(Ambient, Mouthpiece, pulse::InhalerPath::EnvironmentToMouthpiece);
  // Connect Path
  SEFluidCircuitPath& MouthpieceToAirway = m_CombinedInhaler.CreatePath(Mouthpiece, Airway, pulse::InhalerPath::MouthpieceToAirway);
  MouthpieceToAirway.GetResistanceBaseline().SetValue(dLowResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  m_CombinedInhaler.RemovePath(pulse::RespiratoryPath::EnvironmentToAirway);
  m_CombinedInhaler.SetNextAndCurrentFromBaselines();
  m_CombinedInhaler.StateChange();

  //////////////////////
  // GAS COMPARTMENTS //
  SEGasCompartment* gAirway = m_Compartments->GetGasCompartment(pulse::PulmonaryCompartment::Airway);
  SEGasCompartment* gAmbient = m_Compartments->GetGasCompartment(pulse::EnvironmentCompartment::Ambient);
  //////////////////
  // Compartments //
  SEGasCompartment& gMouthpiece = m_Compartments->CreateGasCompartment(pulse::InhalerCompartment::Mouthpiece);
  gMouthpiece.MapNode(Mouthpiece);
  ///////////
  // Links //
  SEGasCompartmentLink& gEnvironmentToMouthpiece = m_Compartments->CreateGasLink(*gAmbient, gMouthpiece, pulse::InhalerLink::EnvironmentToMouthpiece);
  gEnvironmentToMouthpiece.MapPath(EnvironmentToMouthpiece);
  SEGasCompartmentLink& gMouthpieceToAirway = m_Compartments->CreateGasLink(gMouthpiece, *gAirway, pulse::InhalerLink::MouthpieceToAirway);
  gMouthpieceToAirway.MapPath(MouthpieceToAirway);
  ///////////
  // Graph //
  SEGasCompartmentGraph& gCombinedInhaler = m_Compartments->GetRespiratoryAndInhalerGraph();
  gCombinedInhaler.AddGraph(gRespiratory);
  gCombinedInhaler.RemoveLink(pulse::PulmonaryLink::EnvironmentToAirway);
  gCombinedInhaler.AddCompartment(gMouthpiece);
  gCombinedInhaler.AddLink(gEnvironmentToMouthpiece);
  gCombinedInhaler.AddLink(gMouthpieceToAirway);
  gCombinedInhaler.StateChange();

  // I could probably take the generic code I wrote in SetupRespiratory to clone the gas setup into a liquid setup
  // and then call that method here, but this is such a simple circuit... I will leave that as an exercise for somebody else...

   ///////////////////////////////////
  // LIQUID (AEROSOL) COMPARTMENTS //
  SELiquidCompartment* lAirway = m_Compartments->GetLiquidCompartment(pulse::PulmonaryCompartment::Airway);
  SELiquidCompartment* lAmbient = m_Compartments->GetLiquidCompartment(pulse::EnvironmentCompartment::Ambient);
  //////////////////
  // Compartments //
  SELiquidCompartment& lMouthpiece = m_Compartments->CreateLiquidCompartment(pulse::InhalerCompartment::Mouthpiece);
  lMouthpiece.MapNode(Mouthpiece);
  ///////////
  // Links //
  SELiquidCompartmentLink& lEnvironmentToMouthpiece = m_Compartments->CreateLiquidLink(*lAmbient, lMouthpiece, pulse::InhalerLink::EnvironmentToMouthpiece);
  lEnvironmentToMouthpiece.MapPath(EnvironmentToMouthpiece);
  SELiquidCompartmentLink& lMouthpieceToAirway = m_Compartments->CreateLiquidLink(lMouthpiece, *lAirway, pulse::InhalerLink::MouthpieceToAirway);
  lMouthpieceToAirway.MapPath(MouthpieceToAirway);
  ///////////
  // Graph //
  SELiquidCompartmentGraph& lCombinedInhaler = m_Compartments->GetAerosolAndInhalerGraph();
  lCombinedInhaler.AddGraph(lAerosol);
  lCombinedInhaler.RemoveLink(pulse::PulmonaryLink::EnvironmentToAirway);
  lCombinedInhaler.AddCompartment(lMouthpiece);
  lCombinedInhaler.AddLink(lEnvironmentToMouthpiece);
  lCombinedInhaler.AddLink(lMouthpieceToAirway);
  lCombinedInhaler.StateChange();
}

void PulseController::SetupMechanicalVentilation()
{
  Info("Setting Up MechanicalVentilation");
  /////////////////////// Circuit Interdependencies
  SEFluidCircuit& cRespiratory = m_Circuits->GetRespiratoryCircuit();
  SEGasCompartmentGraph& gRespiratory = m_Compartments->GetRespiratoryGraph();
  SELiquidCompartmentGraph& lAerosol = m_Compartments->GetAerosolGraph();
  ///////////////////////

  //Combined Respiratory and Mechanical Ventilation Circuit
  SEFluidCircuit& m_CombinedMechanicalVentilation = m_Circuits->GetRespiratoryAndMechanicalVentilationCircuit();
  m_CombinedMechanicalVentilation.AddCircuit(cRespiratory);
  // Grab connection points/nodes
  SEFluidCircuitNode& Airway = *cRespiratory.GetNode(pulse::RespiratoryNode::Airway);
  SEFluidCircuitNode& Ambient = *cRespiratory.GetNode(pulse::EnvironmentNode::Ambient);
  // Define node on the combined graph, this is a simple circuit, no reason to make a independent circuit at this point
  SEFluidCircuitNode& Connection = m_CombinedMechanicalVentilation.CreateNode(pulse::MechanicalVentilationNode::Connection);
  Connection.GetPressure().Set(Ambient.GetPressure());
  Connection.GetNextPressure().Set(Ambient.GetNextPressure());
  Connection.GetVolumeBaseline().SetValue(std::numeric_limits<double>::infinity(), VolumeUnit::L);
  // Paths
  SEFluidCircuitPath& ConnectionToAirway = m_CombinedMechanicalVentilation.CreatePath(Connection, Airway, pulse::MechanicalVentilationPath::ConnectionToAirway);
  //ConnectionToAirway.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::L_Per_s); //We add this on the fly, it can only be there when explicitly set
  SEFluidCircuitPath& GroundToConnection = m_CombinedMechanicalVentilation.CreatePath(Ambient, Connection, pulse::MechanicalVentilationPath::GroundToConnection);
  GroundToConnection.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::cmH2O);
  m_CombinedMechanicalVentilation.RemovePath(pulse::RespiratoryPath::EnvironmentToAirway);
  m_CombinedMechanicalVentilation.SetNextAndCurrentFromBaselines();
  m_CombinedMechanicalVentilation.StateChange();

  //////////////////////
  // GAS COMPARTMENTS //
  SEGasCompartment* gAirway = m_Compartments->GetGasCompartment(pulse::PulmonaryCompartment::Airway);
  //////////////////
  // Compartments //
  SEGasCompartment& gConnection = m_Compartments->CreateGasCompartment(pulse::MechanicalVentilationCompartment::Connection);
  gConnection.MapNode(Connection);
  ///////////
  // Links //  
  SEGasCompartmentLink& gConnectionToAirway = m_Compartments->CreateGasLink(gConnection, *gAirway, pulse::MechanicalVentilationLink::ConnectionToAirway);
  gConnectionToAirway.MapPath(ConnectionToAirway);
  ///////////
  // Graph //
  SEGasCompartmentGraph& gCombinedMechanicalVentilation = m_Compartments->GetRespiratoryAndMechanicalVentilationGraph();
  gCombinedMechanicalVentilation.AddGraph(gRespiratory);
  gCombinedMechanicalVentilation.RemoveLink(pulse::PulmonaryLink::EnvironmentToAirway);
  gCombinedMechanicalVentilation.AddCompartment(gConnection);
  gCombinedMechanicalVentilation.AddLink(gConnectionToAirway);
  gCombinedMechanicalVentilation.StateChange();

  ///////////////////////////////////
  // LIQUID (AEROSOL) COMPARTMENTS //
  SELiquidCompartment* lAirway = m_Compartments->GetLiquidCompartment(pulse::PulmonaryCompartment::Airway);
  //////////////////
  // Compartments //
  SELiquidCompartment& lConnection = m_Compartments->CreateLiquidCompartment(pulse::MechanicalVentilationCompartment::Connection);
  lConnection.MapNode(Connection);
  ///////////
  // Links //  
  SELiquidCompartmentLink& lConnectionToAirway = m_Compartments->CreateLiquidLink(lConnection, *lAirway, pulse::MechanicalVentilationLink::ConnectionToAirway);
  lConnectionToAirway.MapPath(ConnectionToAirway);
  ///////////
  // Graph //
  SELiquidCompartmentGraph& lCombinedMechanicalVentilation = m_Compartments->GetAerosolAndMechanicalVentilationGraph();
  lCombinedMechanicalVentilation.AddGraph(lAerosol);
  lCombinedMechanicalVentilation.RemoveLink(pulse::PulmonaryLink::EnvironmentToAirway);
  lCombinedMechanicalVentilation.AddCompartment(lConnection);
  lCombinedMechanicalVentilation.AddLink(lConnectionToAirway);
  lCombinedMechanicalVentilation.StateChange();
}

void PulseController::SetupMechanicalVentilator()
{
  Info("Setting Up MechanicalVentilator");
  /////////////////////// Circuit Interdependencies
  SEFluidCircuit& cRespiratory = m_Circuits->GetRespiratoryCircuit();
  SEGasCompartmentGraph& gRespiratory = m_Compartments->GetRespiratoryGraph();
  SELiquidCompartmentGraph& lAerosol = m_Compartments->GetAerosolGraph();
  ///////////////////////

  double tubeVolume_L = 0.3; //4 total tubes - this is per tube
                             //22mm ID * 36" length = pi * (0.022m / 2)^2 * 0.91m = 3.46e-4 m^3 = 0.346 L... so decent ballpark
  double yPieceVolume_L = 0.01;
  double connectionVolume_L = 0.05;
  double tubeResistance_cmH2O_s_Per_L = 0.01; //4 total tubes - this is per tube 
                                              //this is pretty negligable

  /////////////
  // Circuit //
  // Nodes
  SEFluidCircuit& cMechanicalVentilator = m_Circuits->GetMechanicalVentilatorCircuit();
  SEFluidCircuitNode& Ambient = *cRespiratory.GetNode(pulse::EnvironmentNode::Ambient);
  cMechanicalVentilator.AddNode(Ambient);

  SEFluidCircuitNode& Ventilator = cMechanicalVentilator.CreateNode(pulse::MechanicalVentilatorNode::Ventilator);
  Ventilator.GetPressure().Set(Ambient.GetPressure());
  Ventilator.GetVolumeBaseline().SetValue(std::numeric_limits<double>::infinity(), VolumeUnit::L);

  SEFluidCircuitNode& ExpiratoryValve = cMechanicalVentilator.CreateNode(pulse::MechanicalVentilatorNode::ExpiratoryValve);
  ExpiratoryValve.GetPressure().Set(Ambient.GetPressure());
  ExpiratoryValve.GetVolumeBaseline().SetValue(tubeVolume_L, VolumeUnit::L);

  SEFluidCircuitNode& InspiratoryValve = cMechanicalVentilator.CreateNode(pulse::MechanicalVentilatorNode::InspiratoryValve);
  InspiratoryValve.GetPressure().Set(Ambient.GetPressure());
  InspiratoryValve.GetVolumeBaseline().SetValue(tubeVolume_L, VolumeUnit::L);

  SEFluidCircuitNode& ExpiratoryLimb = cMechanicalVentilator.CreateNode(pulse::MechanicalVentilatorNode::ExpiratoryLimb);
  ExpiratoryLimb.GetPressure().Set(Ambient.GetPressure());
  ExpiratoryLimb.GetVolumeBaseline().SetValue(tubeVolume_L, VolumeUnit::L);

  SEFluidCircuitNode& InspiratoryLimb = cMechanicalVentilator.CreateNode(pulse::MechanicalVentilatorNode::InspiratoryLimb);
  InspiratoryLimb.GetPressure().Set(Ambient.GetPressure());
  InspiratoryLimb.GetVolumeBaseline().SetValue(tubeVolume_L, VolumeUnit::L);

  SEFluidCircuitNode& YPiece = cMechanicalVentilator.CreateNode(pulse::MechanicalVentilatorNode::YPiece);
  YPiece.GetPressure().Set(Ambient.GetPressure());
  YPiece.GetVolumeBaseline().SetValue(yPieceVolume_L, VolumeUnit::L);

  SEFluidCircuitNode& Connection = cMechanicalVentilator.CreateNode(pulse::MechanicalVentilatorNode::Connection);
  Connection.GetPressure().Set(Ambient.GetPressure());
  Connection.GetVolumeBaseline().SetValue(connectionVolume_L, VolumeUnit::L);

  //Paths
  SEFluidCircuitPath& EnvironmentToVentilator = cMechanicalVentilator.CreatePath(Ambient, Ventilator, pulse::MechanicalVentilatorPath::EnvironmentToVentilator);
  EnvironmentToVentilator.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::cmH2O);

  SEFluidCircuitPath& VentilatorToExpiratoryValve = cMechanicalVentilator.CreatePath(Ventilator, ExpiratoryValve, pulse::MechanicalVentilatorPath::VentilatorToExpiratoryValve);
  VentilatorToExpiratoryValve.GetResistanceBaseline().SetValue(tubeResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);

  SEFluidCircuitPath& VentilatorToInspiratoryValve = cMechanicalVentilator.CreatePath(Ventilator, InspiratoryValve, pulse::MechanicalVentilatorPath::VentilatorToInspiratoryValve);
  VentilatorToInspiratoryValve.GetResistanceBaseline().SetValue(tubeResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);

  SEFluidCircuitPath& ExpiratoryLimbToExpiratoryValve = cMechanicalVentilator.CreatePath(ExpiratoryLimb, ExpiratoryValve, pulse::MechanicalVentilatorPath::ExpiratoryLimbToExpiratoryValve);
  ExpiratoryLimbToExpiratoryValve.SetNextValve(eGate::Open);

  SEFluidCircuitPath& InspiratoryValveToInspiratoryLimb = cMechanicalVentilator.CreatePath(InspiratoryValve, InspiratoryLimb, pulse::MechanicalVentilatorPath::InspiratoryValveToInspiratoryLimb);
  InspiratoryValveToInspiratoryLimb.SetNextValve(eGate::Open);

  SEFluidCircuitPath& ExpiratoryLimbToYPiece = cMechanicalVentilator.CreatePath(ExpiratoryLimb, YPiece, pulse::MechanicalVentilatorPath::ExpiratoryLimbToYPiece);
  ExpiratoryLimbToYPiece.GetResistanceBaseline().SetValue(tubeResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);

  SEFluidCircuitPath& InspiratoryLimbToYPiece = cMechanicalVentilator.CreatePath(InspiratoryLimb, YPiece, pulse::MechanicalVentilatorPath::InspiratoryLimbToYPiece);
  InspiratoryLimbToYPiece.GetResistanceBaseline().SetValue(tubeResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);

  SEFluidCircuitPath& YPieceToConnection = cMechanicalVentilator.CreatePath(YPiece, Connection, pulse::MechanicalVentilatorPath::YPieceToConnection);

  SEFluidCircuitPath& ConnectionToEnvironment = cMechanicalVentilator.CreatePath(Connection, Ambient, pulse::MechanicalVentilatorPath::ConnectionToEnvironment);

  cMechanicalVentilator.SetNextAndCurrentFromBaselines();
  cMechanicalVentilator.StateChange();

  //Combined Respiratory and Mechanical Ventilator Circuit
  SEFluidCircuit& cCombinedMechanicalVentilator = m_Circuits->GetRespiratoryAndMechanicalVentilatorCircuit();
  cCombinedMechanicalVentilator.AddCircuit(cRespiratory);
  cCombinedMechanicalVentilator.AddCircuit(cMechanicalVentilator);
  SEFluidCircuitNode& Airway = *cCombinedMechanicalVentilator.GetNode(pulse::RespiratoryNode::Airway);
  SEFluidCircuitPath& ConnectionToAirway = cCombinedMechanicalVentilator.CreatePath(Connection, Airway, pulse::CombinedMechanicalVentilatorPath::ConnectionToAirway);
  cCombinedMechanicalVentilator.RemovePath(pulse::RespiratoryPath::EnvironmentToAirway);
  cCombinedMechanicalVentilator.RemovePath(pulse::MechanicalVentilatorPath::ConnectionToEnvironment);
  cCombinedMechanicalVentilator.SetNextAndCurrentFromBaselines();
  cCombinedMechanicalVentilator.StateChange();

  //////////////////////
  // GAS COMPARTMENTS //
  // Grab the Environment Compartment
  SEGasCompartment* eEnvironment = m_Compartments->GetGasCompartment(pulse::EnvironmentCompartment::Ambient);
  // Mechanical Ventilator Compartments
  SEGasCompartment& mVentilator = m_Compartments->CreateGasCompartment(pulse::MechanicalVentilatorCompartment::MechanicalVentilator);
  mVentilator.MapNode(Ventilator);
  SEGasCompartment& mExpiratoryValve = m_Compartments->CreateGasCompartment(pulse::MechanicalVentilatorCompartment::ExpiratoryValve);
  mExpiratoryValve.MapNode(ExpiratoryValve);
  SEGasCompartment& mInspiratoryValve = m_Compartments->CreateGasCompartment(pulse::MechanicalVentilatorCompartment::InspiratoryValve);
  mInspiratoryValve.MapNode(InspiratoryValve);
  SEGasCompartment& mExpiratoryLimb = m_Compartments->CreateGasCompartment(pulse::MechanicalVentilatorCompartment::ExpiratoryLimb);
  mExpiratoryLimb.MapNode(ExpiratoryLimb);
  SEGasCompartment& mInspiratoryLimb = m_Compartments->CreateGasCompartment(pulse::MechanicalVentilatorCompartment::InspiratoryLimb);
  mInspiratoryLimb.MapNode(InspiratoryLimb);
  SEGasCompartment& mYPiece = m_Compartments->CreateGasCompartment(pulse::MechanicalVentilatorCompartment::YPiece);
  mYPiece.MapNode(YPiece);
  SEGasCompartment& mConnection = m_Compartments->CreateGasCompartment(pulse::MechanicalVentilatorCompartment::Connection);
  mConnection.MapNode(Connection);

  // Setup Links //
  SEGasCompartmentLink& mVentilatorToExpiratoryValve = m_Compartments->CreateGasLink(mVentilator, mExpiratoryValve, pulse::MechanicalVentilatorLink::MechanicalVentilatorToExpiratoryValve);
  mVentilatorToExpiratoryValve.MapPath(VentilatorToExpiratoryValve);
  SEGasCompartmentLink& mVentilatorToInspiratoryValve = m_Compartments->CreateGasLink(mVentilator, mInspiratoryValve, pulse::MechanicalVentilatorLink::MechanicalVentilatorToInspiratoryValve);
  mVentilatorToInspiratoryValve.MapPath(VentilatorToInspiratoryValve);
  SEGasCompartmentLink& mExpiratoryLimbToExpiratoryValve = m_Compartments->CreateGasLink(mExpiratoryLimb, mExpiratoryValve, pulse::MechanicalVentilatorLink::ExpiratoryLimbToExpiratoryValve);
  mExpiratoryLimbToExpiratoryValve.MapPath(ExpiratoryLimbToExpiratoryValve);
  SEGasCompartmentLink& mInspiratoryValveToInspiratoryLimb = m_Compartments->CreateGasLink(mInspiratoryValve, mInspiratoryLimb, pulse::MechanicalVentilatorLink::InspiratoryValveToInspiratoryLimb);
  mInspiratoryValveToInspiratoryLimb.MapPath(InspiratoryValveToInspiratoryLimb);
  SEGasCompartmentLink& mExpiratoryLimbToYPiece = m_Compartments->CreateGasLink(mExpiratoryLimb, mYPiece, pulse::MechanicalVentilatorLink::ExpiratoryLimbToYPiece);
  mExpiratoryLimbToYPiece.MapPath(ExpiratoryLimbToYPiece);
  SEGasCompartmentLink& mInspiratoryLimbToYPiece = m_Compartments->CreateGasLink(mInspiratoryLimb, mYPiece, pulse::MechanicalVentilatorLink::InspiratoryLimbToYPiece);
  mInspiratoryLimbToYPiece.MapPath(InspiratoryLimbToYPiece);
  SEGasCompartmentLink& mYPieceToConnection = m_Compartments->CreateGasLink(mYPiece, mConnection, pulse::MechanicalVentilatorLink::YPieceToConnection);
  mYPieceToConnection.MapPath(YPieceToConnection);
  SEGasCompartmentLink& mConnectionToEnvironment = m_Compartments->CreateGasLink(mConnection, *eEnvironment, pulse::MechanicalVentilatorLink::ConnectionToEnvironment);
  mConnectionToEnvironment.MapPath(ConnectionToEnvironment);

  SEGasCompartmentGraph& gMechanicalVentilator = m_Compartments->GetMechanicalVentilatorGraph();
  gMechanicalVentilator.AddCompartment(mVentilator);
  gMechanicalVentilator.AddCompartment(mExpiratoryValve);
  gMechanicalVentilator.AddCompartment(mInspiratoryValve);
  gMechanicalVentilator.AddCompartment(mExpiratoryLimb);
  gMechanicalVentilator.AddCompartment(mInspiratoryLimb);
  gMechanicalVentilator.AddCompartment(mYPiece);
  gMechanicalVentilator.AddCompartment(mConnection);
  gMechanicalVentilator.AddLink(mVentilatorToExpiratoryValve);
  gMechanicalVentilator.AddLink(mVentilatorToInspiratoryValve);
  gMechanicalVentilator.AddLink(mExpiratoryLimbToExpiratoryValve);
  gMechanicalVentilator.AddLink(mInspiratoryValveToInspiratoryLimb);
  gMechanicalVentilator.AddLink(mExpiratoryLimbToYPiece);
  gMechanicalVentilator.AddLink(mInspiratoryLimbToYPiece);
  gMechanicalVentilator.AddLink(mYPieceToConnection);
  gMechanicalVentilator.AddLink(mConnectionToEnvironment);
  gMechanicalVentilator.StateChange();

  //Now do the combined transport setup
  // Grab the Airway from pulmonary
  SEGasCompartment* pAirway = m_Compartments->GetGasCompartment(pulse::PulmonaryCompartment::Airway);
  SEGasCompartmentLink& mConnectionToAirway = m_Compartments->CreateGasLink(mConnection, *pAirway, pulse::MechanicalVentilatorLink::ConnectionToAirway);
  mConnectionToAirway.MapPath(ConnectionToAirway);

  SEGasCompartmentGraph& gCombinedRespiratoryMechanicalVentilator = m_Compartments->GetRespiratoryAndMechanicalVentilatorGraph();
  gCombinedRespiratoryMechanicalVentilator.AddGraph(gRespiratory);
  gCombinedRespiratoryMechanicalVentilator.AddGraph(gMechanicalVentilator);
  gCombinedRespiratoryMechanicalVentilator.RemoveLink(pulse::PulmonaryLink::EnvironmentToAirway);
  gCombinedRespiratoryMechanicalVentilator.RemoveLink(pulse::MechanicalVentilatorLink::ConnectionToEnvironment);
  gCombinedRespiratoryMechanicalVentilator.AddLink(mConnectionToAirway);
  gCombinedRespiratoryMechanicalVentilator.StateChange();

  ///////////////////////////////////
  // LIQUID (AEROSOL) COMPARTMENTS //
  // Grab from pulmonary
  SELiquidCompartment* lAirway = m_Compartments->GetLiquidCompartment(pulse::PulmonaryCompartment::Airway);
  SELiquidCompartment* lEnvironment = m_Compartments->GetLiquidCompartment(pulse::EnvironmentCompartment::Ambient);

  // Mechanical Ventilator Compartments
  SELiquidCompartment& lVentilator = m_Compartments->CreateLiquidCompartment(pulse::MechanicalVentilatorCompartment::MechanicalVentilator);
  lVentilator.MapNode(Ventilator);
  SELiquidCompartment& lExpiratoryValve = m_Compartments->CreateLiquidCompartment(pulse::MechanicalVentilatorCompartment::ExpiratoryValve);
  lExpiratoryValve.MapNode(ExpiratoryValve);
  SELiquidCompartment& lInspiratoryValve = m_Compartments->CreateLiquidCompartment(pulse::MechanicalVentilatorCompartment::InspiratoryValve);
  lInspiratoryValve.MapNode(InspiratoryValve);
  SELiquidCompartment& lExpiratoryLimb = m_Compartments->CreateLiquidCompartment(pulse::MechanicalVentilatorCompartment::ExpiratoryLimb);
  lExpiratoryLimb.MapNode(ExpiratoryLimb);
  SELiquidCompartment& lInspiratoryLimb = m_Compartments->CreateLiquidCompartment(pulse::MechanicalVentilatorCompartment::InspiratoryLimb);
  lInspiratoryLimb.MapNode(InspiratoryLimb);
  SELiquidCompartment& lYPiece = m_Compartments->CreateLiquidCompartment(pulse::MechanicalVentilatorCompartment::YPiece);
  lYPiece.MapNode(YPiece);
  SELiquidCompartment& lConnection = m_Compartments->CreateLiquidCompartment(pulse::MechanicalVentilatorCompartment::Connection);
  lConnection.MapNode(Connection);

  //Links
  SELiquidCompartmentLink& lConnectionToAirway = m_Compartments->CreateLiquidLink(lConnection, *lAirway, pulse::MechanicalVentilatorLink::ConnectionToAirway);
  lConnectionToAirway.MapPath(ConnectionToAirway);

  SELiquidCompartmentLink& lVentilatorToExpiratoryValve = m_Compartments->CreateLiquidLink(lVentilator, lExpiratoryValve, pulse::MechanicalVentilatorLink::MechanicalVentilatorToExpiratoryValve);
  lVentilatorToExpiratoryValve.MapPath(VentilatorToExpiratoryValve);
  SELiquidCompartmentLink& lVentilatorToInspiratoryValve = m_Compartments->CreateLiquidLink(lVentilator, lInspiratoryValve, pulse::MechanicalVentilatorLink::MechanicalVentilatorToInspiratoryValve);
  lVentilatorToInspiratoryValve.MapPath(VentilatorToInspiratoryValve);
  SELiquidCompartmentLink& lExpiratoryLimbToExpiratoryValve = m_Compartments->CreateLiquidLink(lExpiratoryLimb, lExpiratoryValve, pulse::MechanicalVentilatorLink::ExpiratoryLimbToExpiratoryValve);
  lExpiratoryLimbToExpiratoryValve.MapPath(ExpiratoryLimbToExpiratoryValve);
  SELiquidCompartmentLink& lInspiratoryValveToInspiratoryLimb = m_Compartments->CreateLiquidLink(lInspiratoryValve, lInspiratoryLimb, pulse::MechanicalVentilatorLink::InspiratoryValveToInspiratoryLimb);
  lInspiratoryValveToInspiratoryLimb.MapPath(InspiratoryValveToInspiratoryLimb);
  SELiquidCompartmentLink& lExpiratoryLimbToYPiece = m_Compartments->CreateLiquidLink(lExpiratoryLimb, lYPiece, pulse::MechanicalVentilatorLink::ExpiratoryLimbToYPiece);
  lExpiratoryLimbToYPiece.MapPath(ExpiratoryLimbToYPiece);
  SELiquidCompartmentLink& lInspiratoryLimbToYPiece = m_Compartments->CreateLiquidLink(lInspiratoryLimb, lYPiece, pulse::MechanicalVentilatorLink::InspiratoryLimbToYPiece);
  lInspiratoryLimbToYPiece.MapPath(InspiratoryLimbToYPiece);
  SELiquidCompartmentLink& lYPieceToConnection = m_Compartments->CreateLiquidLink(lYPiece, lConnection, pulse::MechanicalVentilatorLink::YPieceToConnection);
  lYPieceToConnection.MapPath(YPieceToConnection);
  SELiquidCompartmentLink& lConnectionToEnvironment = m_Compartments->CreateLiquidLink(lConnection, *lEnvironment, pulse::MechanicalVentilatorLink::ConnectionToEnvironment);
  lConnectionToEnvironment.MapPath(ConnectionToEnvironment);

  //Graph
  SELiquidCompartmentGraph& lCombinedMechanicalVentilator = m_Compartments->GetAerosolAndMechanicalVentilatorGraph();
  //Respiratory Graph
  lCombinedMechanicalVentilator.AddGraph(lAerosol);
  lCombinedMechanicalVentilator.RemoveLink(pulse::PulmonaryLink::EnvironmentToAirway);
  //Mechanical Ventilator Additions
  lCombinedMechanicalVentilator.AddCompartment(lVentilator);
  lCombinedMechanicalVentilator.AddCompartment(lExpiratoryValve);
  lCombinedMechanicalVentilator.AddCompartment(lInspiratoryValve);
  lCombinedMechanicalVentilator.AddCompartment(lExpiratoryLimb);
  lCombinedMechanicalVentilator.AddCompartment(lInspiratoryLimb);
  lCombinedMechanicalVentilator.AddCompartment(lYPiece);
  lCombinedMechanicalVentilator.AddCompartment(lConnection);
  lCombinedMechanicalVentilator.AddLink(lVentilatorToExpiratoryValve);
  lCombinedMechanicalVentilator.AddLink(lVentilatorToInspiratoryValve);
  lCombinedMechanicalVentilator.AddLink(lExpiratoryLimbToExpiratoryValve);
  lCombinedMechanicalVentilator.AddLink(lInspiratoryValveToInspiratoryLimb);
  lCombinedMechanicalVentilator.AddLink(lExpiratoryLimbToYPiece);
  lCombinedMechanicalVentilator.AddLink(lInspiratoryLimbToYPiece);
  lCombinedMechanicalVentilator.AddLink(lYPieceToConnection);
  //lCombinedMechanicalVentilator.AddLink(lConnectionToEnvironment);
  //Connection to Respiratory
  lCombinedMechanicalVentilator.AddLink(lConnectionToAirway);
  //Set it
  lCombinedMechanicalVentilator.StateChange();
}

void PulseController::SetupNasalCannula()
{
  Info("Setting Up Nasal Cannula");
  /////////////////////// Circuit Interdependencies
  SEFluidCircuit& cRespiratory = m_Circuits->GetRespiratoryCircuit();
  SEGasCompartmentGraph& gRespiratory = m_Compartments->GetRespiratoryGraph();
  double OpenResistance_cmH2O_s_Per_L = m_Config->GetDefaultOpenFlowResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L); //open switch resistance is super high (i.e., tight seal)
  ///////////////////////

  //Combined Respiratory and Nasal Cannula Circuit
  SEFluidCircuit& CombinedNasalCannula = m_Circuits->GetRespiratoryAndNasalCannulaCircuit();
  CombinedNasalCannula.AddCircuit(cRespiratory);
  // Grab connection points/nodes
  SEFluidCircuitNode& Airway = *cRespiratory.GetNode(pulse::RespiratoryNode::Airway);
  SEFluidCircuitNode& Ambient = *cRespiratory.GetNode(pulse::EnvironmentNode::Ambient);
  // Define node on the combined graph, this is a simple circuit, no reason to make a independent circuit at this point
  SEFluidCircuitNode& NasalCannula = CombinedNasalCannula.CreateNode(pulse::NasalCannulaNode::NasalCannula);
  NasalCannula.GetVolumeBaseline().SetValue(0.5, VolumeUnit::L); //Need this big enough to mix (not exactly sure why)
  SEFluidCircuitNode& OxygenSource = CombinedNasalCannula.CreateNode(pulse::NasalCannulaNode::NasalCannulaOxygenSource);
  OxygenSource.GetVolumeBaseline().SetValue(std::numeric_limits<double>::infinity(), VolumeUnit::L);
  // Define path on the combined graph, this is a simple circuit, no reason to make a independent circuit at this point
  SEFluidCircuitPath& GroundToOxygenSource = CombinedNasalCannula.CreatePath(Ambient, OxygenSource, pulse::NasalCannulaPath::NasalCannulaPressure);
  GroundToOxygenSource.GetPressureSourceBaseline().SetValue(2000.0, PressureUnit::psi);
  SEFluidCircuitPath& OxygenInlet = CombinedNasalCannula.CreatePath(OxygenSource, NasalCannula, pulse::NasalCannulaPath::NasalCannulaOxygenInlet);
  OxygenInlet.GetResistanceBaseline().SetValue(OpenResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  SEFluidCircuitPath& Seal = CombinedNasalCannula.CreatePath(NasalCannula, Ambient, pulse::NasalCannulaPath::NasalCannulaSeal);
  //No resistance
  // Connect Path
  SEFluidCircuitPath& NasalCannulaToAirway = CombinedNasalCannula.CreatePath(NasalCannula, Airway, pulse::NasalCannulaPath::NasalCannulaToAirway);
  CombinedNasalCannula.RemovePath(pulse::RespiratoryPath::EnvironmentToAirway);
  CombinedNasalCannula.SetNextAndCurrentFromBaselines();
  CombinedNasalCannula.StateChange();

  //////////////////////
  // GAS COMPARTMENTS //
  SEGasCompartment* gAirway = m_Compartments->GetGasCompartment(pulse::PulmonaryCompartment::Airway);
  SEGasCompartment* gAmbient = m_Compartments->GetGasCompartment(pulse::EnvironmentCompartment::Ambient);
  //////////////////
  // Compartments //
  SEGasCompartment& gNasalCannula = m_Compartments->CreateGasCompartment(pulse::NasalCannulaCompartment::NasalCannula);
  gNasalCannula.MapNode(NasalCannula);
  SEGasCompartment& gOxygenSource = m_Compartments->CreateGasCompartment(pulse::NasalCannulaCompartment::NasalCannulaOxygenSource);
  gOxygenSource.MapNode(OxygenSource);
  ///////////
  // Links //
  SEGasCompartmentLink& gOxygenInlet = m_Compartments->CreateGasLink(gOxygenSource, gNasalCannula, pulse::NasalCannulaLink::NasalCannulaOxygenInlet);
  gOxygenInlet.MapPath(OxygenInlet);
  SEGasCompartmentLink& gSeal = m_Compartments->CreateGasLink(*gAmbient, gNasalCannula, pulse::NasalCannulaLink::NasalCannulaSeal);
  gSeal.MapPath(Seal);
  SEGasCompartmentLink& gNasalCannulaToAirway = m_Compartments->CreateGasLink(gNasalCannula, *gAirway, pulse::NasalCannulaLink::NasalCannulaToAirway);
  gNasalCannulaToAirway.MapPath(NasalCannulaToAirway);
  ///////////
  // Graph //
  SEGasCompartmentGraph& gCombinedNasalCannula = m_Compartments->GetRespiratoryAndNasalCannulaGraph();
  gCombinedNasalCannula.AddGraph(gRespiratory);
  gCombinedNasalCannula.RemoveLink(pulse::PulmonaryLink::EnvironmentToAirway);
  gCombinedNasalCannula.AddCompartment(gNasalCannula);
  gCombinedNasalCannula.AddCompartment(gOxygenSource);
  gCombinedNasalCannula.AddLink(gOxygenInlet);
  gCombinedNasalCannula.AddLink(gSeal);
  gCombinedNasalCannula.AddLink(gNasalCannulaToAirway);
  gCombinedNasalCannula.StateChange();
}

void PulseController::SetupNonRebreatherMask()
{
  Info("Setting Up NonRebreather Mask");
  /////////////////////// Circuit Interdependencies
  SEFluidCircuit& cRespiratory = m_Circuits->GetRespiratoryCircuit();
  SEGasCompartmentGraph& gRespiratory = m_Compartments->GetRespiratoryGraph();
  double OpenResistance_cmH2O_s_Per_L = m_Config->GetDefaultOpenFlowResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L); //open switch resistance is super high (i.e., tight seal)
  double PortsResistance_cmH2O_s_Per_L = 0.1;
  ///////////////////////

  //Combined Respiratory and NonRebreather Mask Circuit
  SEFluidCircuit& CombinedNonRebreatherMask = m_Circuits->GetRespiratoryAndNonRebreatherMaskCircuit();
  CombinedNonRebreatherMask.AddCircuit(cRespiratory);
  // Grab connection points/nodes
  SEFluidCircuitNode& Airway = *cRespiratory.GetNode(pulse::RespiratoryNode::Airway);
  SEFluidCircuitNode& Ambient = *cRespiratory.GetNode(pulse::EnvironmentNode::Ambient);
  // Define node on the combined graph, this is a simple circuit, no reason to make a independent circuit at this point
  SEFluidCircuitNode& Mask = CombinedNonRebreatherMask.CreateNode(pulse::NonRebreatherMaskNode::NonRebreatherMask);
  Mask.GetPressure().SetValue(0.0, PressureUnit::cmH2O);
  Mask.GetNextPressure().SetValue(0.0, PressureUnit::cmH2O);
  Mask.GetVolumeBaseline().SetValue(0.5, VolumeUnit::L);
  SEFluidCircuitNode& OxygenSource = CombinedNonRebreatherMask.CreateNode(pulse::NonRebreatherMaskNode::NonRebreatherMaskOxygenSource);
  OxygenSource.GetPressure().SetValue(0.0, PressureUnit::cmH2O);
  OxygenSource.GetNextPressure().SetValue(0.0, PressureUnit::cmH2O);
  OxygenSource.GetVolumeBaseline().SetValue(std::numeric_limits<double>::infinity(), VolumeUnit::L);
  SEFluidCircuitNode& Bag = CombinedNonRebreatherMask.CreateNode(pulse::NonRebreatherMaskNode::NonRebreatherMaskBag);
  Bag.GetPressure().SetValue(0.0, PressureUnit::cmH2O);
  Bag.GetNextPressure().SetValue(0.0, PressureUnit::cmH2O);
  Bag.GetVolumeBaseline().SetValue(1.0, VolumeUnit::L);
  SEFluidCircuitNode& Ports = CombinedNonRebreatherMask.CreateNode(pulse::NonRebreatherMaskNode::NonRebreatherMaskPorts);
  Ports.GetPressure().SetValue(0.0, PressureUnit::cmH2O);
  Ports.GetNextPressure().SetValue(0.0, PressureUnit::cmH2O);
  // Define path on the combined graph, this is a simple circuit, no reason to make a independent circuit at this point
  CombinedNonRebreatherMask.CreatePath(Ambient, OxygenSource, pulse::NonRebreatherMaskPath::NonRebreatherMaskPressure);
  SEFluidCircuitPath& OxygenInlet = CombinedNonRebreatherMask.CreatePath(OxygenSource, Bag, pulse::NonRebreatherMaskPath::NonRebreatherMaskOxygenInlet);
  OxygenInlet.GetResistanceBaseline().SetValue(PortsResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  SEFluidCircuitPath& ReservoirValve = CombinedNonRebreatherMask.CreatePath(Bag, Mask, pulse::NonRebreatherMaskPath::NonRebreatherMaskReservoirValve);
  ReservoirValve.SetNextValve(eGate::Closed);
  SEFluidCircuitPath& Seal = CombinedNonRebreatherMask.CreatePath(Ambient, Mask, pulse::NonRebreatherMaskPath::NonRebreatherMaskSeal);
  Seal.GetResistanceBaseline().SetValue(OpenResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  SEFluidCircuitPath& ExhalationValves = CombinedNonRebreatherMask.CreatePath(Mask, Ports, pulse::NonRebreatherMaskPath::NonRebreatherMaskExhalationValves);
  ExhalationValves.SetNextValve(eGate::Closed);
  SEFluidCircuitPath& Exhalation = CombinedNonRebreatherMask.CreatePath(Ports, Ambient, pulse::NonRebreatherMaskPath::NonRebreatherMaskExhalation);
  Exhalation.GetResistanceBaseline().SetValue(PortsResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  // Connect Path
  SEFluidCircuitPath& MaskToAirway = CombinedNonRebreatherMask.CreatePath(Mask, Airway, pulse::NonRebreatherMaskPath::NonRebreatherMaskToAirway);
  CombinedNonRebreatherMask.RemovePath(pulse::RespiratoryPath::EnvironmentToAirway);
  CombinedNonRebreatherMask.SetNextAndCurrentFromBaselines();
  CombinedNonRebreatherMask.StateChange();

  //////////////////////
  // GAS COMPARTMENTS //
  SEGasCompartment* gAirway = m_Compartments->GetGasCompartment(pulse::PulmonaryCompartment::Airway);
  SEGasCompartment* gAmbient = m_Compartments->GetGasCompartment(pulse::EnvironmentCompartment::Ambient);
  //////////////////
  // Compartments //
  SEGasCompartment& gMask = m_Compartments->CreateGasCompartment(pulse::NonRebreatherMaskCompartment::NonRebreatherMask);
  gMask.MapNode(Mask);
  SEGasCompartment& gOxygenSource = m_Compartments->CreateGasCompartment(pulse::NonRebreatherMaskCompartment::NonRebreatherMaskOxygenSource);
  gOxygenSource.MapNode(OxygenSource);
  SEGasCompartment& gBag = m_Compartments->CreateGasCompartment(pulse::NonRebreatherMaskCompartment::NonRebreatherMaskBag);
  gBag.MapNode(Bag);
  SEGasCompartment& gPorts = m_Compartments->CreateGasCompartment(pulse::NonRebreatherMaskCompartment::NonRebreatherMaskPorts);
  gPorts.MapNode(Ports);
  ///////////
  // Links //
  SEGasCompartmentLink& gOxygenInlet = m_Compartments->CreateGasLink(gOxygenSource, gBag, pulse::NonRebreatherMaskLink::NonRebreatherMaskOxygenInlet);
  gOxygenInlet.MapPath(OxygenInlet);
  SEGasCompartmentLink& gReservoirValve = m_Compartments->CreateGasLink(gBag, gMask, pulse::NonRebreatherMaskLink::NonRebreatherMaskReservoirValve);
  gReservoirValve.MapPath(ReservoirValve);
  SEGasCompartmentLink& gSeal = m_Compartments->CreateGasLink(*gAmbient, gMask, pulse::NonRebreatherMaskLink::NonRebreatherMaskSeal);
  gSeal.MapPath(Seal);
  SEGasCompartmentLink& gExhalationValves = m_Compartments->CreateGasLink(gMask, gPorts, pulse::NonRebreatherMaskLink::NonRebreatherMaskExhalationValves);
  gExhalationValves.MapPath(ExhalationValves);
  SEGasCompartmentLink& gExhalation = m_Compartments->CreateGasLink(gPorts, *gAmbient, pulse::NonRebreatherMaskLink::NonRebreatherMaskExhalation);
  gExhalation.MapPath(Exhalation);
  SEGasCompartmentLink& gMaskToAirway = m_Compartments->CreateGasLink(gMask, *gAirway, pulse::NonRebreatherMaskLink::NonRebreatherMaskToAirway);
  gMaskToAirway.MapPath(MaskToAirway);
  ///////////
  // Graph //
  SEGasCompartmentGraph& gCombinedNonRebreatherMask = m_Compartments->GetRespiratoryAndNonRebreatherMaskGraph();
  gCombinedNonRebreatherMask.AddGraph(gRespiratory);
  gCombinedNonRebreatherMask.RemoveLink(pulse::PulmonaryLink::EnvironmentToAirway);
  gCombinedNonRebreatherMask.AddCompartment(gMask);
  gCombinedNonRebreatherMask.AddCompartment(gOxygenSource);
  gCombinedNonRebreatherMask.AddCompartment(gBag);
  gCombinedNonRebreatherMask.AddCompartment(gPorts);
  gCombinedNonRebreatherMask.AddLink(gOxygenInlet);
  gCombinedNonRebreatherMask.AddLink(gReservoirValve);
  gCombinedNonRebreatherMask.AddLink(gSeal);
  gCombinedNonRebreatherMask.AddLink(gExhalationValves);
  gCombinedNonRebreatherMask.AddLink(gExhalation);
  gCombinedNonRebreatherMask.AddLink(gMaskToAirway);
  gCombinedNonRebreatherMask.StateChange();
}

void PulseController::SetupSimpleMask()
{
  Info("Setting Up Simple Mask");
  /////////////////////// Circuit Interdependencies
  SEFluidCircuit& cRespiratory = m_Circuits->GetRespiratoryCircuit();
  SEGasCompartmentGraph& gRespiratory = m_Compartments->GetRespiratoryGraph();
  double OpenResistance_cmH2O_s_Per_L = m_Config->GetDefaultOpenFlowResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_L); //open switch resistance is super high (i.e., tight seal)
  double PortsResistance_cmH2O_s_Per_L = 0.1;
  ///////////////////////

  //Combined Respiratory and Simple Mask Circuit
  SEFluidCircuit& CombinedSimpleMask = m_Circuits->GetRespiratoryAndSimpleMaskCircuit();
  CombinedSimpleMask.AddCircuit(cRespiratory);
  // Grab connection points/nodes
  SEFluidCircuitNode& Airway = *cRespiratory.GetNode(pulse::RespiratoryNode::Airway);
  SEFluidCircuitNode& Ambient = *cRespiratory.GetNode(pulse::EnvironmentNode::Ambient);
  // Define node on the combined graph, this is a simple circuit, no reason to make a independent circuit at this point
  SEFluidCircuitNode& Mask = CombinedSimpleMask.CreateNode(pulse::SimpleMaskNode::SimpleMask);
  Mask.GetPressure().SetValue(0.0, PressureUnit::cmH2O);
  Mask.GetNextPressure().SetValue(0.0, PressureUnit::cmH2O);
  Mask.GetVolumeBaseline().SetValue(0.5, VolumeUnit::L);
  SEFluidCircuitNode& OxygenSource = CombinedSimpleMask.CreateNode(pulse::SimpleMaskNode::SimpleMaskOxygenSource);
  OxygenSource.GetPressure().SetValue(0.0, PressureUnit::cmH2O);
  OxygenSource.GetNextPressure().SetValue(0.0, PressureUnit::cmH2O);
  OxygenSource.GetVolumeBaseline().SetValue(std::numeric_limits<double>::infinity(), VolumeUnit::L);
  // Define path on the combined graph, this is a simple circuit, no reason to make a independent circuit at this point
  SEFluidCircuitPath& GroundToOxygenSource = CombinedSimpleMask.CreatePath(Ambient, OxygenSource, pulse::SimpleMaskPath::SimpleMaskPressure);
  GroundToOxygenSource.GetPressureSourceBaseline().SetValue(2000.0, PressureUnit::psi);
  SEFluidCircuitPath& OxygenInlet = CombinedSimpleMask.CreatePath(OxygenSource, Mask, pulse::SimpleMaskPath::SimpleMaskOxygenInlet);
  OxygenInlet.GetResistanceBaseline().SetValue(OpenResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  SEFluidCircuitPath& Seal = CombinedSimpleMask.CreatePath(Ambient, Mask, pulse::SimpleMaskPath::SimpleMaskSeal);
  Seal.GetResistanceBaseline().SetValue(OpenResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  SEFluidCircuitPath& Ports = CombinedSimpleMask.CreatePath(Mask, Ambient, pulse::SimpleMaskPath::SimpleMaskPorts);
  Ports.GetResistanceBaseline().SetValue(PortsResistance_cmH2O_s_Per_L, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  // Connect Path
  SEFluidCircuitPath& MaskToAirway = CombinedSimpleMask.CreatePath(Mask, Airway, pulse::SimpleMaskPath::SimpleMaskToAirway);
  CombinedSimpleMask.RemovePath(pulse::RespiratoryPath::EnvironmentToAirway);
  CombinedSimpleMask.SetNextAndCurrentFromBaselines();
  CombinedSimpleMask.StateChange();

  //////////////////////
  // GAS COMPARTMENTS //
  SEGasCompartment* gAirway = m_Compartments->GetGasCompartment(pulse::PulmonaryCompartment::Airway);
  SEGasCompartment* gAmbient = m_Compartments->GetGasCompartment(pulse::EnvironmentCompartment::Ambient);
  //////////////////
  // Compartments //
  SEGasCompartment& gMask = m_Compartments->CreateGasCompartment(pulse::SimpleMaskCompartment::SimpleMask);
  gMask.MapNode(Mask);
  SEGasCompartment& gOxygenSource = m_Compartments->CreateGasCompartment(pulse::SimpleMaskCompartment::SimpleMaskOxygenSource);
  gOxygenSource.MapNode(OxygenSource);
  ///////////
  // Links //
  SEGasCompartmentLink& gOxygenInlet = m_Compartments->CreateGasLink(gOxygenSource, gMask, pulse::SimpleMaskLink::SimpleMaskOxygenInlet);
  gOxygenInlet.MapPath(OxygenInlet);
  SEGasCompartmentLink& gSeal = m_Compartments->CreateGasLink(*gAmbient, gMask, pulse::SimpleMaskLink::SimpleMaskSeal);
  gSeal.MapPath(Seal);
  SEGasCompartmentLink& gPorts = m_Compartments->CreateGasLink(gMask, *gAmbient, pulse::SimpleMaskLink::SimpleMaskPorts);
  gPorts.MapPath(Ports);
  SEGasCompartmentLink& gMaskToAirway = m_Compartments->CreateGasLink(gMask, *gAirway, pulse::SimpleMaskLink::SimpleMaskToAirway);
  gMaskToAirway.MapPath(MaskToAirway);
  ///////////
  // Graph //
  SEGasCompartmentGraph& gCombinedSimpleMask = m_Compartments->GetRespiratoryAndSimpleMaskGraph();
  gCombinedSimpleMask.AddGraph(gRespiratory);
  gCombinedSimpleMask.RemoveLink(pulse::PulmonaryLink::EnvironmentToAirway);
  gCombinedSimpleMask.AddCompartment(gMask);
  gCombinedSimpleMask.AddCompartment(gOxygenSource);
  gCombinedSimpleMask.AddLink(gOxygenInlet);
  gCombinedSimpleMask.AddLink(gSeal);
  gCombinedSimpleMask.AddLink(gPorts);
  gCombinedSimpleMask.AddLink(gMaskToAirway);
  gCombinedSimpleMask.StateChange();
}

void PulseController::SetupExternalTemperature()
{
  Info("Setting Up External Temperature");
  SEThermalCircuit& exthermal = m_Circuits->GetExternalTemperatureCircuit();

  double dNoResistance = m_Config->GetDefaultClosedHeatResistance(HeatResistanceUnit::K_Per_W);
  //Define Nodes
  //Initialize temperatures to a reasonable value (ambient temperature hasn't been read in yet)
  double dAmbientTemperature_K = 295.4; //~72F

  //Note: All resistances are bound by the default open and closed switch resistances
  SEThermalCircuitNode& ThermalEnvironment = exthermal.CreateNode(pulse::ExternalTemperatureNode::Ambient);
  ThermalEnvironment.GetTemperature().SetValue(dAmbientTemperature_K, TemperatureUnit::K);
  SEThermalCircuitNode& Skin = exthermal.CreateNode(pulse::ExternalTemperatureNode::ExternalSkin);
  Skin.GetTemperature().SetValue(dAmbientTemperature_K, TemperatureUnit::K);
  SEThermalCircuitNode& Core = exthermal.CreateNode(pulse::ExternalTemperatureNode::ExternalCore);
  Core.GetTemperature().SetValue(dAmbientTemperature_K, TemperatureUnit::K);
  SEThermalCircuitNode& Clothing = exthermal.CreateNode(pulse::ExternalTemperatureNode::Clothing);
  Clothing.GetTemperature().SetValue(dAmbientTemperature_K, TemperatureUnit::K);
  SEThermalCircuitNode& Enclosure = exthermal.CreateNode(pulse::ExternalTemperatureNode::Enclosure);
  Enclosure.GetTemperature().SetValue(dAmbientTemperature_K, TemperatureUnit::K);
  SEThermalCircuitNode& Active = exthermal.CreateNode(pulse::ExternalTemperatureNode::Active);
  Active.GetTemperature().SetValue(dAmbientTemperature_K, TemperatureUnit::K);
  SEThermalCircuitNode& Ground = exthermal.CreateNode(pulse::ExternalTemperatureNode::ExternalGround);
  Ground.GetTemperature().SetValue(0.0, TemperatureUnit::K);
  Ground.GetNextTemperature().SetValue(0.0, TemperatureUnit::K);
  Ground.SetAsReferenceNode();

  //Define Paths
  //Everything will be properly initialized in Evironment::Reset
  SEThermalCircuitPath& EnvironmentCoreToAbsoluteReferencePath = exthermal.CreatePath(Core, Ground, pulse::ExternalTemperaturePath::ExternalCoreToGround);
  EnvironmentCoreToAbsoluteReferencePath.GetNextHeatSource().SetValue(0.0, PowerUnit::W);
  SEThermalCircuitPath& EnvironmentSkinToAbsoluteReferencePath = exthermal.CreatePath(Skin, Ground, pulse::ExternalTemperaturePath::ExternalSkinToGround);
  EnvironmentSkinToAbsoluteReferencePath.GetNextHeatSource().SetValue(0.0, PowerUnit::W);
  SEThermalCircuitPath& EnvironmentSkinToClothingPath = exthermal.CreatePath(Skin, Clothing, pulse::ExternalTemperaturePath::ExternalSkinToClothing);
  EnvironmentSkinToClothingPath.GetNextResistance().SetValue(dNoResistance, HeatResistanceUnit::K_Per_W);
  SEThermalCircuitPath& AbsoluteReferenceToClothingPath = exthermal.CreatePath(Ground, Clothing, pulse::ExternalTemperaturePath::GroundToClothing);
  AbsoluteReferenceToClothingPath.GetNextHeatSource().SetValue(0.0, PowerUnit::W);
  SEThermalCircuitPath& ClothingToAmbientEnvironmentPath = exthermal.CreatePath(Clothing, ThermalEnvironment, pulse::ExternalTemperaturePath::ClothingToEnvironment);
  ClothingToAmbientEnvironmentPath.GetNextResistance().SetValue(dNoResistance, HeatResistanceUnit::K_Per_W);
  SEThermalCircuitPath& ClothingToEnclosure = exthermal.CreatePath(Clothing, Enclosure, pulse::ExternalTemperaturePath::ClothingToEnclosure);
  ClothingToEnclosure.GetNextResistance().SetValue(dNoResistance, HeatResistanceUnit::K_Per_W);
  SEThermalCircuitPath& AbsoluteReferenceToAmbientEnvironmentPath = exthermal.CreatePath(Ground, ThermalEnvironment, pulse::ExternalTemperaturePath::GroundToEnvironment);
  AbsoluteReferenceToAmbientEnvironmentPath.GetNextTemperatureSource().SetValue(0.0, TemperatureUnit::K);
  SEThermalCircuitPath& AbsoluteReferenceToEnclosurePath = exthermal.CreatePath(Ground, Enclosure, pulse::ExternalTemperaturePath::GroundToEnclosure);
  AbsoluteReferenceToEnclosurePath.GetNextTemperatureSource().SetValue(0.0, TemperatureUnit::K);
  SEThermalCircuitPath& AbsoluteReferenceToActivePath = exthermal.CreatePath(Ground, Active, pulse::ExternalTemperaturePath::GroundToActive);
  AbsoluteReferenceToActivePath.GetNextTemperatureSource().SetValue(0.0, TemperatureUnit::K);
  SEThermalCircuitPath& ActiveToClothing = exthermal.CreatePath(Active, Clothing, pulse::ExternalTemperaturePath::ActiveToClothing);
  ActiveToClothing.SetNextSwitch(eGate::Open);

  exthermal.SetNextAndCurrentFromBaselines();
  exthermal.StateChange();

  SEThermalCompartment& cActive = m_Compartments->CreateThermalCompartment(pulse::TemperatureCompartment::Active);
  cActive.MapNode(Active);
  SEThermalCompartment& cClothing = m_Compartments->CreateThermalCompartment(pulse::TemperatureCompartment::Clothing);
  cClothing.MapNode(Clothing);
  SEThermalCompartment& cEnclosure = m_Compartments->CreateThermalCompartment(pulse::TemperatureCompartment::Enclosure);
  cEnclosure.MapNode(Enclosure);
  SEThermalCompartment& cExternalCore = m_Compartments->CreateThermalCompartment(pulse::TemperatureCompartment::ExternalCore);
  cExternalCore.MapNode(Core);
  SEThermalCompartment& cExternalSkin = m_Compartments->CreateThermalCompartment(pulse::TemperatureCompartment::ExternalSkin);
  cExternalSkin.MapNode(Skin);
  SEThermalCompartment& cExternalGround = m_Compartments->CreateThermalCompartment(pulse::TemperatureCompartment::ExternalGround);
  cExternalGround.MapNode(Ground);
  SEThermalCompartment& cAmbient = m_Compartments->CreateThermalCompartment(pulse::TemperatureCompartment::Ambient);
  cAmbient.MapNode(ThermalEnvironment);

  SEThermalCompartmentLink& lActiveToClothing = m_Compartments->CreateThermalLink(cActive, cClothing, pulse::TemperatureLink::ActiveToClothing);
  lActiveToClothing.MapPath(ActiveToClothing);
  SEThermalCompartmentLink& lClothingToEnclosure = m_Compartments->CreateThermalLink(cClothing, cEnclosure, pulse::TemperatureLink::ClothingToEnclosure);
  lClothingToEnclosure.MapPath(ClothingToEnclosure);
  SEThermalCompartmentLink& lClothingToEnvironment = m_Compartments->CreateThermalLink(cClothing, cAmbient, pulse::TemperatureLink::ClothingToEnvironment);
  lClothingToEnvironment.MapPath(ClothingToAmbientEnvironmentPath);
  SEThermalCompartmentLink& lExternalCoreToGround = m_Compartments->CreateThermalLink(cExternalCore, cExternalGround, pulse::TemperatureLink::ExternalCoreToGround);
  lExternalCoreToGround.MapPath(EnvironmentCoreToAbsoluteReferencePath);
  SEThermalCompartmentLink& lGroundToActive = m_Compartments->CreateThermalLink(cExternalGround, cActive, pulse::TemperatureLink::GroundToActive);
  lGroundToActive.MapPath(AbsoluteReferenceToActivePath);
  SEThermalCompartmentLink& lGroundToClothing = m_Compartments->CreateThermalLink(cExternalGround, cClothing, pulse::TemperatureLink::GroundToClothing);
  lGroundToClothing.MapPath(AbsoluteReferenceToClothingPath);
  SEThermalCompartmentLink& lGroundToEnclosure = m_Compartments->CreateThermalLink(cExternalGround, cEnclosure, pulse::TemperatureLink::GroundToEnclosure);
  lGroundToEnclosure.MapPath(AbsoluteReferenceToEnclosurePath);
  SEThermalCompartmentLink& lGroundToEnvironment = m_Compartments->CreateThermalLink(cExternalGround, cAmbient, pulse::TemperatureLink::GroundToEnvironment);
  lGroundToEnvironment.MapPath(AbsoluteReferenceToAmbientEnvironmentPath);
  SEThermalCompartmentLink& lExternalSkinToGround = m_Compartments->CreateThermalLink(cExternalSkin, cExternalGround, pulse::TemperatureLink::ExternalSkinToGround);
  lExternalSkinToGround.MapPath(EnvironmentSkinToAbsoluteReferencePath);
  SEThermalCompartmentLink& lExternalSkinToClothing = m_Compartments->CreateThermalLink(cExternalSkin, cClothing, pulse::TemperatureLink::ExternalSkinToClothing);
  lExternalSkinToClothing.MapPath(EnvironmentSkinToClothingPath);
}

void PulseController::SetupInternalTemperature()
{
  Info("Setting Up Internal Temperature");
  SEThermalCircuit& cIntemperature = m_Circuits->GetInternalTemperatureCircuit();

  SEThermalCircuitNode& Core = cIntemperature.CreateNode(pulse::InternalTemperatureNode::InternalCore);
  Core.GetTemperature().SetValue(37.0, TemperatureUnit::C);
  // Peripheral Temperature
  SEThermalCircuitNode& Skin = cIntemperature.CreateNode(pulse::InternalTemperatureNode::InternalSkin);
  Skin.GetTemperature().SetValue(33.0, TemperatureUnit::C);
  SEThermalCircuitNode& Ground = cIntemperature.CreateNode(pulse::InternalTemperatureNode::InternalGround);
  Ground.GetTemperature().SetValue(0.0, TemperatureUnit::K);
  Ground.GetNextTemperature().SetValue(0.0, TemperatureUnit::K);
  Ground.SetAsReferenceNode();

  /*Metabolic heat generation, leading to a heat source into the core*/
  SEThermalCircuitPath& TemperatureGroundToCore = cIntemperature.CreatePath(Ground, Core, pulse::InternalTemperaturePath::GroundToInternalCore);
  TemperatureGroundToCore.GetHeatSourceBaseline().SetValue(0.0, PowerUnit::W);
  /*Conduction and convection leading a resistance of heat flow from the core to the skin*/
  SEThermalCircuitPath& CoreToSkin = cIntemperature.CreatePath(Core, Skin, pulse::InternalTemperaturePath::InternalCoreToInternalSkin);
  CoreToSkin.GetResistanceBaseline().SetValue(0.056, HeatResistanceUnit::K_Per_W);

  double skinMassFraction = 0.09; //0.09 is fraction of mass that the skin takes up in a typical human \cite herman2006physics
  SEThermalCircuitPath& CoreToTemperatureGround = cIntemperature.CreatePath(Core, Ground, pulse::InternalTemperaturePath::InternalCoreToGround);
  CoreToTemperatureGround.GetCapacitanceBaseline().SetValue((1.0 - skinMassFraction) * m_InitialPatient->GetWeight(MassUnit::kg) * GetConfiguration().GetBodySpecificHeat(HeatCapacitancePerMassUnit::J_Per_K_kg), HeatCapacitanceUnit::J_Per_K);
  Core.GetHeatBaseline().SetValue(CoreToTemperatureGround.GetCapacitanceBaseline().GetValue(HeatCapacitanceUnit::J_Per_K) * Core.GetTemperature().GetValue(TemperatureUnit::K), EnergyUnit::J);

  SEThermalCircuitPath& SkinToTemperatureGround = cIntemperature.CreatePath(Skin, Ground, pulse::InternalTemperaturePath::InternalSkinToGround);
  SkinToTemperatureGround.GetCapacitanceBaseline().SetValue(skinMassFraction * m_InitialPatient->GetWeight(MassUnit::kg) * GetConfiguration().GetBodySpecificHeat(HeatCapacitancePerMassUnit::J_Per_K_kg), HeatCapacitanceUnit::J_Per_K);
  Skin.GetHeatBaseline().SetValue(SkinToTemperatureGround.GetCapacitanceBaseline().GetValue(HeatCapacitanceUnit::J_Per_K) * Skin.GetTemperature().GetValue(TemperatureUnit::K), EnergyUnit::J);

  cIntemperature.SetNextAndCurrentFromBaselines();
  cIntemperature.StateChange();

  SEThermalCompartment& InternalCore = m_Compartments->CreateThermalCompartment(pulse::TemperatureCompartment::InternalCore);
  InternalCore.MapNode(Core);
  SEThermalCompartment& InternalSkin = m_Compartments->CreateThermalCompartment(pulse::TemperatureCompartment::InternalSkin);
  InternalSkin.MapNode(Skin);
  SEThermalCompartment& InternalGround = m_Compartments->CreateThermalCompartment(pulse::TemperatureCompartment::InternalGround);
  InternalCore.MapNode(Ground);

  SEThermalCompartmentLink& GroundToInternalCore = m_Compartments->CreateThermalLink(InternalGround, InternalCore, pulse::TemperatureLink::GroundToInternalCore);
  GroundToInternalCore.MapPath(TemperatureGroundToCore);
  SEThermalCompartmentLink& InternalCoreToInternalSkin = m_Compartments->CreateThermalLink(InternalGround, InternalCore, pulse::TemperatureLink::InternalCoreToInternalSkin);
  InternalCoreToInternalSkin.MapPath(CoreToSkin);
  SEThermalCompartmentLink& InternalCoreToGround = m_Compartments->CreateThermalLink(InternalGround, InternalCore, pulse::TemperatureLink::InternalCoreToGround);
  InternalCoreToGround.MapPath(CoreToTemperatureGround);
  SEThermalCompartmentLink& InternalSkinToGround = m_Compartments->CreateThermalLink(InternalGround, InternalCore, pulse::TemperatureLink::InternalSkinToGround);
  InternalSkinToGround.MapPath(SkinToTemperatureGround);
}
