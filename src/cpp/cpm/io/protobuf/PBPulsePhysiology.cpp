/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
PUSH_PROTO_WARNINGS()
#include "pulse/cpm/bind/PulsePhysiology.pb.h"
POP_PROTO_WARNINGS()
#include "io/protobuf/PBPulsePhysiology.h"
#include "io/protobuf/PBPhysiology.h"
#include "io/protobuf/PBProperties.h"
#include "physiology/BloodChemistry.h"
#include "physiology/Cardiovascular.h"
#include "physiology/Drugs.h"
#include "physiology/Endocrine.h"
#include "physiology/Energy.h"
#include "physiology/Gastrointestinal.h"
#include "physiology/Hepatic.h"
#include "physiology/Nervous.h"
#include "physiology/Renal.h"
#include "physiology/Respiratory.h"
#include "physiology/Tissue.h"
#include "compartment/fluid/SELiquidCompartmentGraph.h"
#include "compartment/fluid/SELiquidCompartment.h"
#include "circuit/fluid/SEFluidCircuit.h"
#include "controller/Circuits.h"
#include "controller/Compartments.h"

void PBPulsePhysiology::Load(const PULSE_BIND::BloodChemistryData& src, BloodChemistry& dst)
{
  dst.Clear();
  dst.SetUp();
  PBPulsePhysiology::Serialize(src, dst);
}
void PBPulsePhysiology::Serialize(const PULSE_BIND::BloodChemistryData& src, BloodChemistry& dst)
{
  PBPhysiology::Serialize(src.common(), dst);
  if (!src.has_arterialoxygenaverage_mmhg()) { dst.Fatal("Missing ArterialOxygen_mmHg", "BloodChemistry::Serialize"); }
  PBProperty::Load(src.arterialoxygenaverage_mmhg(), *dst.m_ArterialOxygen_mmHg);
  if (!src.has_arterialcarbondioxideaverage_mmhg()) { dst.Fatal("Missing ArterialCarbonDioxide_mmHg", "BloodChemistry::Serialize"); }
  PBProperty::Load(src.arterialcarbondioxideaverage_mmhg(), *dst.m_ArterialCarbonDioxide_mmHg);
}
PULSE_BIND::BloodChemistryData* PBPulsePhysiology::Unload(const BloodChemistry& src)
{
  PULSE_BIND::BloodChemistryData* dst = new PULSE_BIND::BloodChemistryData();
  PBPulsePhysiology::Serialize(src, *dst);
  return dst;
}
void PBPulsePhysiology::Serialize(const BloodChemistry& src, PULSE_BIND::BloodChemistryData& dst)
{
  PBPhysiology::Serialize(src, *dst.mutable_common());
  dst.set_allocated_arterialoxygenaverage_mmhg(PBProperty::Unload(*src.m_ArterialOxygen_mmHg));
  dst.set_allocated_arterialcarbondioxideaverage_mmhg(PBProperty::Unload(*src.m_ArterialCarbonDioxide_mmHg));
}

void PBPulsePhysiology::Load(const PULSE_BIND::CardiovascularData& src, Cardiovascular& dst)
{
  dst.Clear();
  dst.SetUp();
  PBPulsePhysiology::Serialize(src, dst);
}
void PBPulsePhysiology::Serialize(const PULSE_BIND::CardiovascularData& src, Cardiovascular& dst)
{
  PBPhysiology::Serialize(src.common(), dst);
  dst.m_StartSystole = src.startsystole();
  dst.m_HeartFlowDetected = src.heartflowdetected();
  dst.m_EnterCardiacArrest = src.entercardiacarrest();
  dst.m_CardiacCyclePeriod_s = src.cardiaccycleperiod_s();
  dst.m_CurrentCardiacCycleDuration_s = src.currentcardiaccycleduration_s();
  dst.m_LeftHeartElastanceModifier = src.leftheartelastancemodifier();
  dst.m_LeftHeartElastance_mmHg_Per_mL = src.leftheartelastance_mmhg_per_ml();
  dst.m_LeftHeartElastanceMax_mmHg_Per_mL = src.leftheartelastancemax_mmhg_per_ml();
  dst.m_LeftHeartElastanceMin_mmHg_Per_mL = src.leftheartelastancemin_mmhg_per_ml();
  dst.m_RightHeartElastance_mmHg_Per_mL = src.rightheartelastance_mmhg_per_ml();
  dst.m_RightHeartElastanceMax_mmHg_Per_mL = src.rightheartelastancemax_mmhg_per_ml();
  dst.m_RightHeartElastanceMin_mmHg_Per_mL = src.rightheartelastancemin_mmhg_per_ml();

  dst.m_CompressionTime_s = src.compressiontime_s();
  dst.m_CompressionRatio = src.compressionratio();
  dst.m_CompressionPeriod_s = src.compressionperiod_s();

  dst.m_CurrentCardiacCycleTime_s = src.currentcardiaccycletime_s();
  dst.m_CardiacCycleDiastolicVolume_mL = src.cardiaccyclediastolicvolume_ml();
  dst.m_CardiacCycleAortaPressureLow_mmHg = src.cardiaccycleaortapressurelow_mmhg();
  dst.m_CardiacCycleAortaPressureHigh_mmHg = src.cardiaccycleaortapressurehigh_mmhg();
  dst.m_CardiacCyclePulmonaryArteryPressureLow_mmHg = src.cardiaccyclepulmonaryarterypressurelow_mmhg();
  dst.m_CardiacCyclePulmonaryArteryPressureHigh_mmHg = src.cardiaccyclepulmonaryarterypressurehigh_mmhg();
  dst.m_LastCardiacCycleMeanArterialCO2PartialPressure_mmHg = src.lastcardiaccyclemeanarterialco2partialpressure_mmhg();
  dst.m_CardiacCycleStrokeVolume_mL = src.cardiaccyclestrokevolume_ml();

  PBProperty::Load(src.cardiaccyclearterialpressure_mmhg(), *dst.m_CardiacCycleArterialPressure_mmHg);
  PBProperty::Load(src.cardiaccyclearterialco2partialpressure_mmhg(), *dst.m_CardiacCycleArterialCO2PartialPressure_mmHg);
  PBProperty::Load(src.cardiaccyclepulmonarycapillarieswedgepressure_mmhg(), *dst.m_CardiacCyclePulmonaryCapillariesWedgePressure_mmHg);
  PBProperty::Load(src.cardiaccyclepulmonarycapillariesflow_ml_per_s(), *dst.m_CardiacCyclePulmonaryCapillariesFlow_mL_Per_s);
  PBProperty::Load(src.cardiaccyclepulmonaryshuntflow_ml_per_s(), *dst.m_CardiacCyclePulmonaryShuntFlow_mL_Per_s);
  PBProperty::Load(src.cardiaccyclepulmonaryarterypressure_mmhg(), *dst.m_CardiacCyclePulmonaryArteryPressure_mmHg);
  PBProperty::Load(src.cardiaccyclecentralvenouspressure_mmhg(), *dst.m_CardiacCycleCentralVenousPressure_mmHg);
  PBProperty::Load(src.cardiaccycleskinflow_ml_per_s(), *dst.m_CardiacCycleSkinFlow_mL_Per_s);
}
PULSE_BIND::CardiovascularData* PBPulsePhysiology::Unload(const Cardiovascular& src)
{
  PULSE_BIND::CardiovascularData* dst = new PULSE_BIND::CardiovascularData();
  PBPulsePhysiology::Serialize(src, *dst);
  return dst;
}
void PBPulsePhysiology::Serialize(const Cardiovascular& src, PULSE_BIND::CardiovascularData& dst)
{
  PBPhysiology::Serialize(src, *dst.mutable_common());
  dst.set_startsystole(src.m_StartSystole);
  dst.set_heartflowdetected(src.m_HeartFlowDetected);
  dst.set_entercardiacarrest(src.m_EnterCardiacArrest);
  dst.set_cardiaccycleperiod_s(src.m_CardiacCyclePeriod_s);
  dst.set_currentcardiaccycleduration_s(src.m_CurrentCardiacCycleDuration_s);
  dst.set_leftheartelastance_mmhg_per_ml(src.m_LeftHeartElastance_mmHg_Per_mL);
  dst.set_leftheartelastancemodifier(src.m_LeftHeartElastanceModifier);
  dst.set_leftheartelastancemax_mmhg_per_ml(src.m_LeftHeartElastanceMax_mmHg_Per_mL);
  dst.set_leftheartelastancemin_mmhg_per_ml(src.m_LeftHeartElastanceMin_mmHg_Per_mL);
  dst.set_rightheartelastance_mmhg_per_ml(src.m_RightHeartElastance_mmHg_Per_mL);
  dst.set_rightheartelastancemax_mmhg_per_ml(src.m_RightHeartElastanceMax_mmHg_Per_mL);
  dst.set_rightheartelastancemin_mmhg_per_ml(src.m_RightHeartElastanceMin_mmHg_Per_mL);

  dst.set_compressiontime_s(src.m_CompressionTime_s);
  dst.set_compressionratio(src.m_CompressionRatio);
  dst.set_compressionperiod_s(src.m_CompressionPeriod_s);

  dst.set_currentcardiaccycletime_s(src.m_CurrentCardiacCycleTime_s);
  dst.set_cardiaccyclediastolicvolume_ml(src.m_CardiacCycleDiastolicVolume_mL);
  dst.set_cardiaccycleaortapressurelow_mmhg(src.m_CardiacCycleAortaPressureLow_mmHg);
  dst.set_cardiaccycleaortapressurehigh_mmhg(src.m_CardiacCycleAortaPressureHigh_mmHg);
  dst.set_cardiaccyclepulmonaryarterypressurelow_mmhg(src.m_CardiacCyclePulmonaryArteryPressureLow_mmHg);
  dst.set_cardiaccyclepulmonaryarterypressurehigh_mmhg(src.m_CardiacCyclePulmonaryArteryPressureHigh_mmHg);
  dst.set_lastcardiaccyclemeanarterialco2partialpressure_mmhg(src.m_LastCardiacCycleMeanArterialCO2PartialPressure_mmHg);
  dst.set_cardiaccyclestrokevolume_ml(src.m_CardiacCycleStrokeVolume_mL);

  dst.set_allocated_cardiaccyclearterialpressure_mmhg(PBProperty::Unload(*src.m_CardiacCycleArterialPressure_mmHg));
  dst.set_allocated_cardiaccyclearterialco2partialpressure_mmhg(PBProperty::Unload(*src.m_CardiacCycleArterialCO2PartialPressure_mmHg));
  dst.set_allocated_cardiaccyclepulmonarycapillarieswedgepressure_mmhg(PBProperty::Unload(*src.m_CardiacCyclePulmonaryCapillariesWedgePressure_mmHg));
  dst.set_allocated_cardiaccyclepulmonarycapillariesflow_ml_per_s(PBProperty::Unload(*src.m_CardiacCyclePulmonaryCapillariesFlow_mL_Per_s));
  dst.set_allocated_cardiaccyclepulmonaryshuntflow_ml_per_s(PBProperty::Unload(*src.m_CardiacCyclePulmonaryShuntFlow_mL_Per_s));
  dst.set_allocated_cardiaccyclepulmonaryarterypressure_mmhg(PBProperty::Unload(*src.m_CardiacCyclePulmonaryArteryPressure_mmHg));
  dst.set_allocated_cardiaccyclecentralvenouspressure_mmhg(PBProperty::Unload(*src.m_CardiacCycleCentralVenousPressure_mmHg));
  dst.set_allocated_cardiaccycleskinflow_ml_per_s(PBProperty::Unload(*src.m_CardiacCycleSkinFlow_mL_Per_s));
}

void PBPulsePhysiology::Load(const PULSE_BIND::DrugData& src, Drugs& dst)
{
  dst.Clear();
  dst.SetUp();
  PBPulsePhysiology::Serialize(src, dst);
}
void PBPulsePhysiology::Serialize(const PULSE_BIND::DrugData& src, Drugs& dst)
{
  PBPhysiology::Serialize(src.common(), dst);
}
PULSE_BIND::DrugData* PBPulsePhysiology::Unload(const Drugs& src)
{
  PULSE_BIND::DrugData* dst = new PULSE_BIND::DrugData();
  PBPulsePhysiology::Serialize(src, *dst);
  return dst;
}
void PBPulsePhysiology::Serialize(const Drugs& src, PULSE_BIND::DrugData& dst)
{
  PBPhysiology::Serialize(src, *dst.mutable_common());
}

void PBPulsePhysiology::Load(const PULSE_BIND::EndocrineData& src, Endocrine& dst)
{
  dst.Clear();
  dst.SetUp();
  PBPulsePhysiology::Serialize(src, dst);
}
void PBPulsePhysiology::Serialize(const PULSE_BIND::EndocrineData& src, Endocrine& dst)
{
  PBPhysiology::Serialize(src.common(), dst);
}
PULSE_BIND::EndocrineData* PBPulsePhysiology::Unload(const Endocrine& src)
{
  PULSE_BIND::EndocrineData* dst = new PULSE_BIND::EndocrineData();
  PBPulsePhysiology::Serialize(src, *dst);
  return dst;
}
void PBPulsePhysiology::Serialize(const Endocrine& src, PULSE_BIND::EndocrineData& dst)
{
  PBPhysiology::Serialize(src, *dst.mutable_common());
}

void PBPulsePhysiology::Load(const PULSE_BIND::EnergyData& src, Energy& dst)
{
  dst.Clear();
  dst.SetUp();
  PBPulsePhysiology::Serialize(src, dst);
}
void PBPulsePhysiology::Serialize(const PULSE_BIND::EnergyData& src, Energy& dst)
{
  PBPhysiology::Serialize(src.common(), dst);
  dst.m_UsableEnergyStore_J = src.usableenergystore_j();
  dst.m_PeakPowerEnergyStore_J = src.peakpowerenergystore_j();
  dst.m_MediumPowerEnergyStore_J = src.mediumpowerenergystore_j();
  dst.m_EnduranceEnergyStore_J = src.enduranceenergystore_j();

  PBProperty::Load(src.bloodph(), *dst.m_BloodpH);
  PBProperty::Load(src.bicarbonatemolarity_mmol_per_l(), *dst.m_BicarbonateMolarity_mmol_Per_L);
}
PULSE_BIND::EnergyData* PBPulsePhysiology::Unload(const Energy& src)
{
  PULSE_BIND::EnergyData* dst = new PULSE_BIND::EnergyData();
  PBPulsePhysiology::Serialize(src, *dst);
  return dst;
}
void PBPulsePhysiology::Serialize(const Energy& src, PULSE_BIND::EnergyData& dst)
{
  PBPhysiology::Serialize(src, *dst.mutable_common());
  dst.set_usableenergystore_j(src.m_UsableEnergyStore_J);
  dst.set_peakpowerenergystore_j(src.m_PeakPowerEnergyStore_J);
  dst.set_mediumpowerenergystore_j(src.m_MediumPowerEnergyStore_J);
  dst.set_enduranceenergystore_j(src.m_EnduranceEnergyStore_J);

  dst.set_allocated_bloodph(PBProperty::Unload(*src.m_BloodpH));
  dst.set_allocated_bicarbonatemolarity_mmol_per_l(PBProperty::Unload(*src.m_BicarbonateMolarity_mmol_Per_L));
}

void PBPulsePhysiology::Load(const PULSE_BIND::GastrointestinalData& src, Gastrointestinal& dst)
{
  dst.Clear();
  dst.SetUp();
  PBPulsePhysiology::Serialize(src, dst);
}
void PBPulsePhysiology::Serialize(const PULSE_BIND::GastrointestinalData& src, Gastrointestinal& dst)
{
  PBPhysiology::Serialize(src.common(), dst);
}
PULSE_BIND::GastrointestinalData* PBPulsePhysiology::Unload(const Gastrointestinal& src)
{
  PULSE_BIND::GastrointestinalData* dst = new PULSE_BIND::GastrointestinalData();
  PBPulsePhysiology::Serialize(src, *dst);
  return dst;
}
void PBPulsePhysiology::Serialize(const Gastrointestinal& src, PULSE_BIND::GastrointestinalData& dst)
{
  PBPhysiology::Serialize(src, *dst.mutable_common());
}

void PBPulsePhysiology::Load(const PULSE_BIND::HepaticData& src, Hepatic& dst)
{
  dst.Clear();
  dst.SetUp();
  PBPulsePhysiology::Serialize(src, dst);
}
void PBPulsePhysiology::Serialize(const PULSE_BIND::HepaticData& src, Hepatic& dst)
{
  PBPhysiology::Serialize(src.common(), dst);
}
PULSE_BIND::HepaticData* PBPulsePhysiology::Unload(const Hepatic& src)
{
  PULSE_BIND::HepaticData* dst = new PULSE_BIND::HepaticData();
  PBPulsePhysiology::Serialize(src, *dst);
  return dst;
}
void PBPulsePhysiology::Serialize(const Hepatic& src, PULSE_BIND::HepaticData& dst)
{
  PBPhysiology::Serialize(src, *dst.mutable_common());
}

void PBPulsePhysiology::Load(const PULSE_BIND::NervousData& src, Nervous& dst)
{
  dst.Clear();
  dst.SetUp();
  PBPulsePhysiology::Serialize(src, dst);
}
void PBPulsePhysiology::Serialize(const PULSE_BIND::NervousData& src, Nervous& dst)
{
  PBPhysiology::Serialize(src.common(), dst);
  // We assume state have to be after all stabilization
  dst.m_BaroreceptorFeedbackStatus = src.baroreceptorfeedbackstatus();
  dst.m_BaroreceptorSaturationStatus = src.baroreceptorsaturationstatus();
  dst.m_ArterialOxygenBaseline_mmHg = src.arterialoxygenbaseline_mmhg();
  dst.m_ArterialCarbonDioxideBaseline_mmHg = src.arterialcarbondioxidebaseline_mmhg();
  dst.m_BaroreceptorActiveTime_s = src.baroreceptoractivetime_s();
  dst.m_BaroreceptorEffectivenessParameter = src.baroreceptoreffectivenessparameter();
  dst.m_BaroreceptorMeanArterialPressureBaseline_mmHg = src.baroreceptormeanarterialpressurebaseline_mmhg();
  dst.m_BaroreceptorSaturationTime_s = src.baroreceptorsaturationtime_s();
  dst.m_LastMeanArterialPressure_mmHg = src.lastmeanarterialpressure_mmhg();
  dst.m_PreviousBloodVolume_mL = src.previousbloodvolume_ml();
  dst.m_TotalSympatheticFraction = src.totalsympatheticfraction();
}
PULSE_BIND::NervousData* PBPulsePhysiology::Unload(const Nervous& src)
{
  PULSE_BIND::NervousData* dst = new PULSE_BIND::NervousData();
  PBPulsePhysiology::Serialize(src, *dst);
  return dst;
}
void PBPulsePhysiology::Serialize(const Nervous& src, PULSE_BIND::NervousData& dst)
{
  PBPhysiology::Serialize(src, *dst.mutable_common());
  dst.set_baroreceptorfeedbackstatus(src.m_BaroreceptorFeedbackStatus);
  dst.set_baroreceptorsaturationstatus(src.m_BaroreceptorSaturationStatus);
  dst.set_arterialoxygenbaseline_mmhg(src.m_ArterialOxygenBaseline_mmHg);
  dst.set_arterialcarbondioxidebaseline_mmhg(src.m_ArterialCarbonDioxideBaseline_mmHg);
  dst.set_baroreceptoractivetime_s(src.m_BaroreceptorActiveTime_s);
  dst.set_baroreceptoreffectivenessparameter(src.m_BaroreceptorEffectivenessParameter);
  dst.set_baroreceptormeanarterialpressurebaseline_mmhg(src.m_BaroreceptorMeanArterialPressureBaseline_mmHg);
  dst.set_baroreceptorsaturationtime_s(src.m_BaroreceptorSaturationTime_s);
  dst.set_lastmeanarterialpressure_mmhg(src.m_LastMeanArterialPressure_mmHg);
  dst.set_previousbloodvolume_ml(src.m_PreviousBloodVolume_mL);
  dst.set_totalsympatheticfraction(src.m_TotalSympatheticFraction);
}

void PBPulsePhysiology::Load(const PULSE_BIND::RenalData& src, Renal& dst)
{
  dst.Clear();
  dst.SetUp();
  PBPulsePhysiology::Serialize(src, dst);
}
void PBPulsePhysiology::Serialize(const PULSE_BIND::RenalData& src, Renal& dst)
{
  PBPhysiology::Serialize(src.common(), dst);
  dst.m_Urinating = src.urinating();
  dst.m_leftAfferentResistance_mmHg_s_Per_mL = src.leftafferentresistance_mmhg_s_per_ml();
  dst.m_rightAfferentResistance_mmHg_s_Per_mL = src.rightafferentresistance_mmhg_s_per_ml();
  dst.m_leftSodiumFlowSetPoint_mg_Per_s = src.leftsodiumflowsetpoint_mg_per_s();
  dst.m_rightSodiumFlowSetPoint_mg_Per_s = src.rightsodiumflowsetpoint_mg_per_s();

  PBProperty::Load(src.urineproductionrate_ml_per_min(), *dst.m_urineProductionRate_mL_Per_min_runningAvg);
  PBProperty::Load(src.urineosmolarity_mosm_per_l(), *dst.m_urineOsmolarity_mOsm_Per_L_runningAvg);
  PBProperty::Load(src.sodiumconcentration_mg_per_ml(), *dst.m_sodiumConcentration_mg_Per_mL_runningAvg);
  PBProperty::Load(src.sodiumexcretionrate_mg_per_min(), *dst.m_sodiumExcretionRate_mg_Per_min_runningAvg);
  PBProperty::Load(src.leftsodiumflow_mg_per_s(), *dst.m_leftSodiumFlow_mg_Per_s_runningAvg);
  PBProperty::Load(src.rightsodiumflow_mg_per_s(), *dst.m_rightSodiumFlow_mg_Per_s_runningAvg);
  PBProperty::Load(src.leftrenalarterialpressure_mmhg(), *dst.m_leftRenalArterialPressure_mmHg_runningAvg);
  PBProperty::Load(src.rightrenalarterialpressure_mmhg(), *dst.m_rightRenalArterialPressure_mmHg_runningAvg);
}
PULSE_BIND::RenalData* PBPulsePhysiology::Unload(const Renal& src)
{
  PULSE_BIND::RenalData* dst = new PULSE_BIND::RenalData();
  PBPulsePhysiology::Serialize(src, *dst);
  return dst;
}
void PBPulsePhysiology::Serialize(const Renal& src, PULSE_BIND::RenalData& dst)
{
  PBPhysiology::Serialize(src, *dst.mutable_common());
  dst.set_urinating(src.m_Urinating);
  dst.set_leftafferentresistance_mmhg_s_per_ml(src.m_leftAfferentResistance_mmHg_s_Per_mL);
  dst.set_rightafferentresistance_mmhg_s_per_ml(src.m_rightAfferentResistance_mmHg_s_Per_mL);
  dst.set_leftsodiumflowsetpoint_mg_per_s(src.m_leftSodiumFlowSetPoint_mg_Per_s);
  dst.set_rightsodiumflowsetpoint_mg_per_s(src.m_rightSodiumFlowSetPoint_mg_Per_s);

  dst.set_allocated_urineproductionrate_ml_per_min(PBProperty::Unload(*src.m_urineProductionRate_mL_Per_min_runningAvg));
  dst.set_allocated_urineosmolarity_mosm_per_l(PBProperty::Unload(*src.m_urineOsmolarity_mOsm_Per_L_runningAvg));
  dst.set_allocated_sodiumconcentration_mg_per_ml(PBProperty::Unload(*src.m_sodiumConcentration_mg_Per_mL_runningAvg));
  dst.set_allocated_sodiumexcretionrate_mg_per_min(PBProperty::Unload(*src.m_sodiumExcretionRate_mg_Per_min_runningAvg));
  dst.set_allocated_leftsodiumflow_mg_per_s(PBProperty::Unload(*src.m_leftSodiumFlow_mg_Per_s_runningAvg));
  dst.set_allocated_rightsodiumflow_mg_per_s(PBProperty::Unload(*src.m_rightSodiumFlow_mg_Per_s_runningAvg));
  dst.set_allocated_leftrenalarterialpressure_mmhg(PBProperty::Unload(*src.m_leftRenalArterialPressure_mmHg_runningAvg));
  dst.set_allocated_rightrenalarterialpressure_mmhg(PBProperty::Unload(*src.m_rightRenalArterialPressure_mmHg_runningAvg));
}

void PBPulsePhysiology::Load(const PULSE_BIND::RespiratoryData& src, Respiratory& dst)
{
  dst.Clear();
  dst.SetUp();
  PBPulsePhysiology::Serialize(src, dst);
}
void PBPulsePhysiology::Serialize(const PULSE_BIND::RespiratoryData& src, Respiratory& dst)
{
  PBPhysiology::Serialize(src.common(), dst);

  dst.m_BreathingCycle = src.breathingcycle();
  dst.m_NotBreathing = src.notbreathing();
  dst.m_TopBreathTotalVolume_L = src.topbreathtotalvolume_l();
  dst.m_TopBreathAlveoliVolume_L = src.topbreathalveolivolume_l();
  dst.m_TopBreathPleuralVolume_L = src.topbreathpleuralvolume_l();
  dst.m_TopBreathAlveoliPressure_cmH2O = src.topbreathalveolipressure_cmh2o();
  dst.m_TopBreathDriverPressure_cmH2O = src.topbreathdriverpressure_cmh2o();
  dst.m_TopBreathPleuralPressure_cmH2O = src.topbreathpleuralpressure_cmh2o();
  dst.m_LastCardiacCycleBloodPH = src.lastcardiaccyclebloodph();
  dst.m_TopCarinaO2 = src.topcarinao2();
  dst.m_TopBreathElapsedTime_min = src.topbreathelapsedtime_min();
  dst.m_BottomBreathElapsedTime_min = src.bottombreathelapsedtime_min();
  dst.m_BottomBreathTotalVolume_L = src.bottombreathtotalvolume_l();
  dst.m_BottomBreathAlveoliVolume_L = src.bottombreathalveolivolume_l();
  dst.m_BottomBreathPleuralVolume_L = src.bottombreathpleuralvolume_l();
  dst.m_BottomBreathAlveoliPressure_cmH2O = src.bottombreathalveolipressure_cmh2o();
  dst.m_BottomBreathDriverPressure_cmH2O = src.bottombreathdriverpressure_cmh2o();
  dst.m_BottomBreathPleuralPressure_cmH2O = src.bottombreathpleuralpressure_cmh2o();
  dst.m_PeakAlveolarPressure_cmH2O = src.peakalveolarpressure_cmh2o();
  dst.m_MaximalAlveolarPressure_cmH2O = src.maximalalveolarpressure_cmh2o();
  PBProperty::Load(src.bloodphrunningaverage(), *dst.m_BloodPHRunningAverage);
  PBProperty::Load(src.meanairwaypressure_cmh2o(), *dst.m_MeanAirwayPressure_cmH2O);

  dst.m_ArterialO2PartialPressure_mmHg = src.arterialo2partialpressure_mmhg();
  dst.m_ArterialCO2PartialPressure_mmHg = src.arterialco2partialpressure_mmhg();
  dst.m_BreathingCycleTime_s = src.breathingcycletime_s();
  dst.m_DriverPressure_cmH2O = src.driverpressure_cmh2o();
  dst.m_ElapsedBreathingCycleTime_min = src.elapsedbreathingcycletime_min();
  dst.m_IERatioScaleFactor = src.ieratioscalefactor();
  dst.m_PeakInspiratoryPressure_cmH2O = src.peakinspiratorypressure_cmh2o();
  dst.m_PeakExpiratoryPressure_cmH2O = src.peakexpiratorypressure_cmh2o();
  dst.m_PreviousTargetAlveolarVentilation_L_Per_min = src.previoustargetalveolarventilation_l_per_min();
  dst.m_VentilationFrequency_Per_min = src.ventilationfrequency_per_min();
  dst.m_VentilationToTidalVolumeSlope = src.ventilationtotidalvolumeslope();
  PBProperty::Load(src.arterialo2runningaverage_mmhg(), *dst.m_ArterialO2RunningAverage_mmHg);
  PBProperty::Load(src.arterialco2runningaverage_mmhg(), *dst.m_ArterialCO2RunningAverage_mmHg);

  dst.m_ExpiratoryHoldFraction = src.expiratoryholdfraction();
  dst.m_ExpiratoryReleaseFraction = src.expiratoryreleasefraction();
  dst.m_ExpiratoryRiseFraction = src.expiratoryrisefraction();
  dst.m_InspiratoryHoldFraction = src.inspiratoryholdfraction();
  dst.m_InspiratoryReleaseFraction = src.inspiratoryreleasefraction();
  dst.m_InspiratoryRiseFraction = src.inspiratoryrisefraction();
  dst.m_InspiratoryToExpiratoryPauseFraction = src.inspiratorytoexpiratorypausefraction();

  dst.m_leftAlveoliDecrease_L = src.leftalveolidecrease_l();
  dst.m_rightAlveoliDecrease_L = src.rightalveolidecrease_l();

  dst.m_ActiveConsciousRespirationCommand = src.activeconsciousrespirationcommand();

  dst.m_RespiratoryComplianceOverride_L_Per_cmH2O = src.respiratorycomplianceoverride_l_per_cmh2o();
  dst.m_RespiratoryResistanceOverride_cmH2O_s_Per_L = src.respiratoryresistanceoverride_cmh2o_s_per_l();
}
PULSE_BIND::RespiratoryData* PBPulsePhysiology::Unload(const Respiratory& src)
{
  PULSE_BIND::RespiratoryData* dst = new PULSE_BIND::RespiratoryData();
  PBPulsePhysiology::Serialize(src, *dst);
  return dst;
}
void PBPulsePhysiology::Serialize(const Respiratory& src, PULSE_BIND::RespiratoryData& dst)
{
  PBPhysiology::Serialize(src, *dst.mutable_common());

  dst.set_breathingcycle(src.m_BreathingCycle);
  dst.set_notbreathing(src.m_NotBreathing);
  dst.set_topbreathtotalvolume_l(src.m_TopBreathTotalVolume_L);
  dst.set_topbreathalveolivolume_l(src.m_TopBreathAlveoliVolume_L);
  dst.set_topbreathpleuralvolume_l(src.m_TopBreathPleuralVolume_L);
  dst.set_topbreathalveolipressure_cmh2o(src.m_TopBreathAlveoliPressure_cmH2O);
  dst.set_topbreathdriverpressure_cmh2o(src.m_TopBreathDriverPressure_cmH2O);
  dst.set_topbreathpleuralpressure_cmh2o(src.m_TopBreathPleuralPressure_cmH2O);
  dst.set_lastcardiaccyclebloodph(src.m_LastCardiacCycleBloodPH);
  dst.set_topcarinao2(src.m_TopCarinaO2);
  dst.set_topbreathelapsedtime_min(src.m_TopBreathElapsedTime_min);
  dst.set_bottombreathelapsedtime_min(src.m_BottomBreathElapsedTime_min);
  dst.set_bottombreathtotalvolume_l(src.m_BottomBreathTotalVolume_L);
  dst.set_bottombreathalveolivolume_l(src.m_BottomBreathAlveoliVolume_L);
  dst.set_bottombreathpleuralvolume_l(src.m_BottomBreathPleuralVolume_L);
  dst.set_bottombreathalveolipressure_cmh2o(src.m_BottomBreathAlveoliPressure_cmH2O);
  dst.set_bottombreathdriverpressure_cmh2o(src.m_BottomBreathDriverPressure_cmH2O);
  dst.set_bottombreathpleuralpressure_cmh2o(src.m_BottomBreathPleuralPressure_cmH2O);
  dst.set_peakalveolarpressure_cmh2o(src.m_PeakAlveolarPressure_cmH2O);
  dst.set_maximalalveolarpressure_cmh2o(src.m_MaximalAlveolarPressure_cmH2O);
  dst.set_allocated_bloodphrunningaverage(PBProperty::Unload(*src.m_BloodPHRunningAverage));
  dst.set_allocated_meanairwaypressure_cmh2o(PBProperty::Unload(*src.m_MeanAirwayPressure_cmH2O));

  dst.set_arterialo2partialpressure_mmhg(src.m_ArterialO2PartialPressure_mmHg);
  dst.set_arterialco2partialpressure_mmhg(src.m_ArterialCO2PartialPressure_mmHg);
  dst.set_breathingcycletime_s(src.m_BreathingCycleTime_s);
  dst.set_driverpressure_cmh2o(src.m_DriverPressure_cmH2O);
  dst.set_elapsedbreathingcycletime_min(src.m_ElapsedBreathingCycleTime_min);
  dst.set_ieratioscalefactor(src.m_IERatioScaleFactor);
  dst.set_peakinspiratorypressure_cmh2o(src.m_PeakInspiratoryPressure_cmH2O);
  dst.set_peakexpiratorypressure_cmh2o(src.m_PeakExpiratoryPressure_cmH2O);
  dst.set_previoustargetalveolarventilation_l_per_min(src.m_PreviousTargetAlveolarVentilation_L_Per_min);
  dst.set_ventilationfrequency_per_min(src.m_VentilationFrequency_Per_min);
  dst.set_ventilationtotidalvolumeslope(src.m_VentilationToTidalVolumeSlope);
  dst.set_allocated_arterialo2runningaverage_mmhg(PBProperty::Unload(*src.m_ArterialO2RunningAverage_mmHg));
  dst.set_allocated_arterialco2runningaverage_mmhg(PBProperty::Unload(*src.m_ArterialCO2RunningAverage_mmHg));

  dst.set_expiratoryholdfraction(src.m_ExpiratoryHoldFraction);
  dst.set_expiratoryreleasefraction(src.m_ExpiratoryReleaseFraction);
  dst.set_expiratoryrisefraction(src.m_ExpiratoryRiseFraction);
  dst.set_inspiratoryholdfraction(src.m_InspiratoryHoldFraction);
  dst.set_inspiratoryreleasefraction(src.m_InspiratoryReleaseFraction);
  dst.set_inspiratoryrisefraction(src.m_InspiratoryRiseFraction);
  dst.set_inspiratorytoexpiratorypausefraction(src.m_InspiratoryToExpiratoryPauseFraction);

  dst.set_leftalveolidecrease_l(src.m_leftAlveoliDecrease_L);
  dst.set_rightalveolidecrease_l(src.m_rightAlveoliDecrease_L);

  dst.set_activeconsciousrespirationcommand(src.m_ActiveConsciousRespirationCommand);

  dst.set_respiratorycomplianceoverride_l_per_cmh2o(src.m_RespiratoryComplianceOverride_L_Per_cmH2O);
  dst.set_respiratoryresistanceoverride_cmh2o_s_per_l(src.m_RespiratoryResistanceOverride_cmH2O_s_Per_L);
}

void PBPulsePhysiology::Load(const PULSE_BIND::TissueData& src, Tissue& dst)
{
  dst.Clear();
  dst.SetUp();
  PBPulsePhysiology::Serialize(src, dst);
}
void PBPulsePhysiology::Serialize(const PULSE_BIND::TissueData& src, Tissue& dst)
{
  PBPhysiology::Serialize(src.common(), dst);
  dst.m_RestingTissueGlucose_g = src.restingtissueglucose_g();
  dst.m_RestingBloodGlucose_mg_Per_mL = src.restingbloodglucose_mg_per_ml();
  dst.m_RestingBloodLipid_mg_Per_mL = src.restingbloodlipid_mg_per_ml();
  dst.m_RestingBloodInsulin_mg_Per_mL = src.restingbloodinsulin_mg_per_ml();
  dst.m_RestingFluidMass_kg = src.restingfluidmass_kg();
}
PULSE_BIND::TissueData* PBPulsePhysiology::Unload(const Tissue& src)
{
  PULSE_BIND::TissueData* dst = new PULSE_BIND::TissueData();
  PBPulsePhysiology::Serialize(src, *dst);
  return dst;
}
void PBPulsePhysiology::Serialize(const Tissue& src, PULSE_BIND::TissueData& dst)
{
  PBPhysiology::Serialize(src, *dst.mutable_common());
  dst.set_restingtissueglucose_g(src.m_RestingTissueGlucose_g);
  dst.set_restingbloodglucose_mg_per_ml(src.m_RestingBloodGlucose_mg_Per_mL);
  dst.set_restingbloodlipid_mg_per_ml(src.m_RestingBloodLipid_mg_Per_mL);
  dst.set_restingbloodinsulin_mg_per_ml(src.m_RestingBloodInsulin_mg_Per_mL);
  dst.set_restingfluidmass_kg(src.m_RestingFluidMass_kg);
}
