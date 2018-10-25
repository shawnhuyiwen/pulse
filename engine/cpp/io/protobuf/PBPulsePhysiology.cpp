/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/PBPulsePhysiology.h"
#include "io/protobuf/PBPhysiology.h"
#include "io/protobuf/PBProperties.h"
#include "bind/pulse/PulsePhysiology.pb.h"
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

using namespace pulse::proto;

void PBPulsePhysiology::Load(const BloodChemistryData& src, BloodChemistry& dst)
{
  PBPulsePhysiology::Serialize(src, dst);
  dst.SetUp();
}
void PBPulsePhysiology::Serialize(const BloodChemistryData& src, BloodChemistry& dst)
{
  PBPhysiology::Serialize(src.common(), dst);
  if (!src.has_arterialoxygenaverage_mmhg()) { dst.Fatal("Missing ArterialOxygen_mmHg", "BloodChemistry::Serialize"); }
  PBProperty::Load(src.arterialoxygenaverage_mmhg(), *dst.m_ArterialOxygen_mmHg);
  if (!src.has_arterialcarbondioxideaverage_mmhg()) { dst.Fatal("Missing ArterialCarbonDioxide_mmHg", "BloodChemistry::Serialize"); }
  PBProperty::Load(src.arterialcarbondioxideaverage_mmhg(), *dst.m_ArterialCarbonDioxide_mmHg);
}
BloodChemistryData* PBPulsePhysiology::Unload(const BloodChemistry& src)
{
  BloodChemistryData* dst = new BloodChemistryData();
  PBPulsePhysiology::Serialize(src, *dst);
  return dst;
}
void PBPulsePhysiology::Serialize(const BloodChemistry& src, BloodChemistryData& dst)
{
  PBPhysiology::Serialize(src, *dst.mutable_common());
  dst.set_allocated_arterialoxygenaverage_mmhg(PBProperty::Unload(*src.m_ArterialOxygen_mmHg));
  dst.set_allocated_arterialcarbondioxideaverage_mmhg(PBProperty::Unload(*src.m_ArterialCarbonDioxide_mmHg));
}

void PBPulsePhysiology::Load(const CardiovascularData& src, Cardiovascular& dst)
{
  PBPulsePhysiology::Serialize(src, dst);
  dst.SetUp();
}
void PBPulsePhysiology::Serialize(const CardiovascularData& src, Cardiovascular& dst)
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
CardiovascularData* PBPulsePhysiology::Unload(const Cardiovascular& src)
{
  CardiovascularData* dst = new CardiovascularData();
  PBPulsePhysiology::Serialize(src, *dst);
  return dst;
}
void PBPulsePhysiology::Serialize(const Cardiovascular& src, CardiovascularData& dst)
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

void PBPulsePhysiology::Load(const DrugData& src, Drugs& dst)
{
  PBPulsePhysiology::Serialize(src, dst);
  dst.SetUp();
}
void PBPulsePhysiology::Serialize(const DrugData& src, Drugs& dst)
{
  PBPhysiology::Serialize(src.common(), dst);
}
DrugData* PBPulsePhysiology::Unload(const Drugs& src)
{
  DrugData* dst = new DrugData();
  PBPulsePhysiology::Serialize(src, *dst);
  return dst;
}
void PBPulsePhysiology::Serialize(const Drugs& src, DrugData& dst)
{
  PBPhysiology::Serialize(src, *dst.mutable_common());
}

void PBPulsePhysiology::Load(const EndocrineData& src, Endocrine& dst)
{
  PBPulsePhysiology::Serialize(src, dst);
  dst.SetUp();
}
void PBPulsePhysiology::Serialize(const EndocrineData& src, Endocrine& dst)
{
  PBPhysiology::Serialize(src.common(), dst);
}
EndocrineData* PBPulsePhysiology::Unload(const Endocrine& src)
{

  EndocrineData* dst = new EndocrineData();
  PBPulsePhysiology::Serialize(src, *dst);
  return dst;
}
void PBPulsePhysiology::Serialize(const Endocrine& src, EndocrineData& dst)
{
  PBPhysiology::Serialize(src, *dst.mutable_common());
}

void PBPulsePhysiology::Load(const EnergyData& src, Energy& dst)
{
  PBPulsePhysiology::Serialize(src, dst);
  dst.SetUp();
}
void PBPulsePhysiology::Serialize(const EnergyData& src, Energy& dst)
{
  PBPhysiology::Serialize(src.common(), dst);
  dst.m_UsableEnergyStore_J = src.usableenergystore_j();
  dst.m_PeakPowerEnergyStore_J = src.peakpowerenergystore_j();
  dst.m_MediumPowerEnergyStore_J = src.mediumpowerenergystore_j();
  dst.m_EnduranceEnergyStore_J = src.enduranceenergystore_j();

  PBProperty::Load(src.bloodph(), *dst.m_BloodpH);
  PBProperty::Load(src.bicarbonatemolarity_mmol_per_l(), *dst.m_BicarbonateMolarity_mmol_Per_L);
}
EnergyData* PBPulsePhysiology::Unload(const Energy& src)
{
  EnergyData* dst = new EnergyData();
  PBPulsePhysiology::Serialize(src, *dst);
  return dst;
}
void PBPulsePhysiology::Serialize(const Energy& src, EnergyData& dst)
{
  PBPhysiology::Serialize(src, *dst.mutable_common());
  dst.set_usableenergystore_j(src.m_UsableEnergyStore_J);
  dst.set_peakpowerenergystore_j(src.m_PeakPowerEnergyStore_J);
  dst.set_mediumpowerenergystore_j(src.m_MediumPowerEnergyStore_J);
  dst.set_enduranceenergystore_j(src.m_EnduranceEnergyStore_J);

  dst.set_allocated_bloodph(PBProperty::Unload(*src.m_BloodpH));
  dst.set_allocated_bicarbonatemolarity_mmol_per_l(PBProperty::Unload(*src.m_BicarbonateMolarity_mmol_Per_L));
}

void PBPulsePhysiology::Load(const GastrointestinalData& src, Gastrointestinal& dst)
{
  PBPulsePhysiology::Serialize(src, dst);
  dst.SetUp();
  // We assume state is from after all stabilization
  dst.m_DecrementNutrients = true;
}
void PBPulsePhysiology::Serialize(const GastrointestinalData& src, Gastrointestinal& dst)
{
  PBPhysiology::Serialize(src.common(), dst);
}
GastrointestinalData* PBPulsePhysiology::Unload(const Gastrointestinal& src)
{
  GastrointestinalData* dst = new GastrointestinalData();
  PBPulsePhysiology::Serialize(src, *dst);
  return dst;
}
void PBPulsePhysiology::Serialize(const Gastrointestinal& src, GastrointestinalData& dst)
{
  PBPhysiology::Serialize(src, *dst.mutable_common());
}

void PBPulsePhysiology::Load(const HepaticData& src, Hepatic& dst)
{
  PBPulsePhysiology::Serialize(src, dst);
  dst.SetUp();
}
void PBPulsePhysiology::Serialize(const HepaticData& src, Hepatic& dst)
{
  PBPhysiology::Serialize(src.common(), dst);
}
HepaticData* PBPulsePhysiology::Unload(const Hepatic& src)
{
  HepaticData* dst = new HepaticData();
  PBPulsePhysiology::Serialize(src, *dst);
  return dst;
}
void PBPulsePhysiology::Serialize(const Hepatic& src, HepaticData& dst)
{
  PBPhysiology::Serialize(src, *dst.mutable_common());
}

void PBPulsePhysiology::Load(const NervousData& src, Nervous& dst)
{
  PBPulsePhysiology::Serialize(src, dst);
  dst.SetUp();
}
void PBPulsePhysiology::Serialize(const NervousData& src, Nervous& dst)
{
  PBPhysiology::Serialize(src.common(), dst);
  // We assume state have to be after all stabilization
  dst.m_FeedbackActive = true;
  dst.m_ArterialOxygenSetPoint_mmHg = src.arterialoxygensetpoint_mmhg();
  dst.m_ArterialCarbonDioxideSetPoint_mmHg = src.arterialcarbondioxidesetpoint_mmhg();
}
NervousData* PBPulsePhysiology::Unload(const Nervous& src)
{
  NervousData* dst = new NervousData();
  PBPulsePhysiology::Serialize(src, *dst);
  return dst;
}
void PBPulsePhysiology::Serialize(const Nervous& src, NervousData& dst)
{
  PBPhysiology::Serialize(src, *dst.mutable_common());
  dst.set_arterialoxygensetpoint_mmhg(src.m_ArterialOxygenSetPoint_mmHg);
  dst.set_arterialcarbondioxidesetpoint_mmhg(src.m_ArterialCarbonDioxideSetPoint_mmHg);
}

void PBPulsePhysiology::Load(const RenalData& src, Renal& dst)
{
  PBPulsePhysiology::Serialize(src, dst);
  dst.SetUp();
}
void PBPulsePhysiology::Serialize(const RenalData& src, Renal& dst)
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
RenalData* PBPulsePhysiology::Unload(const Renal& src)
{
  RenalData* dst = new RenalData();
  PBPulsePhysiology::Serialize(src, *dst);
  return dst;
}
void PBPulsePhysiology::Serialize(const Renal& src, RenalData& dst)
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

void PBPulsePhysiology::Load(const RespiratoryData& src, Respiratory& dst)
{
  PBPulsePhysiology::Serialize(src, dst);
  dst.SetUp();
}
void PBPulsePhysiology::Serialize(const RespiratoryData& src, Respiratory& dst)
{
  PBPhysiology::Serialize(src.common(), dst);
  dst.m_InitialExpiratoryReserveVolume_L = src.initialexpiratoryreservevolume_l();
  dst.m_InitialFunctionalResidualCapacity_L = src.initialfunctionalresidualcapacity_l();
  dst.m_InitialInspiratoryCapacity_L = src.initialinspiratorycapacity_l();
  dst.m_InitialResidualVolume_L = src.initialresidualvolume_l();

  dst.m_BreathingCycle = src.breathingcycle();
  dst.m_NotBreathing = src.notbreathing();
  dst.m_TopBreathTotalVolume_L = src.topbreathtotalvolume_l();
  dst.m_TopBreathAlveoliVolume_L = src.topbreathalveolivolume_l();
  dst.m_TopBreathDeadSpaceVolume_L = src.topbreathdeadspacevolume_l();
  dst.m_TopBreathPleuralPressure_cmH2O = src.topbreathpleuralpressure_cmh2o();
  dst.m_LastCardiacCycleBloodPH = src.lastcardiaccyclebloodph();
  dst.m_TopCarinaO2 = src.topcarinao2();
  dst.m_TopBreathElapsedTime_min = src.topbreathelapsedtime_min();
  dst.m_BottomBreathElapsedTime_min = src.bottombreathelapsedtime_min();
  dst.m_BottomBreathTotalVolume_L = src.bottombreathtotalvolume_l();
  dst.m_BottomBreathAlveoliVolume_L = src.bottombreathalveolivolume_l();
  dst.m_BottomBreathDeadSpaceVolume_L = src.bottombreathdeadspacevolume_l();
  dst.m_BottomBreathPleuralPressure_cmH2O = src.bottombreathpleuralpressure_cmh2o();
  PBProperty::Load(src.bloodphrunningaverage(), *dst.m_BloodPHRunningAverage);

  dst.m_ArterialO2PartialPressure_mmHg = src.arterialo2partialpressure_mmhg();
  dst.m_ArterialCO2PartialPressure_mmHg = src.arterialco2partialpressure_mmhg();
  dst.m_BreathingCycleTime_s = src.breathingcycletime_s();
  dst.m_DefaultDrivePressure_cmH2O = src.defaultdrivepressure_cmh2o();
  dst.m_DriverInspirationTime_s = src.driverinspirationtime_s();
  dst.m_DriverPressure_cmH2O = src.driverpressure_cmh2o();
  dst.m_DriverPressureMin_cmH2O = src.driverpressuremin_cmh2o();
  dst.m_ElapsedBreathingCycleTime_min = src.elapsedbreathingcycletime_min();
  dst.m_IEscaleFactor = src.iescalefactor();
  dst.m_InstantaneousFunctionalResidualCapacity_L = src.instantaneousfunctionalresidualcapacity_l();
  dst.m_MaxDriverPressure_cmH2O = src.maxdriverpressure_cmh2o();
  dst.m_PeakRespiratoryDrivePressure_cmH2O = src.peakrespiratorydrivepressure_cmh2o();
  dst.m_PreviousTargetAlveolarVentilation_L_Per_min = src.previoustargetalveolarventilation_l_per_min();
  dst.m_VentilationFrequency_Per_min = src.ventilationfrequency_per_min();
  dst.m_VentilationToTidalVolumeSlope = src.ventilationtotidalvolumeslope();
  PBProperty::Load(src.arterialo2runningaverage_mmhg(), *dst.m_ArterialO2RunningAverage_mmHg);
  PBProperty::Load(src.arterialco2runningaverage_mmhg(), *dst.m_ArterialCO2RunningAverage_mmHg);

  dst.m_ConsciousBreathing = src.consciousbreathing();
  dst.m_ConsciousRespirationPeriod_s = src.consciousrespirationperiod_s();
  dst.m_ConsciousRespirationRemainingPeriod_s = src.consciousrespirationremainingperiod_s();
  dst.m_ExpiratoryReserveVolumeFraction = src.expiratoryreservevolumefraction();
  dst.m_InspiratoryCapacityFraction = src.inspiratorycapacityfraction();
  dst.m_ConsciousStartPressure_cmH2O = src.consciousstartpressure_cmh2o();
  dst.m_ConsciousEndPressure_cmH2O = src.consciousendpressure_cmh2o();
}
RespiratoryData* PBPulsePhysiology::Unload(const Respiratory& src)
{
  RespiratoryData* dst = new RespiratoryData();
  PBPulsePhysiology::Serialize(src, *dst);
  return dst;
}
void PBPulsePhysiology::Serialize(const Respiratory& src, RespiratoryData& dst)
{
  PBPhysiology::Serialize(src, *dst.mutable_common());
  dst.set_initialexpiratoryreservevolume_l(src.m_InitialExpiratoryReserveVolume_L);
  dst.set_initialfunctionalresidualcapacity_l(src.m_InitialFunctionalResidualCapacity_L);
  dst.set_initialinspiratorycapacity_l(src.m_InitialInspiratoryCapacity_L);
  dst.set_initialresidualvolume_l(src.m_InitialResidualVolume_L);

  dst.set_breathingcycle(src.m_BreathingCycle);
  dst.set_notbreathing(src.m_NotBreathing);
  dst.set_topbreathtotalvolume_l(src.m_TopBreathTotalVolume_L);
  dst.set_topbreathalveolivolume_l(src.m_TopBreathAlveoliVolume_L);
  dst.set_topbreathdeadspacevolume_l(src.m_TopBreathDeadSpaceVolume_L);
  dst.set_topbreathpleuralpressure_cmh2o(src.m_TopBreathPleuralPressure_cmH2O);
  dst.set_lastcardiaccyclebloodph(src.m_LastCardiacCycleBloodPH);
  dst.set_topcarinao2(src.m_TopCarinaO2);
  dst.set_topbreathelapsedtime_min(src.m_TopBreathElapsedTime_min);
  dst.set_bottombreathelapsedtime_min(src.m_BottomBreathElapsedTime_min);
  dst.set_bottombreathtotalvolume_l(src.m_BottomBreathTotalVolume_L);
  dst.set_bottombreathalveolivolume_l(src.m_BottomBreathAlveoliVolume_L);
  dst.set_bottombreathdeadspacevolume_l(src.m_BottomBreathDeadSpaceVolume_L);
  dst.set_bottombreathpleuralpressure_cmh2o(src.m_BottomBreathPleuralPressure_cmH2O);
  dst.set_allocated_bloodphrunningaverage(PBProperty::Unload(*src.m_BloodPHRunningAverage));

  dst.set_arterialo2partialpressure_mmhg(src.m_ArterialO2PartialPressure_mmHg);
  dst.set_arterialco2partialpressure_mmhg(src.m_ArterialCO2PartialPressure_mmHg);
  dst.set_breathingcycletime_s(src.m_BreathingCycleTime_s);
  dst.set_defaultdrivepressure_cmh2o(src.m_DefaultDrivePressure_cmH2O);
  dst.set_driverinspirationtime_s(src.m_DriverInspirationTime_s);
  dst.set_driverpressure_cmh2o(src.m_DriverPressure_cmH2O);
  dst.set_driverpressuremin_cmh2o(src.m_DriverPressureMin_cmH2O);
  dst.set_elapsedbreathingcycletime_min(src.m_ElapsedBreathingCycleTime_min);
  dst.set_iescalefactor(src.m_IEscaleFactor);
  dst.set_instantaneousfunctionalresidualcapacity_l(src.m_InstantaneousFunctionalResidualCapacity_L);
  dst.set_maxdriverpressure_cmh2o(src.m_MaxDriverPressure_cmH2O);
  dst.set_peakrespiratorydrivepressure_cmh2o(src.m_PeakRespiratoryDrivePressure_cmH2O);
  dst.set_previoustargetalveolarventilation_l_per_min(src.m_PreviousTargetAlveolarVentilation_L_Per_min);
  dst.set_ventilationfrequency_per_min(src.m_VentilationFrequency_Per_min);
  dst.set_ventilationtotidalvolumeslope(src.m_VentilationToTidalVolumeSlope);
  dst.set_allocated_arterialo2runningaverage_mmhg(PBProperty::Unload(*src.m_ArterialO2RunningAverage_mmHg));
  dst.set_allocated_arterialco2runningaverage_mmhg(PBProperty::Unload(*src.m_ArterialCO2RunningAverage_mmHg));

  dst.set_consciousbreathing(src.m_ConsciousBreathing);
  dst.set_consciousrespirationperiod_s(src.m_ConsciousRespirationPeriod_s);
  dst.set_consciousrespirationremainingperiod_s(src.m_ConsciousRespirationRemainingPeriod_s);
  dst.set_expiratoryreservevolumefraction(src.m_ExpiratoryReserveVolumeFraction);
  dst.set_inspiratorycapacityfraction(src.m_InspiratoryCapacityFraction);
  dst.set_consciousstartpressure_cmh2o(src.m_ConsciousStartPressure_cmH2O);
  dst.set_consciousendpressure_cmh2o(src.m_ConsciousEndPressure_cmH2O);
}

void PBPulsePhysiology::Load(const TissueData& src, Tissue& dst)
{
  PBPulsePhysiology::Serialize(src, dst);
  dst.SetUp();
}
void PBPulsePhysiology::Serialize(const TissueData& src, Tissue& dst)
{
  PBPhysiology::Serialize(src.common(), dst);
  dst.m_RestingTissueGlucose_g = src.restingtissueglucose_g();
  dst.m_RestingBloodGlucose_g_Per_L = src.restingbloodglucose_g_per_l();
  dst.m_RestingBloodLipid_g_Per_L = src.restingbloodlipid_g_per_l();
  dst.m_RestingBloodInsulin_g_Per_L = src.restingbloodinsulin_g_per_l();
  dst.m_RestingPatientMass_kg = src.restingpatientmass_kg();
  dst.m_RestingFluidMass_kg = src.restingfluidmass_kg();
}
TissueData* PBPulsePhysiology::Unload(const Tissue& src)
{
  TissueData* dst = new TissueData();
  PBPulsePhysiology::Serialize(src, *dst);
  return dst;
}
void PBPulsePhysiology::Serialize(const Tissue& src, TissueData& dst)
{
  PBPhysiology::Serialize(src, *dst.mutable_common());
  dst.set_restingtissueglucose_g(src.m_RestingTissueGlucose_g);
  dst.set_restingbloodglucose_g_per_l(src.m_RestingBloodGlucose_g_Per_L);
  dst.set_restingbloodlipid_g_per_l(src.m_RestingBloodLipid_g_Per_L);
  dst.set_restingbloodinsulin_g_per_l(src.m_RestingBloodInsulin_g_Per_L);
  dst.set_restingpatientmass_kg(src.m_RestingPatientMass_kg);
  dst.set_restingfluidmass_kg(src.m_RestingFluidMass_kg);
}